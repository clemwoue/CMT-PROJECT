#include <stdio.h>
#include <math.h>
#include <time.h>


/*-------------------------- TOOLS AND SUBFUNCTION FOR THE "models.c" FILE ------------------------------*/

//here are the values needed for the functions, and declared while compiling or directly in the main file

extern int ROWS;
extern int COLS;
extern float coeff_of_initial_abstraction;


/*
------------------function calculating the average from values and their respective percentage, ie. the pondering sum between two vectors--------------------

@Input:     -  (pointer to int) the values
            -  (pointer to int) the corresponding percentage 
            -  (int)the number of element to average 

@Output:   - (float) calculated average 
*/


float average(int* values,int* percentage, int size) { 
    float c = 0; 
    for (int i = 0; i < size; i++) { 
        c += (float)values[i] * percentage[i]/100; 
        } 
    return c; 
}
/*
--------------------- function that get cumulated values of an array ---------------------

@Input:     - memory location of the values array
            - memory location for the cumulated values 
            - number of value to cumulate 

@Output:   void
*/


void cumulated_sum(float input[], float output[], int size) {
    output[0] = input[0];
    for (int i = 1; i < size; i++) {
        output[i] = output[i-1] + input[i];
    }
}


/*
--------------------- Structure: Cell ---------------------*/


// Define Cell structure
struct Cell {
    int CN;              // Curve Number
    float RO;        // Ruissellement
    float received;  // Infiltration
};

/*
---------------------Function to fill a matrix of Structure Cell with CN values following a heterogeneous disposition ---------------------

@Input: - memory location for the matrix of Structure Cell
        - number of soil types 
        - CN values corresponding to the soils types
        - percentage of each soil types
@Output:   void

@Description: the function fill the CN field of each Cell structure in a given Matrix according to an heteregeneous disposition, 
                the different soil types will be in 'Block', no mixing
*/


void fill_nonmixed(struct Cell matrix[ROWS][COLS],int soils_diversity, int *CN_values, int *percentages) {
    int total_cells = ROWS * COLS;

    // get the number of each CN value in the matrix based on their percentages
    int zones[soils_diversity ];

    for(int i=0; i<soils_diversity ;i++){
        zones[i]=(total_cells*percentages[i])/100;
    };
    
    

    int counters[soils_diversity ]; //initialize counters for each CN type
    for(int i=0;i<soils_diversity ;i++){
        counters[i]=0;
    }


    for(int row=0; row<ROWS ;row++){ //initialize Cell matrix
        for(int col=0; col<COLS; col++){
            matrix[row][col].CN = 0;
            matrix[row][col].RO = 0.0;
            matrix[row][col].received = 0.0;
        }}
    
    for(int row=0; row<ROWS ;row++){  //filling the matrix, respectiong the number of CN for each type of soil
        for(int col=0; col<COLS; col++){
 
            for(int i=0; i<soils_diversity ;i++){
                if( matrix[row][col].CN ==0 && counters [i]<zones[i]){
                    matrix[row][col].CN = CN_values[i];
                    counters[i]++;
                }}}}

    
    int assigned_cells = 0; //a check up to assure every cell is initialised with a CN value

   for (int i = 0; i < ROWS ; i++){
        for (int j = 0; j < COLS ; j++){
          for(int i=0; i<soils_diversity ;i++){
            if (matrix[i][j].CN ==CN_values[i]){assigned_cells++;}
            }}}

}


/*
---------------------Function to fill a matrix of Structure Cell with CN values following a homogeneous disposition ---------------------

@Input: - memory location for the matrix of Structure Cell
        - number of soil types 
        - CN values corresponding to the soils types
        - percentage of each soil types

@Output:   void

@Description: The function fills the CN field of each Cell structure in a given matrix according to a homogeneous arrangement. 
The different soil types are distributed more or less randomly, following an urban model where the most draining soils 
surround the concrete soils, based on the assumption that CN values are placed in descending order.
*/

void fill_mixed(struct Cell matrix[ROWS][COLS], int soils_diversity, int *CN_values, int *percentages) {
    int total_cells = ROWS * COLS;

    // Initialize the random number generator
    srand(time(NULL));

    // Get the number of each CN value in the matrix based on their percentages
    int zones[soils_diversity];
    for (int i = 0; i < soils_diversity; i++) {
        zones[i] = (total_cells * percentages[i]) / 100;
    }

    // Initialize the Cell matrix for no memory ambiguity
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            matrix[row][col].CN = 0;
            matrix[row][col].RO = 0.0;
            matrix[row][col].received = 0.0;
        }
    }

    // Step 1: Add small scattered concrete blocks
    while (zones[0] > 0) {
        // Generate random positions for the blocks
        int block_row = rand() % (ROWS - 5); // Reserve rows and cols to avoid overflows and stay in the matrix dimensions
        int block_col = rand() % (COLS - 5); 

        for (int i = block_row; i < block_row + 3 && zones[0] > 0; i++) {
            for (int j = block_col; j < block_col + 3 && zones[0] > 0; j++) {
                if (matrix[i][j].CN == 0) {  // Check if the cell is free
                    matrix[i][j].CN = CN_values[0];  
                    
                    zones[0]--;
                }}}}

    // Fill randomly from the biggest to the smallest CN 
    for (int d = 1; d < soils_diversity; d++) {
        while (zones[d] > 0) {
            int i = rand() % ROWS;
            int j = rand() % COLS;

            if (matrix[i][j].CN == 0) {
                matrix[i][j].CN = CN_values[d];
                zones[d]--;
            }}}
}

/*
--------------- Function calculating the amount of water entering a certain  area ---------------------

@Input:     - memory location of the structure Cell 
            - row position in the matrix
            - column position in the matrix

@Output:    - received water 

@description: The design of this function is to consider that all the runoff water at a location flows to the surrounding areas. 
24 surrounding cells are considered, with the closest neighbors providing twice as much water as the more distant cells.  */
extern float coeff_of_initial_abstraction;

float addwater(struct Cell* area, int row, int col){
    float coeff_contribution_neighbors=0.0625; // 1/16 of the total runoff
    float coeff_contribution_neighborsofneighbors=0.03125;// 1/32 of the total run off
    float addwater = 0.0f;

   if(row==0 ||row==1||row==ROWS-1 ||row==ROWS-2 || col==0 ||col==1||col==COLS-2 ||col==COLS-1){
        return 0.0; //avoid edge cases
    }
    int neighbors[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},   // relative position of neighbors
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}   
   };

    int neighborsofneighbors[16][2]={
      {-2, 0}, {2, 0}, {0, -2}, {0, 2}, 
      {-2, -2}, {2, 2}, {2, -2}, {-2, 2},
      {-2,-1}, {-1,-2}, {1,-2}, {2,-1},
      {-2,1}, {-1,2}, {1,2}, {2,1},
   };

   // adding the contribution water of neighbors'Run Off
   for (int k = 0; k < 8; k++) {

                
      addwater += ((area + neighbors[k][0]+100*neighbors[k][1])->RO )* coeff_contribution_neighbors;
   }
  // adding the contribution water of neighborsofneighbors'Run Off
   for (int k = 0; k < 16; k++) {

                    addwater += ((area + neighborsofneighbors[k][0]+100*neighborsofneighbors[k][1])->RO )* coeff_contribution_neighborsofneighbors;


   }
   return addwater;
}

/*
--------------------- Function calculating amount of run off leaving a certain area ---------------------

@Input:     - amount of rain precipitation over a period
            - amount of water entering the area from runoff from neighboring areas
            - Cn indicator value of the soil's area

@Output:    - run off of the area 
@Description: simplified version of based on proportion, using SCS Formulas*/

   float dailyrunoff(float rain, float ADDWATER, int CN){

   float S= 25400 / CN - 254; // estimate Maximal soil moisture retention
   float I_a= 0.2*S; // initial abstraction (threshold)


    // cumulative precipitation and water
    
    float Wcum=ADDWATER+rain;

    // cumulative run off
    if (Wcum<I_a){
       return 0.0f; // Return 0 if cumulative water is less than threshold
       printf("%f < %f", Wcum, I_a);
        }else{
           return  (Wcum-I_a)* (Wcum-I_a) / (Wcum+ (1-coeff_of_initial_abstraction) *S); // Return calculated runoff
    }
   }
