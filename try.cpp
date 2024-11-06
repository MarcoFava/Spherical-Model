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
        try_num = 0;
    }




    /*################################ SAVE ALL PARAMETERS ################################*/
    // the parameters are saved in this order: dim, L, T_i, T_f, n_T_steps, n_MC, burnin, out_num.
    vector<double> parameters = read_param(filename_in);
    int dim = (int)parameters[0];
    int L = (int)parameters[1];
    double T_i = parameters[2];
    double T_f = parameters[3];
    int n_T_steps = (int)parameters[4];
    int n_MC = (int)parameters[5];
    int burnin = (int)parameters[6];
    int out_num = (int)parameters[7];

    if(burnin>=n_MC){
        cerr << "Error: The burn-in time is longer then number of MCMC cycles." << endl;
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
        // double T = T_i + i*dT;
        double T = pow(10,-i);

        //initialise the system with temperature T
        System my_sys(dim,L,T);

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
            my_sys.metropolis();     //run algo for one cycle, one algo is N=L^dim steps
        }
        for(int j=burnin; j<n_MC; j++){
            my_sys.metropolis();     //run algo for one cycle, one algo is N=L^dim steps
            my_sys.update();         //update energy and magnetisation after every cycle, only after the burn-in time 
            
            // Calculate the percentage of completion
            double progress = 100.0 * (j - burnin + 1) / (n_MC - burnin);

            // Progress bar visualization
            #pragma omp critical
            {
                cout << "\r[Thread " <<  "] ";
                cout << "[";
                int barWidth = 50;
                int pos = barWidth * progress / 100.0;
                for (int k = 0; k < barWidth; ++k) {
                    if (k < pos) cout << "=";
                    else if (k == pos) cout << ">";
                    else cout << " ";
                }
                cout << "] " << int(progress) << " %   ";
                cout.flush();
            }
            // comment/uncomment this part when using multiple/single temperature 
            // my_sys.compute_all();
            // my_sys.export_data();         //export the data
            // cout << " Cycle number " << j+1 << " out of " << n_MC << ", <e> = " << my_sys.e_ << endl;

        }

        //compute all needed values at the end of all MCMC cycles
        my_sys.compute_all();

        //save the ordered data inside the matrix results
        results(i,0) = my_sys.T_;
        results(i,1) = my_sys.e_;
        results(i,2) = my_sys.m_;
        results(i,3) = my_sys.cv_;
        results(i,4) = my_sys.chi_;
    }
    /*#####################################################################################*/



    /*################################### SAVE THE DATA ###################################*/
    //write the first rows
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
}



    
/*################################ READ INPUT PARAMETERS ################################*/
vector<double> read_param(string file_in){
    // read the input parameters from an extern file
    // the parameters are saved in this order: dim, L, T_i, T_f, n_T_steps, n_MC, burnin, out_num.

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

    return parameters;
}

