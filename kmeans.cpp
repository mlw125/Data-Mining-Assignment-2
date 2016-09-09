// Created by: Matthew Williams

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
using namespace std;

struct point
{        // if the values of x are not the same then the centroids can not be same
        // so keep running
    int x = 0;
    int y = 0;
    int cluster = 0;
};

int MAX_ITERATIONS = 0;

void kmeans(vector<point*>, int);
bool stop (int, vector<point *>, vector<point *>);
vector<point *> getRandomCentroids(vector<point *>, int);
void allocateToCluster(vector<point *>, vector<point *>);
vector<point *> getNewCentroids(vector<point *>, int);

int main(int argc, char * argv[])
{
    // get the k value from the command line
    int k = atoi(argv[1]);

    // if the k is 1 or less than we can't do anything
    if(k < 2)
    {
        cout << "The value of K is wrong, needs to be greater than one.\n";
    } // end if
    else
    {
            // read the file to get the data points
        string fileName = argv[2];
        char outputNumber = fileName[5];
        vector<point *> points;

        fstream reader;
        reader.open(fileName.c_str());

        int readInt = 0;

        // get the values from the file
        while (!reader.eof())
        {
            point * newPoint = new point;

            reader >> readInt;
            newPoint->x = readInt;

            reader >> readInt;
            newPoint->y = readInt;

            points.push_back(newPoint);
        } // end while
        reader.close();

        // so we have a back up in case the centroids are constantly changing
        MAX_ITERATIONS = points.size();

        // perform clustering
        kmeans(points, k);

        // output the results to a file
        string outputName = "output";
        outputName += outputNumber;
        outputName += ".txt";

        fstream writer;
        writer.open(outputName.c_str(),fstream::out);

        // write out the points to a file
        for (int x = 0; x < points.size(); x++)
        {
            writer << points[x]->x << " " << points[x]->y << " " << points[x]->cluster;
            if ((x+1) != points.size())
                writer << endl;
        } // end for
        writer.close();
    } // end else

    system("pause");
    return 0;
} // end main()

void kmeans(vector<point *> points, int k)
{
    // generate clusters based on # of points / k
    // find the mean of the distance of each clustering, their mean distance becomes centroid
    // then allocate the points to the centroid they are closest to
    // then go through each clustering and generate new centroid

    // vectors to hold the centroids and old centroids
    vector<point *> centroids = getRandomCentroids(points, k);
    vector<point *> oldCentroids;

    // fill the old centroids vector up with garbage data
    for(int x = 0; x < centroids.size(); x++)
    {
        point * newPoint = new point;
        oldCentroids.push_back(newPoint);
    } // end for

    // loop until the algorithm should stop
    int iterations = 0;
    while( stop(iterations, oldCentroids, centroids) == false)
    {
        cout << "\n\nRun: " << iterations+1 << endl;
        for(int x = 0; x < oldCentroids.size(); x++)
        {
            cout << "Centroid " << x+1 << ": (" << oldCentroids[x]->x << ", " << oldCentroids[x]->y << ")\n";
        } // end for
        for(int x = 0; x < points.size(); x++)
        {
            cout << "Point: " << "(" << points[x]->x << ", " << points[x]->y << ") Cluster: " << points[x]->cluster << endl;
        } // end for
        // set the old centroid list to be the same as the new one
        for(int x = 0; x < centroids.size(); x++)
            oldCentroids[x] = centroids[x];

        // increment the number of iterations we have done
        iterations++;
        // check if the points have a better cluster to belong to
        allocateToCluster(points, centroids);
        // get a new set of centroids based on the changes made in the function above
        centroids = getNewCentroids(points, k);
    } // end while

    cout << "\nCentroids did not change. Program Done.\n";

    return;
} // end kmeans()

bool stop (int currentIteration, vector<point *> oldCentroids, vector<point *> centroids)
{
    // if we have looped through the size of the points list
    // then just stop the k-means algorithm
    if( currentIteration > MAX_ITERATIONS)
        return true;
    // otherwise check each value from the old and new centroid list
    for(int x = 0; x < centroids.size(); x++)
    {
        // if the values of x are not the same then the centroids can not be same
        // so keep running
        if( oldCentroids[x]->x != centroids[x]->x)
            return false;
        // if the values of y are not the same then the centroids can not be same
        // so keep running
        if( oldCentroids[x]->y != centroids[x]->y)
            return false;
    } // end for
    // otherwise just quit the program, just a safety precaution
    return true;
} // end stop()

vector<point *> getRandomCentroids(vector<point *> points, int k)
{
    // arbitrarily separate the points into k groups
    int partitions = (points.size() / k) + 1;
    // counter will go until it reaches partitions and then will go back to one
    int counter = 0;
    // current cluster is the current cluster we are adding the points to
    int currentCluster = 1;
    vector<point *> centroids;
    int xSum = 0;
    int ySum = 0;

    // loop through all points in the data
    for(int x = 0; x < points.size(); x++)
    {
        // set the cluster group for the current point to whatever cluster group we're looking at.
        points[x]->cluster = currentCluster;
        // add it's x and y values to the sum
        xSum += points[x]->x;
        ySum += points[x]->y;

        // if we have reached the number of points that should be in the cluster or have reached the
        // end of the list then create a new centroid for that cluster
        if(counter+1 == partitions || (x+1) == points.size())
        {
            // create the arbitrary centroid, just the mean of the data in the cluster
            point * newPoint = new point;
            // the x and y values will be the mean of all points in the cluster group
            newPoint->x = xSum / counter;
            newPoint->y = ySum / counter;
            newPoint->cluster = currentCluster;

            centroids.push_back(newPoint);

            newPoint = NULL;

            // reset the variables
            counter = 0;
            currentCluster++;
            xSum = 0;
            ySum = 0;
        } // end if
        else
            counter++;
    } // end for

    cout << "\nRandom Centroid calculated based on mean point in randomly selected cluster.\n";
    return centroids;
} // end getRandomCentroids()

void allocateToCluster(vector<point *> points, vector<point *> centroids)
{
    cout << "\nAdd points to new clusters based on distance.\n";
    // loop through each point
    for(int x = 0; x < points.size(); x++)
    {
        int distance = -1;
        // loop through each centroids to compare the distances
        for(int y = 0; y < centroids.size(); y++)
        {
            // if the distance is the initial value then this is the current shortest distance
            // and be will be the point's cluster group
            if(distance == -1)
            {
                // distance formula
                distance = sqrt(pow((points[x]->x - centroids[y]->x), 2) + pow((points[x]->y - centroids[y]->y), 2));
                points[x]->cluster = centroids[y]->cluster;
            } // end if
            // if the distance to another cluster point is closer than the previous one
            // then assign the new cluster group to the point
            else
            {
                // distance formula
                int distance2 = sqrt(pow((points[x]->x - centroids[y]->x), 2) + pow((points[x]->y - centroids[y]->y), 2));
                // if the new cluster point is closer than the previous one
                if(distance > distance2)
                {
                    // set the distance to the new one for the next centroid, whether or not there is one.
                    distance = distance2;
                    points[x]->cluster = centroids[y]->cluster;
                }// end if
            } // end else
        } // end for
    } // end for
} // end allocateToCluster

vector<point *> getNewCentroids(vector<point *> points, int k)
{
    vector<point *> centroids;
    // go through each cluster
    for(int x = 0; x < k; x++)
    {
        // holds the current cluster we are looking at
        int cluster = x + 1;
        // sum of the x and y values from each cluster
        int sumX = 0;
        int sumY = 0;
        // for getting the average of each cluster
        int counter = 0;

        // loop through all points
        for(int y = 0; y < points.size(); y++)
        {
            // if their cluster is the current one then we will add the values
            // to the sum.
            if(points[y]->cluster == cluster)
            {
                sumX += points[y]->x;
                sumY += points[y]->y;
                counter++;
            } // end if
        } // end for

        // if is just in case one of the clusters doesn't have any points
        // for example using k=4 for input1.txt crashes the program without
        // if statement.
        if(counter != 0)
        {
            // create a centroid and add it to the list
            point * newPoint = new point;
            newPoint->x = sumX / counter;
            newPoint->y = sumY / counter;
            newPoint->cluster = cluster;
            centroids.push_back(newPoint);
        } // end if
    } // end for

    cout << "\nNew Centroids found by calculating the mean of each cluster.\n";

    return centroids;
} // end getNewCentroids
