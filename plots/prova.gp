# Set terminal and output format
set terminal pngcairo size 800,600

# Set labels for the axes
set xlabel "Row Number"
set ylabel "Value"

filename="~/compphys_examples/sph_model/data/data_d2_L5_T0.10_1.txt"

# First plot: e as a function of row number
set output "plot_e.png"
set title "Energy (e) vs Row Number"
plot filename using 0:1 with lines title "e"

# Second plot: m as a function of row number
set output "plot_m.png"
set title "Magnetization (m) vs Row Number"
plot filename using 0:2 with lines title "m"

# Third plot: cv as a function of row number
set output "plot_cv.png"
set title "Heat Capacity (cv) vs Row Number"
plot filename using 0:3 with lines title "cv"

# Fourth plot: chi as a function of row number
set output "plot_chi.png"
set title "Susceptibility (chi) vs Row Number"
plot filename using 0:4 with lines title "chi"

# Reset the output to default
set output
