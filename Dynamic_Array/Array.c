#include <stdio.h>
#include <stdlib.h>

typedef struct { int *data, size, capacity; } Array;

void arr_init(Array *arr, int n); // initializes the array with the starting size
void arr_append(Array *arr, int x); // adds an integer at the end of the array
void arr_appendArray(Array *arr, int *other, int size); // adds the element of another array at the end of the array
void arr_removeAt(Array *arr, int index); // removes the integer at the index
int  arr_pop(Array *arr); // removes the integer at the end of the array
void arr_print(Array *arr); // prints the whole array
void arr_destroy(Array *arr); // destroys the array


int main() {
	Array arr;
	arr_init(&arr, 5);
	arr_print(&arr);

	arr_append(&arr, 10);
	arr_append(&arr, 5);
	arr_print(&arr);

	int newArray[] = {4,3,2,1};
	int newArraySize = sizeof(newArray) / sizeof(int);
	


	arr_appendArray(&arr, &newArray[0], newArraySize);
	arr_print(&arr);

	


	arr_destroy(&arr);
	return 0;
}


void arr_init(Array *arr, int n) {
	arr->size = 0;
	arr->capacity = n;
	arr->data = malloc(arr->capacity * sizeof(int));
}

void arr_append(Array *arr, int x) {
	if (arr->size == arr->capacity) {
		arr->capacity *= 2;
		arr->data = malloc(arr->capacity * sizeof(int));
	}

	arr->data[arr->size] = x;
	arr->size++;
}

void arr_appendArray(Array *arr, int *other, int size) {
	for (int i=0; i<size; i++) {
		arr_append(arr, other[i]);
	}
}

void arr_removeAt(Array *arr, int index) {
	return;
}

int arr_pop(Array *arr) {
	int temp = arr->data[arr->size];
	arr->size--;
	return temp;
}

void arr_print(Array *arr) {
	printf("[");
	for (int i=0; i<arr->size; i++) {
		if (i == arr->size-1) {
			printf("%d", arr->data[i]);
		} else {
			printf("%d, ", arr->data[i]);
		}
	}
	printf("]\n");
}

void arr_destroy(Array *arr) {
	free(arr->data);
}

