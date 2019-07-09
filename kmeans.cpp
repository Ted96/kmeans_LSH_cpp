
// sillhouete()
// run()  ,  run_all()
// contructor


#include "kmeans.h"


void kmeans_run_all_and_output(int n_clusters, vector<Datapoint> &dataset, string &output_file)
{


	kmeans * kmeans_instance;

//
//	kmeans_instance = new kmeans(n_clusters, dataset, 1, 2, 1);
//	kmeans_instance->run();
//	kmeans_instance->evaluate_and_output(output_file);
//	delete kmeans_instance;

	if ( 1)
	for (int init = 1; init <= 2; ++init)
	{

		for (int assign = 1; assign <= 3; ++assign)
		{

			for (int update = 1; update <= 2; ++update)
			{

					kmeans_instance = new kmeans(n_clusters, dataset, init, assign, update);
					kmeans_instance->run();
					kmeans_instance->evaluate_and_output(output_file);
					delete kmeans_instance;



			}

		}
	}


};


bool kmeans::break_conditions_met(vector<vector<double>> new_centers)
{

	vector<double> changes;
	std::transform(centers.begin(), centers.end(),
	               new_centers.begin(),
	               std::back_inserter(changes),
	               [](vector<double> c1, vector<double> c2) {
		               double dist = 0;
		               for (int i = 0; i < c1.size(); ++i)
		               {
			               dist += (c1[i] - c2[i]) * (c1[i] - c2[i]);
		               }
		               return sqrt(dist);       // !! sqrt()
	               });

	cout << " Update changes:: " << accumulate(changes.begin(), changes.end(), 0.0)<<endl;
//	for (int i = 0; i < changes.size(); i++)
//	{
//		if (changes[i] == 0.0)
//		{
//			printf(" CL_%d = 0 |", i);
//		}
//		else
//		{
//			printf(" CL_%d= %.4f |", i, changes[i]);
//		}
//	}
//	cout << "\n";

	if (accumulate(changes.begin(), changes.end(), 0.0) < 0.001)
	{
		cout << "\n\tBREAKING :: due to few cluster update() changes.\n";
		return true;
	}

	return false;
}


bool kmeans::break_conditions_met(int iteration)
{

	static int changes_previous_iteration = 1000;
	static int loop = 0;


	if (max_kmeans_iterations != -1 && iteration + 1 >= max_kmeans_iterations)
	{
		cout << "\n\tBREAKING :: due to max num iterations  @current_iter=" << iteration << "\n";
		changes_previous_iteration = 1000;
		loop = 0;
		return true;
	}

	if (changes_per_iteration < dataset.size() * 0.001)
	{
		cout << "\n\tBREAKING :: due to few points assign() changes. @current_iter=" << iteration << "\n";
		changes_previous_iteration = 1000;
		loop = 0;
		return true;
	}

	if (changes_per_iteration > changes_previous_iteration)
			loop++;


	if (loop >= 5 )
	{
		cout << "\n\tBREAKING :: due to loop detected @current_iter=" << iteration << "\n";
		changes_previous_iteration = 1000;
		loop = 0;
		return true;
	}

	changes_previous_iteration = changes_per_iteration;

	return false;
}


vector<double> kmeans::silhouette()
{


	double d,s, sum = 0.0, sum_distancesA, sum_distancesB, A, B;
	int cA, cB, countA, countB ,dp1 , dp2;

	vector<double> ss(k, 0);
	vector<int> count(k, 0);

	struct HASH
	{
		size_t operator()(const pair<int, int> &x) const
		{
			return hash<long long>()(((long long) x.first) ^ (((long long) x.second) << 32));
		}
	};

	unordered_map<pair<int, int>, double, HASH> map_distances;




	for (int cluster = 0; cluster < k; cluster++)
	{

		for (int i = 0; i < dataset.size(); ++i)
		{

			if (y_clusters[i] != cluster)
				continue;

			countA = 1;
			countB = 0;
			sum_distancesA = 0.0;
			sum_distancesB = 0.0;


			//current cluster
			cA = y_clusters[i];

			//next best cluster
			cB = find_2nd_best_cluster(dataset[i], centers, cA);


			for (int j = 0; j < dataset.size(); ++j)
			{

				if (j == i  ||  (y_clusters[j] != cA && y_clusters[j]!=cB)  )
					continue;

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

				auto cached = map_distances.find(make_pair(dp1, dp2));

				// distance (i,j)  not found
				if (cached == map_distances.end())
				{
					d = dataset[i].dist(dataset[j]); //calculate actual distance
					map_distances[make_pair(dp1, dp2)] = d;
				}
					// distance (i,j) found !
				else
					d = cached->second;


				if (y_clusters[j] == cA)
				{
					countA++;
					sum_distancesA += d;
				}
				else if (y_clusters[j] == cB)
				{
					countB++;
					sum_distancesB += d;
				}
				else
					cout<<"error on silhouette unkown cluster?!!!\n";

			}

        if ( countB == 0)
        	countB = countA;

			A = sum_distancesA / (double) countA;
			B = sum_distancesB / (double) countB;

			s = (B - A) / max(A, B);


			ss[cluster] += s;
			count[ cluster] ++;
			sum += s;

		}


	}

	for( int i=0; i<k; ++i)
		if ( count[i] != 0 )
			ss[i] /= (double)count[i];


	ss.push_back( sum / (double) dataset.size() );

	return ss;
}


void kmeans::evaluate_and_output(string &output_file_append)
{

	cout << "\n\n\t------  EVALUATING RESULTS:: \n";

	auto s = silhouette();
	int i;
	extern char metric;

	ofstream dout;
	dout.open(output_file_append, ofstream::app);

	cout << "\n\tS = " << s.back() << endl;
	dout << "Algorithm: ";
	dout << modes[0] << "x" << modes[1] << "x" << modes[2] << "x" << endl;
	dout << "Metric: ";
	if (metric == 'e')
		dout << "Euclidean" << endl;
	else
		dout << "Cosine" << endl;

	for (int c = 0; c < k; c++)
	{

		int size = 0;


		for (int d = 0; d < dataset.size(); ++d)
		{

			if (y_clusters[d] == c)
				size++;
		}

		dout << "CLUSTER-" << c + 1 << " {size: " << size << ", centroid: ";

		if (modes[2] == 2)
			dout << dataset[medoid_centers[c]].getId();
		else if(modes[2]== 1)
		{
			dout << "[";

			auto dd = centers[c];

			for ( i = 0; i < dd.size() - 1; i++)
				dout << dd[i] << ",";
			dout << dd[i] << "]";
		}

		dout<<"}\n";
	}

	dout<<"clustering_time: "<<timer<<endl;

	dout << "Silhouette: [";
	for (i = 0; i < s.size() - 1; i++)
		dout << s[i] << ",";
	dout << s[i] << "]\n";


	if ( complete )
		for (int c = 0; c < k; c++)
		{
			dout << "CLUSTER-" << c + 1 << " {";

			for (int d = 0; d < dataset.size(); ++d)
				if (y_clusters[d] == c)
					dout << dataset[d].getId()<<", ";

			dout<<"}\n";

		}

	dout<<"\n\n";
	dout.close();
}
















//////////                    general                          //////////////////////////////////


void kmeans::run()
{


	int iteration = 0;

	cout << "\n\t\t\t---- RUNING KMEANS ---\n\n";

	clock_t start = clock();

	centers = init();

	while ( true )
	{
		cout << "------------------ iteration: " << iteration << endl;

		y_clusters = assign();

		if (break_conditions_met(iteration))
			break;

		auto new_centers = update();

		if (break_conditions_met(new_centers))
			break;

		centers = new_centers;
		iteration++;
	}

	this->timer = (clock() - start) /  (double) CLOCKS_PER_SEC;

	cout << "\n\t\t\t----DONE RUNING KMEANS ---\n\n";
}






vector<vector<double>> kmeans::init()
{

	switch (modes[0])
	{
		case 1 :
			return init_random();
		default:
		case 2:
			return init_kmeans_plusplus();
	}
}

vector<int> kmeans::assign()
{


	switch (modes[1])
	{
		default:
		case 1 :
			return assign_loyd();
		case 2:
		case 3:
			return assign_lsh();
	}

}

vector<vector<double> > kmeans::update()
{


	switch (modes[2])
	{
		default:
		case 1 :
			return update_loyd();
		case 2:
			return update_medoids();
	}

}





// ---------------------------------  CONSTRUCTORS -------------------------------------------------


kmeans::kmeans(int k, vector<Datapoint> &dataset, int I_mode, int A_mode, int U_mode) : k(k), dataset(dataset)
{

	cout << "\n\nCREATED K-" << k << " -MEANS for " << dataset.size() << " datapoints\n";


	modes.push_back(I_mode);
	modes.push_back(A_mode);
	modes.push_back(U_mode);


	//centers.resize(k);
	y_clusters.reserve(dataset.size());


	vector<vector<double> > create_centers_2d_vector(k, vector<double>(dimension, 0.0));
	//centers = create_centers_2d_vector;


	//vector<int> fck_stl_initialization = {-1, 0};
	for (int i = 0; i < dataset.size(); ++i)
		y_clusters.push_back(-1);

}
