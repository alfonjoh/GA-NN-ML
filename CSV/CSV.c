#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Table.c"

struct Table ReadTableFromCSV(char* file_name){
    FILE* file = fopen(file_name, "r");
    if (file == NULL){
        printf("Could not open file %s\n", file_name);
        exit(1);
    }

    size_t max_line_size = 512;
    size_t max_columns = 128;

    char* line = (char*)malloc(max_line_size * sizeof(char));
    fgets(line, (int)max_line_size, file);
    char* token = strtok(line, ",");
    size_t columns_size = 0;
    char** columns = (char**)malloc(max_columns * sizeof(char*));
    while (token != NULL){
        columns[columns_size] = (char*)malloc(strlen(token) * sizeof(char));
        strcpy(columns[columns_size], token);
        token = strtok(NULL, ",");
        columns_size++;
    }

    free(line);

    struct Table table = CreateTable();
    AddColumns(&table, columns, columns_size);

    // Free columns
    for (int i = 0; i < columns_size; ++i){
        free(columns[i]);
    }
    free(columns);

    float* row = (float*)malloc(columns_size * sizeof(float));
    while (fgets(line, (int)max_line_size, file)){
        token = strtok(line, ",");
        for (int i = 0; i < columns_size; ++i){
            row[i] = strtof(token, NULL);
            token = strtok(NULL, ",");
        }
        TableAddRow(&table, row);
    }
    free(row);

    fclose(file);
    return table;
}

