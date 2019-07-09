#pragma once
#include <string>



extern char metric;

extern int dimension ;
extern int num_points;
extern bool check_g;


extern float capacity_percent_lsh;


extern std::string in_file, c_file, out_file;

extern int k ;
extern int L ;
extern double w ;       // 0.1  ---  2.0


extern int M ;
extern int probes ;


extern int n_clusters ;
extern int max_kmeans_iterations ;
extern bool complete ;