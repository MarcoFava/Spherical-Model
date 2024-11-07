#include "system.hpp"


using namespace std;


vector<double> read_param(string file_in);



int main(int argc, char* argv[])
{


//checking the number of command-line arguments:
if((argc != 2) && (argc != 3)){
    // Get the name of the executable file
    string executable_name = argv[0];

    // Give instruction on how to use the inputs
    cerr << argc << " " << argv << endl;
    cerr << "Error: Wrong number of input arguments." << endl;
    cerr << "Usage: " << executable_name << " <string file name> (optional)<int test number>" << endl;

    // Exit program indicating a problem
    return 1;   
}

// Assign file name
string filename_in = argv[1];

// If given, assign trial number
if(argc == 3){
    try_num = atoi(argv[2]);    // try_num will be used to debug
}else{
    try_num = 0;                // try_num is an extern variable
}




/*################################ SAVE ALL PARAMETERS ################################*/
// the parameters are saved in this order: dim, L, T_i, T_f, J, H, n_T_steps, theta, n_MC, burnin, out_num.
vector<double> parameters = read_param(filename_in);
int dim = (int)parameters[0];           // dimension of the system
int L = (int)parameters[1];             // leght of the system (N = L^dim) 
double T_i = parameters[2];             // initial temperature of the simulation
double T_f = parameters[3];             // final temperature of the simulation
double J = parameters[4];             // final temperature of the simulation
double H = parameters[5];             // final temperature of the simulation
int n_T_steps = (int)parameters[6];     // number of temperature steps between T_i and T_f
double theta = parameters[7];           // metropolis' parameter, in radians
int n_MC = (int)parameters[8];          // number of Markov Chain Monte Carlo cycles
int burnin = n_MC * parameters[9];        // burn-in time
out_num = (int)parameters[10];           // extern parameter to change the name of the output file

if(burnin>=n_MC){
    cerr << "Error: The burn-in time is longer than the number of MCMC cycles." << endl;
    return 1;
}


// some more useful variables

// handle the simulation with a single temperature
double dT;
if(n_T_steps == 1){
    dT = 0;
}else{
    dT = (T_f-T_i)/(n_T_steps-1);
}

//variables to format the data
const int width = 12;
const int prec  = 4;
/*#####################################################################################*/



/*################################# OPEN OUTPUT FILE ##################################*/
// do it before the simulation because in case of encountering problems we don't waste time

//find a file name
string filepath = "data/";
string filename_out = filepath + "data_d" + to_string(dim) + "_L" + to_string(L) + "_" + to_string(out_num) + ".txt";

//check if the file is not already existing
ifstream infile(filename_out);
if(infile.good()){
    string user_input;
    cout << "File \"" << filename_out << "\" already exists. Do you want to overwrite it? (y/n): ";
    cin >> user_input;
    if((user_input != "y") && (user_input != "Y")){
        cout << "Exiting the program" << endl;
        return 0;
    }
}

//open the file stream
ofstream ofile;
ofile.open(filename_out);
if(!ofile){
    cerr << "Error opening file: " << filename_out << endl;
    return 1;
}else{
    cout << "Writing in this file: " << filename_out << endl;
}
/*#####################################################################################*/



/*################################ RUN THE SIMULATION #################################*/
//create a matrix for saving the results of every thread
arma::mat results = arma::mat(n_T_steps, 5); 

//LOOP OVER THE TEMPERATURE from T_i to T_f with a stepsize of dT
#pragma omp parallel for schedule(dynamic)
for(int i=0 ; i < n_T_steps ; i++)
{
    //update the temperature
    double T = T_i + i*dT;

    //initialise the system with temperature T
    System my_sys(dim, L, T, J, H, theta);

    #pragma omp critical
    {
    // check that everything is proceeding correctly
    cout    << "T=" 
            << setw(width) << setprecision(prec) << scientific << my_sys.T_ 
            << ", e=" 
            << setw(width) << setprecision(prec) << scientific << my_sys.e_ 
            << ", m=" 
            << setw(width) << setprecision(prec) << scientific << my_sys.m_ 
            << ", cv=" 
            << setw(width) << setprecision(prec) << scientific << my_sys.cv_
            << ", chi=" 
            << setw(width) << setprecision(prec) << scientific << my_sys.chi_
            << endl;
    }
    
    //run MCMC for n_MC cycles
    //start computing the data only after the burn-in time
    for(int j=0; j<burnin; j++){     
        // dynamically change the value of theta:
        double d_MC = double(j)/n_MC;
        my_sys.theta_ = theta*(1-d_MC)*(1-d_MC);//*(1-d_MC)*(1-d_MC);

        my_sys.metropolis();     //run algo for one cycle, one algo is N=L^dim steps
        my_sys.update();         //update energy and magnetisation after every cycle, only after the burn-in time 
        
        // comment/uncomment this part when using multiple/single temperature 
        // my_sys.compute_all();
        // my_sys.export_data();         //export the data
        // if((j+1) % (n_MC/20) == 0){
        //     cout << " Cycle number " << j+1 << " out of " << n_MC << ", theta = " << my_sys.theta_ << endl;
        // }

    }
    // my_sys.theta_ = 0.1;
    for(int j=burnin; j<n_MC; j++){
        // dynamically change the value of theta:
        // double d_MC = j/n_MC;
        // my_sys.theta_ = theta*(1-d_MC);//*(1-d_MC)*(1-d_MC);

        my_sys.metropolis();     //run algo for one cycle, one algo is N=L^dim steps
        my_sys.update();         //update energy and magnetisation after every cycle, only after the burn-in time 
        
        // comment/uncomment this part when using multiple/single temperature 
        // my_sys.compute_all();
        // my_sys.export_data();         //export the data
        // if((j+1) % (n_MC/20) == 0){
        //     cout << " Cycle number " << j+1 << " out of " << n_MC << ", <e> = " << my_sys.e_ << endl;
        // }

    }

    //compute all needed values at the end of all MCMC cycles
    my_sys.compute_all();

    //save the ordered data inside the matrix results
    results(i,0) = my_sys.T_;
    results(i,1) = my_sys.e_;
    results(i,2) = my_sys.m_;
    results(i,3) = abs(my_sys.cv_);
    results(i,4) = abs(my_sys.chi_);
}
/*#####################################################################################*/



/*################################### SAVE THE DATA ###################################*/
// write the first rows
ofile       << "## This simulation has been run with the following parameters:"
            << "    |dim = "    << dim
            << "    , L =  "    << L
            << "    , T_i =  "  << T_i
            << "    , T_f =  "  << T_f
            << "|"
            << endl
            << "## The values are stored with the order: e, m, cv, chi."
            << endl;


//save the data previously stored inside the matrix results
for(int i=0; i<n_T_steps; i++){
    ofile   << setw(width) << setprecision(prec) << scientific << results(i,0)
            << setw(width) << setprecision(prec) << scientific << results(i,1)
            << setw(width) << setprecision(prec) << scientific << results(i,2)
            << setw(width) << setprecision(prec) << scientific << results(i,3)
            << setw(width) << setprecision(prec) << scientific << results(i,4)
            << endl;
}
/*#####################################################################################*/


/*################################ PRINT FINAL MESSAGE ################################*/
cout   << "The program terminated after doing " << n_MC 
            << " MCMC cycles per "<<n_T_steps<<" temperature steps in a range [" 
            << T_i << "," << T_f << "] and the data was saved in the file " << filename_out 
            << endl;
/*#####################################################################################*/


return 0;
} /*END OF MAIN*/



    
/*################################ READ INPUT PARAMETERS ################################*/
vector<double> read_param(string file_in){
    // read the input parameters from an extern file
    // the parameters are saved in this order: dim, L, T_i, T_f, J, H, n_T_steps, theta, n_MC, burnin, out_num.

    // Vectors to store each parameter
    vector<double> parameters;

    // Create a filestream instance "myfile" and use it to read the file
    fstream myfile;
    myfile.open(file_in);
    if (myfile.is_open()) {

        // Some temporary variables
        string line;
        double value;

        // Read file line by line
        while (getline(myfile, line)) {
            
            // Skip lines starting with '#' and empty lines
            if ((line[0] == '#') || (line[0] == '\r') || (line[0] == '\n')){
                continue;
            }else if (line.empty()) {
                continue;
            }

            stringstream mysstream(line);
            mysstream >> value;
            parameters.push_back(value);
        }
    } else {
        cerr << "Unable to open the file " << file_in << endl;
    }

    // Close the input file
    myfile.close();

    if(parameters.size() != 11){
        cerr << "Wrong number of parameters inside file \""<<file_in<<"\"\n"
             << "Expected number: 11, saved in the following order: \n"
             << "dim, L, T_i, T_f, J, H, n_T_steps, theta, n_MC, burnin, out_num." << endl;
        exit(0);
    }else{
        cout << "\nParameters correctly read from input file: \""<<file_in<<"\". "
             << "Parameters saved in the following order: \n"
             << "dim, L, T_i, T_f, J, H, n_T_steps, theta, n_MC, burnin, out_num." << endl;

        for(int i=0; i<parameters.size(); i++){
            cout << parameters[i] << "  ";
        }
        cout << "\n" << endl;
    }

    return parameters;
}

