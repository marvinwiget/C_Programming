#include <stdio.h>
#include <stdlib.h>


typedef struct { int *data, rows, cols; } Matrix;

Matrix* createMatrix(int rows, int cols);
void printMatrix(Matrix *m);

int main() {
	Matrix *m1 = createMatrix(3, 3);
	m1->data[2] = 1;
	m1->data[3] = 2;
	printMatrix(m1);		
	return 0;
}

Matrix* createMatrix(int rows, int cols) {
	Matrix *m = malloc(sizeof(Matrix));
	m->data = calloc(rows*cols, sizeof(int));
	m->rows = rows;
	m->cols = cols;
	return m;
}

void printMatrix(Matrix *m) {
	printf("[");
	if (m->rows > 0) printf("\n");
	for (int i=0; i<m->rows; i++) {
		printf("[");
		for (int j=0; j<m->cols; j++) {
			if (j == m->cols-1) printf("%d",m->data[i * m->cols + j]);
			else printf("%d, ", m->data[i * m->cols + j]);
		}
		printf("]\n");
	}
	printf("]\n");
}
