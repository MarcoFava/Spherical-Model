#ifndef __system_hpp__
#define __system_hpp__


#include <stdio.h>
#include <armadillo>
#include <cmath>
#include <random>
#include <iostream>
#include <iomanip> //to format the output data
// #include <eigen3/Eigen/Dense>
// #include <eigen-master/unsupported/Eigen/CXX11/Tensor>
// #include <libtorch/include/torch/csrc/api/include/torch/torch.h>

// #include <string>
#include <fstream>
#include <vector>


// let's debug :)
#define HEY std::cout<<"I am here: " << __FILE__<< ": " << __LINE__ << std::endl;
extern int try_num;
extern int out_num;



class System
{
    private:
        std::ofstream ofile;                    // output streamfile
        int width_ = 12, prec_ = 4;             // format the output
        arma::SpMat<double> interaction_;       // matrix of interactions

    public:
        int dim_, L_, N_;                       // dimension of the System, length of the tensor, N=L^dim total number of spins
        arma::vec spin_vec_;
        std::vector<double> track_E_, track_M_;
        double E_, M_, T_, Beta_;               // Energy, Magnetisation, Temperature, 1/Temperature
        double J_, H_;                          // Energy of interaction, Magnetic field
        double e_, m_, cv_, chi_;               // Energy/Number of spins, Magnetisation/Number of spins, Specific heat/Number of spins, Susceptivity/Number of spins
        double theta_;                          // Metropolis variable
        double N_sqrt_, Beta_2;


    // constructor
    System(int dim_in, int L_in, double T_in, double J_in, double H_in, double theta_in);

    // deconstructor
    ~System();

    // open the file given and checks that is not an already existing file being overwritten
    void open_file(std::string file_path, std::string filename);

    // initialise the system
    void initialise();

    // normalise the system so that sum{s_i^2}=N
    void normalise(arma::vec &vector);

    // compute the total energy of the given spin vector
    double tot_E(arma::vec &spin_vec_in);

    // compute the total magnetisation of the given spin vector
    double tot_M(arma::vec &spin_vec_in);

    // compute all the parameters of the System
    void compute_all();

    // expected value of the values inside the vector: <values^power>
    double exp_value(const std::vector<double>& values, int power);

    // updates the values inside track_E_ and track_M_
    void update();

    // evolve the System using MCMC
    void metropolis();

    // export data in files
    void export_data();

    // prints the structure of a sparse matrix to screen
    void print_sp_matrix_structure(); //(const arma::SpMat<double>& A)

    
};




#endif /*__system_hpp__*/