##############

# Define the default arguments
file_path = input_folder/
name = inputfile
in_filename = $(file_path)$(name).txt
try_num = 1  				# optional because is used to debug
arg = $(in_filename) $(try_num)

# Compiler and flags
# usefull: -Wall
CXX = g++
CXXFLAGS = -std=c++11 -O3 #-fopenmp

# Main file
MAIN_FILE = sph_mod.cpp

# Source folders
SRC_FOLDERS = src

# Header folders
HDR_FOLDERS = include



# Source files
SRC_FILES = $(wildcard $(addsuffix /*.cpp, $(SRC_FOLDERS)))

# Object files
OBJS = $(notdir $(SRC_FILES:.cpp=.o)) $(notdir $(MAIN_FILE:.cpp=.o))

# Executable name
TARGET = $(MAIN_FILE:.cpp=.exe)



all: compile link run ## Compile, link and run the program

compile: ## Rule to compile all the .cpp files
	$(CXX) $(CXXFLAGS) -I $(HDR_FOLDERS) -c $(MAIN_FILE) $(SRC_FILES)

link: ## Rule to link all the .o files in one executable file
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) -larmadillo

run: ## Rule to run the executable with default arguments
	date & time -f "\t%E real,\t%U user,\t%S sys" ./$(TARGET) $(arg)

run_simulation: ## Rule to run a bigger simulation, in general runs more than one time the code
	./$(TARGET) $(file_path)inputfile.txt 1
	./$(TARGET) $(file_path)inputfile1.txt 1

clean: ## Remove object files and executables
	rm -f *.o *.exe



# Help target
help: ## Display this help message
	@echo "$$(tput bold)Available rules:$$(tput sgr0)"
	@echo
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-15s\033[0m %s\n", $$1, $$2}'



# Default target: help
.DEFAULT_GOAL := help

# Phony targets
.PHONY: all compile link run run_simulation clean help

##############

# # Define the default arguments for the C++ code
# file_path = input_folder/
# name = prova
# in_filename = $(file_path)$(name).txt
# try_num = 1  				# optional because is used to debug
# arg = $(in_filename) $(try_num)

# # Compiler and flags for C++ code
# # usefull: -Wall
# CXX = g++
# CXXFLAGS = -std=c++11 -O3 -fopenmp

# # Main C++ file
# MAIN_FILE = sph_mod.cpp

# # Source and Header folders for C++ code
# SRC_FOLDERS = src
# HDR_FOLDERS = include

# # Source and Object files for C++ code
# SRC_FILES = $(wildcard $(addsuffix /*.cpp, $(SRC_FOLDERS)))
# OBJS = $(notdir $(SRC_FILES:.cpp=.o)) $(notdir $(MAIN_FILE:.cpp=.o))

# # Executable name for C++ code
# TARGET = $(MAIN_FILE:.cpp=.exe)

# # LaTeX variables
# LATEX_MAIN = Tesi_Modello_Sferico/main.tex   # Change this if your main LaTeX file has a different name
# BUILD_DIR = build       # Directory to store the compiled PDF
# PDF_FILE = $(BUILD_DIR)/main.pdf

# # Tectonic command (LaTeX compiler)
# TECTONIC = tectonic

# # All target for compiling C++ files
# all: compile link run ## Compile, link and run the program

# # Rules to compile C++ files, link the object files and run the executable
# compile: ## Rule to compile all the .cpp files 
# 	$(CXX) $(CXXFLAGS) -I $(HDR_FOLDERS) -c $(MAIN_FILE) $(SRC_FILES)
# link: ## Rule to link all the .o files in one executable file
# 	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) -larmadillo
# run: ## Rule to run the executable with default arguments
# 	time ./$(TARGET) $(arg)

# # Rule to compile LaTeX files
# pdf: $(PDF_FILE) ## Compile LaTeX project to a PDF
# 	@echo "LaTeX compilation completed: $(PDF_FILE)"

# $(PDF_FILE): $(LATEX_MAIN) ## Compiles the LaTeX file into a PDF
# 	mkdir -p $(BUILD_DIR)
# 	$(TECTONIC) -o $(BUILD_DIR) $(LATEX_MAIN)

# # Rule to watch for changes and continuously compile LaTeX
# watch: ## Watch for changes in LaTeX files and recompile automatically
# 	$(TECTONIC) -X watch

# # Clean C++ and LaTeX outputs
# clean: ## Remove object files, executables, and compiled LaTeX PDFs
# 	rm -f *.o *.exe
# 	rm -rf $(BUILD_DIR)

# # Run bigger C++ simulations
# run_simulation: ## Run a bigger simulation (multiple runs)
# 	./$(TARGET) $(file_path)inputfile.txt 0
# 	./$(TARGET) $(file_path)inputfile.txt 1
# 	./$(TARGET) $(file_path)inputfile1.txt 0
# 	./$(TARGET) $(file_path)inputfile2.txt 1

# # Help target
# help: ## Display this help message
# 	@echo "$$(tput bold)Available rules:$$(tput sgr0)"
# 	@echo
# 	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-15s\033[0m %s\n", $$1, $$2}'

# # Default target: help
# .DEFAULT_GOAL := help

# # Phony targets
# .PHONY: all compile link run run_simulation pdf watch clean help


##############

# # Define the default arguments for the C++ code
# file_path = input_folder/
# name = prova
# filename = $(file_path)$(name).txt
# try_num = 1  				# optional because is used to debug
# arg = $(filename) $(try_num)

# # Compiler and flags for C++ code
# CXX = g++
# CXXFLAGS = -std=c++11 -O3 -fopenmp

# # Main C++ file
# MAIN_FILE = sph_mod.cpp

# # Source and Header folders for C++ code
# SRC_FOLDERS = src
# HDR_FOLDERS = include

# # Source and Object files for C++ code
# SRC_FILES = $(wildcard $(addsuffix /*.cpp, $(SRC_FOLDERS)))
# OBJS = $(notdir $(SRC_FILES:.cpp=.o)) $(notdir $(MAIN_FILE:.cpp=.o))

# # Executable name for C++ code
# TARGET = $(MAIN_FILE:.cpp=.exe)

# # All target for compiling both C++ and LaTeX
# all: compile link run pdf ## Compile C++ and LaTeX

# # Rule to compile C++ files
# compile: ## Rule to compile all the .cpp files
# 	$(CXX) $(CXXFLAGS) -I $(HDR_FOLDERS) -c $(MAIN_FILE) $(SRC_FILES)

# # Rule to link C++ object files
# link: ## Rule to link all the .o files in one executable file
# 	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) -larmadillo

# # Rule to run the C++ executable
# run: ## Rule to run the executable with default arguments
# 	time ./$(TARGET) $(arg)

# # Run bigger C++ simulations
# run_simulation: ## Run a bigger simulation (multiple runs)
# 	./$(TARGET) $(file_path)inputfile.txt 0
# 	./$(TARGET) $(file_path)inputfile.txt 1
# 	./$(TARGET) $(file_path)inputfile1.txt 0
# 	./$(TARGET) $(file_path)inputfile2.txt 1




# # LaTeX variables
# LATEX_MAIN = Tesi_Modello_Sferico/main.tex
# PDF_FILE = build/main.pdf
# BUILD_DIR = build

# # Rule to compile LaTeX files to PDF using pdflatex + biber
# pdf: $(PDF_FILE)

# $(PDF_FILE): $(LATEX_MAIN)
# 	mkdir -p $(BUILD_DIR)
# 	# First LaTeX pass to generate auxiliary files
# 	pdflatex -output-directory=$(BUILD_DIR) $(LATEX_MAIN)
# 	# Run Biber for bibliography processing
# 	biber $(BUILD_DIR)/main
# 	# Run LaTeX again to incorporate bibliography
# 	pdflatex -output-directory=$(BUILD_DIR) $(LATEX_MAIN)
# 	pdflatex -output-directory=$(BUILD_DIR) $(LATEX_MAIN)
