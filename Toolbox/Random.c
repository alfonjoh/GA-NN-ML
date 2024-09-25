
#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <math.h>

float randf(){
    return (float)rand() / (float)RAND_MAX;
}

float randfArr(float* arr, size_t s){
    for (int i = 0; i < s; ++i){
        arr[i] = rand();
    }
}

float randn() {
    // Generate two independent random floats between 0 and 1
    float U1 = randf();
    float U2 = randf();

    // Apply the Box-Muller transform
    float R = sqrt(-2.0f * log(U1));  // sqrt(-2 * ln(U1))
    float Theta = 2.0f * M_PI * U2;   // 2 * PI * U2

    // Return the normally distributed random number (Z0)
    return R * cos(Theta);
}

float* randnArr(float* arr, size_t s){
    for (int i = 0; i < s; ++i){
        arr[i] = rand();
    }
}