# Project: Modeling Infiltration and Runoff on Different Soil Types

General Description

This project aims to model infiltration and runoff processes on various soil types using Soil Conservation Service (SCS) formulas. It combines C programs for calculations and MATLAB scripts for visualizing the results. The studied scenarios include variations in soil type proportions and disposition (non-mixed vs. mixed).


## The program will:  
1. Read the initial precipitation data from provided input files ("*1daysimulation.txt*" in "*Data*").  
2. Simulate infiltration and runoff processes under different soil configurations using the SCS method ("*models.c*").  
3. Visualize all results, saving key graphs and outputs in the "*Results*" folder as .png files. 

## Project Structure
- "Data" contains input files such as soil configurations and precipitation data.
- "Internal" contains intermediate files generated during simulations. These files are automatically updated by the program and should not be manually modified.
- "Code" contains the C and MATLAB scripts used for calculations and visualizations.
- "Results" contains saved .png files of graphs.

## Inputs and Outputs

**Inputs**

- "Data/1daysimulation.txt": Text file containing simulation data for precipitation over 24 hours.
- "Data/input.txt": Text file to regulate simulation parameters, including percentage of concrete, gravel and greenspace for scenario 1 and 2, lenght and width of the land studied in QUESTION 1 and 2, and the minimum and maximum percentage of concrete for QUESTION 3.

**Internal Files**

- "Internal/intensity1.txt": Semicolon-delimited file storing intensities data.
- "Internal/intensity2.txt": Semicolon-delimited file storing intensities data.

- "Internal/nonmixed.csv": Comma-delimited file storing non mixed land datas.
- "Internal/mixed.csv": Comma-delimited file storing mixed land datas.
- "Internal/infiltration_rate_nonmixed.csv": Semicolon-delimited file containing land intensities data.
- "Internal/infiltration_rate_mixed.csv": Semicolon-delimited file containing land intensities data.

- "Internal/nonmixed_{PercentageConcrete}.txt": A comma-delimited file containing infiltration results for non-mixed land configurations.
- "Internal/mixed_{PercentageConcrete}.txt":A comma-delimited file containing infiltration results for mixed land configurations.
- "Internal/proportion_{PercentageConcrete}.txt":A comma-delimited file containing infiltration results of proportion based model
**Outputs**
"*Results*" contains several image files, each has a custom name of format: 
- "Results/intensity1.png": image file.
- "Results/intensity2.png": image file.
- "Results/nonmixed.png": image file.
- "Results/nonmixed.png": image file.
- "Results/infiltration_vs_concrete.png": image file.
- "Results/infiltration_comparison.png": image file.
- "Results/cumulation_comparison.png": image file.


## Implementation Details

**Overview:**
- The simulation is implemented in C, it uses input values, which contains parameters that can be selected by the user in "*Data/input.txt*". And it performs all the necessary computations.
- The computed results are exported by the C program into TXT and CSV files located in "*Internal*" for further analysis.
- MATLAB is then used to process these files and generate visualizations, providing insights into the simulation outcomes.

**Structure**: In the directory "*Code/*" are located:


- "*final_main.c*": This file serves as the main program, integrating all the functions from the below files to execute the complete simulation process and exports all the results into the CSV and TXT files in "*Internal*". It uses the functionalities provided by importing "*models.c*" as a module and the TXT file in "*Data*".
              - "*models.c*": Functions that calculate the intensities of runoff and infiltration for different scenarios. It uses the functionalities provided by importing "*tools.c*" and "*import_and_export.c*" as modules
              - "*tools.c*": Auxiliary functions supporting the simulation models "*models.c*".
              - "*import_and_export.c*": Functions used to read precipitation data from "*Internal/1daysimulation.txt*" file and to export results of the functions in "*models.c*" into CSV or TXT files.


- "*visu_main.m*": Main script that organizes the visualization workflow. It reads all CSV and TXT files from the "*Internal*" directory, generates plots in separate windows, and saves the images as PNG files in the "*Results*" directory. The script utilizes the following functions from other files in  "*Code/*":
              - "*visualisation_massbalance.m*", "*visualisation_matrix.m"*. "*compare_infiltration.m*" and "*infiltration_vs_concrete.m*" (which internally uses  "*load_n_sum.m*").


## Instructions

To reproduce the results described in the report, follow these steps:
1. Prepare the Input File
      - Open the file located at "*Data/input.txt*". Modify the parameters as needed, following the comments provided in the file, and save the changes.

2. Run the Automation Script
      -  Open MATLAB and navigate to the Code/ directory.
      -  Run the automatisation.m script. This script will:
               -  Compile the C code using GCC.
               -  Read the input parameters from "*Data/input.txt*".
               -  Execute the compiled C program with the provided input parameters.
               -  Export simulation results (e.g., TXT and CSV files) to the Internal/ directory
               -  Automatically call the "*Code/visu_main.m*" script to process and visualize the results.
               -  The visualizations are automatically saved as PNG files in the "*Results/*" directory for further analysis.



## Requirements

Versions of C and Matlab used are as follows.
```
$ gcc --version
gcc (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0

$ matlab --version
9.14.0.2206163 (R2023a)
```
The C libraries utilized were the following:
```
<stdio.h>
<stdlib.h>
<math.h>
$ ldd --version
version of the GNU C Library (glibc) is 2.31
```

## Credits

### Data

The data files "*1daysimulation.txt*" come from [Meteo Suisse](https://www.meteosuisse.admin.ch/services-et-publications/applications/valeurs-mesurees-et-reseaux-de-mesure.html#param=messwerte-niederschlag-10min&table=true&station=LSN).The hourly precipitation data for December 7, 2024, in Lausanne is selected.

### Formulae

1. Amorim Brandão, A. R., Schwamback, D., Ballarin, A. S., Ramirez-Avila, J. J., Vasconcelos, J. G., & Oliveira, P. T. S. (n.d.). *Toward a better understanding of curve number and initial abstraction values from a global watershed dataset perspective*. SSRN. Retrieved from [https://ssrn.com](https://ssrn.com)

2. Muthukrishnan, S. (n.d.). *Effects of initial abstraction and urbanization on estimated runoff using CN technology*. Academia.edu. Retrieved from [https://academia.edu](https://academia.edu)

3. Ponce, V. M. (n.d.). *Hawkins initial abstraction: Toward better estimation*. Retrieved from [https://ponce.sdsu.edu/hawkins_initial_abstraction.pdf](https://ponce.sdsu.edu/hawkins_initial_abstraction.pdf)

4. Dingman, S. L. (2015). *Physical Hydrology* (3rd ed., Chapter 10). Waveland Press.

5. U.S. Department of Agriculture Natural Resources Conservation Service (NRCS). (1986). *Urban hydrology for small watersheds* (Technical Release 55, TR-55). Retrieved from [https://nrcs.usda.gov](https://nrcs.usda.gov)

6. Soulis, N. D. (Ed.). (n.d.). *Soil Conservation Service Curve Number (SCS-CN) Method: Current applications, remaining challenges, and future perspectives*. Retrieved from [Soil_Conservation_Service_Curve_Number_SCSCN_Method_Current_Applications_Remaining_Challenges_and_Future_Perspectives.pdf](Soil_Conservation_Service_Curve_Number_SCSCN_Method_Current_Applications_Remaining_Challenges_and_Future_Perspectives.pdf)



### Note on AI

While this project was primarily based on manual research and development, we occasionally used artificial intelligence (AI) tools to assist with repetitive tasks, such as generating export functions, and resolving minor coding issues. This helped improve efficiency and allowed us to focus on more critical aspects of the work.

