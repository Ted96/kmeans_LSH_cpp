#include <utility>
#include <iomanip>
#include <list>
#include <bitset>
#include "Hash_core+lsh.h"
#include "distance_functions.h"

using namespace std;

#include "globals.h"


              ////////////////////////////    hypercube   //////////////////////////



/////////////////////////////////////////// H Y P E R C U B E   S E A R C H   //////////////////////////////////
set<Datapoint *> Hypercube::search(Datapoint &q, double maxRange) {


    set<Datapoint *> neighbours;

    double minD = (metric == 'e') ? 99999 : 2;
    double distance;
    double R = maxRange;   // range

    //cout << "searching HYPERCUBE for "<<q.getId()<<endl;
    // cout<<"\nElements in buccet_"<<hash_value << " :: ";

    bool limit_reached = 0;

    int counter_points = 0, counter_probes = 0;

    int hash_value =    (metric == 'e')
            ? cube->hash_eucl_hypercube(q)
            : cube->hash_cos(q);

    int current_vertex = hash_value % num_buckets;
    //cout << "   hash(" << metric << ")=" << current_vertex << " =" << std::bitset<10>(current_vertex).to_string() << endl;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`
    //generate a series of vertices to check for neighbours.
    vector<int> vertices_to_check;
    int next_vertex, i;

    ///////////          hamming distance  = 1  ///////////
    for (i = 0; i < probes && i < dimension_cube; ++i) {

        next_vertex = (current_vertex ^ 1U << i);
       // cout << "\n\t " << std::bitset<10>(next_vertex).to_string() << " = " << next_vertex;
        vertices_to_check.push_back(next_vertex);
    }

    ///////////          hamming distance = 2   ////////////
    if (i < probes) {

        for (int j = 0; j < vertices_to_check.size(); ++j) {
            // cout<<"\n";
            for (int i = j; i < dimension_cube - 1; i++) {

                next_vertex = (vertices_to_check[j]) ^ 1U << i + 1;
                //cout << "\n\t*" << std::bitset<10>(next_vertex).to_string() << " = " << next_vertex;
                vertices_to_check.push_back(next_vertex);
            }
        }
    }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`


    while (!limit_reached) {

        for (auto &d : cube->table[current_vertex]) {   // for each element in vertex  #  =  G(query)

            //cout<<"checking buccet_ " << current_vertex<<endl;
            distance = d->dist(q);

            if (R == 0 && distance < minD) // nearest search
            {
                minD = distance;
                neighbours.clear();
                neighbours.insert(d);
                // cout << " new MIN_Dist ::" << neighbours[0]->getId();
            }

            if (R != 0 && distance < R)    // range search
                neighbours.insert(d);

            counter_points++;

            // break ON VECTORS SEARCHED
            if (M != 0)
                if (counter_points > M) {
                    limit_reached = true;
                    break;
                }

        }

        // break ON VERTICES EXPLORED
        if (counter_probes > probes || counter_probes >= vertices_to_check.size()) {
            limit_reached = true;
            break;
        }

        // next vertex to search  ( hamming distance  = 1)
        current_vertex = vertices_to_check[counter_probes];
        counter_probes++;
    }

    return neighbours;
}




////////////////////// hashing eucl for hyper cube/////////////////////////////


int Hashtable_class::hash_eucl_hypercube(Datapoint &d) {

    vector<double> p = d.get_coordinates();  // (cache) // call d.get_coordinates() 1 time.

    int h_value;

    double product = 0;

    string h_values;


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

        h_value = ((int) floor((product + t[i]) / ::w));

        //cout<< " cube hash_E = "<< h_value<<endl;

        // 2--- concatenate hash values into a single one
        h_values.push_back('0' + (h_value % 2 == 0));
    }


    return stoi(h_values, nullptr, 2);
}











//-------------------------------------------------------------------------------//
// ---                               EASY STUFF                          -----



Hypercube::Hypercube(int dimension_vectors, int dimension_cube , int probes , int max_points=0) :
        dimension_vectors(dimension_vectors) , probes(probes) , M(max_points) {


    this->dimension_cube = dimension_cube;
    this->num_buckets = 1;
    for (int i = 0; i < this->dimension_cube; ++i)
        this->num_buckets *= 2;


    // create N#  random  v  , t  , r   for each hash function H to choose from
    if (1 || ::metric == 'e') {
        for (int i = 0; i < 10 * this->dimension_cube; ++i) {    //!!bucc
            v.push_back(random_gen_vec(this->dimension_vectors));
            t.push_back(random_gen<double>('u', 0.0, ::w));
            r.push_back(random_gen<int>('u', 100, 150));
        }
    }

    //create 1 hashtable which will work as a cube.
    this->cube = new Hashtable_class(this->num_buckets, dimension_vectors, this->v, this->t, this->r);
    this->cube->id = -1;

    cout << "\tcreated hypercube 2^" << this->dimension_cube <<" = "<<num_buckets<< endl;
}



// ----------------------------------   I N S E R T   ---------------------------------------
void Hypercube::insert(Datapoint *d) {

    if (metric == 'e')
        cube->table[cube->hash_eucl_hypercube(*d) % num_buckets].push_back(d);
    else
        cube->table[cube->hash_cos(*d) % num_buckets].push_back(d);
}


void Hypercube::insert(vector<Datapoint> &data) {

    for(Datapoint & d:  data)
        this->insert( & d);
}



void Hypercube::print() {

    cout<<endl;
    cube->print_table();
}



// ----------------      destructor      --------------------

Hypercube::~Hypercube() {
    delete cube;
}

