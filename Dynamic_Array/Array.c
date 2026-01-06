#include <stdio.h>
#include <stdlib.h>

typedef struct { int *data, size, capacity; } Array;

void arr_init(Array *arr, int n); // initializes the array with the starting size
void arr_append(Array *arr, int x); // adds an integer at the end of the array
void arr_removeAt(Array *arr, int index); // removes the integer at the index
int  arr_pop(Array *arr); // removes the integer at the end of the array
void arr_destroy(Array *arr); // destroys the array


int main() {
	Array arr;
	arr_init(&arr, 5);
	arr_append(&arr, 10);


	arr_destroy(&arr);
	return 0;
}


void arr_init(Array *arr, int n) {
	arr->size = 0;
	arr->capacity = n;
	arr->data = malloc(arr->capacity * sizeof(int));
}

void arr_append(Array *arr, int x) {
	return;
}

void arr_removeAt(Array *arr, int index) {
	return;
}

int arr_pop(Array *arr) {
	return 0;
}

void arr_destroy(Array *arr) {
	free(arr->data);
}

