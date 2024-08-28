#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define MAX_EQUATIONS 100

// Esta función resuelve la ecuación cuadrática.
void quadratic_solver(double a, double b, double c, double *sol1_real, double *sol1_imag, double *sol2_real, double *sol2_imag) {
    double discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        *sol1_real = (-b + sqrt(discriminant)) / (2 * a);
        *sol2_real = (-b - sqrt(discriminant)) / (2 * a);
        *sol1_imag = *sol2_imag = 0;
    } else if (discriminant == 0) {
        *sol1_real = *sol2_real = -b / (2 * a);
        *sol1_imag = *sol2_imag = 0;
    } else {
        *sol1_real = *sol2_real = -b / (2 * a);
        *sol1_imag = sqrt(-discriminant) / (2 * a);
        *sol2_imag = -(*sol1_imag);
    }
}

// Función para leer los coeficientes desde un archivo CSV y resolver en paralelo
void read_csv_and_solve_parallel(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo %s\n", filename);
        return;
    }

    double a[MAX_EQUATIONS], b[MAX_EQUATIONS], c[MAX_EQUATIONS];
    int count = 0;

    // Leer los datos del archivo secuencialmente
    while (fscanf(file, "%lf,%lf,%lf", &a[count], &b[count], &c[count]) != EOF && count < MAX_EQUATIONS) {
        count++;
    }

    fclose(file);

    // Resolver las ecuaciones en paralelo
    #pragma omp parallel for schedule(static) num_threads(omp_get_max_threads())
    for (int i = 0; i < count; i++) {
        double sol1_real = 0, sol1_imag = 0, sol2_real = 0, sol2_imag = 0;

        if (a[i] == 0) {
            printf("El coeficiente 'a' no puede ser cero. Entrada %d ignorada.\n", i + 1);
            continue;
        }

        quadratic_solver(a[i], b[i], c[i], &sol1_real, &sol1_imag, &sol2_real, &sol2_imag);

        #pragma omp critical
        {
            printf("Para la ecuacion %d con a=%.2lf, b=%.2lf, c=%.2lf:\n", i + 1, a[i], b[i], c[i]);
            if (sol1_imag == 0) {
                printf("Solucion 1: %.2lf\n", sol1_real);
            } else {
                printf("Solucion 1: %.2lf + %.2lfi\n", sol1_real, sol1_imag);
            }

            if (sol2_imag == 0) {
                printf("Solucion 2: %.2lf\n", sol2_real);
            } else {
                printf("Solucion 2: %.2lf + %.2lfi\n", sol2_real, sol2_imag);
            }
        }
    }
}

int main() {
    const char *filename = "test.csv";
    double start, end;
    double cpu_time_used;

    start = omp_get_wtime();

    read_csv_and_solve_parallel(filename);

    end = omp_get_wtime();
    cpu_time_used = end - start;

    printf("Tiempo de ejecución paralelizado: %f segundos\n", cpu_time_used);

    return 0;
}
