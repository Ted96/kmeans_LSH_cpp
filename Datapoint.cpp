#include "Datapoint.h"


extern char metric;

Datapoint::Datapoint(const vector<double> &coordinates, string id) : coordinates(coordinates), id(id) {}

void Datapoint::print() {

    cout << "@" << id << "<" << coordinates[0] << "," << coordinates[1] << ">";

}

double Datapoint::dist(vector<double> &d2) {

    double d = 0;

     if (metric == 'c') {


        double norm_d2 = 0, norm_d1 = 0, product = 0;

        for (int i = 0; i < coordinates.size(); i++)
        {
            norm_d1 += coordinates[i] * coordinates[i];
            norm_d2 += d2[i] * d2[i];

            product += coordinates[i]   * d2[i];
        }

        norm_d1 = sqrt(norm_d1);
        norm_d2 = sqrt(norm_d2);

        d = 1 - product / (norm_d1 * norm_d2) ;

    } else {

         for (int i = 0; i < coordinates.size(); i++)
             d += (coordinates[i] - d2[i]) * (coordinates[i] - d2[i]);

         d = sqrt(d);

     }
    //cout<<"D(dp,v)->" << d<<endl;
    return d;
}


double Datapoint::dist(Datapoint &d2) {


    double d = 0;

    if (metric == 'c') {


        double norm_d2 = 0, norm_d1 = 0, product = 0;

        for (int i = 0; i < coordinates.size(); i++)
        {
            norm_d1 += coordinates[i] * coordinates[i];
            norm_d2 += d2.coordinates[i] * d2.coordinates[i];

            product += coordinates[i]   * d2.coordinates[i];
        }

        norm_d1 = sqrt(norm_d1);
        norm_d2 = sqrt(norm_d2);

        d = 1 - product / (norm_d1 * norm_d2) ;

    }
    else
        {

        for (int i = 0; i < coordinates.size(); i++)
            d += (double) (coordinates[i] - d2.coordinates[i]) * (coordinates[i] - d2.coordinates[i]);

        d = sqrt(d);
    }

    //  cout<<"D(dp,v)->" << d<<endl;
    return d;
}


vector<double> Datapoint::get_coordinates() {
    return coordinates;
}

string Datapoint::getId() const {
    return id;
}
