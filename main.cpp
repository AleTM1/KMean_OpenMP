/****************************************************
 *
 * Compile with:
 * g++ -fopenmp main.cpp -o project_kmean
 *
 * Run with:
 * ./project_kmean <input.csv> <num_threads> <k> <initialization_alg>('random', 'kmeanpp')
 *
 *****************************************************/


#include "initializer.cpp"
#include "misc.cpp"


std::vector<Point> kMeansClustering(std::vector<Point>(*init_centroids)(const std::vector<Point>&, int&), std::vector<Point>& data, int k, int epochs, double eps = 0.00001){
    double tstart, tstop;
    int count = 0;
    double rel_diff = 1.;
    int clusters_size[k];
    std::vector<Point> centroids = init_centroids(data, k);

    // tmp variables
    std::vector<Point> new_centroids = std::vector<Point>(k);
    int partial_clusters_size[omp_get_max_threads()][k];
    std::vector<Point> partial_new_centroids[omp_get_max_threads()];
    double partial_rel_diff[k];

    for (int i=0; i<omp_get_max_threads(); i++)
    {
        partial_new_centroids[i] = std::vector<Point>(k);
        for (int j=0; j<k; j++)
        {
            partial_clusters_size[i][j] = 0;
            partial_new_centroids[i][j].to_zero(j);
        }
    }

    int try_num = 1;
    tstart = omp_get_wtime();
    while (rel_diff > eps){
        if (count > epochs){
            centroids = init_centroids(data, k); // initialize again if it does not converge
            count = 0;
            try_num++;
            tstart = omp_get_wtime();
        }

        // assign each point to the cluster of the closest centroid, prepare the sum, increment clusters size.
#pragma omp parallel for num_threads(omp_get_max_threads()) default(none) shared(centroids, partial_new_centroids, partial_clusters_size, data) schedule(static, 64)
            for(auto & p : data)
            {
                double best_distance = 100000;
                for (auto &c: centroids) {
                    double dist = c.compute_distance(p);
                    if (dist < best_distance) {
                        best_distance = dist;
                        p.cluster = c.cluster;
                    }
                }
                partial_new_centroids[omp_get_thread_num()][p.cluster] += p;
                partial_clusters_size[omp_get_thread_num()][p.cluster]++;
            }


#pragma omp parallel for num_threads(k) default(none) shared(new_centroids, partial_new_centroids, partial_clusters_size, partial_rel_diff) firstprivate(centroids, clusters_size, k) schedule(static, 64)
        for (int i=0; i < k; i++)
        {
            new_centroids[i].to_zero(i);
            clusters_size[i] = 0;
            for (int j = 0; j < omp_get_max_threads(); j++)
            {
                // merge partial results
                new_centroids[i] += partial_new_centroids[j][i];
                clusters_size[i] += partial_clusters_size[j][i];
                // clean tmp vars
                partial_new_centroids[j][i].to_zero(i);
                partial_clusters_size[j][i] = 0;
            }
            new_centroids[i] /= clusters_size[i];  // updates directly a shared var because each thread writes in a predetermined different location (since schedule static 1 and different thread -> different i -> different locations)
            partial_rel_diff[i] = new_centroids[i].compute_rel_diff(centroids[i]);
        }

        rel_diff = 0.;
        for (double& r:partial_rel_diff)
            rel_diff += r;
        rel_diff /= k;

        centroids = new_centroids;
        count++;
    }

    tstop = omp_get_wtime();
    printf("Attempt number: %d\n", try_num);
    printf("Clustering execution time: %f\n", tstop - tstart);

    return centroids;
}


int main(int argc, const char *argv[]) {
    omp_set_dynamic(0);
    int epochs = 50;

    // parsing arguments
    if (argc < 3) {
        std::cerr << "usage: project_kmean <input.csv> <num_thread> <k> <initialization>" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string fname = argv[1];
    omp_set_num_threads(atoi(argv[2]));
    int k = atoi(argv[3]);
    std::vector<Point> (*init_centroids) (const std::vector<Point>&, int&) = (std::string(argv[4]) == "random") ? &initialize_centroids_randomly: &initialize_centroids_kmeanpp;

    // load data
    std::vector<Point> data = load_csv(fname);
    std::vector<Point> centroids;

    // start clustering
    double tstart, tstop;
    tstart = omp_get_wtime();
    centroids = kMeansClustering(init_centroids, data, k, epochs);
    tstop = omp_get_wtime();
    printf("Total execution time: %f\n", tstop - tstart);

    double sse = compute_sse(data, centroids);
    printf("SSE: %f\n", sse);

    return 0;
}