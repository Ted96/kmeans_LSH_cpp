#ifndef PRJ_HASH_H
#define PRJ_HASH_H


#include <vector>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <cmath>
#include <array>
#include <set>


#include "Datapoint.h"


using namespace std;

set<Datapoint *> aaaaaaaa(Datapoint &query, double radius, vector<Datapoint> &whole_dataset, int mode);


class Hashtable_class
{

  private:
	//int size;
	int id;
	int num_buckets;
	int dimension;

	//hash random variables
	std::vector<int> vi;
	std::vector<int> ti;

	std::vector<vector<float>> v;
	std::vector<float> t;
	std::vector<int> r;
	int w;

	vector<vector<int> > *g_values;

  public:

	std::list<Datapoint *> *table;


	explicit Hashtable_class(int, int, vector<vector<float>>, vector<float>, vector<int>);
	~Hashtable_class();

	void insert(Datapoint *d);

	pair<int, vector<int>> hash_eucl(Datapoint &d);
	int hash_cos(Datapoint &d);
	int hash_eucl_hypercube(Datapoint &d);

	set<Datapoint *> search(Datapoint &q, double maxRange);


	void print_table();

	friend class Hypercube;
};


class H_family
{

  private:
	std::vector<vector<float>> v;
	std::vector<float> t;
	vector<int> r;

	int dimension;
	int L;

  public:

	vector<Hashtable_class *> hashtables;

	explicit H_family(int num_buckets, int L, int dimension);

	~H_family();


	set<Datapoint *> search(Datapoint &q, double maxRange);

	void insert(Datapoint *d);    //insert to all
	void insert(vector<Datapoint> &whole_dataset);    //insert whole dataset to all
};


/////////////////////////////////////// H Y P E R C U B E   ////////////////////////////////////////////////
class Hypercube
{

  private:
	std::vector<vector<float>> v;
	std::vector<float> t;
	vector<int> r;

	int dimension_cube;
	int dimension_vectors;
	int num_buckets;

	int M;
	int probes;


  public:
	explicit Hypercube(int dimension_vectors, int dimension_cube, int probes, int Max_points);
	~Hypercube();


	Hashtable_class *cube;

	set<Datapoint *> search(Datapoint &q, double maxRange);

	void insert(Datapoint *d);
	void insert(vector<Datapoint> &whole_dataset);

	void print();
};

#endif //PRJ_HASH_H
