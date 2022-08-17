//
// Created by alessandro on 16/08/22.
//

#include "base_kmean.cpp"

void assign_closest_cluster(std::vector<Point>& data, std::vector<Point>& centroids){
    for (Point& p : data){
        double min_dist = 1000000;
        for (const Point& c : centroids){
            double d = p.compute_distance(c);
            if (d < min_dist){
                min_dist = d;
                p.cluster = c.cluster;
            }
        }
    }
}


std::vector<Point> kMedoidsClustering(std::vector<Point>(*init_centroids)(const std::vector<Point>&, int&), std::vector<Point>& data, int k, int epochs, double eps = 0.00001){
    double tstart, tstop;
    int count = 0;
    double rel_diff = 1.;
    std::vector<Point> centroids = init_centroids(data, k);

    assign_closest_cluster(data, centroids);

    // tmp variables
    /*std::vector<Point> new_centroids = std::vector<Point>(k);
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
     */

    double current_sse = compute_sse(data, centroids);
    int size = (int)data.size();
    int total_count = (int)pow((double)size, (double)k);
    int try_num = 1;
    tstart = omp_get_wtime();
    while (count < total_count){
        if (false){
            centroids = init_centroids(data, k); // initialize again if it does not converge
            count = 0;
            try_num++;
            tstart = omp_get_wtime();
        }

        int value = count;
        int pos = 0;
        int idxes[k];
        while (pos < k){
            idxes[pos++] = value % size;
            value = (value - (value % size))/size;
        }

        bool check = true;
        for (int i=0; i<k-1; i++)
            for (int j=i+1; j<k; j++)
                if (idxes[i] == idxes[j])
                    check = false;

        if (check) {
            std::vector<Point> new_centroids;
            for (auto i: idxes)
                new_centroids.push_back(data[i]);
            assign_closest_cluster(data, new_centroids);
            double new_sse = compute_sse(data, new_centroids);
            if (new_sse > current_sse) {
                current_sse = new_sse;
                centroids = new_centroids;
            }
        }

        count++;
        std::cout<<count<<std::endl;
    }

    tstop = omp_get_wtime();
    printf("Attempt number: %d\n", try_num);
    printf("Clustering execution time: %f\n", tstop - tstart);

    assign_closest_cluster(data, centroids);
    return centroids;
}