//
// Created by ted on 12/3/18.
//

#include "gtest/gtest.h"



#include "globals.h"
#include "main's_functions.h"
#include "distance_functions.h"
#include "Hash_core+lsh.h"
#include "kmeans.h"



#include <vector>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <list>
#include <cmath>
#include <array>
#include <set>
#include <unordered_map>


TEST( Dataset, all)
{

	c_file = "options.conf";
	in_file="inKmeans";
  //  parse_arguments  (argc, argv);
	parse_config_file();
	parse_input_file();

    num_points = 5000;  ////////////////////////////////////////////////////////////debug

	EXPECT_EQ(5000 , create_dataset( num_points).size());
	//EXPECT_EQ (9, addt(9));

}

TEST( Parameters_global ,all ){

	EXPECT_NEAR(0.0 , w , 2.0);
	EXPECT_LT(0 , L);
	EXPECT_LT(1 , k);


}

TEST(distance_fucntions , all){


	parse_config_file();
	parse_input_file();

    num_points = 1000;  ////////////////////////////////////////////////////////////debug
	auto dataset = create_dataset(1000);

	vector< vector<double>> vectors_random;

	vectors_random.push_back( dataset[100].get_coordinates());
	vectors_random.push_back( dataset[2].get_coordinates());
	vectors_random.push_back( dataset[44].get_coordinates());
	vectors_random.push_back( dataset[750].get_coordinates());

	auto pair_results = find_nearest(dataset[100] ,vectors_random, true,false );

	EXPECT_EQ( 0 ,   pair_results.first);
	EXPECT_EQ( 0.0 , pair_results.second);


}

TEST(silhouette , all)
{
	c_file="options.conf";
	parse_config_file();
	parse_input_file();

    num_points = 1000;  ////////////////////////////////////////////////////////////debug
	auto dataset = create_dataset(1000);
	auto kmeans_instance = new kmeans(n_clusters, dataset, 1, 2, 1);
	kmeans_instance->run();
	auto s = kmeans_instance->silhouette().back();

	EXPECT_NEAR( 0 , s, 1.0);
	delete kmeans_instance;
}



int main(int argc, char **argv)
{

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();


}