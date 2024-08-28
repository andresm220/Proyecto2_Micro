#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_EQUATIONS 100 // Define un máximo para el número de ecuaciones

// Función para resolver la fórmula cuadrática de manera secuencial
void quadratic_solver_sequential(double a, double b, double c, double *sol1_real, double *sol1_imag, double *sol2_real, double *sol2_imag) {
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

// Función para imprimir las soluciones de la ecuación
void print_solutions(double sol1_real, double sol1_imag, double sol2_real, double sol2_imag) {
    if (sol1_imag == 0) {
        printf("Solución 1: %.2lf\n", sol1_real);
    } else {
        printf("Solución 1: %.2lf + %.2lfi\n", sol1_real, sol1_imag);
    }

    if (sol2_imag == 0) {
        printf("Solución 2: %.2lf\n", sol2_real);
    } else {
        printf("Solución 2: %.2lf + %.2lfi\n", sol2_real, sol2_imag);
    }
}

// Función para leer los coeficientes desde un archivo CSV y resolver secuencialmente
void read_csv_and_solve_sequential(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo %s\n", filename);
        return;
    }

    double a, b, c;
    double sol1_real, sol1_imag, sol2_real, sol2_imag;
    int equation_number = 0;

    while (fscanf(file, "%lf,%lf,%lf", &a, &b, &c) != EOF) {
        equation_number++;
        if (a == 0) {
            printf("El coeficiente 'a' no puede ser cero. Entrada ignorada en ecuación %d.\n", equation_number);
            continue;
        }

        printf("Resolviendo ecuación %d con a=%.2lf, b=%.2lf, c=%.2lf:\n", equation_number, a, b, c);
        quadratic_solver_sequential(a, b, c, &sol1_real, &sol1_imag, &sol2_real, &sol2_imag);
        print_solutions(sol1_real, sol1_imag, sol2_real, sol2_imag);
    }

    fclose(file);
}

int main() {
    const char *filename = "test.csv";
    clock_t start, end;
    double cpu_time_used;

    // Medir tiempo de ejecución
    start = clock();

    // Leer y resolver las ecuaciones cuadráticas desde el archivo CSV (versión secuencial)
    read_csv_and_solve_sequential(filename);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Tiempo de ejecución secuencial: %f segundos\n", cpu_time_used);

    return 0;
}
