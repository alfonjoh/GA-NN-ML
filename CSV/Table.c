#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

size_t capacity_steps[] = {4, 8, 24, 64, 82, 128, 196, 312, 512, 1024, 2048 };
size_t increment_after_capacity_steps = 2048;

struct Table{
    char** columns;
    float** data;
    struct NormalizeInfo* normalize_info;
    bool normalized;
    size_t rows_size;
    size_t rows_capacity;
    size_t columns_size;
};

struct Table CreateTable(){
    struct Table table;

    table.rows_capacity = capacity_steps[0];
    table.rows_size = 0;

    table.columns_size = 0;

    table.data = (float**)malloc(table.rows_capacity * sizeof(float*));

    table.normalized = false;

    return table;
}

void FreeTable(struct Table* table){
    for (size_t i = 0; i < table->columns_size; ++i){
        free(table->columns[i]);
    }
    free(table->columns);
    for (size_t i = 0; i < table->rows_size; ++i){
        free(table->data[i]);
    }
    free(table->data);
}

void AddColumns(struct Table* table, char** columns, size_t columns_size){
    table->columns_size = columns_size;
    table->columns = (char**)malloc(columns_size * sizeof(char*));

    for (size_t i = 0; i < columns_size; ++i){
        table->columns[i] = (char*)malloc(strlen(columns[i]) * sizeof(char));
        strcpy(table->columns[i], columns[i]);
    }
}

size_t NextCapacity(struct Table* table){
    for (size_t i = 0; i < sizeof(capacity_steps)/sizeof(size_t)-1; ++i){
        if (table->rows_capacity == capacity_steps[i]){
            return capacity_steps[i+1];
        }
    }
    return table->rows_capacity + increment_after_capacity_steps;
}

void TableAddRow(struct Table* table, float* row){
    if (table->rows_size == table->rows_capacity){
        table->rows_capacity += increment_after_capacity_steps;
        table->data = (float**)realloc(table->data, table->rows_capacity * sizeof(float*));
    }
    table->data[table->rows_size] = (float*)malloc(table->columns_size * sizeof(float));
    memcpy(table->data[table->rows_size], row, table->columns_size * sizeof(float));
    table->rows_size++;
}

void NormalizeTable(struct Table* table){
    if (table->normalized){
        printf("Table already normalized\n");
        return;
    }
    table->normalized = true;

    table->normalize_info = (struct NormalizeInfo*)malloc(table->columns_size * sizeof(struct NormalizeInfo));

    for (size_t i = 0; i < table->columns_size; ++i){
        table->normalize_info[i].min = table->data[0][i];
        table->normalize_info[i].max = table->data[0][i];

        for (size_t j = 1; j < table->rows_size; ++j){
            if (table->data[j][i] < table->normalize_info[i].min){
                table->normalize_info[i].min = table->data[j][i];
            }
            if (table->data[j][i] > table->normalize_info[i].max){
                table->normalize_info[i].max = table->data[j][i];
            }
        }

        for (size_t j = 0; j < table->rows_size; ++j){
            table->data[j][i] = Normalize(table->data[j][i], &table->normalize_info[i]);
        }
    }
}

void DenormalizeTable(struct Table* table){
    if (!table->normalized){
        printf("Table not normalized\n");
        return;
    }
    table->normalized = false;

    for (size_t i = 0; i < table->columns_size; ++i){
        for (size_t j = 0; j < table->rows_size; ++j){
            table->data[j][i] = table->data[j][i] * (table->normalize_info[i].max - table->normalize_info[i].min) + table->normalize_info[i].min;
        }
    }
}

void DropColumn(struct Table* table, size_t index, float* column){
    free(table->columns[index]);
    memcpy(table->columns[index], table->columns[index+1], (table->columns_size - index - 1) * sizeof(char*));

    char** temp_columns = table->columns;
    table->columns = (char**)realloc(table->columns, (table->columns_size - 1) * sizeof(char*)); // NOLINT(*-suspicious-realloc-usage)
    if (table->columns == NULL){
        free(temp_columns);
    }

    for (size_t i = 0; i < table->rows_size; ++i){
        if (column != NULL){
            column[i] = table->data[i][index];
        }
        for (size_t j = index; j < table->columns_size-1; ++j){
            table->data[i][j] = table->data[i][j+1];
        }
        float* temp_data = table->data[i];
        table->data[i] = (float*)realloc(table->data[i], (table->columns_size - 1) * sizeof(float)); // NOLINT(*-suspicious-realloc-usage)
        if (table->data[i] == NULL){
            free(temp_data);
        }
    }
    table->columns_size--;
}

void TableHead(struct Table* table){
    for (size_t i = 0; i < table->columns_size; ++i){
        printf("%s ", table->columns[i]);
    }
    printf("\n");
    for (size_t i = 0; i < table->rows_size && i < 5; ++i){
        for (size_t j = 0; j < table->columns_size; ++j){
            printf("%f ", table->data[i][j]);
        }
        printf("\n");
    }
}
