# K-Mean with OpenMP

Parallel implementation of K-Mean algorithm with OpenMP. 

## How to use

Compile with:

```
g++ -fopenmp main.cpp -o project_kmean
```

Execute with:

```
./project_kmean <input.csv> <num_threads> <k> <initialization_alg>
```
- `<input.csv>` : input data file name (10 features).
- `<num_threads>` : number of threads.
- `<k>` : number of clusters.
- `<initialization_alg>` : set how to define the initial centroids ('random' or 'kmeanpp').


## Results

The algorithm returns the execution times of the different parts and the silhouette score (SSE).
