#include "globals.h"

char metric     = 'e';

int dimension   = 203;


int num_points = 20;
float capacity_percent_lsh  = 0.15;
bool check_g    =   true;


std::string in_file, c_file="options.conf", out_file;

int k = 4;
int L = 5;
double w    = 2.0;     // 0.1  ---  2.0



int n_clusters  = 20;
int max_kmeans_iterations  = 35;
bool complete   = false;



int M = 0;
int probes      = 20;
