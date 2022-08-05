#include "Point.h"

std::vector<Point> initialize_centroids(const std::vector<Point>& data, int& k){
    std::vector<Point> centroids;
    /*
    for (int i=0; i<k; i++) {
        Point cent = Point(data[i].coordinates);
        cent.cluster = i;
        centroids.push_back(cent);
    }
     */
    double c0[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    Point cent0 = Point(c0);
    cent0.cluster = 0;
    centroids.push_back(cent0);

    double c1[] = {0., 0.2, -0.2, 0.2, -0.2, .3, 0.2, -0.2, 0.2, -0.2};
    Point cent1 = Point(c1);
    cent1.cluster = 1;
    centroids.push_back(cent1);

    double c2[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    Point cent2 = Point(c2);
    cent2.cluster = 2;
    centroids.push_back(cent2);

    double c3[] = {1, 1, 1, 1, 1,0,0,0,0,0};
    Point cent3 = Point(c3);
    cent3.cluster = 3;
    centroids.push_back(cent3);
    // TODO randomize

    return centroids;
}
