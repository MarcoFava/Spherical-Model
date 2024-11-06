import matplotlib.pyplot as plt
import numpy as np
import math



# Change the file name where the data is stored
file_name = 'prova.txt'
file_dir  = "//wsl.localhost/Ubuntu/home/marcofava/compphys_examples/sph_model/"

# Load the data and divide it in different vectors
# The values are stored with the order: e, m, cv, chi.
data = np.loadtxt(file_dir + file_name)
E_average = data[:,0]
M_average = data[:,1]
c_v = data[:,2]
chi = data[:,3]

n_MCMC = np.size(E_average)

x = np.arange(n_MCMC)

# plt.plot(x,E_average)
# plt.grid()
# plt.show()
plt.plot(x,M_average)
plt.title("Spherical model")
plt.xlabel("$N_{MC}$")
plt.ylabel("M")
plt.grid()
plt.show()
# plt.plot(x,c_v)
# plt.grid()
# plt.show()
# plt.plot(x,chi)
# plt.grid()
# plt.show()