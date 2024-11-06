import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import math
from scipy.optimize import curve_fit

# x1 = [22**2, 8**3, 10**3, 13**3, 15**3]
# y1 = [0.43, 0.47, 1.75, 8.94, 26.81]

# plt.plot(np.log(x1),np.log(y1))
# plt.scatter(np.log(x1),np.log(y1))
# plt.grid()
# # plt.show()

# times = [3.32, 3.09, 3.115, 3.119, 3.497]
# av = np.mean(times)
# print(av)



# File path
dim: int = 2
L: int = 5
T = '2.00'
path = '//wsl.localhost/Ubuntu/home/marcofava/compphys_examples/sph_model/data/'
# path = '~/compphys_examples/sph_model/data/'
extension = '.txt'
name = 'data_d' + str(dim) + '_L' + str(L) + '_T' + T
filename = path + name + extension
# filename = 'data_3x30_2.txt'


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



def plot_data_single_T(filename_in):
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

     # plt.plot(x,e)
     # plt.xlabel('$n_{MC}$')
     # plt.ylabel('<e>')
     # plt.grid()
     # plt.show()

     plt.plot(x,m)
     plt.xlabel('$n_{MC}$')
     plt.ylabel('<m>')
     plt.grid()
     plt.show()

     # plt.plot(x,cv)
     # plt.xlabel('$n_{MC}$')
     # plt.ylabel('$c_V$')
     # plt.grid()
     # plt.show()

     # plt.plot(x,chi)
     # plt.xlabel('$n_{MC}$')
     # plt.ylabel('$\chi$')
     # plt.grid()
     # plt.show()



def plot_data_multi_T(filename_in):
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

     # plt.plot(T,e)
     # plt.xlabel('T')
     # plt.ylabel('<e>')
     # plt.grid()
     # plt.show()

     plt.plot(T,m)
     plt.xlabel('T')
     plt.ylabel('<m>')
     plt.grid()
     plt.show()

     # plt.loglog(T,cv)
     # plt.xlabel('T')
     # plt.ylabel('$c_V$')
     # plt.grid()
     # plt.show()

     # plt.loglog(T,chi)
     # plt.xlabel('T')
     # plt.ylabel('$\chi$')
     # plt.grid()
     # plt.show()


# plot_data(filename)
# dim, L, T, data = extract_data(filename)
# data = np.array(data)
# n_1 = np.size(data,0)
# t_1 = 32*60 + 45
# ratio_1 = n_1/t_1
# print(f'ratio try_num=1 = {ratio_1}')


# path1 = path + 'important/'
# name1 = name + '_1'
# filename1 = path1 + name1 + extension
# plot_data(filename1)
# dim, L, T, data = extract_data(filename1)
# data = np.array(data)
# n_0 = np.size(data,0)
# t_0 = 28*60 + 30
# ratio_0 = n_0/t_0
# print(f'ratio try_num=0 = {ratio_0}')


# path0 = path + 'important/'
# name0 = name + '_0'
# filename0 = path0 + name0 + extension
# plot_data(filename0)
# dim, L, T, data = extract_data(filename0)
# data = np.array(data)
# n_0 = np.size(data,0)
# t_0 = 28*60 + 30
# ratio_0 = n_0/t_0
# print(f'ratio try_num=0 = {ratio_0}')


dim = 3
L = 5
name = 'data_d' + str(dim) + '_L' + str(L) + '_0'
# name = 'data_d2_L5_T10.00_0'
filename = path + name + extension
plot_data_multi_T(filename)
# dim = 3
# L = 7
# name = 'old/data_d' + str(dim) + '_L' + str(L) + '_2'
# filename = path + name + extension
# plot_data_multi_T(filename)


# try_num = [2, 4, 8, 16, 32, 1410065408]
# for n in try_num:
#      name = 'try_num_' + str(n)
#      filename = path + name + extension
#      plot_data_single_T(filename)
# k = 50000
# n=0
# name = 'try_num_' + str(n)
# filename = path + name + extension
# # Convert the list of lists to a numpy array for easier processing
# dim, L, T, data = extract_data(filename)
# data = np.array(data)
# e = data[:k,0]
# m = data[:k,1]
# cv = data[:k,2]
# chi = data[:k,3]

# # number of Montecarlo cycles
# x = [i+1 for i in range(k)]

# plt.plot(x,cv)
# plt.xlabel('$n_{MC}$')
# plt.ylabel('<e>')
# plt.grid()

# ####################
# dim = 2
# L = 5
# T0 = '0.10'
# theta0 = '0.1'
# name = 'data_d' + str(dim) + '_L' + str(L) + '_T' + T0
# filename = path + name + extension
# # Convert the list of lists to a numpy array for easier processing
# dim, L, T, data = extract_data(filename)
# data = np.array(data)
# e = data[:,0]
# m = data[:,1]
# cv = data[:,2]
# chi = data[:,3]
# k = np.size(e)

# # number of Montecarlo cycles
# x = [i+1 for i in range(k)]
# plt.plot(x,cv)                                                   ### -- ###


# ####################
# T1 = '0.10_1'
# theta1 = '0.4'
# name = 'data_d' + str(dim) + '_L' + str(L) + '_T' + T1
# filename = path + name + extension
# # Convert the list of lists to a numpy array for easier processing
# dim, L, T, data = extract_data(filename)
# data = np.array(data)
# e = data[:,0]
# m = data[:,1]
# cv = data[:,2]
# chi = data[:,3]
# k = np.size(e)

# # number of Montecarlo cycles
# x = [i+1 for i in range(k)]
# plt.plot(x,cv)                                                   ### -- ###


# ####################
# T2 = '0.10_2'
# theta2 = '0.2'
# name = 'data_d' + str(dim) + '_L' + str(L) + '_T' + T2
# filename = path + name + extension
# # Convert the list of lists to a numpy array for easier processing
# dim, L, T, data = extract_data(filename)
# data = np.array(data)
# e = data[:,0]
# m = data[:,1]
# cv = data[:,2]
# chi = data[:,3]
# k = np.size(e)

# # number of Montecarlo cycles
# x = [i+1 for i in range(k)]
# plt.plot(x,cv)                                                   ### -- ###


# ####################
# T3 = '0.10_3'
# theta3 = '0.1_0'
# name = 'data_d' + str(dim) + '_L' + str(L) + '_T' + T3
# filename = path + name + extension
# # Convert the list of lists to a numpy array for easier processing
# dim, L, T, data = extract_data(filename)
# data = np.array(data)
# e = data[:,0]
# m = data[:,1]
# cv = data[:,2]
# chi = data[:,3]
# k = np.size(e)

# # number of Montecarlo cycles
# x = [i+1 for i in range(k)]
# plt.plot(x,cv)                                                   ### -- ###

# lgd = [theta0,theta1,theta2,theta3]
# plt.legend(lgd)
# plt.grid()
# plt.show()


# def exponential_model(x, y0, b):
#     return y0 + np.exp(-b * x)

# for i in range(2):
#      dim = 2
#      L = 5
#      # T = '0.10'
#      # if(i):
#      #      T = T + '_' + str(i)

     
#      T = '0.00_' + str(i)
     
     
#      name = 'data_d' + str(dim) + '_L' + str(L) + '_T' + T
#      filename = path + name + extension

#      # Convert the list of lists to a numpy array for easier processing
#      dim, L, T, data = extract_data(filename)
#      data = np.array(data)
#      e = data[:,0]
#      m = data[:,1]
#      cv = data[:,2]
#      chi = data[:,3]
#      k = np.size(e)

#      # number of Montecarlo cycles
#      x = [i+1 for i in range(k)]
#      # plt.plot(x,chi)                                                   ### -- ###
#      x_data = x[100000:]
#      y_data = cv[100000:]
#      # Perform the fit
#      initial_guess = [1.0, 0.3]
#      popt, pcov = curve_fit(exponential_model, x_data, y_data, p0=initial_guess)

#      # Extract the optimal parameters
#      fitted_y0, fitted_b = popt

#      print(f"Fitted parameters: y0 = {fitted_y0}, b = {fitted_b}")
     

# # lgd = ['0.1','0.4','0.2','0.1_0','0.4_0','0.2_0']
# lgd = ['old','new']
# plt.legend(lgd)
# plt.grid()
# plt.show()