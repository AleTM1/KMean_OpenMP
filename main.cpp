/****************************************************
 *
 * Compile with:
 * g++ -fopenmp main.cpp -o project_kmean
 *
 * Run with:
 * ./project_kmean <input.csv> <kmean_variant>('kmean', 'kmedoids') <num_threads> <k> <initialization_alg>('random', 'kmeanpp')
 *
 *****************************************************/


#include "base_kmean.cpp"
#include "kmedoids.cpp"


int main(int argc, const char *argv[]) {
    omp_set_dynamic(0);
    int epochs = 5;

    // parsing arguments
    if (argc < 3) {
        std::cerr << "usage: project_kmean <input.csv> <kmean_variant> <num_thread> <k> <initialization>" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string fname = argv[1];
    std::string clustering_variant = argv[2];
    omp_set_num_threads(atoi(argv[3]));
    int k = atoi(argv[4]);
    std::vector<Point> (*init_centroids) (const std::vector<Point>&, int&) = (std::string(argv[5]) == "random") ? &initialize_centroids_randomly: &initialize_centroids_kmeanpp;

    // load data
    std::vector<Point> data = load_csv(fname);
    std::vector<Point> centroids;

    // start clustering
    double tstart, tstop;
    tstart = omp_get_wtime();
    if (std::string(clustering_variant) == "kmean")
        centroids = kMeansClustering(init_centroids, data, k, epochs);
    else
        centroids = kMedoidsClustering(init_centroids, data, k, epochs);
    tstop = omp_get_wtime();
    printf("Total execution time: %f\n", tstop - tstart);

    double sse = compute_sse(data, centroids);
    printf("SSE: %f\n", sse);

    for (auto e:centroids) {
        std::cout<<std::endl;
        for (auto f: e.coordinates)
            std::cout << f << " ";
    }
    std::cout<<std::endl;

    return 0;
}