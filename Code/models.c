#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "tools.c"
#include "import_and_export.c"

/*
This file contain the two models, each exporting run off intensity and infiltration intensity based on the Soils Conservation Service-Curve Number Formulas   
      - 'based_on_proportion' return the datas based on a given proportion of the different soil types (concrete, gravel and greenspaces)
      - 'based-on_disposition' return the datas based on a given disposition pf soil types 
*/

      


/*
--------------------- Function calculating the intensity of run off and infiltration from the proportion of soil types over a given period of rainfall ---------------------

@Input:     - differents soils types charachterized by their CN values 'CN_values'
            - proportion of each soil type, each value need to be between 0 and 100, and the sum must be 100
            - the number of type of soils (size of CN_values and percentages)
            - array of rainfall intensity datas [mm/h] (advice: use a rainfall dataset of a few hours period)
            - name of the output file 
@Output:   doesnt return anything but export the values of 
           rainfall, run off, infiltration intensities in [mm/h] in text files

@Description:  SCS formulas are used. 
               For a realistic application of the formulas, we account for an initial amount of water that must be absorbed before runoff and soil infiltration can take place. 
               This value is calculated and referred to as I_a .
               To hope reach and exceed this values wre're applying the SCS formulas to the cumulated rain.

*/


void based_on_proportion(int * CN_values ,int * proportions,int soils_diversity, const char* filename_raindatas, const char * filename_intensities){


    float rain_intensity[999];  //large enough memory place for raindatas
    int hours = read_precipitation(filename_raindatas,rain_intensity); //function  that reads the datas described in tools.c file

    float CN_average= average(CN_values,proportions,soils_diversity); //getting the average CN indice using the funcion "average" defined in tools.c


    const float S = 25400 / CN_average - 254;  // the variable S calculated here is the maximum retention [mm].
                                               // it is a "Soil Conservation Service" formula
                                               // constant numbers used are choosen to use SI units

    const float I_a= coeff_of_initial_abstraction*S;  // Ia is the initial abstraction, amount of rainfall need to runoff and infiltration to start
                                                     // the variable coeff_of_initial_abstraction is discussed in the report

    // to be more realistic, we assume rain water is accumulating over the day, to reach the initial abstraction
    float Pcum[hours];
    cumulated_sum(rain_intensity,Pcum,24);  //using the "cumulated_sum" function from tools.c to get the cumulated precipitation at each hour over the period


    float Icum[hours]; //initialize the cumulated Infiltration [mm]

    float ROcum[hours]; //initialize the cumulated Run OFF water[mm]
    


    for(int i=0;i<hours;i++){

        bool Ia_not_exceedeed= Pcum[i]<I_a; //condition for run off and infiltration to occur

        if (Ia_not_exceedeed){
            Icum[i]=0;
            ROcum[i]=0;
        }else{
            ROcum[i]= (Pcum[i]-I_a)* (Pcum[i]-I_a) / (Pcum[i]+ (1-coeff_of_initial_abstraction)*S);  //SCS formulas
            Icum[i]=Pcum[i]- I_a - ROcum[i];  //mass balance
        }
    }
    
    float Irate[24]; // infiltration rate [mm/h]
    float ROrate[24]; // rate of run off [mm/h]

    for(int i=0;i<24;i++){  //getting the run off and infiltration rate from the cumulated values 
            if (i==0){
                Irate[i]=Icum[i];
                ROrate[i]=ROcum[i];
            }else{
                Irate[i]=(Icum[i]-Icum[i-1]); 
                ROrate[i]=(ROcum[i]-ROcum[i-1]);
            }
    }
//Export the intensities, function described in tools.c
    export_intensities(rain_intensity,ROrate,Irate,hours,filename_intensities);

}


/*
--------------------- Function calculating the run off and infiltration intensities from a the disposition of soils  ---------------------

@Input:     - adress of a matrix of structure 'Cell' (described in tools.c) representing the land disposition of soils 
            - array of rainfall intensity datas [mm/h] (advice: use a rainfall dataset of a few hours period)


@Output:   doesnt return anything but export intensities values of run off and infiltration [mm/(hour*surface)] 
            over the whole land in text files

@Description: This function calculates the runoff and infiltration intensities, but applies a different mass balance approach than previously. 
The incoming water used to calculate runoff and infiltration at a given 
location is not only the rainfall, but also includes a portion of runoff from surrounding areas. 
This model accounts for the movement of water, acknowledging that water doesn't directly runoff from the land.
*/




void based_on_disposition(struct Cell matrix[ROWS][COLS], const char *filename_raindatas, const char *filename_intensities) {

    float rain_intensity[999];    
    int hours = read_precipitation(filename_raindatas, rain_intensity);// read rain datas

    // Initialize memory 
    float Pcum[hours]; // Précipitations cumulées
    float ROcum[hours]; //cumulated run off
    float Irate[hours]; //infiltration per hour over the land 
    float ROrate[hours];// run off per hour over the land
                        
    cumulated_sum(rain_intensity, Pcum, hours);         

    for (int i = 0; i < hours; i++) {
        ROcum[i] = 0.0;
    } // Initialize the values to zero to avoid memory ambiguity.

    for (int t = 0; t < hours; t++) { //for each hour...
    

    //...calculate a fisrt time the Run off at each location, sored in the Cell structure
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {

            matrix[row][col].RO = dailyrunoff(Pcum[t], 0.0, matrix[row][col].CN);
            }}

    // ...get the water received at each location 
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                matrix[row][col].received =0.0;
                matrix[row][col].received += addwater(&matrix[row][col], row, col); // function described in tools.c
            }}

    //... calculate the new run off, considering a new mass balance
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                matrix[row][col].RO = dailyrunoff(Pcum[t], matrix[row][col].received, matrix[row][col].CN);
                ROcum[t] += matrix[row][col].RO- matrix[row][col].received;

            }}}

// From the cumulated Runoff, get the run off and infiltration intensities

    for (int i = 0; i < hours; i++) {
        if (i == 0 ) {
            ROrate[i] = ROcum[i];
        } else if(rain_intensity[i]==0) {
            ROrate[i] = 0.0;
        }else{
            ROrate[i] = ROcum[i] - ROcum[i - 1];
        }
        Irate[i]=0.0;
        if(ROrate[i]!=0){
          Irate[i]= rain_intensity[i]*(ROWS*COLS)-ROrate[i];  
        }}


//Xxport the intensities, function described in tools.c
  export_intensities(rain_intensity, ROrate, Irate, hours, filename_intensities);

}