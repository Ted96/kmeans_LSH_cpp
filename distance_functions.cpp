#include "distance_functions.h"

extern char metric;


pair<int, double> find_nearest(Datapoint &x, vector<vector<double>> &candidates, bool exact_distance, bool verbose)
{


	vector<double> distances;

	double min_dist = 10000000;
	double d;
	int index_of_min;


	vector<double> origin = x.get_coordinates();

	if (verbose)
		printf("// Dnears dp_%s :", x.getId().c_str());

	for (int i = 0; i < candidates.size(); i++)
	{

		//d = x.dist(candidates[i]);
		d = 0.0;


		if (metric == 'e')
		{
			for (int j = 0; j < origin.size(); ++j)
			{

				d += (origin[j] - candidates[i][j]) * (origin[j] - candidates[i][j]);

				if (exact_distance == false && d > min_dist)         //already passed min_dist. check no further
					goto SKIP;
			}
		}
		else if (metric == 'c')
		{
			d = x.dist(candidates[i]);
		}


		// distances.push_back(d);
		if (d < min_dist)
		{
			min_dist = d;
			index_of_min = i;
		}


		SKIP:    // go to next iteration
		if (verbose)
		{
			printf("// (to %d)= %.3f ", i, sqrt(d));

			if (!exact_distance && d != min_dist && metric == 'e')
				printf("~");
		}
	}

	if (exact_distance)
		return make_pair(index_of_min, sqrt(min_dist));
	else
		return make_pair(index_of_min, min_dist);

}


int find_2nd_best_cluster(Datapoint &x, vector<vector<double>> &clusters, int current_cluster)
{


	static int fails = 0;

	if (clusters.size() < 2)
		cout << " size problem on find 2nd best cluster\n";

	// printf("// \nDnears2 dp_%s :", x.getId().c_str());

	vector<double> distances;

	double min_dist = 10000000;
	double second_min_dist = 1000000;
	int index_of_best = 0, index_of_2nd_best = 0;
	double d;

	vector<double> origin = x.get_coordinates();


	for (int i = 0; i < clusters.size(); i++)
	{

		d = 0.0;
		//d = x.dist(candidates[i]);

		if (metric == 'e')
			for (int j = 0; j < origin.size(); ++j)
				d += (origin[j] - clusters[i][j]) * (origin[j] - clusters[i][j]);

		else if (metric == 'c')
			d = x.dist(clusters[i]);


		// printf(" (to %d)= %.3f //", i, sqrt(d));

		// distances.push_back(d);
		if (d < min_dist)
		{
			second_min_dist = min_dist;
			index_of_2nd_best = index_of_best;

			min_dist = d;
			index_of_best = i;
		}
		else if (d < second_min_dist)
		{
			second_min_dist = d;
			index_of_2nd_best = i;
		}

	}

//    cout << "\n debug find2best of dp_"<<x.getId()<<"\n\t1st= CLu" << index_of_best << " (" << sqrt(min_dist)
//            << ")\n\t2nd= CLu" << index_of_2nd_best << " (" << sqrt(second_min_dist) << ")\n";



	if ( x.getId() == "3000" || x.getId() == "4999")
		cout << "\tLSH Fails so far = " << fails << endl;

	if (index_of_best != current_cluster)
	{
		fails++;
		return index_of_best;
	}


	return index_of_2nd_best;

}


double calculate_min_dist_betweeen_clusters(vector<vector<double>> &clusters)
{

	//     O( n^2 )       pls show mercy

	double d, min_dist = 3000000.0;

	//for each center  i  in cluster::
	for (int i = 0; i < clusters.size() - 1; ++i)
	{

		Datapoint temp(clusters[i], "temp_dist_between_clusters");

		// for each of the other centers in clusters  : calculate min dist from i.

		//(create a subvector of clusters).
		vector<vector<double >> remaining(clusters.begin() + i + 1, clusters.end());
		// cout << "remaining=" << remaining.size() << endl;

		d = find_nearest(temp, remaining, false, false).second;

		if (d < min_dist)
			min_dist = d;

	}

	return min_dist;
}


vector<float> random_gen_vec(int dimension)
{

	vector<float> v;
	v.reserve(dimension);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<float> d(0, 1);

	for (int i = 0; i < dimension; ++i)
	{
		v.push_back(d(gen));
	}

	return v;
}
