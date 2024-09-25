
#include "../NN/NN.c"
#include <stdio.h>

#define FITNESS_FUNCTION float (*fitness_function)(struct NN* nn, void* context)

struct GA{
    float mutation_rate;
    size_t population_size;
    struct NN* population;
    struct NN* new_population;
    size_t* layers;
    size_t layers_size;
    float* fitness;
    float *cumulative_probabilities;
    float* probabilities;

    void* context;
    FITNESS_FUNCTION;

    float best_fitness;
    float average_fitness;
};

struct GA InitGA(float mutation_rate, size_t population_size, size_t* layers, size_t layers_size, void* context, FITNESS_FUNCTION){
    struct GA ga;
    ga.mutation_rate = mutation_rate;
    ga.population_size = population_size;
    ga.population = (struct NN*)malloc(population_size * sizeof(struct NN));
    ga.new_population = (struct NN*)malloc(population_size * sizeof(struct NN));
    ga.layers = (size_t*)malloc(layers_size * sizeof(size_t));
    ga.layers_size = layers_size;
    memcpy(ga.layers, layers, layers_size * sizeof(size_t));
    ga.fitness = (float*)calloc(population_size, sizeof(float));
    ga.cumulative_probabilities = (float*)malloc(population_size * sizeof(float));
    ga.probabilities = (float*)malloc(population_size * sizeof(float));
    ga.context = context;
    ga.fitness_function = fitness_function;

    for (int i = 0; i < ga.population_size; ++i){
        ga.population[i] = CreateNN(ga.layers, ga.layers_size);
        NNActivateSigmoid(&ga.population[i]);
    }

    return ga;
}

void FreeGA(struct GA* ga){
    for (int i = 0; i < ga->population_size; ++i){
        FreeNN(&ga->population[i]);
    }
    for (int i = 0; i < ga->population_size; ++i){
        FreeNN(&ga->new_population[i]);
    }
    free(ga->population);
    free(ga->new_population);
    free(ga->layers);
    free(ga->cumulative_probabilities);
    free(ga->fitness);
    free(ga->probabilities);
}

void GAUpdateFitness(struct GA* ga){
    for (int i = 0; i < ga->population_size; ++i){
        ga->fitness[i] = ga->fitness_function(&ga->population[i], ga->context);
        if (i == 0 || ga->fitness[i] > ga->best_fitness){
            ga->best_fitness = ga->fitness[i];
        }
    }

    float sum = 0;
    for (int i = 0; i < ga->population_size; ++i){
        sum += ga->fitness[i];
    }

    ga->average_fitness = sum / (float)ga->population_size;

    float cumulative = 0;
    for (int i = 0; i < ga->population_size; ++i){
        ga->probabilities[i] = ga->fitness[i] / sum;
        cumulative += ga->probabilities[i];
        ga->cumulative_probabilities[i] = cumulative;
    }
}

int GetRandParent(struct GA* ga){
    float r = randf();
    int left = 0, right = ga->population_size - 1;
    while (left < right){
        int mid = (left + right) / 2;
        if (r < ga->cumulative_probabilities[mid]){
            right = mid;
        } else {
            left = mid + 1;
        }
    }

    return left;
}

struct NN NNCrossover(struct GA* ga, struct NN* nn1, struct NN* nn2){
    struct NN new_nn = CreateNN(ga->layers, ga->layers_size);

    for (int i = 0; i < nn1->layers_size; ++i){
        for (int j = 0; j < nn1->layers[i].size; ++j){
            struct Neuron* neuron1 = &nn1->layers[i].neurons[j];
            struct Neuron* neuron2 = &nn2->layers[i].neurons[j];
            struct Neuron* new_neuron = &new_nn.layers[i].neurons[j];

            if (randf() < 0.5f){
                new_neuron->bias = neuron1->bias;
                for (int k = 0; k < nn1->layers[i].input_size; ++k){
                    new_neuron->weights[k] = neuron1->weights[k];
                }
            } else {
                new_neuron->bias = neuron2->bias;
                for (int k = 0; k < nn1->layers[i].input_size; ++k){
                    new_neuron->weights[k] = neuron2->weights[k];
                }
            }
        }
    }

    return new_nn;
}

void GASelectNewPopulation(struct GA* ga){
    for (int i = 0; i < ga->population_size; ++i){
        int parent1 = GetRandParent(ga);
        int parent2 = GetRandParent(ga);

        ga->new_population[i] = NNCrossover(ga, &ga->population[parent1], &ga->population[parent2]);
    }

    for (int i = 0; i < ga->population_size; ++i){
        FreeNN(&ga->population[i]);
    }

    struct NN* temp = ga->population;
    ga->population = ga->new_population;
    ga->new_population = temp;

}

void GAMutate(struct GA* ga){
    for (int i = 0; i < ga->population_size; ++i){
        for (int j = 0; j < ga->population[i].layers_size; ++j){
            for (int k = 0; k < ga->population[i].layers[j].size; ++k){
                if (randf() < ga->mutation_rate){
                    ga->population[i].layers[j].neurons[k].bias = randn();
                    for (int l = 0; l < ga->population[i].layers[j].input_size; ++l){
                        float rnd = randn();
                        ga->population[i].layers[j].neurons[k].weights[l] = rnd;
                    }
                }
            }
        }
    }
}

void GANextGeneration(struct GA* ga){
    GAUpdateFitness(ga);
    GASelectNewPopulation(ga);
    GAMutate(ga);
}

void GATrain(struct GA* ga, size_t generations){
    for (int i = 0; i < generations; ++i){
        GANextGeneration(ga);
        printf("Generation %d, Best fitness: %f, Average fitness: %f\n", i, ga->best_fitness, ga->average_fitness);
    }
}
