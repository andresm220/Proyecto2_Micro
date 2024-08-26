#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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

// Función para leer un número desde la entrada del usuario con validación
double read_number(const char *prompt) {
    double value;
    char term;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf%c", &value, &term) != 2 || term != '\n') {
            printf("Entrada inválida. Por favor, ingrese un número válido.\n");
            while (getchar() != '\n'); // Limpiar el buffer de entrada
        } else {
            return value;
        }
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

int main() {
    double a, b, c;
    double sol1_real, sol1_imag, sol2_real, sol2_imag;
    clock_t start, end;
    double cpu_time_used;

    // Solicitar los coeficientes al usuario con validación
    a = read_number("Ingrese el coeficiente a: ");
    b = read_number("Ingrese el coeficiente b: ");
    c = read_number("Ingrese el coeficiente c: ");

    // Verificar que 'a' no sea cero
    if (a == 0) {
        printf("El coeficiente 'a' no puede ser cero.\n");
        return 1;
    }

    // Medir tiempo de ejecución
    start = clock();

    // Resolver la ecuación cuadrática (versión secuencial)
    quadratic_solver_sequential(a, b, c, &sol1_real, &sol1_imag, &sol2_real, &sol2_imag);

    // Imprimir las soluciones
    print_solutions(sol1_real, sol1_imag, sol2_real, sol2_imag);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Tiempo de ejecución secuencial: %f segundos\n", cpu_time_used);

    return 0;
}
