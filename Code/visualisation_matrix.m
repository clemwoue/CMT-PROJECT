%This function is designed to represent the disposition of soil types, 
% charachterized by their CN values, in a land.

%@Input: name referring to the data csv file corresponding to a matrix and
%assigned values organized such as : "Row,Col,CN,Runoff,Received" in each
%line
%@Outputs: save the plot in png format 
function visualisation_matrix(filename)


    % Load data from the CSV file
    try
        data = readmatrix(filename, 'Range', 2); % Ignore the header
    catch ME
        disp('Error reading the file:');
        disp(ME.message);
        return;
    end

    % Check if data is loaded correctly
    if isempty(data)
        error('Error: Data was not loaded correctly.');
    end

    % Extract the dimensions of the matrix
    ROWS = max(data(:, 1)) + 1; % Max row index + 1 
    COLS = max(data(:, 2)) + 1; % Max column index + 1 

    % Create the CN matrix
    %We are only interested in the third column which is the CN value
    CN_matrix = reshape(data(:, 3), COLS, ROWS)';

    % Create a custom colormap: gray, dark orange-gray, dark green
    custom_colormap = [
        0.5, 0.5, 0.5;   % Gray for concrete
        0.6, 0.4, 0.2;   % Dark orange-gray for gravel
        0.0, 0.4, 0.0    % Dark green for grass
    ];

    % Associate CN values with colors
    CN_values = [98, 86, 77];

    % Normalize CN to map to the colormap
    norm_CN_matrix = interp1(CN_values, 1:length(CN_values), CN_matrix, 'nearest', 'extrap');

    % Display
    figure;
    imagesc(norm_CN_matrix);
    colormap(custom_colormap); % Apply the custom colormap
    axis equal tight;
    title('Disposition of soil types');
    xlabel('Width [hm](hectometer)');
    ylabel('Width [hm](hectometer)');

    % Create custom legend with color squares
    hold on;

    % Draw colored patches for legend
    % Patch for Concrete 
    patch([0 1 1 0]*0.1, [0 0 1 1]*0.1, [0.5, 0.5, 0.5 ], 'EdgeColor', 'none');
    % Patch for Gravel 
    patch([0 1 1 0]*0.1, [0 0 1 1]*0.1 + 1.2, [0.6, 0.4, 0.2], 'EdgeColor', 'none');
    % Patch for Grass 
    patch([0 1 1 0]*0.1, [0 0 1 1]*0.1 + 2.4, [0.0, 0.4, 0.0], 'EdgeColor', 'none');

    % Add a legend with text
    legend({'Concrete', 'Gravel', 'Greenspace'}, 'Location', 'best');

    % Save the figure
    [~, baseFilename, ~] = fileparts(filename);
    savePath = sprintf('../Results/%s.png', baseFilename);  

    saveas(gcf, savePath);


end



