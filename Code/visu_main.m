clear;
clc;

%Load the inputs (ignoring the text after the semicolon)
fileID = fopen('../Data/input.txt', 'r');  % Open file for reading
data = textscan(fileID, '%f%*s', 'Delimiter', ';');  % Read numeric values only
fclose(fileID);  % Close the file

% Extract only numeric data situated in the first semi-column of each line
inputs = data{1};  

%%%%%%%%%% VISUALISATION QUESTION 1: DIFFERENT PROPORTION OF SOIL TYPES %%%%%%%%%%%%%%%%%%%%%


% get the right path of the text files
file_scenario1= '../Internal/intensity1.txt';
file_scenario2= '../Internal/intensity2.txt';
visualisation_massbalance(file_scenario1,inputs(1),inputs(2), inputs(3)); %adding the percentage of soil type from the inputs
visualisation_massbalance(file_scenario2, inputs(4), inputs(5),inputs(6));


%%%%%%%%%   VISUALISATION QUESTION 2: DIFFERENT DISPOSITION OF SOIL TYPES %%%%%%%%%%%%%%%%%%%%%

% visualisation of disposition of soil types 

file_nonmixed = '../Internal/nonmixed.csv';
file_mixed = '../Internal/mixed.csv';


visualisation_matrix(file_nonmixed);
visualisation_matrix(file_mixed);

% comparison of infiltration of two differently disposed land


file1='../Internal/infiltration_rate_mixed.csv';
file2='../Internal/infiltration_rate_nonmixed.csv';

compare_infiltration(file1,file2);


%%%%%%%%% VISUALISATION QUESTION 3: MODEL COMPARISON


infiltration_vs_concrete(inputs(9),inputs(10),inputs(7), inputs(8));
 
