#include "Activation.c"
#include "../Toolbox/Random.c"
#include "../Toolbox/Math.c"
#include <stdlib.h>


struct Neuron{
    float* weights;
    float bias;

};

struct Neuron CreateNeuron(size_t size){
    struct Neuron n;
    n.weights = (float*)calloc(size, sizeof(float));
    return n;
}

void NeuronActivateSigmoid(struct Neuron* neuron, size_t inputSize){
    for (int i = 0; i < inputSize; ++i){
        neuron->weights[i] = Sigmoid(randn());
    }
    //neuron->bias = randn();
    neuron->bias = 0;
}

void NeuronActivateReLU(struct Neuron* neuron, size_t inputSize){
    for (int i = 0; i < inputSize; ++i){
        neuron->weights[i] = ReLU(randn());
    }
    //neuron->bias = randn();
    neuron->bias = 0;
}

float NeuronFwd(struct Neuron* neuron, float* input, size_t inputSize){
    return dot_product(neuron->weights, input, inputSize) + neuron->bias;
}

void FreeNeuron(struct Neuron* node){
    free(node->weights);
}
