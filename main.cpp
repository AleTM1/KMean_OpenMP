/****************************************************
 *
 * Check CMake ->
 * program name: main.cpp
 * executable name: project_kmean
 *
 * ************************
 *
 * Compile with:
 * g++ -fopenmp main.cpp -o project_kmean
 *
 * Run with:
 * OMP_NUM_THREADS=8 ./project_kmean
 *
 *****************************************************/

#include <fstream>
#include <sstream>
#include <omp.h>
#include "initializer.cpp"

//#define FEATS 10



// TODO: initialize random - automatic FEATS selector - Siluoetthe score



std::vector<Point> load_csv(const std::string& filename){
    std::vector<Point> data;
    double row[FEATS];
    std::string line, word;
    std::fstream file (filename, std::ios::in);
    if(file.is_open())
    {
        while(getline(file, line))
        {
            std::stringstream str(line);
            int i = 0;
            while(getline(str, word, ','))
                row[i++] = std::stod(word);
            data.emplace_back(row);
        }
    }
    else
        std::cout<<"Could not open the file\n";
    return data;
}


std::vector<Point> kMeansClustering(std::vector<Point>& data, std::vector<Point> centroids, int k, int epochs, double eps = 0.001){
    int count = 0;
    double rel_diff = 1.;
    int clusters_size[k];
    std::vector<Point> new_centroids = std::vector<Point>(k);

    // tmp variables
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

    while (count < epochs and rel_diff > eps){

        // assign each point to the cluster of the closest centroid, prepare the sum, increment clusters size.
#pragma omp parallel for num_threads(omp_get_max_threads()) default(none) shared(centroids, partial_new_centroids, partial_clusters_size, data) schedule(static)
            for(int i=0; i<data.size(); i++)
            {
                Point& p = data[i];
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


#pragma omp parallel for num_threads(k) default(none) shared(new_centroids, partial_new_centroids, partial_clusters_size, partial_rel_diff) firstprivate(centroids, clusters_size, k) schedule(static, 1)
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

    return centroids;
}


int main() {
    std::string fname = "data500000_10.csv";
    int k = 4;
    int epochs = 20;
    std::vector<Point> data;
    std::vector<Point> centroids;

    data = load_csv(fname);

    double tstart, tstop;
    tstart = omp_get_wtime();
    centroids = initialize_centroids(data, k);
    centroids = kMeansClustering(data, centroids, k, epochs);
    tstop = omp_get_wtime();

    printf("Total execution time: %f\n", tstop - tstart);
    for (auto &i: centroids)
        i.stampa();

    return 0;
}