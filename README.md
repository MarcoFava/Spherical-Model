# Numerical Implementation of the Spherical Model
This repository contains the codes developed for the bachelor's thesis of Marco Fava at the University of Trento, under the supervision of Professor Stefano Giorgini.  

This thesis investigates the numerical implementation of the spherical model using Monte Carlo simulations, focusing on the efficiency of different Metropolis algorithms in achieving thermalisation and exploring equilibrium states.  
The work compares standard Metropolis methods with adaptive approaches, particularly a mixed $\theta$-strategy, to evaluate their impact on convergence speed and accuracy. Key thermodynamic quantities, such as internal energy, magnetisation, specific heat, and susceptibility, are analysed across different system sizes and dimensions.  
The results demonstrate that adaptive methods, especially the mixed $\theta$-method, significantly enhance
convergence efficiency compared to conventional approaches, enabling faster sampling of equilibrium states. The thermodynamic properties obtained from the simulations align closely with theoretical expectations, though minor deviations due to finite-size effects are observed.  
This work provides a strong foundation for further studies on the spherical model, suggesting potential directions for future research into critical exponents, autocorrelation functions, and the model’s behaviour under varying external magnetic fields.  


## Structure  
The repository includes:  
- [`include`](include/): contains the header files (`.hpp`), where the *`System`* class is defined.  
- [`src`](src/): contains the source files (`.cpp`), which include the implementation of the *`System`* class and other functions.  
- [`input`](input_folder/): holds the input files required for running simulations.  
- [`plots`](plots/): contains Python scripts (`.py`) for generating various plots from the simulation data.  
- [`thesis`](thesis/): contains the PDF version of the thesis.  
- [`makefile`](makefile): includes all the rules necessary to compile and run the simulation.  

## Usage  

### Running the Simulation  
To run the simulation, modify the parameters in the [*input file*](input_folder/inputfile.txt) as described in the thesis. Then, execute the following command:  
```bash  
make all  
```  
If the executable `sph_mod.exe` already exists, you can run the simulation directly with:  
```bash  
make run  
```  

### Generating Plots  
To visualise the results, update the file paths in [`plots.py`](plots/plots.py) to match your data directory. Use the following functions to generate plots:  
- `plot_data_single_T()`: plots data as a function of Monte Carlo cycles for a fixed temperature.  
- `plot_data_multi_T()`: plots data as a function of temperature for multiple simulations.