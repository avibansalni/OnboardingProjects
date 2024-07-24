#include <iostream>
#include <cstring>


void* malloc(size_t size) {
    return operator new(size);
}

void* calloc(size_t num, size_t size) {
    void* ptr = operator new(num * size);
    std::memset(ptr, 0, num * size);
    return ptr;
}

void free(void* ptr) {
    operator delete(ptr);
}

void* realloc(void* ptr, size_t size) {
    void* newPtr = operator new(size);
    std::memcpy(newPtr, ptr, size);
    operator delete(ptr);
    return newPtr;
}

int main() {
    
    int* arr1 = static_cast<int*>(malloc(5 * sizeof(int)));
    for (int i = 0; i < 5; ++i) {
        arr1[i] = i + 1;
    }
    for (int i = 0; i < 5; ++i) {
        std::cout << arr1[i] << " ";
    }
    std::cout << std::endl;
    free(arr1);

    int* arr2 = static_cast<int*>(calloc(5, sizeof(int)));
    for (int i = 0; i < 5; ++i) {
        std::cout << arr2[i] << " ";
    }
    std::cout << std::endl;

    int* arr3 = static_cast<int*>(realloc(arr2,  10 * sizeof(int)));

    for (int i = 0; i < 10; ++i) {
        arr3[i] = i + 1;
    }

    for (int i = 0; i < 10; ++i) {
        std::cout << arr3[i] << " ";
    }

    free(arr3);
    return 0;
}