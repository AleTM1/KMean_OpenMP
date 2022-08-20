from sklearn.cluster import KMeans
from sklearn_extra.cluster import KMedoids
import matplotlib.pyplot as plt
import pandas as pd


def read():
    df = pd.read_csv("data10000_3.csv")
    X = df.to_numpy()

    kmeans = KMeans(n_clusters=3, random_state=0).fit(X)
    kmedoids = KMedoids(n_clusters=3, random_state=0).fit(X)
    print(kmeans.cluster_centers_) # could be overlapped
    print(kmedoids.cluster_centers_) # could be overlapped

    plt.scatter(X[:,0], X[:,1], color="red", s=10)
    plt.scatter(kmedoids.cluster_centers_[:, 0], kmedoids.cluster_centers_[:, 1], color="black", s=30)
    plt.scatter(kmeans.cluster_centers_[:, 0], kmeans.cluster_centers_[:, 1], color="blue")
    plt.show()  # could be overlapped


if __name__ == "__main__":
    read()