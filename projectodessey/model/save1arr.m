function save1arr(name, data)
    formatSpec = '%.4f\n';

    fileID = fopen(name, 'w');
    fprintf(fileID, formatSpec, data);
    fclose(fileID);
end