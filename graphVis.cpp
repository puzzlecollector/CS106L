/* By Minsuk (Luke) Kim (mkim14)
CS106L Fall 2016-2017

Reusing code from Spring 2016 (dropped out in the middle of the quarter).
Fixed some stylistic issues.

The program executes the following four steps:
1. Prompts the user for the name of the file containing the graph to visualize.
2. Prompts the user for the number of seconds to run the algorithm
3. Places each node into its initial configuration.
4. While the specified number of seconds has not elapsed
    1) Compute the net forces on each node.
    2) Move each node by the specified amount
    3) Display current state of the graph using the provided library.
*/
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <vector>
#include <string>
#include <utility>
#include "SimpleGraph.h"
using namespace std;

const double k = 0.01; // 10^(-3), but we may experiment with different values.
const double pi = 3.14159265358979323; // approximate value of π

void Welcome();
void promptFileName(string &filename);
void promptSeconds(double &seconds);
void process(SimpleGraph &G,int &n,string &filename);
void solve(SimpleGraph &G,int &n,double &timeLimit);

// Main method
int main() {
    Welcome();
    string filename;
    promptFileName(filename);
    double timeLimit;
    promptSeconds(timeLimit);
    int n; // n: number of nodes
    SimpleGraph G; // this is the graph we are going to work with.
    process(G,n,filename);
    solve(G,n,timeLimit);
    cout << "end of simulation, thanks for using!" << endl; //just a signpost signifying end of simulation.
    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}

// Asks the user for file name.
// It is stated that the files are all well formed, so no need for extra checking.
void promptFileName(string &filename){
    cout << "Please enter the name of the file:" << endl;
    cin >> filename;
}

// Prompt for the number of seconds to run algorithm.
void promptSeconds(double &seconds){
    cout << "Please enter the number of seconds you want the algorithm to run:" << endl;
    cin >> seconds;
    while (seconds <= 0.0){
        cout << "Please enter a positive number:" << endl;
        cin >> seconds;
    }
}

/* sets up the graph */
void process(SimpleGraph &G, int &n, string &filename){
    ifstream fin(filename.c_str());
    double a,b; // a,b means connected from a to b.
    fin >> n; // number of nodes from 0~n-1.
    // get information for edge connectivity.
    while (fin >> a >> b){ // using the fact that files are well formed, and there are two numbers per line.
        Edge edge;
        edge.start = a, edge.end = b;
        G.edges.push_back(edge);
    }
    // get information for position of nodes.
    for (int i = 0; i < n; i++){
        Node node;
        node.x = cos(2*pi*i/n), node.y = sin(2*pi*i/n);
        G.nodes.push_back(node);
    }
    InitGraphVisualizer(G);
    DrawGraph(G);
}

// solves the problem i.e. carries out step 4 of the description in the header.
void solve(SimpleGraph &G, int &n, double &timeLimit){
    clock_t startTime = clock();
    while (double(clock()-startTime)/CLOCKS_PER_SEC < timeLimit){
        // 1. Compute the net forces on each node.
        vector< pair<double,double> > diff; // first: dx, second: dy
        for (int i = 0; i < n; i++){
            diff.push_back(make_pair(0.0,0.0));
        }
        // repelling force.
        for (int i = 0; i < n; i++){
            for (int j = i+1; j < n; j++){
                double x0 = G.nodes[i].x, y0 = G.nodes[i].y;
                double x1 = G.nodes[j].x, y1 = G.nodes[j].y;
                double FRepel = k/sqrt((y1-y0)*(y1-y0) + (x1-x0)*(x1-x0));
                double theta = atan2(y1-y0,x1-x0);
                diff[i].first -= FRepel*cos(theta);
                diff[i].second -= FRepel*sin(theta);
                diff[j].first += FRepel*cos(theta);
                diff[j].second += FRepel*sin(theta);
             }
         }
         // attracting force
         for (int i = 0; i < (int)G.edges.size(); i++){
            int startNode = G.edges[i].start;
            int endNode = G.edges[i].end;
            double x0 = G.nodes[startNode].x, y0 = G.nodes[startNode].y;
            double x1 = G.nodes[endNode].x, y1 = G.nodes[endNode].y;
            double FAttract = k*((y1-y0)*(y1-y0) + (x1-x0)*(x1-x0));
            double theta = atan2(y1-y0,x1-x0);
            diff[startNode].first += FAttract*cos(theta);
            diff[startNode].second += FAttract*sin(theta);
            diff[endNode].first -= FAttract*cos(theta);
            diff[endNode].second -= FAttract*sin(theta);
         }
         // 2. Move each node by the specified amount.
         for (int i = 0; i < n; i++){
             G.nodes[i].x += diff[i].first;
             G.nodes[i].y += diff[i].second;
         }
         // 3. Display the current state of the graph using the provided library.
         DrawGraph(G);
    }
}
