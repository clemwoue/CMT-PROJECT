% A function that visualizes intensity and cumulative infiltration
% differences of two different datasets of infiltration
%@Input: - two strings containing the names of the files with the data
%@Output: saves the plot in PNG format
function compare_infiltration(file1, file2)

mixed_data = readmatrix(file1);
nonmixed_data = readmatrix(file2);
% Check if the files are correctly read
if isempty(mixed_data) || isempty(nonmixed_data)
   error('Error: The files are empty or do not exist.');
end
% Create a time vector (assuming a constant time step of one unit)
time = (1:length(mixed_data))';
% Extract infiltration data
mixed_infiltration = mixed_data(:,3);
nonmixed_infiltration = nonmixed_data(:,3);

% Plot the data as bar plots
figure;
hold on;
% Bar plot for "mixed" soil
bar(time-0.15, mixed_infiltration, 0.3, 'FaceColor', [0.55, 0.71, 0.0], 'DisplayName', 'Mixed Soil'); 
% Bar plot for "nonmixed" soil
bar(time + 0.15, nonmixed_infiltration, 0.3, 'FaceColor', [0.83, 0.13, 0.18], 'DisplayName', 'Non-Mixed Soil'); 
% Add legends and annotations
title('Infiltration Per Hour');
xlabel('Time (hours)');
ylabel('Infiltration (mm)');
legend('Mixed Land', 'Non-Mixed Land');
legend('show', 'Location', 'best');
grid on;
% Set axis limits
xlim([1 length(mixed_data)]); % Limit the time axis
ylim([0 max(mixed_infiltration)]); % Limit the infiltration axis to the max of "mixed output" data
hold off;

saveas(gcf, '../Results/intensity_comparison.png');

% Calculate the cumulative infiltration over the previous hours
cumulative_mixed_infiltration = cumsum(mixed_infiltration);
cumulative_nonmixed_infiltration = cumsum(nonmixed_infiltration);
% Create the figure for cumulative curves
figure;
hold on;
% Cumulative curve for "mixed" soil
plot(time, cumulative_mixed_infiltration, '-o', 'LineWidth', 2, 'DisplayName', 'Cumulative Infiltration Mixed Soil', 'Color', [0.55, 0.71, 0.0]);
% Cumulative curve for "nonmixed" soil
plot(time, cumulative_nonmixed_infiltration, '-s', 'LineWidth', 2, 'DisplayName', 'Cumulative Infiltration Non-Mixed Soil', 'Color', [0.83, 0.13, 0.18]);
% Add legends and annotations
title('Cumulative Infiltration Over Time');
xlabel('Time (hours)');
ylabel('Cumulative Infiltration (mm)');
legend('Mixed Land', 'Non-Mixed Land');
grid on;
hold off;

saveas(gcf, '../Results/cumulation_comparison.png');
end
