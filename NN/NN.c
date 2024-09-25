#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dense.c"
#include "../Toolbox/Arrays.c"

struct NN{
    struct Dense* layers;
    size_t layers_size;
};

struct NN CreateNN(size_t* layers, size_t layersSize){
    struct NN nn;
    nn.layers_size = layersSize-1;
    nn.layers = (struct Dense*)malloc(nn.layers_size * sizeof(struct Dense));

    nn.layers[0] = CreateDense(layers[0], layers[1]);
    for (int i = 1; i < nn.layers_size; ++i){
        nn.layers[i] = CreateDense(layers[i], layers[i+1]);
    }

    return nn;
}

void NNActivateSigmoid(struct NN* nn){
    for (int i = 0; i < nn->layers_size; ++i){
        DenseActivateSigmoid(&nn->layers[i]);
    }
}

void NNActivateReLU(struct NN* nn){
    for (int i = 0; i < nn->layers_size; ++i){
        DenseActivateReLU(&nn->layers[i]);
    }
}

void NNFwd(struct NN* nn, float* input, float* output){
    // Calculate max number of neurons in a layer for efficient memory allocation

    size_t maxNeurons = 0;
    for (int i = 0; i < nn->layers_size; ++i){
        if (nn->layers[i].size > maxNeurons){
            maxNeurons = nn->layers[i].size;
        }
    }

    float* tempInput  = (float*)malloc(maxNeurons * sizeof(float));
    float* tempOutput = (float*)malloc(maxNeurons * sizeof(float));

    memcpy(tempInput, input, nn->layers[0].input_size * sizeof(float));

    for (int i = 0; i < nn->layers_size; ++i){
        DenseFwd(&nn->layers[i], tempInput, tempOutput);

        // Swap pointers to avoid copying data
        float* temp = tempInput;
        tempInput = tempOutput;
        tempOutput = temp;
    }

    memcpy(output, tempInput, nn->layers[nn->layers_size-1].size * sizeof(float));
    free(tempInput);
    free(tempOutput);
}

void PrintNN(struct NN* nn){
    for (int i = 0; i < nn->layers_size; ++i){
        for (int j = 0; j < nn->layers[i].size; ++j){
            for (int k = 0; k < nn->layers[i].input_size; ++k){
                printf("%f-", nn->layers[i].neurons[j].weights[k]);
            }
            printf("  ");
        }
        printf("\n");
    }
}

void FreeNN(struct NN* nn){
    for (int i = 0; i < nn->layers_size; ++i){
        FreeDense(&nn->layers[i]);
    }
    free(nn->layers);
}
