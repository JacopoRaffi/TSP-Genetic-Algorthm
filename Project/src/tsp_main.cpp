#include <vector> 
#include <iostream>
#include <random>
#include <sstream>
#include <limits>
#include <fstream>

#include "./tsp_classes/tsp_seq.hpp"

using namespace std;

vector<pair<double, double>> read_coord_file(string file){
    ifstream read_file(file);
    string coordinates;
    stringstream parser;

    vector<pair<double, double>> cities;

    while(getline(read_file, coordinates)){
        parser.str(coordinates);
        string x, y;
        parser >> x >> y;
        cities.push_back(make_pair(stod(x), stod(y)));
        parser.clear();
    }

    return cities;
}

inline double euclidean_distance(pair<double, double> coord_a, pair<double, double> coord_b){
    return sqrt(pow(coord_b.first - coord_a.first, 2) + pow(coord_b.second - coord_a.second, 2));
}

Graph graph_init(vector<pair<double, double>>& cities, int seed){ 
    Graph g(cities.size());

    //lower triangular matrix (un-directed graph) so I save, more or less, half space 
    //Start from 1 because I exclude the diagonal
    for(int i = 1; i < 10; i++){ //10 is just for test in my pc
        g[i] = vector<double>(i);
        for(int j = 0; j < i; j++){
            g[i][j] = euclidean_distance(cities[i], cities[j]);
        }
    }
    return g;
}
 

int main(int argc, char *argv[]){
    if(argc < 6){
        cerr << "Usage: " << argv[0] << " workers file iterations population mode mutationRate(optional) crossoverRate(optional) seed(optional) start(optional)\n";
        return -1;
    }
    
    int workers = atoi(argv[1]);
    string file_coordinates = argv[2];
    int generations = atoi(argv[3]);
    int population_size = atoi(argv[4]);
    string mode = argv[5];
    double mutation_rate = 0.3;
    int selection_number = 0.6 * population_size;
    int seed = 1234;
    int start_vertex = 0;

    if(argv[6]){
        mutation_rate = atof(argv[6]);
    }

    if(argv[7]){
        selection_number = atof(argv[8]) * population_size;
    }

    if(argv[8]){
        seed = atoi(argv[9]);
    }

    if(argv[9]){
        start_vertex = atoi(argv[10]);
    }
    
    vector<pair<double, double>> cities = read_coord_file("../cities.txt");
    Graph g = graph_init(cities, seed);

    /*for(int i = 1; i < 10; i++){
        for(int j = 0; j < i; j++){
            cout << g[i][j] << " ";
        }
        cout << "\n";
    }*/

    if(mode == "sq"){ //sequential mode
        TSPSeq tsp(g, population_size, start_vertex);
        tsp.genetic_algorithm(generations, mutation_rate, selection_number);
    }

    if(mode == "par"){ //naive C++ threads mode

    }

    if(mode == "ff"){ //fastFlow

    }

    return 0;
}