// In this file all the functions, declarated in System.hpp header, are defined


#include "system.hpp"

using namespace std;

int try_num;     // Debug
int out_num;     // Output number

// ------------ Define the constructor System ------------
System::System(int dim_in, int L_in, double T_in, double J_in, double H_in, double theta_in) : 
    dim_(dim_in), L_(L_in), N_(pow(L_in,dim_in)), N_sqrt_(sqrt(N_)), 
    T_(T_in), Beta_(1/T_in), Beta_2(Beta_*Beta_), theta_(theta_in), J_(J_in), H_(H_in)
{
    // set the random generator's seed:
    // https://arma.sourceforge.net/docs.html#rng_seed
    // arma::arma_rng::set_seed_random();       //done outside class because of threads dangerous

    // initialise the system
    initialise();
    E_ = tot_E(spin_vec_);
    // update();
    // compute_all();

    // initialise the metropolis variable
    // theta_ = 0.1;
}




// ------------ Define the deconstructor of System ------------
//this function is used to close all the opened stream files and 
//clear/reset all the elemets to store values once the class is out of scope
System::~System()
{
    ofile.close();
    interaction_.reset();
    spin_vec_.reset();
    track_E_.clear();
    track_M_.clear();
}




// ------------ Define the function open_file ------------
//this function is used to open the file stream with path "file_path" and name "filename"
//this function also makes sure that the given file name and path is not already existing causing the old file to be overwritten
//if the given file is already existing asks if the user wants to overwrite or to enter a different file name
void System::open_file(string file_path, string filename)
{
    // if there is no filename, write the default filename:
    if (filename == "") {
        // format the T value with a specific number of decimal places (2) to set the file name
        stringstream ss;
        ss << fixed << setprecision(2) << T_;
        string formattedT = ss.str();

        // write the default filename
        filename = file_path + "data_d" + to_string(dim_) + "_L" + to_string(L_) + "_T" + formattedT + "_" + to_string(out_num) + ".txt";
    } else {
        filename = file_path + filename + ".txt";
    }

    
    // check if the file exists
    while (true) {
        std::ifstream infile(filename);
        
        if(infile.good()){
            std::string user_input;
            std::cout << "File \"" << filename << "\" already exists. Do you want to overwrite it? (y/n): ";
            std::cin >> user_input;
            
            if (user_input == "y" || user_input == "Y") {
                break; // user wants to overwrite, exit the loop and proceed
            } else if (user_input == "n" || user_input == "N") {
                std::cout << "Do you want to specify a new filename? (y/n): ";
                std::cin >> user_input;

                if (user_input == "y" || user_input == "Y") {
                    std::cout << "Please enter a new filename (without path and extension, \".txt\" by default): ";
                    std::string new_filename;
                    std::cin >> new_filename;
                    filename = file_path + new_filename + ".txt";
                    infile.open(filename); // check the new filename
                } else {
                    std::cout << "Operation aborted. Exiting program." << std::endl;
                    std::exit(0);  // exit the program with status code 0
                }
            } else {
                std::cout << "Operation aborted. Exiting program." << std::endl;
                std::exit(0);  // exit the program with status code 0
            }
        } else {
            break;      // File does not exist, proceed with this filename
        }
    }

    // Open the file in write mode
    ofile.open(filename);

    if (!ofile) {
        cerr << "Error opening file for writing: " << filename << endl;
        std::exit(0);  // exit the program with status code 0
    } else {
        cout << "Writing in this file: " << filename << endl;
    }
}




// ------------ Define the function initialise ------------
//this function is used to initialise the hyperlattice of the system and the interaction matrix
void System::initialise()
{
    // __________ let's first initialise the interaction matrix: __________
    // set the size to a matrix NxN
    interaction_.set_size(N_,N_);

    // per each dimension the only elements of interaction are those for which the index is 
    // L^(d-1) and N-L^(d-1)
    // write the final cyclic matrix of interaction 
    int L_d;
    for (int d = 0; d < dim_; d++) {
        L_d = pow(L_, d);
        for (int i = 0; i < N_; i++) {
            int j1 = (i + L_d) % N_;
            int j2 = (i - L_d + N_) % N_;
            interaction_(i, j1) = 1.;
            interaction_(i, j2) = 1.;
        }
    }

    // we already moltiply the interaction matrix for J/2 (the 1/2 factor is to avoid double counting)
    interaction_ *= 0.5 * J_;

    
    // // write the values for the first row
    // // per each dimension the only elements of interaction are those for which the index is 
    // // L^(d-1) and N-L^(d-1)
    // int L_d;
    // for( int d=0; d<dim_; d++){
    //     L_d = pow(L_,d);
    //     interaction_(0,L_d) = 1;
    //     interaction_(0,N_-L_d) = 1;
    // }

    // // write the final cyclic matrix of interaction 
    // // (to avoid double counting we are going to transform it in a tridiagonal matrix)
    // // (elements on the diagonal will be always zero so start form i,j=1)
    // for(int i=1; i<N_; i++){
    //     for(int j=1; j<N_-i; j++){

    //         interaction_(i,i+j) = interaction_(0,j);
    //     }
    // }
    // for(int i=1; i<N_; i++){
    //     for(int j=0; j<N_; j++){

    //         interaction_(i,(i+j)%N_) = interaction_(0,j);
    //     }
    // }
    


    // __________ initialise the system __________
    // initialise the system to a vector of N=L^d random values from a uniform distribution in [-0.5,0.5] (it'll be later normalised)
    spin_vec_ = arma::randu(N_) - 0.5;
    
    // normalise the system to have that sum(s_i^2) = N
    normalise(spin_vec_);
}




// ------------ Define the function normalise ------------
//this function normalises the system so that sum{s_i^2}=N
//more precisely transforms the given vector to the same vector normalised so it has: |norm(vector)|^2 = N
void System::normalise(arma::vec &vector){
    vector *= N_sqrt_ / arma::norm(vector,2);
}




// ------------ Define the function tot_E ------------
//this function computes the total energy of the given spin vector
//computed as -K*sum{s_i*s_j} - h*sum{s_i}, K=J/kT and h=H/kT (so it's a unitless energy)
//the first sum is over the closest neighbours without double counting
double System::tot_E(arma::vec &spin_vec_in)
{
    double int_energy1 = - arma::dot(spin_vec_in, interaction_ * spin_vec_in); // note that the interaction matrix is already moltiplied by K/2
    double mag_energy = - H_ * arma::accu(spin_vec_in);
    
    return int_energy1 + mag_energy;
}




// ------------ Define the function tot_M ------------
//this function computes the total magnetisation of the given spin vector 
//computed as the sum over all the spins in the system
double System::tot_M(arma::vec &spin_vec_in)
{
    return arma::accu(spin_vec_in);
}




// ------------ Define the function update ------------
//this function keeps track of the energy and magnetisation values
void System::update()
{
    track_E_.push_back(E_);             //total energy
    M_ = tot_M(spin_vec_);              //computed here, and not inside metropolis() because it's faster
    track_M_.push_back(abs(M_));        //total magnetisation
}




// ------------ Define the function compute_all ------------
//this function computes the values of all parameters of the system
void System::compute_all()
{
    e_ = exp_value(track_E_,1)/N_;                          //average energy (normalised to the number of spins)
    // e_ = arma::mean(track_E);
    m_ = exp_value(track_M_,1)/N_;                          //average magnetisation (normalised to the number of spins)
    // m_ = arma::mean(track_M);
    cv_ = (exp_value(track_E_,2)/N_ - e_*e_*N_)*Beta_2;        //specific heat capacity (normalised to the number of spins)
    // cv_ = arma::var(track_E_)*Beta_2_N;                         //Beta^2 Var(E) / N
    chi_ = (exp_value(track_M_,2)/N_ - m_*m_*N_)*Beta_;        //susceptibility (normalised to the number of spins)
    // chi_ = arma::var(track_M_)*Beta_N;                          //Beta Var(M) / N
}




// ------------ Define the function exp_value ------------
//this function computes the expected value of the values inside the vector: <values>
//with this function we can compute <x> if power=1 or we can compute <x^2> if power=2; for our purpose no other expectation values are needed
double System::exp_value(const std::vector<double>& values, int power)
{
    // check that the vector is not empty
    if (values.empty()) {
        std::cerr << "Error: Cannot calculate the expected value for an empty vector." << std::endl;
        return 0.0;
    }

    double sum = 0.0;

    //for our computations we will need only <x> and <x^2>
    if(power == 1){
        for(double val : values){
            sum += val;
        }
    }else if(power == 2){
        for(double val : values){
            sum += val*val;
        }
    }

    //return <values^power>
    return sum / values.size();
}




// ------------ Define the function metropolis ------------
//this function evolves the system by doing one cycle of Markov Chain Monte Carlo
void System::metropolis()
{

    // if(try_num == 0){
        
    //     int i;                      // Position of the candidate spin to flip
    //     double tral_s;
    //     double trial_E;              // new spin proposal
    //     double deltaE;
    //     double r;                   // random number between 0 and 1 to accept or not the candidate

    //     arma::vec trial_spin_vec;   // trial spin vector


    //     for(int k=0; k<N_; k++) // loop over the number of spins
    //     {
    //         // find the position of a random spin to change
    //         i = arma::randi(arma::distr_param(0,N_-1));

    //         // find a proposal value for the random spin
    //         tral_s = (2 * arma::randu() - 1) * N_sqrt_;
    //         trial_spin_vec = spin_vec_;
    //         trial_spin_vec(i) = tral_s;
    //         normalise(trial_spin_vec);

    //         // compute the difference of energy deltaE
    //         trial_E = tot_E(trial_spin_vec);
    //         deltaE = trial_E - E_;

    //         // generate a random number r distributed in U(0,1)
    //         r = arma::randu();

    //         // if(/*r < prob(s')/prob(s) = e^(-deltaE/T)*/){
    //             // change the spin
    //             // update energy and magnetisation
    //         // }

    //         if(r < exp(-deltaE*Beta_)){
    //             E_ = trial_E;
    //             // M_ = tot_M(spin_vec_);   // this will be done in the function update() cause it's useless to do it here
    //             spin_vec_ = trial_spin_vec;
    //         }

    //         // cout << "deltaE=" << deltaE << "    r=" << r << "   E=" << track_E_.back() << endl;
    //     }
    // }else if(try_num == 1){
    for(int k=0; k<N_; k++) // loop over the number of spins
    {
        arma::vec trial_spin_vec;   // trial spin vector
        // double theta_ = 0.1;         // define a small rotation angle theta (in radians)
        double trial_E;             // energy of trial system
        double deltaE;
        double r;                   // random number between 0 and 1 to accept or not the candidate

        // generate a random orthogonal vector u
        arma::vec u = arma::randn<arma::vec>(N_);   // use randn() or randu() ???
        u -= arma::dot(u, spin_vec_) / N_ * spin_vec_; // arma::dot(spin_vec_, spin_vec_) == N_
        normalise(u);  // normalize u to have the same norm as spin_vec_

        // Calculate the rotated vector B
        trial_spin_vec = cos(theta_) * spin_vec_ + sin(theta_) * u;

        // compute the difference of energy deltaE
        trial_E = tot_E(trial_spin_vec);
        deltaE = trial_E - E_;

        // generate a random number r distributed in U(0,1)
        r = arma::randu();

        /* if(r < prob(s')/prob(s) = e^(-deltaE/T)){
                change the spin
                update energy and magnetisation
            }*/

        if(r < exp(-deltaE*Beta_)){
            E_ = trial_E;
            // M_ = tot_M(spin_vec_);   // this will be done in the function update() cause it's useless to do it here
            spin_vec_ = trial_spin_vec;
        }

        // cout << "deltaE=" << deltaE << "    r=" << r << "   E=" << E_ << endl;
    }
    // }

}




// ------------ Define the function export_data ------------
//this function exports all the data on the files previously opened
void System::export_data()
{
    // store the computed values
    static bool is_first_write = true;

    if(is_first_write){
        // open stream file
        std::string file_path = "data/";
        std::string filename = "";                                       // if left like "", then the default filename will be used
        // std::string filename = "try_num_" + to_string(try_num);          // if left like "", then the default filename will be used
        open_file(file_path,filename);

        ofile   << "## This system has the following parameters:"
                << "    |dim = " << dim_
                << "    , L =  "  << L_
                << "    , T =  "  << T_ 
                << "|          "
                << endl
                << "## The values are stored with the order: e, m, cv, chi."
                << endl;
        is_first_write = false;
    }
    ofile   << setw(width_) << setprecision(prec_) << scientific << e_ 
            << setw(width_) << setprecision(prec_) << scientific << m_
            << setw(width_) << setprecision(prec_) << scientific << cv_
            << setw(width_) << setprecision(prec_) << scientific << chi_
            << endl;
}




// ------------ Define the function print_sp_matrix_structure ------------
// This function prints the structure of a sparse matrix to screen (arma::SpMat<int>)
// It is useful to visualise the matricies during the coding process
void System::print_sp_matrix_structure() //(const arma::SpMat<double>& A)
{
    using namespace arma;
    arma::SpMat<double> A = interaction_;

    // Declare a C-style 2D array of strings.
    string S[A.n_rows][A.n_cols];

    // Initialise all the strings to " ".
    for (int i = 0; i < A.n_rows; i++)
    {
        for (int j = 0; j < A.n_cols; j++)
        {
            S[i][j] = " ";
        }
    }

    // Next, we want to set the string to a dot at each non-zero element.
    // To do this, we use the special loop iterator from the sp_mat class
    // to help us loop over only the non-zero matrix elements.
    SpMat<double>::const_iterator it = A.begin();
    SpMat<double>::const_iterator it_end = A.end();

    int nnz = 0;
    for (it; it != it_end; ++it)
    {
        S[it.row()][it.col()] = "•";
        nnz++;
    }

    // Finally, print the matrix to screen.
    // cout << endl;
    // cout << "    ";
    // for (int i = 0; i < A.n_cols; i++)
    // {
        // cout << i+1 << " ";             // it'd be better to format
    // }
    cout << endl;
    for (int i = 0; i < A.n_rows; i++)
    {
        // cout << i+1 << " ";         // it'd be better to format
        cout << "| ";
        for (int j = 0; j < A.n_cols; j++)
        {
            cout << S[i][j] << " ";
        }
        cout << "|\n";
    }

    cout << endl;
    cout << "matrix size: " << A.n_rows << "x" << A.n_cols << endl;
    cout << "non-zero elements: " << nnz << endl;
    cout << endl;
}

