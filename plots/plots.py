import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import math
from scipy.optimize import curve_fit

# x1 = [7**3, 10**3]
# y1 = [34*60, (60+46)*60]

# plt.plot(np.log(x1),np.log(y1))
# plt.scatter(np.log(x1),np.log(y1))
# # plt.loglog(x1,y1)
# plt.grid()
# plt.show()

# times = [3.32, 3.09, 3.115, 3.119, 3.497]
# av = np.mean(times)
# print(av)



# File path
path = '//home/marcofava/compphys_examples/Spherical-Model/data/'
path = '//wsl.localhost/Ubuntu/home/marcofava/compphys_examples/Spherical-Model/data/'
# path = '~/compphys_examples/Spherical-Model/data/'
extension = '.txt'

def filename(name_in):
     """
     arg: name_in
     returns filename = path + name_in + extension
     """
     return path + name_in + extension


### FUNCTION TO EXTRACT THE DATA
def extract_data(filename_in):
     """
     Function to read a file and extract the data from it
     
     Args:
          path_in: path where the file is located
          filename_in: name of the file
     
     Returns:
     --------
     dim : int
          dimension of the system
     L: int
          lenght of the hypercube of dimension dim
     T : float
          temperature of the system
     data: set of data. it contains 4 colums:     e (average energy per number of spins), 
                                                  m (average magnetisation per number of spins), 
                                                  cv (specific heat), 
                                                  chi (susceptivity).
     """
     # Initialize a list to store the data
     data = []

     # Open the file and process it
     with open(filename_in, 'r') as file:
          # Read all the lines
          lines = file.readlines()

          # Take the first line to extract the system parameters
          first_line = lines[0].strip()
          
          # Extract dim, L, T from the first line
          parts = first_line.split('|')[1].split(',')
          dim = int(parts[0].split('=')[1].strip())
          L = int(parts[1].split('=')[1].strip())
          T = float(parts[2].split('=')[1].strip())
          
          # Process the remaining lines
          data = [list(map(float,line.split())) for line in lines[2:]]
     return dim, L, T, data



def plot_data_single_T(filename_in, what_plot=[1,1,1,1]):
     """
     plots the data e, m, cv and chi stored in the given file
     """

     # Convert the list of lists to a numpy array for easier processing
     dim, L, T, data = extract_data(filename_in)
     data = np.array(data)
     e = data[:,0]
     m = data[:,1]
     cv = data[:,2]
     chi = data[:,3]

     # number of Montecarlo cycles
     x = [i+1 for i in range(np.size(e))]

     if what_plot[0]:
          plt.plot(x,e)
          plt.xlabel('$n_{MC}$')
          plt.ylabel('<e>')
          plt.grid()
          plt.show()

     if what_plot[1]:
          plt.plot(x,m)
          plt.xlabel('$n_{MC}$')
          plt.ylabel('<m>')
          plt.grid()
          plt.show()

     if what_plot[2]:
          plt.plot(x,cv)
          plt.xlabel('$n_{MC}$')
          plt.ylabel('$c_V$')
          plt.grid()
          plt.show()

     if what_plot[3]:
          plt.plot(x,chi)
          plt.xlabel('$n_{MC}$')
          plt.ylabel('$\chi$')
          plt.grid()
          plt.show()



def plot_data_multi_T(filename_in, what_plot=[1,1,1,1]):
     """
     plots the data T, e, m, cv and chi stored in the given file
     """

     # Convert the list of lists to a numpy array for easier processing
     dim, L, T, data = extract_data(filename_in)
     data = np.array(data)
     T = data[:,0]
     e = data[:,1]
     m = data[:,2]
     cv = abs(data[:,3])
     chi = abs(data[:,4])

     if what_plot[0]:
          plt.plot(T,e)
          plt.scatter(T,e)
          plt.xlabel('T')
          plt.ylabel('<e>')
          plt.grid()
          plt.show()

     if what_plot[1]:
          plt.plot(T,m)
          plt.scatter(T,m)
          plt.xlabel('T')
          plt.ylabel('<m>')
          plt.grid()
          plt.show()

     if what_plot[2]:
          plt.plot(T,cv)
          plt.scatter(T,cv)
          plt.xlabel('T')
          plt.ylabel('$c_V$')
          plt.grid()
          plt.show()

     if what_plot[3]:
          plt.plot(T,chi)
          plt.scatter(T,chi)
          plt.xlabel('T')
          plt.ylabel('$\chi$')
          plt.grid()
          plt.show()



what_plot = [0,0,1,0]
# plot_data_single_T(filename("data_d2_L5_T10.00_2"),what_plot)
# plot_data_single_T(filename("data_d2_L5_T1.00_3"),what_plot)
# plot_data_single_T(filename("data_d3_L3_T5.00_1"),what_plot)
# # # # # plot_data_multi_T(filename("data_d3_L3_1"),what_plot)
# plot_data_multi_T(filename("data_d3_L5_1"),what_plot)
# plot_data_multi_T(filename("data_d3_L5_2"),what_plot)

# plot_data_single_T(filename("save_data/data_d3_L3_T1.00_10"),what_plot)
# plot_data_multi_T(filename("data_d3_L8_0"))#,what_plot)
# plot_data_multi_T(filename("data_d3_L3_2"),what_plot)
# plot_data_multi_T(filename("data_d3_L3_1"),what_plot)

a = [0,0,0,0]
# a[0] = 1
# a[1] = 1
# a[2] = 1
# a[3] = 1
# # plot_data_single_T(filename("data_d3_L5_T1.00_10"),a) # theta=0.1, 3'02''
# plot_data_single_T(filename("data_d3_L5_T1.00_11"),a) # theta=0.05, 3'00''
# plot_data_single_T(filename("data_d3_L5_T1.00_12"),a) # theta=0.4, 3'10''
# plot_data_single_T(filename("qdata_d3_L5_T1.00_13"),a) # adapt_theta=0.4, MC=100k->50%, 4'56''
# # plot_data_single_T(filename("data_d3_L5_T1.00_14"),a) # adapt_theta=0.4, MC=30k->50%, 0'49''
# plot_data_single_T(filename("data_d3_L5_T1.00_15"),a) # adapt_theta=0.4, MC=20k->50%, compute after burnin, 0'34''
# plot_data_single_T(filename("data_d3_L5_T1.00_16"),a) # adapt_theta=0.4, MC=10k->50%, compute after burnin, 0'19''
# plot_data_single_T(filename("data_d3_L5_T1.00_17"),a) # adapt_theta=0.4, MC=8k->50%, compute after burnin, 0'16''
# plot_data_single_T(filename("data_d3_L5_T1.00_18"),a) # adapt_theta=0.4, MC=30k->50%, compute after burnin, 0'55''
# plot_data_single_T(filename("data_d3_L5_T1.00_19"),a) # ALL_adapt_theta=0.4, MC=30k->50%, compute after burnin, 1'05''
# plot_data_single_T(filename("data_d3_L5_T1.00_20"))#,a) # adapt_theta=0.4, MC=30k->50%+20k, compute after burnin, 1'05''
# # plot_data_single_T(filename("data_d3_L5_T1.00_21"),a) # adapt_theta=0.4, MC=30k->50%+70k, compute after burnin, 1'05''

# plot_data_single_T(filename("important/data_d3_L5_T1.00_0"),a) # 1'57''


b = [0,0,0,0]
# b[0] = 1
# b[1] = 1
# b[2] = 1
# b[3] = 1
# plot_data_multi_T(filename("data_d3_L5_30"),b) # 5^3, T: [0.1,7]/8, MC:30k, 5'56''
# plot_data_multi_T(filename("data_d3_L5_31"),b) # 5^3, T: [0.1,1]/8, MC:20k, 1'50''
# plot_data_multi_T(filename("data_d3_L5_32"),b) # 5^3, T: [0.2,10]/28, MC:20k, 6'06''
# plot_data_multi_T(filename("data_d5_L3_33"),b) # 3^5, T: [1,10]/8, MC:20k, 5'48''
# plot_data_multi_T(filename("data_d5_L3_34"),b) # 3^5, T: [7,10]/8, MC:20k, 9'01''
# plot_data_multi_T(filename("data_d3_L10_35"),b) # 10^3, T: [3.5,5.5]/12, MC:20k, 
# plot_data_multi_T(filename("data_d4_L4_40"),b) # 4^4, T: [1,8]/12, MC:20k, 8'48''
# plot_data_multi_T(filename("data_d4_L4_41"),b) # 4^4, T: [5,6.5]/8, MC:20k,



## DO SOME FINAL PLOTS
save = 0
ciccio = [0,0,0,0,0]
# ciccio[0] = 1
# ciccio[1] = 1
# ciccio[2] = 1
# ciccio[3] = 1
ciccio[4] = 1

# ENERGY PLOTS
if ciccio[0]:
     if save:
          plt.figure(figsize=(6,4), dpi=300)

     # data basic algo
     dim, L, T, data = extract_data(filename("important/data_d3_L5_T1.00_0"))
     data = np.array(data)
     e = data[:,0]
     m = data[:,1]
     cv = data[:,2]
     chi = data[:,3]
     x = [i+1 for i in range(np.size(e))]
     plt.plot(x,e, label="method 0")

     # data basic adaptive
     dim, L, T, data = extract_data(filename("data_d3_L5_T1.00_10"))
     data = np.array(data)
     e = data[:,0]
     m = data[:,1]
     cv = data[:,2]
     chi = data[:,3]
     x = [i+1 for i in range(np.size(e))]
     plt.plot(x,e, label=r"$\theta=0.1$rad")

     # data ultra adaptive
     dim, L, T, data = extract_data(filename("data_d3_L5_T1.00_13"))
     data = np.array(data)
     e = data[:,0]
     m = data[:,1]
     cv = data[:,2]
     chi = data[:,3]
     x = [i+1 for i in range(np.size(e))]
     plt.plot(x,e, label=r"adaptive $\theta$")

     # data ultra adaptive
     dim, L, T, data = extract_data(filename("data_d3_L5_T1.00_21"))
     data = np.array(data)
     e = data[:,0]
     m = data[:,1]
     cv = data[:,2]
     chi = data[:,3]
     x = [i+1 for i in range(np.size(e))]
     plt.plot(x[15000:],e[15000:], label="mixed adaptive metropolis")





     plt.ylim(-2.51,-2.4)
     plt.xlim(0,1e5)
     plt.title(r"Average normalised energy as a function of MC cycles")
     plt.xlabel(r"$n_{MC}$")
     plt.ylabel(r"$<e> [\mathbf{J}]$")
     plt.legend()
     plt.grid()
     if save:
          plt.savefig('C:/Users/ADMIN/Desktop/TESI/images/energy_plot.pdf', dpi=300, bbox_inches='tight')
     else:
          plt.show()


# CV PLOTS OVER TEMPERATURE
# 4.3 , 6.5 , 8.68
if ciccio[1]:
     if save:
          plt.figure(figsize=(6,4), dpi=300)

     plt.hlines(0.5,0,8,'k', linestyle='-.', linewidth=1.5)

     dim, L, T, data = extract_data(filename("data_d3_L5_32"))
     data = np.array(data)
     T = data[:,0]
     cv = abs(data[:,3])
     plt.plot(T,cv,label="d=3, L=5")
     plt.scatter(T,cv, s=8)

     dim, L, T, data = extract_data(filename("data_d3_L10_35"))
     data = np.array(data)
     T = data[:,0]
     cv = abs(data[:,3])
     plt.plot(T,cv,'k',label="d=3, L=10")
     plt.scatter(T,cv, s=8,c='k')

     dim, L, T, data = extract_data(filename("data_d4_L4_40"))
     data = np.array(data)
     T = data[:,0]
     cv = abs(data[:,3])
     plt.plot(T,cv,label="d=4, L=4")
     plt.scatter(T,cv, s=8)

     dim, L, T, data = extract_data(filename("data_d5_L3_33"))
     data = np.array(data)
     T = data[:,0]
     cv = abs(data[:,3])
     plt.plot(T,cv,label="d=5, L=3")
     plt.scatter(T,cv, s=8)

     plt.vlines(4.3,0,1,'r', linestyle='--', linewidth=1.5)
     plt.text(3.1, 0.25, 'T=4.3', color='black', fontsize=14)
     plt.vlines(6.5,0,1,'r', linestyle='--', linewidth=1.5)
     plt.text(5.25, 0.24, 'T=6.5', color='black', fontsize=14)
     plt.vlines(8.7,0,1,'r', linestyle='--', linewidth=1.5)
     plt.text(7.35, 0.22, 'T=8.7', color='black', fontsize=14)



     plt.ylim(0,0.6)
     plt.xlim(0.5,10)
     plt.title(r"Normalised Specific Heat vs. Temperature")
     plt.xlabel(r"$\mathbf{T} \quad [\mathbf{J/k_B}]$")
     plt.ylabel(r"$\mathbf{c_V} \quad [\mathbf{k_B}]$")
     plt.legend()
     plt.grid()
     if save:
          plt.savefig('C:/Users/ADMIN/Desktop/TESI/images/cV_temp_plot.pdf', dpi=300, bbox_inches='tight')
     else:
          plt.show()
          

# ENERGY PLOTS OVER TEMPERATURE
if ciccio[2]:
     if save:
          plt.figure(figsize=(6,4), dpi=300)

     dim, L, T, data = extract_data(filename("data_d3_L5_32"))
     data = np.array(data)
     T = data[:,0]
     e = data[:,1]
     plt.plot(T,e,label="d=3, L=5")
     plt.scatter(T,e, s=8)

     dim, L, T, data = extract_data(filename("data_d4_L4_40"))
     data = np.array(data)
     T = data[:,0]
     e = data[:,1]
     plt.plot(T,e,label="d=4, L=4")
     plt.scatter(T,e, s=8)

     dim, L, T, data = extract_data(filename("data_d5_L3_33"))
     data = np.array(data)
     T = data[:,0]
     e = data[:,1]
     plt.plot(T,e,label="d=5, L=3")
     plt.scatter(T,e, s=8)

     plt.vlines(4.3,-5,0,'r', linestyle='--', linewidth=1.5)
     plt.text(3, -.8, 'T=4.3', color='black', fontsize=14)

     plt.vlines(6.5,-5,0,'r', linestyle='--', linewidth=1.5)
     plt.text(5.25, -1.7, 'T=6.5', color='black', fontsize=14)

     plt.vlines(8.7,-5,0,'r', linestyle='--', linewidth=1.5)
     plt.text(7.35, -1.8, 'T=8.7', color='black', fontsize=14)

     x = np.linspace(-1,11)
     plt.plot(x,x/2-3,'b-.', linewidth=1, label=r"T/2-3")
     plt.plot(x,x/2-4,'r-.', linewidth=1, label=r"T/2-4")
     plt.plot(x,x/2-5,'g-.', linewidth=1, label=r"T/2-5")

     plt.ylim(-4.6,-0.25)
     plt.xlim(0,10.1)
     plt.title(r"Normalised Energy vs. Temperature")
     plt.xlabel(r"$\mathbf{T} \quad [\mathbf{J/k_B}]$")
     plt.ylabel(r"$\mathbf{<e>} \quad [\mathbf{J}]$")
     plt.legend()
     plt.grid()

     if save:
          plt.savefig('C:/Users/ADMIN/Desktop/TESI/images/energy_temp_plot.pdf', dpi=300, bbox_inches='tight')
     else:
          plt.show()


# MAGNETISATION PLOTS OVER TEMPERATURE
if ciccio[3]:
     if save:
          plt.figure(figsize=(6,4), dpi=300)

     dim, L, T, data = extract_data(filename("data_d3_L5_32"))
     data = np.array(data)
     T = data[:,0]
     m = data[:,2]
     plt.plot(T,m,label="d=3, L=5")
     plt.scatter(T,m, s=8)

     dim, L, T, data = extract_data(filename("data_d4_L4_40"))
     data = np.array(data)
     T = data[:,0]
     m = data[:,2]
     plt.plot(T,m,label="d=4, L=4")
     plt.scatter(T,m, s=8)

     dim, L, T, data = extract_data(filename("data_d5_L3_33"))
     data = np.array(data)
     T = data[:,0]
     m = data[:,2]
     plt.plot(T,m,label="d=5, L=3")
     plt.scatter(T,m, s=8)

     plt.vlines(4.3,0,1,'r', linestyle='--', linewidth=1.5)
     plt.text(2.7, 0.12, 'T=4.3', color='black', fontsize=14)

     plt.vlines(6.5,0,1,'r', linestyle='--', linewidth=1.5)
     plt.text(6.6, 0.23, 'T=6.5', color='black', fontsize=14)

     plt.vlines(8.7,0,1,'r', linestyle='--', linewidth=1.5)
     plt.text(7.4, 0.45, 'T=8.7', color='black', fontsize=14)

     x = np.linspace(0,10)
     plt.plot(x,(1-x/4.3)**0.5,'b-.', linewidth=1, label=r"$(1-\frac{T}{T_C})^{\frac{1}{2}}, T_C=4.3$")
     plt.plot(x,(1-x/6.5)**0.5,'r-.', linewidth=1, label=r"$(1-\frac{T}{T_C})^{\frac{1}{2}}, T_C=6.5$")
     plt.plot(x,(1-x/8.7)**0.5,'g-.', linewidth=1, label=r"$(1-\frac{T}{T_C})^{\frac{1}{2}}, T_C=8.7$")
     plt.ylim(0.1,1)
     plt.xlim(0,10.1)
     plt.title(r"Normalised Magnetisation vs. Temperature")
     plt.xlabel(r"$\mathbf{T} \quad [\mathbf{J/k_B}]$")
     plt.ylabel(r"$\mathbf{<m>}$")
     plt.legend()
     plt.grid()

     if save:
          plt.savefig('C:/Users/ADMIN/Desktop/TESI/images/mag_temp_plot.pdf', dpi=300, bbox_inches='tight')
     else:
          plt.show()


# CHI PLOTS OVER TEMPERATURE
if ciccio[4]:
     if save:
          plt.figure(figsize=(6,4), dpi=300)

     plt.vlines(4.3,0,3,'r', linestyle='--', linewidth=1.5)
     plt.text(4.5, 2.75, 'T=4.3', color='black', fontsize=14)
     plt.vlines(6.5,0,1,'r', linestyle='--', linewidth=1.5)
     plt.text(6, 1.1, 'T=6.5', color='black', fontsize=14)
     plt.vlines(8.7,0,1,'r', linestyle='--', linewidth=1.5)
     plt.text(8.2, 1.1, 'T=8.7', color='black', fontsize=14)

     dim, L, T, data = extract_data(filename("data_d3_L5_32"))
     data = np.array(data)
     T = data[:,0]
     chi = abs(data[:,4])
     plt.plot(T,chi,label="d=3, L=5")
     plt.scatter(T,chi, s=8)

     dim, L, T, data = extract_data(filename("data_d3_L10_35"))
     data = np.array(data)
     T = data[:,0]
     chi = abs(data[:,4])
     plt.plot(T,chi,'k',label="d=3, L=10")
     plt.scatter(T,chi, s=8,c='k')

     dim, L, T, data = extract_data(filename("data_d4_L4_40"))
     data = np.array(data)
     T = data[:,0]
     chi = abs(data[:,4])
     plt.plot(T,chi,label="d=4, L=4")
     plt.scatter(T,chi, s=8)

     dim, L, T, data = extract_data(filename("data_d5_L3_33"))
     data = np.array(data)
     T = data[:,0]
     chi = abs(data[:,4])
     plt.plot(T,chi,label="d=5, L=3")
     plt.scatter(T,chi, s=8)



     # plt.ylim(0,0.6)
     plt.xlim(0.5,10)
     plt.title(r"Normalised Susceptibility vs. Temperature")
     plt.xlabel(r"$\mathbf{T} \quad [\mathbf{J/k_B}]$")
     plt.ylabel(r"$\mathbf{\chi} \quad [\mathbf{1/J}]$")
     plt.legend()
     plt.grid()
     if save:
          plt.savefig('C:/Users/ADMIN/Desktop/TESI/images/chi_temp_plot.pdf', dpi=300, bbox_inches='tight')
     else:
          plt.show()
          

