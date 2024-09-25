#include <stdio.h>
#include "../../GA/GA.c"
#include "../../CSV/CSV.c"

struct ExecData{
    struct Table* xs;
    float* ys;
};

float fitness(struct NN* nn, void* context){
    struct ExecData* exec_data = (struct ExecData*)context;
    struct Table* xs = exec_data->xs;
    float* ys = exec_data->ys;

    // Test the model on each row in the dataset
    int correct = 0;
    float prediction[4] = {0};
    for (int i = 0; i < xs->rows_size; ++i){
        NNFwd(nn, xs->data[i], prediction);

        size_t prediction_index = arg_max(prediction, 4);
        size_t target_index = arg_max(ys + i * 4, 4);

        if (prediction_index == target_index){
            correct++;
        }
    }

    return 0.0001 + (float)correct / (float)xs->rows_size;
}

int main(){
    struct Table* xs = (struct Table*)malloc(sizeof(struct Table));
    *xs = ReadTableFromCSV("train.csv");

    // Price range
    float* predictions = (float*)malloc(xs->rows_size * sizeof(float));
    DropColumn(xs, 20, predictions);

    NormalizeTable(xs);

    // Convert prices range to one hot
    float* ys = (float *)malloc(xs->rows_size * 4 * sizeof(float));
    for (int i = 0; i < xs->rows_size; ++i){
        one_hot(ys + i * 4, 4, (size_t)predictions[i]);
    }

    struct ExecData train_data = {xs, ys};

    // Define layers
    size_t layers[] = {xs->columns_size, 32, 32, 12, 4};

    struct GA ga = InitGA(
            0.001f,
            100,
            layers,
            sizeof(layers) / sizeof(size_t),
            &train_data,
            fitness);


    printf("Begin training\n");
    GATrain(&ga, 10000);

    FreeTable(xs);
    free(xs);
    free(predictions);
    free(ys);


    return 0;
}
