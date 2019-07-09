#include <iostream>
#include <vector>

#include "globals.h"
#include "main's_functions.h"
#include "distance_functions.h"
#include "kmeans.h"

//#include "gtest/gtest.h"


using namespace std;

//  w /      r     /  cap     = result

//0.1    800-1200     0.05    =100% uniform!     r=w/e
//1.0    1000-1200    0.05    =66%   50% spikes
// 1.5-2.0                    =45%   20% spikes


int main(int argc, char **argv) {


    parse_arguments  (argc, argv);
	parse_config_file();
	parse_input_file();


 //   num_points = 5000;  ////////////////////////////////////////////////////////////debug
//
//    cout << "\t\t<--- START "<<n_clusters <<"MEANS" << " --->\n\n/metric="
//        << metric << " k=" << k << " L=" << L
//         << " M=" << M << " probes=" << probes << "\n" << c_file << "\\\\//    "
//         << in_file << "(" << num_points << ") ->> " << out_file << "\n\n";

	auto dataset    =  create_dataset(num_points);

	kmeans_run_all_and_output(n_clusters, dataset, out_file);


    printf("\n\t\t <--- END KMEANS --->\n");
    return 0;
}