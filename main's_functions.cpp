
#include <fstream>
#include "main's_functions.h"
#include "globals.h"

using namespace std;


void parse_config_file()
{

	ifstream in;

	string parameter, value;
	// open the input stream to read the key
	in.open(c_file.c_str());

	// check if the file was OK
	if (in)
		while (!in.eof())
		{

			in >> parameter;

			//cout<<" PARAMETER="<<parameter<<endl;
			if (parameter == "number_of_hash_tables:")
				in >> ::L;
			if (parameter == "number_of_hash_functions:")
				in >> ::k;
			if (parameter == "number_of_clusters:")
				in >> ::n_clusters;
			if (parameter == "max_kmeans_iterations:")
				in >> ::max_kmeans_iterations;
			if (parameter == "check_Gs_in_lsh:")
				in >> ::check_g;

		}


	else
		cout<<" problem reading config file:"<<c_file<<" (now using default settings)\n";
}

void parse_arguments(int argc, char **argv)
{

	//parse command line arguments using getopt()


	bool input_file_given = false;

	int c = 0;

	while ((c = getopt(argc, argv, "had:c:o:i:")) != -1)

		switch (c)
		{
			case 'a':
				complete = true;
				break;
			case 'c':
				c_file = optarg;
				break;
			case 'i':
				in_file = optarg;
				input_file_given = true;
				break;
			case 'o':
				out_file = optarg;
				break;
			case 'd':
				metric = static_cast<char>(optarg[0] + 32);
				if (metric != 'c' && metric != 'e')
				{
					cout << "!! no such metric. Use <Euclidean> or <Cosine>\nNow using: Euclidean \n";
					metric = 'e';
				}
				break;
			case 'h':
			default:
				cout << "RUN WITH ./cluster –i <input file> –c <config file> "
				        "-d <metric> -ο <output file> -a\n\t-a = -complete \n\t-d = Metric = 'E' or 'C' ";
				exit(-99);
		}

		if ( input_file_given == false){

			cout<< "plz do not run this program without giving the -i parameter...\nType input file name :";
			cin>> in_file;
		}

}


void parse_input_file()
{


	ifstream in;

	in.open(in_file);
	if (!in)
	{
		cout << "Oops. Cannot open " << in_file << endl;
		exit(-5);
	}

	//count /n in the file
	num_points = (int) (count(istreambuf_iterator<char>(in), istreambuf_iterator<char>(), '\n'));

	//rewind file
	in.clear();
	in.seekg(0);

	string line;
	getline(in, line);

	int count = 0;
	//read each character and count spaces.     (!) theres one extra at the end. ??
	for (char c : line)
		if (c == ' ' || c == '\t' || c == ',')
			count++;


	dimension = count;

	cout << "vector_dimension=" << dimension << endl;
	in.clear();
	in.seekg(0);

}


vector<Datapoint> create_dataset(int num_points)
{


	ifstream in;
	in.open(in_file);
	if (!in)
	{
		cout << "Cannot open " << in_file << endl;
		exit(-5);
	}

	char line[5000];                //temp <- getline()
	string id;                      //temp <- id
	vector<double> point;           //temp <- datapoint
	int vectors_read_so_far = 0;    //counter
	char *token;                    //get every word separated by space or comma


	vector<Datapoint> dataset;


	if (in.eof() || !in.is_open())
		cout << "\n\n[problem\n\n";


	while (in.getline(line, 5000))
	{

		if (0 || vectors_read_so_far < num_points)
		{  // debuging  (read num_points)

			//get id = string
			token = strtok(line, " ,\t");
			id = token;
			//cout << "+id_" << id << endl;

			//get 1st coordinate
			token = strtok(NULL, " ,\t");

			//get other coordinates
			while (token != NULL)
			{
				point.push_back(atof(token));
				token = strtok(NULL, " ,\t");
			}

			//make a datapoint d  and save it to a list
			Datapoint d(point, id);
			dataset.push_back(d);
			point.clear();
		}

		vectors_read_so_far++;
	}

	cout << "\n read " << dataset.size() << " vectors.\n";
	in.clear();
	in.close();


	return dataset;


}

