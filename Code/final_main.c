#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "models.c"








const int gravel=85;
const int concrete= 98;  
const int greenspace=69;


float coeff_of_initial_abstraction=0.05;


int ROWS;
int COLS;




int main(int argc, char *argv[]){






   ///////////////////////// QUESTION 1 /////////////////////////////////////////
   // ///////HOW DIFFERENT SOIL TYPES AFFECT INFILTRATION OF WATER IN SOILS/////
   /*description: comparing  different proportion of soils types by using CN values and SCS formulas */


 




               int CN3[3]={concrete, gravel, greenspace}; 
               int land1[3]= {atof(argv[1]),atof(argv[2]),atof(argv[3])}; // 60% of concrete, 20% of gravel, 20% green spaces
               int land2[3]= {atof(argv[4]),atof(argv[5]),atof(argv[6])}; // 40% of concrete, 30% of gravel, 30% green spaces
               int soils_diversity=3;
   //  data used: a simulation of precipitation stored in "1daysimulation.txt"


   const char* datas_rainfall_intensity= "../Data/1daysimulation.txt";




 


    based_on_proportion(CN3,land1,soils_diversity,datas_rainfall_intensity,"../Internal/intensity1.txt");// 'based_on_proportion' is deeply described in models.c, this function export:
    based_on_proportion(CN3,land2,soils_diversity,datas_rainfall_intensity,"../Internal/intensity2.txt");//    outputs:        - rain intensity (rate in [mm/h],run off intensity and infiltration intensity
                                                                                         // inputs :           - from rainfall intensity datas and the different CN values, the number of CN values used (soils_diversity) and their respective proportion
    
   //on matlab, some plots are made to visualise the rate of run off and infiltration over the time




    ///////////////////////// QUESTION 2 /////////////////////////////////////////
   // ///////HOW DISPOSITION AFFECT INFILTRATION OF WATER IN SOILS/////
   /*description: we want to compare two different disposition: one where the different soil type are not mixed  and one where the repartion is mixed, more homogeneous
   */
   ROWS=atof(argv[7]);
   COLS=atof(argv[8]);


   //allocating memory adress for the matrix of structure cells (this structure is described in tools.c) representing the land
   struct Cell nonmixed_land[ROWS][COLS];
   struct Cell mixed_land[ROWS][COLS];
   //for those two soils the proportion of soil types is the same but we are reparting it differently using two functions:
   fill_nonmixed(nonmixed_land,soils_diversity, CN3, land1);
   fill_mixed(mixed_land, soils_diversity, CN3, land1);  // thoses two function's are explained in tools.c


  
   export_matrix_to_csv("../Internal/nonmixed.csv",nonmixed_land); //exporting the CN values repartition of the matrix to visualize it
   export_matrix_to_csv("../Internal/mixed.csv", mixed_land);
  
   based_on_disposition(nonmixed_land, datas_rainfall_intensity, "../Internal/infiltration_rate_nonmixed.csv");//function explained in models.c
   based_on_disposition(mixed_land, datas_rainfall_intensity, "../Internal/infiltration_rate_mixed.csv");// it export a text file with the rain intensity [mm/h]and the runoff and  the rainfall in mm/h over the whole land
                       //  -matrix        -raindatas                  -name and path of outfile






   /////////////////////////////QUESTION 3/////////////////////////////////////////////
   /////  COMPARISON OF MODELS/////
   /*description: we want to use our models above to evaluate the infiltration over a land based on different percentage of concrete
                  the two disposition of land and the proportion based model will be studied
                  this can tell us for example the equivalent percentage of concrete between the two disposition for a given infiltration*/
  




   int soils_diversity2 = 2; // Two soil types: concrete and greenspace


   int CN_values[2] = {concrete, greenspace}; 




   int pmin_of_concrete = atof(argv[9]);  // Minimum percentage of concrete
   int pmax_of_concrete = atof(argv[10]);  // Maximum percentage of concrete
   int step = 10;              // Increment step for percentage




   struct Cell nonmixed_land2[ROWS][COLS]; // Grid for non-mixed land configuration
   struct Cell mixed_land2[ROWS][COLS];   // Grid for mixed land configuration


   char filename_nonmixed2[50]; // File name for non-mixed results
   char filename_mixed2[50];    // File name for mixed results
   char filename_proportion[50]; 
   for (int p = pmin_of_concrete; p <= pmax_of_concrete; p += step) {
       int percentages[2] = {p, 100 - p}; // Concrete and grass percentages varying


      
       sprintf(filename_nonmixed2, "../Internal/nonmixed_%d.txt", p); //make different filename at each step
       sprintf(filename_mixed2, "../Internal/mixed_%d.txt", p);
       sprintf(filename_proportion, "../Internal/proportion_%d.txt", p);
      
       fill_nonmixed(nonmixed_land2, soils_diversity2, CN_values, percentages); // Fill the matrix with soil configurations
       fill_mixed(mixed_land2, soils_diversity2, CN_values, percentages);


      
       based_on_disposition(nonmixed_land2, datas_rainfall_intensity, filename_nonmixed2); // Calculate and export infiltration results
       based_on_disposition(mixed_land2, datas_rainfall_intensity, filename_mixed2);
       based_on_proportion(CN_values,percentages,soils_diversity2,datas_rainfall_intensity,filename_proportion);
   }




   return 0;
}
