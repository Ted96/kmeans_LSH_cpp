#ifndef PRJ_DATAPOINTS_H
#define PRJ_DATAPOINTS_H

#include <vector>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;


class Datapoint {

public:

    Datapoint(const vector<double> &coordinates, string id);
    void     print();
    double  dist(Datapoint &d2);
    double  dist(vector<double> &d2);

    string      getId() const;
    vector<double> get_coordinates();


private:

    vector <double> coordinates;
    string id;
};


#endif //PRJ_DATAPOINTS_H
