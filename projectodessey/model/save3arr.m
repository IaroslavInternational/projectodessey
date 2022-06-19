function save3arr(name, data)
    formatSpec = '%.4f,%.4f,%.4f\n';

    fileID = fopen(name, 'w');
    fprintf(fileID, formatSpec, data);
    fclose(fileID);
end