#include <stdio.h>
#include <stdlib.h>

/*This file contain the function permitting exporting and importing datas, used in the 'models.c' file*/



//---------function to import precipitation over a day-------
//get the rain datas simulated over one day (amount of rain per hour over N hours [mm/h])
//  return an int 

int read_precipitation(const char* filename, float *memoryplace_rain_datas ) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        return 1;
    }

    int count = 0;  // Compteur pour les lignes contenant des nombres
    float value;

    // Lire chaque ligne du fichier
    while (fscanf(file, "%f", &value) == 1) {
        // Si un nombre a été trouvé, on le stocke dans le tableau et on incrémente le compteur
        memoryplace_rain_datas[count] = value;
        count++;
    }
    fclose(file);
    return count;
}

//-----------------------Function to export the data from the "based_on_proportion" function---------------
// functions to export datas as text file such as 
// precipitation rate [mm/h];runoff rate [mm/h];infiltration rate [mm/h]

void export_intensities(float P[], float Q[], float I[], int size, const char* output_filename) {
    FILE *file = fopen(output_filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "precipitation;runoff;infiltration\n");
    for (int i = 0; i < size; i++) {
        fprintf(file, "%.2f;%.2f;%.2f\n", P[i], Q[i], I[i]);
    }

    fclose(file);
}

/*
---------------------Function to export a matrix of struct Cell to a csv file ---------------------

@Input:     - name for the output file
            - memory location of the matrix to export

*/

// Function to export a matrix of struct Cell to a csv file 
void export_matrix_to_csv(const char *filename, struct Cell matrix[ROWS][COLS]) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "Row,Col,CN,Runoff,Received\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fprintf(file, "%d,%d,%d,%.2f,%.2f\n", i, j, matrix[i][j].CN, matrix[i][j].RO, matrix[i][j].received);
        }
    }

    fclose(file);
    printf("Matrix exported to '%s'.\n", filename);
}




