% Function to load and sum the infiltrations
% Input: name of the data file
% Output: the sum of the datas from the file

function tot = load_n_sum(fichiers)
    tot = zeros(1, length(fichiers));
    for i = 1:length(fichiers)
        try
            data = readmatrix(fichiers(i), 'OutputType', 'double');
            if size(data, 2) >= 3
                tot(i) = sum(data(:, 3)); % sum infiltration column
            else
                warning("Le fichier %s n'a pas assez de colonnes.", fichiers(i));
                tot(i) = NaN;
            end
        catch
            warning("Erreur lors du chargement du fichier %s.", fichiers(i));
            tot(i) = NaN;
        end
    end
end
