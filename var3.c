#include "omp.h"
#include "time.h"
#include <stdio.h>
#include <stdlib.h>

#define N 4096
#define THREADS 12
#define REPEATS 100

float **create_random(void) {
  int min = 1;
  int max = 10;
  float **res = malloc(N * sizeof(float *));
  for (int i = 0; i < N; i++) {
    res[i] = malloc(N * sizeof(float));
    for (int j = 0; j < N; j++) {
      res[i][j] = min + (float)rand() / RAND_MAX * (max - min);
    }
  }
  return res;
}

float **create_static(float num) {
  float **res = malloc(N * sizeof(float *));
  for (int i = 0; i < N; i++) {
    res[i] = malloc(N * sizeof(float));
    for (int j = 0; j < N; j++) {
      res[i][j] = num;
    }
  }
  return res;
}

void free_arr(float **arr) {
  for (int i = 0; i < N; i++) {
    free(arr[i]);
  }
  free(arr);
}
void print_arr(char *name, float **arr) {
  printf("Array: %s\n", name);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%.3f ", arr[i][j]);
    }
    printf("\n");
  }
}

int main() {
  srand(time(NULL));
  omp_set_num_threads(THREADS);
  int i, j, rank, size;
  float **A = create_random();
  float **B = create_random();
  float **C = create_random();
  float **Y = create_random();

  double full_time = 0;
  for (int repeats = 0; repeats < REPEATS; repeats++) {
    double start_time = omp_get_wtime();
#pragma omp parallel for private(i, j)
      for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
          Y[i][j] = (A[i][j] + B[i][j] + C[i][j] / B[i][j]) * C[i][j];
        }
      }
    double end_time = omp_get_wtime();
    full_time += end_time - start_time;
  }
  printf("N: %d; Threads: %d; Repeats: %d; Avg. time: %f\n", N, THREADS,
         REPEATS, full_time / REPEATS);

  // print_arr("A", A);
  // print_arr("B", B);
  // print_arr("C", C);
  // print_arr("Y", Y);

  free_arr(A);
  free_arr(B);
  free_arr(C);
  free_arr(Y);
}
