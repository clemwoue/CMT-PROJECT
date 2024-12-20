%@Description: This function represents the daily infiltration according to the
%percentage of concrete.
%Input: - the minimum percentage of concrete
%       - the maximum percentage of concrete
%       - the width of the land
%       - the length of the land
%Outputs: Saves the plot in PNG format

function infiltration_vs_concrete(pmin, pmax, width_land, length_land)
    step = 10;
    % Define the percentages of CN (concrete)
    percentages = pmin:step:pmax;
    
    % Initialize file names for "nonmixed," "mixed," and "proportion"
    nonmixed_files = strcat("../Internal/nonmixed_", string(percentages), ".txt");
    mixed_files = strcat("../Internal/mixed_", string(percentages), ".txt");
    proportion_files = strcat("../Internal/proportion_", string(percentages), ".txt");  % Files for the third curve
    
    % Load and sum infiltration data for "nonmixed," "mixed," and "proportion"
    infiltration_nonmixed = load_n_sum(nonmixed_files);
    infiltration_mixed = load_n_sum(mixed_files);
    infiltration_proportion = load_n_sum(proportion_files);  % Load the third curve

    % Multiply each value by the number of cells in the land
    infiltration_proportion = infiltration_proportion * width_land * length_land;

    % Plot the curves with specific colors
    figure;
    plot(percentages, infiltration_nonmixed, '-o', 'Color', [0.83, 0.13, 0.18], 'DisplayName', 'Non-mixed'); 
    hold on;
    plot(percentages, infiltration_mixed, '-s', 'Color', [0.55, 0.71, 0.0], 'DisplayName', 'Mixed'); 
    plot(percentages, infiltration_proportion, '-^', 'Color', [0.0, 0.45, 0.74], 'DisplayName', 'Proportion'); % Third curve
    
    hold off;
    
    % Add labels and a legend
    xlabel('Percentage of Concrete (%)');
    ylabel('Total Infiltration (mm)');
    title('Infiltration vs Percentage of Concrete');
    legend;
    grid on;

    % Save the plot
    saveas(gcf, '../Results/infiltration_vs_concrete.png');
end