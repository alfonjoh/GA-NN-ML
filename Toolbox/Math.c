
#include <stdio.h>
struct NormalizeInfo{
    float min;
    float max;
};

float Normalize(float value, struct NormalizeInfo* info){
    return (value - info->min) / (info->max - info->min);
}

float dot_product(float* a, float* b, size_t size){
    float sum = 0;
    for (int i = 0; i < size; ++i){
        sum += a[i]*b[i];
    }
    return sum;
}

void one_hot(float* arr, size_t size, size_t index){
    for (int i = 0; i < size; ++i){
        arr[i] = 0;
    }
    arr[index] = 1;
}

size_t arg_max(float* arr, size_t size){
    float max = arr[0];
    size_t index = 0;
    for (int i = 1; i < size; ++i){
        if (arr[i] > max){
            max = arr[i];
            index = i;
        }
    }
    return index;
}
