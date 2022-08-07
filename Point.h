//
// Created by alessandro on 05/08/22.
//

#ifndef PROJECT_KMEAN_POINT_H
#define PROJECT_KMEAN_POINT_H

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <omp.h>
#include <random>
#include <algorithm>

#define FEATS 5


struct Point{
    double coordinates[FEATS]{};     // coordinates
    int cluster = -1;     // no default cluster

    Point(){
        to_zero(-1);
    }

    explicit Point(const double coords[FEATS]) : cluster(-1){
        for (int i=0; i<FEATS; i++)
            coordinates[i] = coords[i];
    }

    void to_zero(int i){
        cluster = i;
        for (double & coordinate : coordinates)
            coordinate = 0.;
    }

    double compute_distance(const Point& p) const {
        double total_d = 0;
        for (int i=0; i<FEATS; i++)
            total_d += (this->coordinates[i] - p.coordinates[i]) * (this->coordinates[i] - p.coordinates[i]);
        return total_d;
    }

    void operator+=(const Point& p){
        for (int i=0; i<FEATS; i++)
            this->coordinates[i] += p.coordinates[i];
    }

    void operator/=(const int& cardinality){
        for (double & coordinate : coordinates)
            coordinate /= (double)cardinality;
    }

    bool operator==(const Point& p) const {
        for (int i=0; i<FEATS; i++)
            if (this->coordinates[i] != p.coordinates[i])
                return false;
        return true;
    }

    double compute_rel_diff(const Point& p) const {
        double rel_diff = 0.;
        for (int i=0; i<FEATS; i++)
            rel_diff += std::abs(this->coordinates[i] - p.coordinates[i]);
        return rel_diff;
    }

};



#endif //PROJECT_KMEAN_POINT_H
