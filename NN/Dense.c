#include <stdlib.h>
#include "Neuron.c"

struct Dense{
    struct Neuron* neurons;
    size_t input_size;
    size_t size;
};

struct Dense CreateDense(size_t input_size, size_t size){
    struct Dense dense;
    dense.input_size = input_size;
    dense.size = size;
    dense.neurons = (struct Neuron*)malloc(size * sizeof(struct Neuron));

    for (int i = 0; i < size; ++i){
        dense.neurons[i] = CreateNeuron(input_size);
    }

    return dense;
}

void DenseActivateSigmoid(struct Dense* dense){
    for (int i = 0; i < dense->size; ++i){
        NeuronActivateSigmoid(&dense->neurons[i], dense->input_size);
    }
}

void DenseActivateReLU(struct Dense* dense){
    for (int i = 0; i < dense->size; ++i){
        NeuronActivateReLU(&dense->neurons[i], dense->input_size);
    }
}

void DenseFwd(struct Dense* dense, float* input, float* output){
    // input has layer->input_size as size and output has size layer->size
    for (int i = 0; i < dense->size; ++i){
        output[i] = NeuronFwd(&dense->neurons[i], input, dense->input_size);
    }
}

void FreeDense(struct Dense* dense){
    for (int i = 0; i < dense->size; ++i){
        FreeNeuron(&dense->neurons[i]);
    }
    free(dense->neurons);
}
