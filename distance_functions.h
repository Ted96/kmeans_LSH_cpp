//
// Created by ted on 10/19/18.
//

#ifndef PRJ_SUPPORT_FUNCTIONS_H
#define PRJ_SUPPORT_FUNCTIONS_H

#include <stdlib.h>
#include <algorithm>
#include <random>
#include <iostream>
#include <fstream>

#include "Datapoint.h"

using namespace std;


pair<int, double> find_nearest(Datapoint &x, vector<vector<double>> &candidates, bool slow_exact_distance , bool verbose=false);
double  calculate_min_dist_betweeen_clusters(vector<vector<double>> &clusters);
int     find_2nd_best_cluster(Datapoint &x, vector<vector<double>> &clusters, int current_cluster);


template<typename T>
T random_gen(char distrib, T u_min, T u_max) {

// returns  "u"niformly random int/float  between min-max
//       or "g"aussian  random int/float  with m = 0 , s =1

    T number;
    std::random_device rd;
    std::mt19937 gen(rd());

    if (distrib == 'n' || distrib == 'g') {

        std::normal_distribution<float> d(0, 1);

        number = (T) d(gen);


    } else if (distrib == 'u') {

        std::uniform_real_distribution<> dist(u_min, u_max);
        number = (T) dist(gen);
        //number = static_cast<T>((rand()) % (int)max + min);

    } else {
        number = rand();
    }

    //cout << number << endl;

    return number;
}

vector<float> random_gen_vec(int dimension);





double euclDistanceNN(Datapoint &q, vector <Datapoint> &dataset, double R);
double cosDistanceNN(Datapoint &datapoint, vector <Datapoint> list);


#endif //PRJ_SUPPORT_FUNCTIONS_H
