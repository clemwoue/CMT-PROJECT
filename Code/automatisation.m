

%              CODE TO AUTOMATIZE THE PROGRAM

% Name of the C source file and the compiled program
sourceFile = 'final_main.c';
compiledProgram = 'finalmain.out';

% Compile the C code
[status, cmdout] = system(sprintf('gcc %s  -o %s', sourceFile, compiledProgram));
assert(status == 0, 'Error during C code compilation:\n%s', cmdout);


% Load the inputs from the file (ignoring the text after the semicolon)
fileID = fopen('../Data/input.txt', 'r');  % Open file for reading
data = textscan(fileID, '%f%*s', 'Delimiter', ';');  % Read numeric values only
fclose(fileID);  % Close the file

% Extract only numeric data and not their explaination
inputs = data{1};  
% Convert the values from the 'inputs' vector to string format for the command
inputs_str = arrayfun(@(x) num2str(x), inputs, 'UniformOutput', false);


% Run the C program
cmd = sprintf('./%s %s  ', compiledProgram, strjoin(inputs_str, ' ')); % Create the command

% Execute the system command
[status, cmdout] = system(cmd);


% Check if the command was executed successfully
if status ~= 0
    error('Error executing C program:\n %s', cmdout);
end
% Launch MATLAB visualizations
disp('Launching MATLAB visualizations...');


try
   visu_main;
catch
   error('Error during the execution of the program');
end
disp('Process completed successfully.');
