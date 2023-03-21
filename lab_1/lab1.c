#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

/* Функция генерации случайного числа в диапазоне [min, max] */
double rand_uniform(double min, double max) {
    double r = (double)rand() / RAND_MAX;
    return min + r * (max - min);
}

/* Функция сортировки выбором */
void selection_sort(double arr[], int n) {
    int i, j, min_idx;
    double temp;
    for (i = 0; i < n-1; i++) {
        min_idx = i;
        for (j = i+1; j < n; j++) {
            if (arr[j] < arr[min_idx])
                min_idx = j;
        }
        temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}

/* Функция для рассчета значения X на этапе Reduce */
double reduce(double arr[], int n) {
    double min = arr[0];
    for (int i = 0; i < n; i++) {
        if (arr[i] > 0 && arr[i] < min) {
            min = arr[i];
        }
    }

    double sum_sin = 0;
    for (int i = 0; i < n; i++) {
        if ((int)(arr[i] / min) % 2 == 0) {
            sum_sin += sin(arr[i]);
        }
    }

    return sum_sin;
}

int main(int argc, char * argv[]) {
    int i, j, N;
    double A = 504;
    struct timeval T1, T2;
    long delta_ms;

    if (argc < 2) {
        printf("Please provide a command-line argument for N.\n");
        return 1;
    }
    N = atoi(argv[1]);

    gettimeofday(&T1, NULL); /* запомнить текущее время T1 */
    for (i = 0; i < 100; i++) { /* 100 экспериментов */
        srand(i); /* инициализировать начальное значение ГСЧ */

        double M1[N], M2[N/2], M2_copy[N/2];

        /* Этап Generate */
        for (j = 0; j < N; j++) {
            M1[j] = rand_uniform(1, A);
        }

        for (j = 0; j < N/2; j++) {
            M2[j] = rand_uniform(A, 10 * A);
        }

        /* Этап Map */
        for (j = 0; j < N; j++) {
            M1[j] = pow(M1[j] / (M_PI + 1e-16), 3);
        }

        /* Этап Map */
        M2_copy[0] = M2[0];
        for (j = 1; j < N/2; j++) {
            M2_copy[j] = M2[j];
            M2[j] = fmod(tan(M2_copy[j] + M2_copy[j-1]), A);
        }

        /* Этап Merge */
        for (j = 0; j < N/2; j++) {
            if (M1[j] > 0) {
                M2[j] = pow(M1[j], M2[j]);
            }
        }

        /* Этап Sort */
        selection_sort(M2, N/2);
        double X = reduce(M1, N/2);

        printf("\nX = %lf\n", X);
    }

    gettimeofday(&T2, NULL); /* запомнить текущее время T2 */
    delta_ms = 1000*(T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
    printf("\nN = %d. Milliseconds passed: %ld\n", N, delta_ms); /* T2 - T1 */
    return 0;
}
