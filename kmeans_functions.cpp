
// the good stuff is here.


#include "kmeans.h"
#include "distance_functions.h"

#include <unordered_map>
#include <iomanip>


extern char metric;
// ------------------------------------------------------------------------------------------------//
//-------------------------       INITIALIZATION OF CENTERS      ---------------------------------//
// ------------------------------------------------------------------------------------------------//

vector<vector<double>> kmeans::init_kmeans_plusplus()
{

	cout << "\tKMEANS_init (kmeans++):: \n";

	vector<Datapoint, std::allocator<Datapoint>>::iterator it, it2;

	vector<int> centers_chosen;

	double sum_distances, distance;
	vector<double> distances;
	vector<double> possibilities;

	//first random center
	centers_chosen.push_back(random_gen<int>('u', 0, dataset.size() - 1));
	this->centers.clear();
	this->centers.push_back(dataset[centers_chosen[0]].get_coordinates());

	// choose the other random centers
	while (centers_chosen.size() < k)
	{

		it = dataset.begin();
		for (int i = 0; i < dataset.size(); ++i, it.operator++())
		{

			if (std::find(centers_chosen.begin(), centers_chosen.end(), i) != centers_chosen.end())
			{
				//cout << " [[" << i << "]] already here :p\n";
				distances.push_back(0.0);
				continue;
			}

			distance = find_nearest(*it, this->centers, true).second;

			distances.push_back(distance);

		}

		sum_distances = accumulate(distances.begin(), distances.end(), 0.0);
		auto random = random_gen<double>('u', 0.0, 1.0);
		double sum = 0.0;

		for (int j = 0; j < dataset.size(); ++j)
		{


			sum += distances[j] / sum_distances;
			if (sum > random)
			{
				centers_chosen.push_back(j);
				centers.push_back(dataset[j].get_coordinates());
				break;
			}

		}

		possibilities.clear();
		distances.clear();
	}


//	cout << "\nbetter ++ centers=  (from items): ";
	for (int l = 0; l < centers_chosen.size(); ++l)
		cout << centers_chosen[l] + 1 << " ";
	cout << "\n\n";

	return this->centers;    //!!return necesary?

};

vector<vector<double>> kmeans::init_random()
{
	//cout << "\tKMEANS_init (random):: ";

	set<int> random_datapoint_indexes;

	//no duplicates when randoming!!
	while (random_datapoint_indexes.size() != k)
		random_datapoint_indexes.insert(random_gen<int>('u', 0, dataset.size() - 1));

	for (auto r : random_datapoint_indexes)
		//add it to array of centers.
		this->centers.push_back(dataset[r].get_coordinates());


	return this->centers;    //!!return necesary?
};


// ------------------------------------------------------------------------------------------------//
//-------------------------         ASSIGNEMENT      ----------------------------------------------//
// ------------------------------------------------------------------------------------------------//

vector<int> kmeans::assign_loyd()
{

	cout << "\tASSIGN() (loyd)::\n";

	this->changes_per_iteration = 0;

	//for each datapoint in dataset, find its nearest cluster.
	for (int i = 0; i < this->dataset.size(); ++i)
	{

		// find index of minimum distance.  this is gonna be the new center
		int nearest_cluster = find_nearest(dataset[i], this->centers, true).first;

		if (this->y_clusters[i] != nearest_cluster)
		{
			this->y_clusters[i] = nearest_cluster;
			this->changes_per_iteration++;
		}

	}

//	for (int j = 0; j < y_clusters.size(); ++j)
//	{
//
//		cout << "dp_" << j + 1 << " -> " << y_clusters[j] << " ";
//		if (j % 11 == 10) cout << endl;
//	}
//	cout << endl;

	cout << "+" << this->changes_per_iteration << " datapoint changes" << endl;

	return y_clusters;        //!!resolve  void or return??

};

vector<int> kmeans::assign_lsh()
{


	cout << "\n\tASSIGN()";
	if (modes[1] == 2)
		cout<<" LSH ::\n";
	if (modes[1] == 3)
		cout<<" CUBE ::\n";

	int debug_count_changes = 0;
	int range_count_changes = 0;
	int total_count_changes = 0;
	int true_count_changes  = 0;

	int nearest_cluster, iteration = 0;

	// for each datapoint in dataset:     keep a flag:   is it assigned to a cluster at previous iteration or not ?
	vector<int> was_assigned_at_iteration(dataset.size(), -1);


	set<Datapoint *> search_results;

	auto new_y_clusters = this->y_clusters;

	double maxR = metric == 'e' ? 1.7 : 1.30;

	double R = calculate_min_dist_betweeen_clusters(this->centers) / 2;

	while (iteration < 12 && R < maxR &&   total_count_changes / (double) dataset.size() < 0.88)
	{
		// perform R-range search for each cluster c  and assign datapoints into clusters


		cout<<"R";
		//cout << "\n\t\t<<R>><<R>><<R>>> " << iteration << " <<<R>><<R>><<R>><<R>>";

		for (int c = 0; c < centers.size(); c++)
		{

			Datapoint center(centers[c], "assign_hash");
			//search_results = ((Hypercube *) helper_data_structure)->search(center, R); // if
			search_results = aaaaaaaa( center , R, dataset , modes[1] -1);

			//printf("\nCsearch| R=%.3f  Cl_%-3d [%3d results] ::", R, c, (int) search_results.size());

			for (auto &d : search_results)
			{

				//  printf(" dp_%-3s(%.2f)",d->getId().c_str(), d->dist(centers[c]));

				int numeric_id = stoi(d->getId()) - 1;

				if (was_assigned_at_iteration[numeric_id] == -1)  // not assigned at all   ,  so far
				{
					new_y_clusters[numeric_id] = c;
					was_assigned_at_iteration[numeric_id] = iteration;
					//   printf("+  ");
				}

				else if (was_assigned_at_iteration[numeric_id] == iteration)  // was just assigned in this range search
				{
					vector<vector<double_t >> candidates;
					candidates.push_back(centers[new_y_clusters[numeric_id]]);
					candidates.push_back(centers[c]);

					if (find_nearest(*d, candidates, false).first == 1)
						new_y_clusters[numeric_id] = c;

					//    printf("!%d ",new_y_clusters[numeric_id]);

					candidates.clear();
				}
				else
				{
					if (new_y_clusters[numeric_id] == c)
					{
						//    printf("_* ");
					}
					else
					{
						//   printf(" * ");
					}

				}

			}
		}
		//cout << "\n";


		range_count_changes = count_if(
				was_assigned_at_iteration.begin(), was_assigned_at_iteration.end(),
				[=](int when) { return (when == iteration); }
		);

		total_count_changes += range_count_changes;
//		cout << "\n\t\t" << range_count_changes << " range-search changes !!   | ratio= "
//		     << total_count_changes / (double) dataset.size() << endl;


		// do 1 more good iteration of radius = 80% * maxDistance   (  max distance=1.4e / 1.1c  so no need to radius search...)
		if (metric == 'e' && R > 0.7 && R < 0.8)
			R = 1.15;
		else if (metric == 'c' && R > 0.7 && R < 0.85)
			R = 0.98;
		else
			R *= 2;

		iteration++;

	}


	//for each datapoint not found at search in dataset, find its nearest cluster.
	for (int i = 0; i < this->dataset.size(); ++i)
	{

		if (was_assigned_at_iteration[i] != -1)
			continue;

		true_count_changes++;
		nearest_cluster = find_nearest(dataset[i], this->centers, false).first;

		//printf("<(H cube) leftover:dp_%3s| ", (dataset[i].getId()).c_str());
		// printf("\t-->@CLU #%d\n", nearest_cluster);

		if (new_y_clusters[i] != nearest_cluster)
		{
			new_y_clusters[i] = nearest_cluster;
			debug_count_changes++;
		}

	}


	cout << "\t\t+" << debug_count_changes << " / " << true_count_changes
	     << " leftover true changes !!  (after range_changes)";


//    cout<<"new clusters:";
//    for (int j = 0; j < y_clusters.size(); ++j)
//        cout << y_clusters[j] << " ";
//    cout << endl;

	total_count_changes += debug_count_changes;


	this->changes_per_iteration = 0;
	for (int i = 0; i < new_y_clusters.size(); i++)
		if (new_y_clusters[i] != y_clusters[i])
			this->changes_per_iteration++;


	cout << "\n\t\t << Assignements >> (" << changes_per_iteration<< ") :: "<< total_count_changes << " / " << dataset.size() << endl;
	return new_y_clusters;

};


vector<vector<double>> kmeans::update_loyd()
{


	vector<int> count_points_per_center(k, 0);

	vector<vector<double> > new_centers(k, vector<double>(dimension, 0.0));

	cout << "\n\tUPDATE() (loyd)::\n";


	int i = 0;
	for (auto &dp : dataset)
	{

		// calculate (#k) sums of all datapoints of each cluster
		std::transform(
				new_centers[y_clusters[i]].begin(),
				new_centers[y_clusters[i]].end(),
				dp.get_coordinates().begin(),
				new_centers[y_clusters[i]].begin(),
				std::plus<double>()
		);

//
//        cout << "\nitem_" << dp.getId() << " (clu_" << y_clusters[i] << " ) , sum=";
//        for (int j = 0; j < 4; ++j) {
//            cout << new_centers[y_clusters[i]][j] << " |";
//        }
//
		count_points_per_center[y_clusters[i]]++;
		i++;
	}


	for (auto check: count_points_per_center)
	{
		if (check == 0) cout << "\n DIVISION BY ZERO @UPDATE()   (F)\n";
	}

	// new center (mean) =    sum of distances  /  number of points
	for (int cluster = 0; cluster < k; cluster++)
	{

		if (count_points_per_center[cluster] != 0)
		{
			std::transform(
					new_centers[cluster].begin(),
					new_centers[cluster].end(),
					new_centers[cluster].begin(),
					std::bind2nd
							(
									std::divides<double>(),
									count_points_per_center[cluster]
							)
			);

//			cout << "Clu " << cluster << " [ " << count_points_per_center[cluster]
//			     << " ]\n";
		}
		else
			cout << "\nClu " << cluster << " has no datapoints!!   :(\n";

	}

	cout << endl;
	return new_centers;
};


vector<vector<double>> kmeans::update_medoids()
{


	vector<vector<double> > new_centers;
	medoid_centers.clear();
	cout << "\n\tUPDATE() (medoids)::\n";

	int best_medoid = -1;
	double min_sum_dist, d, sum_distances_from_i;
	int dp1, dp2;

	struct HASH
	{
		size_t operator()(const pair<int, int> &x) const
		{
			return hash<long long>()(((long long) x.first) ^ (((long long) x.second) << 32));
		}
	};


	// new center (mean) =    sum of distances  /  number of points
	for (int c = 0; c < k; c++)
	{

		//cout << "Medoid for C" << c << ": \n";

		//hash map for better complexity ::  cache distances (i,j)  of every i,j in cluster c
		unordered_map<pair<int, int>, double, HASH> map_distances;

		min_sum_dist = 9999990.0;

		for (int i = 0; i < dataset.size(); i++)
		{
			if (y_clusters[i] != c)
				continue;

			//cout << " distances from: dp_" << i + 1 << "\n";
			sum_distances_from_i = 0.0;

			for (int j = 0; j < dataset.size(); j++)
			{
				if (y_clusters[j] != c || j == i)
					continue;


				//         dp1  =  smallest of i,j           dp2  = biggest of i,j
				if (i <= j)
				{
					dp1 = i;
					dp2 = j;
				}
				else
				{
					dp2 = i;
					dp1 = j;
				}

				// search hash_map
				auto cached = map_distances.find(make_pair(dp1, dp2));

				// distance (i,j)  not found

				if (cached == map_distances.end())
				{
					d = dataset[i].dist(dataset[j]);
					//cout << "\t++ D( " << setw(2) << dp1 + 1 << " , " << setw(2) << dp2 + 1 << " ) = " << setw(3) << d << endl;
					map_distances[make_pair(dp1, dp2)] = d;
				}
					// distance (i,j) found !
				else
				{
					d = cached->second;
					//cout << "\t   D( " << setw(2) << dp1 + 1 << " , " << setw(2) << dp2 + 1 << " ) = " << setw(3) << d << endl;
				}
				sum_distances_from_i += d;
			}


			if (sum_distances_from_i < min_sum_dist)
			{
				min_sum_dist = sum_distances_from_i;
				best_medoid = i;
			}

		}


//		cout << "best medoid = dp_" << best_medoid + 1
//		     << "           MMMMMMMMMMMMMM\n";
		new_centers.push_back(dataset[best_medoid].get_coordinates());
		medoid_centers.push_back(best_medoid);

	}

	return new_centers;
}
