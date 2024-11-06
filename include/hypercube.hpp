#ifndef __hypercube__hpp__
#define __hypercube__hpp__

#include <stdio.h>
#include <armadillo>
#include <vector>




// template class for Hypercube
template<int D, int L>
class Hypercube
{
    public:
    // Use Armadillo objects based on dimension
    arma::Mat<double> dim_2; // For D = 2
    arma::Cube<double> dim_3;  // For D = 3
    arma::field<arma::Cube<double>> dim_4; // For D = 4
    arma::field<arma::field<arma::Cube<double>>> dim_5; // For D = 5
    arma::field<arma::field<arma::field<arma::Cube<double>>>> dim_6; // For D = 6

    // public:
    // constructor
    Hypercube();

    // deconstructor
    ~Hypercube();

    //


};




#endif /*__hypercube__cpp__*/