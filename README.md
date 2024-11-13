# Numerical Implementation of the Spherical Model
This repository contains the codes developed for the bachelor's thesis of Marco Fava at the University of Trento, under the supervision of Professor Stefano Giorgini.  

This thesis investigates the numerical implementation of the spherical model using Monte Carlo simulations, focusing on the efficiency of different Metropolis algorithms in achieving thermalisation and exploring equilibrium states.  
The work compares standard Metropolis methods with adaptive approaches, particularly a mixed $\theta$-strategy, to evaluate their impact on convergence speed and accuracy. Key thermodynamic quantities, such as internal energy, magnetisation, specific heat, and susceptibility, are analysed across different system sizes and dimensions.  
The results demonstrate that adaptive methods, especially the mixed $\theta$-method, significantly enhance
convergence efficiency compared to conventional approaches, enabling faster sampling of equilibrium states. The thermodynamic properties obtained from the simulations align closely with theoretical expectations, though minor deviations due to finite-size effects are observed.  
This work provides a strong foundation for further studies on the spherical model, suggesting potential directions for future research into critical exponents, autocorrelation functions, and the model’s behaviour under varying external magnetic fields.  

The repository includes the [`include`](include/) folder, which contains the header files with the extension `.hpp`, the [`src`](src/) folder containing source files with the extension `.cpp`, and the [**plots**](plots/) folder containing Python and MATLAB files with extensions `.py` and `.m`, respectively.