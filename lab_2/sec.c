#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <ipp.h>

void selectionSort(double array[], int size);
double minElement(double array[], int size);

int main(int argc, char *argv[]) {
    unsigned int i, N;
    N = atoi(argv[1]);
    int A = 300;
    for (i = 0; i < 5; i++) {
        srand(i);
        double *M1 = malloc(N * sizeof(double));
        for (int j = 0; j < N; j++) {
            M1[j] = rand_r(&i) % A;
        }
        double *M2 = malloc(N / 2 * sizeof(double));
        for (int j = 0; j < N / 2; j++) {
            M2[j] = rand_r(&i) % (9 * A) + A;
        }

        // COMPUTING
        IppStatus status = ippsDivC_64f_I(M_PI, M1, N);
        if (status != ippStsNoErr) {
            printf("Error: %s\n", ippGetStatusString(status));
            exit(1);
        }
        status = ippsPowx_64f_I(3, M1, N);
        if (status != ippStsNoErr) {
            printf("Error: %s\n", ippGetStatusString(status));
            exit(1);
        }

        // COPY ARRAY M2
        double *M2_copy = malloc(N / 2 * sizeof(double));
        for (int j = 0; j < N / 2; j++) {
            M2_copy[j] = M2[j];
        }

        // M2[j] = tan(M[j] - M[j-1])
        status = ippsCopy_64f(M2_copy, M2, N / 2);
        if (status != ippStsNoErr) {
            printf("Error: %s\n", ippGetStatusString(status));
            exit(1);
        }
        status = ippsAdd_64f(M2_copy, M2_copy + 1, M2 + 1, N / 2 - 1);
        if (status != ippStsNoErr) {
            printf("Error: %s\n", ippGetStatusString(status));
            exit(1);
        }
        status = ippsTan_64f_A50(M2, M2, N / 2);
        if (status != ippStsNoErr) {
            printf("Error: %s\n", ippGetStatusString(status));
            exit(1);
        }
        free(M2_copy);

        // M2[i] = M1[i]^M2[i]
        status = ippsPow_64f(M1, M2, M2, N / 2);
        if (status != ippStsNoErr) {
            printf("Error: %s\n", ippGetStatusString(status));
            exit(1);
        }

        selectionSort(M2, N / 2);

        double X = 0;
        double minInM2 = minElement(M2, N / 2);
        for (int j = 0; j < N / 2; j++) {
            if ((int)floor(M2[j] / minInM2) % 2 == 0) {
                X += sin(M2[j]);
            }
        }
        free(M1);
        free(M2);
        printf("\nSum of good numbers=%f.\n", X);
    }
    return 0;
}

void selectionSort(double array[], int size) {
    for (int i = 0; i < size; i++) {
        int minPosition = i;
        for (int j = i + 1; j < size; j++) {
            if (array[minPosition] > array[j]) {
                minPosition = j;
            }
        }
        double tmp = array[minPosition];
        array[minPosition] = array[i];
        array[i] = tmp;
    }
}

double minElement(double array[], int size) {
    double min = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] < min) {
            min = array[i];
        }
    }
    return min ? min : -1.0;
}