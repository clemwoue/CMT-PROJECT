%This function is designed to represent the hourly intensities of rain, runoff, and infiltration.
%@Input: name referring to the data text file
%@Outputs: save the plot in png format

function visualisation_massbalance(filename,concrete,gravel,greenspace)

try
    data = readmatrix(filename, 'Delimiter', ';');
catch ME
    disp('Error reading the file:');
    disp(ME.message);
    return;
end
    % Check if data is loaded correctly
    if isempty(data)
        error('Error: The data set is empty.');
    end

    % Extract columns 
    precipitation = data(:, 1); % Column 1: Precipitation
    runoff = data(:, 2); % Column 2: Runoff
    infiltration = data(:, 3); % Column 3: Infiltration

    % Time (hours) based on the size of the data
    hours = 1:length(precipitation);

    % Prepare data for the plot
    event_matrix = [precipitation, runoff, infiltration];

    % Bar width and offsets
    bar_width = 0.3; % Width of a bar
    offsets = [-bar_width, 0, bar_width]; % Bar offsets

    % Create the figure
    figure;
    hold on;

    % Plot bars for each variable
    h1 = bar(hours + offsets(1), event_matrix(:, 1), bar_width, 'FaceColor', [0.2, 0.6, 0.8], 'DisplayName', 'Precipitation');
    h2 = bar(hours + offsets(2), event_matrix(:, 2), bar_width, 'FaceColor', [0.8, 0.2, 0.2], 'DisplayName', 'Runoff');
    h3 = bar(hours + offsets(3), event_matrix(:, 3), bar_width, 'FaceColor', [0.4, 0.8, 0.4], 'DisplayName', 'Infiltration');

    % Title and axes
    xlabel('Hours');
    ylabel('Intensity (mm/h)');
title(sprintf('%d %% concrete, %d %% gravel, %d %% greenspace', concrete, gravel, greenspace));

    % Legend
    legend([h1, h2, h3], {'Precipitation', 'Runoff', 'Infiltration'}, 'Location', 'best');

    % Adjust Y-axis limits for better visualization
    ylim([0, max(event_matrix(:)) * 1.1]); % Add a 10% margin

    hold off;
 %save the figure in png format in the Results folder
    [~, baseFilename, ~] = fileparts(filename);
    savePath = sprintf('../Results/%s.png', baseFilename);  

    saveas(gcf, savePath);
end


