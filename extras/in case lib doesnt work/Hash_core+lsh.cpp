#include <utility>
#include <iomanip>
#include <list>
#include <bitset>


#include "Hash_core+lsh.h"
#include "distance_functions.h"

#include "globals.h"


using namespace std;

static int counterH = 0;


set<Datapoint *> aaaaaaaa(Datapoint &query, double radius, vector<Datapoint> &whole_dataset, int mode)
{

	static bool is_initialized_hf = false;
	static bool is_initialized_hc = false;


	static unique_ptr<H_family>     h_family(new H_family((int) (num_points * capacity_percent_lsh), L, dimension));
	static unique_ptr<Hypercube>    cube(    new Hypercube( dimension, log10(num_points) , 40 , 0));


	if (  is_initialized_hf  == false)
	{
		h_family->insert( whole_dataset);
		is_initialized_hf = true;
	}
	if (  is_initialized_hc  == false)
	{
		cube->insert( whole_dataset);
		is_initialized_hc = true;
	}


	if ( mode == 1)
		return h_family->search( query , radius );
	else if( mode == 2)
		return cube->search( query , radius);


};



pair<int, vector<int>> Hashtable_class::hash_eucl(Datapoint &d) {


    vector<double> p = d.get_coordinates();  // (cache) // call d.get_coordinates() 1 time.

    vector<int> g_values;

    double product = 0;

    // 1--- hash k times
    for (int i = 0; i < k; ++i) {

        if (0)
            cout << "H" << id << "/" << i
                 << "--about to hash ("
                 << " t?=" << t[i]
                 << " w?=" << ::w
                 << " v?=" <<
                 v[i][1] << "," << v[i][2] << "," << v[i][3] << ">\n";

        //dot product p*v
        for (int j = 0; j < v[i].size(); j++)
            product += p[j] * v[i][j];

        g_values.push_back((int) floor((product + t[i]) / ::w));    //bucc uniform

    }

    // 2--- concatenate hash values into a single one
    long long ri;
    long long sumOVFL = 0;
    int M = 2147483643;

    for (int i = 0; i < k; ++i) {

        ri = r[i] * g_values[i];

        sumOVFL +=   ri >= 0 ?   ri % M   :   (ri % M + M) % M;  //remainder operation. (positive modulo)
    }


    // 3 --- final single hash value.
    sumOVFL = sumOVFL % M;

//    printf(" G%d(%4s)= " ,this->id , d.getId().c_str());
//    for ( auto gi : g_values)
//        printf("%2d ",gi);
    //cout <<"   --> " << sumOVFL % num_buckets;



    // 4 - also store the g_values!!!!!!!!


    return make_pair((int) sumOVFL , g_values); //% num_buckets;
}



// -----------------------------------------------------------------------------------------------------------//
// -----------------------------------------   S E A R C H      H A S H    -----------------------------------//
// -----------------------------------------------------------------------------------------------------------//

set<Datapoint *> Hashtable_class::search(Datapoint &q, double maxRange) {

    set<Datapoint *> neighbours_set;

    double distance;
    double R = maxRange;   // range

    //cout << "searching hash for";
    //cout << endl;
    //cout<<"\nElements in buccet_"<< h << " :: ";


    int index;
    vector <int> g;
    if (metric == 'e')
    {
        auto hash_and_g = hash_eucl(q);

        index =  hash_and_g.first % num_buckets;
        g = hash_and_g.second;

    }
    else
    {
        index= hash_cos(q) % num_buckets;
    }

//    ofstream dout;
//    dout.open("debugLSH_g.txt", ofstream::app);
//    dout<<"\n ---- id= "<<id<<" ----"<<endl;
//    dout<<"G(query) = ";
//    for (auto gi: g )
//        dout<<setw(2)<< gi <<" ";
//    dout<<endl;

    int i=0;
    //cout<<"search lsh(): found candidates= "<<table[index].size()<<endl;
    for (auto &d : table[index] ) {   // for each element in bucket #  =  G(query)

        //continue if Gs dont match
//
        if( metric=='e' && check_g == true )
        	if(g != g_values[index][i] )
	        continue;


        distance = d->dist(q);

        if ( distance < R)    //  range search
        {
            //check g values!
//            if ( 1 && metric == 'e' && g == g_values[index][i])
//            {
//
//                dout << "+ dp_"<<setw(3)<<d->getId() <<" "<< "g=";
//                for (auto gi: g_values [index][i] )
//                    dout<<setw(2)<< gi <<" ";
//                dout<<endl;

                neighbours_set.insert(d);
            //}
        }

        i++;
    }

    return neighbours_set;
}


set<Datapoint *> H_family::search(Datapoint &q, double maxRange = 0) {

    set<Datapoint *> results_per_table;
    set<Datapoint *> final_results;


    for (int i = 0; i < this->L; i++) {

        results_per_table =  hashtables[i]->search(q, maxRange);
        final_results.insert(results_per_table.begin(), results_per_table.end());

    }

    return final_results;
}


////////////////////////   H A S H   C O S I N E     ///////////////////////////////////
int Hashtable_class::hash_cos(Datapoint &d) {

    vector<double> p = d.get_coordinates();  // cache     // call d.get_coordinates()  1 time.

    double product = 0;
    string h_values;

    //------------------------sphere projection --------------------------------
    for (int i = 0; i < k; ++i) {

        if (0)
            cout << "H" << id << "/" << i
                 << "--about to hash_cos ("
                 << "  , v?=" <<
                 v[i][1] << "," << v[i][2] << "," << v[i][3] << "," << v[i][4] << ">\n";

        //dot product p*v
        for (int j = 0; j < v[i].size(); j++)
            product += p[j] * v[i][j];

        //  concatenate (add a  '1' or '0' depending on product)
        h_values.push_back(('0' + (product > 0)));
    }

    return stoi(h_values, nullptr, 2);
}






// ---                                       EASY STUFF                          -----


// ----------------------------------   C O N S T R U C T O R S  ---------------------------------------

Hashtable_class::Hashtable_class(int num_buckets, int dimension, vector<vector<float>> family_vectors,
                                 vector<float> family_t, vector<int> r) {

    //size = 0;   //!unused

    this->r = std::move(r);
    this->dimension = dimension;
    this->num_buckets = num_buckets;
    this->table     = new std::list <  Datapoint *  >[num_buckets];
    this->g_values  = new std::vector<  vector<int> >[num_buckets];

    // pick k pairs from already random generated   vectors (R^d)  and   t (R)
    for (int i = 0; i < ::k; ++i) {

        int random_pair = random_gen<int>('u', 0, static_cast<int>(family_t.size() - 1));

        v.push_back(family_vectors[random_pair]);
        t.push_back(family_t[random_pair]);
    }

    id = counterH++;

    cout << "\tcreated h_table_" << id << "  /" << num_buckets << " buckets!" << endl;

}


H_family::H_family(int num_buckets, int L, int dimension) {

    this->L = L;
    this->dimension = dimension;          // !!dim


    int tablesize;

    if (metric == 'c') {
        tablesize = 1;
        for (int i = 0; i < ::k; ++i) {
            tablesize *= 2;
        }
    }
    if (metric == 'e') {
        tablesize = num_buckets;
    }

    cout << " creating H family (" << metric << ") size=" << tablesize << " buckets\n";

    // create ( L*k )  random  v  , t  , r   for each hash function H to choose from     //!!param
    for (int i = 0; i < ::k * ::L; ++i) {
        v.push_back(random_gen_vec(this->dimension));
        t.push_back(random_gen<float>('u', 0.0, ::w));
        r.push_back(random_gen<int>('u', -30, 30));     //80 120  = 40%
    }

    //create L hashtables
    for (int i = 0; i < L; ++i)
        this->hashtables.push_back(new Hashtable_class(tablesize, dimension, this->v, this->t, this->r));
}











// ----------------------------------   I N S E R T   ---------------------------------------

void H_family::insert(Datapoint *d) {

    for (auto table : hashtables)
        table->insert(d);
}


void H_family::insert(vector<Datapoint> &data) {

    for (Datapoint &d: data)
        this->insert(&d);
}


void Hashtable_class::insert(Datapoint *d) {
//inserts a pointer to a Datapoint class element (originally stored in stl:list @main.cpp )

    if (metric == 'e')
    {
        auto hash_and_g = hash_eucl(*d);
        int index = hash_and_g.first % num_buckets;

        table[index].push_back(d);
        g_values[index].push_back( hash_and_g.second);
        // !!g
    }
    else if (metric == 'c')
        table[hash_cos(*d) % num_buckets].push_back(d);

//    size++;
}


void Hashtable_class::print_table() {

// for each non empty buccet: print each item of chain-------------

    list<Datapoint *>::iterator elem;
    int count_points;

    cout << "\nH" << id << " -------------------------------------\n";
    for (int i = 0; i < num_buckets; ++i) {


        if (table[i].size() == 0)
            continue;
        cout << "\t{Bucc" << std::setw(4) << i << "} ";

        count_points = 0;
        for (elem = table[i].begin(); elem != table[i].end(); ++elem, ++count_points) {
            if (count_points % 1 == 0)    // print every ?? items
                cout << "*" << (*elem)->getId() << " ";
        }

        cout << endl;
    }
    cout << "H" << id << " ---------------------------------";



// count empty buckets as %  of total-------------------
    int count = 0;

    for (int i = 0; i < num_buckets; i++)
        if (table[i].empty())
            count++;

    cout << "H[" << id << "] (" << num_buckets << ")bucs ::" << std::setw(2)
         << 100 - (int) (((float) count / num_buckets) * 100) << "% full.\n";

}

// ----------------      destructors      --------------------

Hashtable_class::~Hashtable_class() {
    delete[] table;
    delete[] g_values;
}

H_family::~H_family() {
    for (int i = 0; i < L; ++i)
        //hashtables[i]-> ~Hashtable_class();     //nope
        delete (hashtables[i]);
}
