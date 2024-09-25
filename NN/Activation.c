#include <math.h>

float Sigmoid(float f){
    return (float)(1/(1+exp(-f)));
}

float ReLU(float f){
    return (float)fmax(0.f, f);
}
