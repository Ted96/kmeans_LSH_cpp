#ifndef KMEANS_KMEANS_H
#define KMEANS_KMEANS_H

#include <vector>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <list>
#include <cmath>
#include <array>
#include<set>
#include <unordered_map>

#include "Datapoint.h"
#include "distance_functions.h"
#include "Hash_core+lsh.h"

extern int dimension;
extern int max_kmeans_iterations;
extern bool complete;

using namespace std;

class kmeans {


  private:

    int k;
    vector<vector<double> > centers;
    vector<int>             y_clusters;
    vector<Datapoint>       &dataset;



	void *                  helper_data_structure;

	vector <int>            modes;

	int                     changes_per_iteration;
	vector<int>             medoid_centers;
	double                  timer;

    bool break_conditions_met( vector<vector<double>> new_centers );
	bool break_conditions_met( int iteration );

  public:


    kmeans(int k, vector<Datapoint> &dataset, int I_mode, int A_mode, int U_mode);


    void run();

    vector<vector<double>> init();

    vector<int> assign();

    vector<vector<double> > update();



    vector<vector<double>> init_kmeans_plusplus();
    vector<vector<double>> init_random();

    vector<int> assign_loyd();
	vector<int> assign_lsh();

    vector<vector<double>>update_loyd();
    vector<vector<double>>update_medoids();


    void evaluate_and_output(string &output_file_append);
    vector <double>  silhouette();

};



void kmeans_run_all_and_output(int n_clusters, vector<Datapoint> &dataset, string &output_file);

#endif //KMEANS_KMEANS_H
