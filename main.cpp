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

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#define FEATS 2


/*


int main()
{
    double tstart, tstop;
    tstart = omp_get_wtime();

#pragma omp parallel default(none)
    {
        say_hello();
    }

    tstop = omp_get_wtime();
    printf("Elapsed time: %f\n", tstop - tstart);
    return 0;
}

*/

struct Point{
    double coordinates[FEATS]{};     // coordinates
    int cluster;     // no default cluster

    Point():cluster(-1){
        for (double & coordinate : coordinates)
            coordinate = 0.;
    }

    explicit Point(const double coords[FEATS]) : cluster(-1){
        for (int i=0; i<FEATS; i++)
            coordinates[i] = coords[i];
    }

    double compute_distance(const Point& p){
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

    void operator+(const Point& p){
        for (int i=0; i<FEATS; i++)
            this->coordinates[i] += p.coordinates[i];
    }

    void stampa(){
        std::cout<<coordinates[0]<<" "<<coordinates[1]<<" "<<std::endl;
    }

};

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


std::vector<Point> initialize_centroids(const std::vector<Point>& data, int& k){
    std::vector<Point> centroids;
    /*
    for (int i=0; i<k; i++) {
        Point cent = Point(data[i].coordinates);
        cent.cluster = i;
        centroids.push_back(cent);
    }
     */
    // TODO randomize

    return centroids;
}


std::vector<Point> kMeansClustering(std::vector<Point>& data, std::vector<Point> centroids, int k, int epochs){
    int count = 0;
    while (count < epochs){

        // assign each point to the cluster of the closest centroid
        for (auto &p : data){
            double best_distance = 1000000;
            for (auto &c: centroids){
                double dist = c.compute_distance(p);
                if (dist < best_distance){
                    best_distance = dist;
                    p.cluster = c.cluster;
                }
            }
        }

        // compute new centroids
        centroids = std::vector<Point>(k);
        int clusters_size[k];
        for (int i=0; i<k; i++) {
            clusters_size[i] = 0;
            centroids[i].cluster = i;
        }
        for (auto &p : data) {
            centroids[p.cluster] += p;
            clusters_size[p.cluster]++;
        }
        for (int i=0; i<k; i++)
            centroids[i] /= clusters_size[i];

        for(auto & i : centroids)
            i.stampa();
        std::cout << std::endl;

        count++;
    }

    return centroids;
}


int main() {
    std::string fname = "data/foo.csv";
    int k = 3;
    int epochs = 10;
    std::vector<Point> data;
    std::vector<Point> centroids;

    data = load_csv(fname);
    centroids = initialize_centroids(data, k);

    centroids = kMeansClustering(data, centroids, k, epochs);

    for (auto &i: centroids)
    i.stampa();

    return 0;
}