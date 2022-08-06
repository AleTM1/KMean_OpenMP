#include "Point.h"
#include <random>
#include <algorithm>


bool contains(std::vector<Point>& vec, Point& p){
    for (auto & el : vec)
        if (p == el)
            return true;
    return false;
}


std::vector<Point> initialize_centroids_randomly(const std::vector<Point>& data, int& k) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, data.size());
    std::vector<Point> centroids;

    int clust = 0;
    while (centroids.size() < k) {
        Point sel_elem = data[distrib(gen)];
        if (!contains(centroids, sel_elem))
        {
            sel_elem.cluster = clust++;
            centroids.push_back(sel_elem);
        }
    }
    return centroids;
}

std::vector<Point> initialize_centroids_kmeanpp(const std::vector<Point>& data, int& k) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, data.size());

    std::vector<Point> centroids;
    Point starting_point = data[distrib(gen)];
    starting_point.cluster = 0;
    centroids.push_back(starting_point);

    int clust = 1;
    while (centroids.size() < k) {
        Point sel_elem = data[distrib(gen)];
        if (!contains(centroids, sel_elem))
        {
            sel_elem.cluster = clust++;
            centroids.push_back(sel_elem);
        }
    }
    return centroids;
}