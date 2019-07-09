#ifndef KMEANS_PARSE_FILES_FUNCTIONS_H
#define KMEANS_PARSE_FILES_FUNCTIONS_H


#include <algorithm>
#include <getopt.h>
#include <cstring>
#include "Datapoint.h"


void parse_arguments(int argc, char **argv);
void parse_config_file();
void parse_input_file();

vector<Datapoint> create_dataset(int num_points);


#endif //KMEANS_PARSE_FILES_FUNCTIONS_H
