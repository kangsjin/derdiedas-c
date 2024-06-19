#include <stdio.h>
#include <stdlib.h>

int *generateRandomNumberArray(int maximum, int n) {
    int *arr = (int *)malloc(n * sizeof(int));
    int *randomArray = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < maximum; i++) {
        arr[i] = i;
    }

    for (int i = 0; i < n; i++) {
        int r = rand() % maximum;
        randomArray[i] = arr[r];
        arr[r] = arr[maximum - 1];
        maximum--;
    }

    free(arr);
    return randomArray;
}

// Function to generate an array of unique random numbers
void generateUniqueRandomNumbers(int *arr, int length, int min, int max) {
    int range = max - min + 1;
    if (length > range) {
        printf("Length of array is greater than the range of unique numbers available.\n");
        return;
    }

    int *temp = malloc(range * sizeof(int));
    for (int i = 0; i < range; i++) {
        temp[i] = min + i;
    }

    // Shuffle the temporary array
    for (int i = range - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tempValue = temp[i];
        temp[i] = temp[j];
        temp[j] = tempValue;
    }

    // Copy the first 'length' elements to the output array
    for (int i = 0; i < length; i++) {
        arr[i] = temp[i];
    }

    free(temp);
}
