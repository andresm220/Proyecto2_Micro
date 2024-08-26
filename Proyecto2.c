#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

// Función para resolver la fórmula cuadrática de manera paralelizada
void quadratic_solver_parallel(double a, double b, double c, double *sol1_real, double *sol1_imag, double *sol2_real, double *sol2_imag) {
    double discriminant = b * b - 4 * a * c;
    
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            // Calcular la parte interna de la raíz
            if (discriminant >= 0) {
                *sol1_real = (-b + sqrt(discriminant)) / (2 * a);
                *sol2_real = (-b - sqrt(discriminant)) / (2 * a);
                *sol1_imag = *sol2_imag = 0;
            } else {
                *sol1_real = *sol2_real = -b / (2 * a);
                *sol1_imag = sqrt(-discriminant) / (2 * a);
                *sol2_imag = -(*sol1_imag);
            }
        }

        #pragma omp section
        {
            // No hay cálculos adicionales necesarios en esta sección para el paralelo
            // Este es un ejemplo de cómo se pueden distribuir múltiples tareas.
        }
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

int main() {
    double a, b, c;
    double sol1_real, sol1_imag, sol2_real, sol2_imag;
    double start, end;
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
    start = omp_get_wtime(); // Captura el tiempo de inicio

    // Resolver la ecuación cuadrática (versión paralelizada)
    quadratic_solver_parallel(a, b, c, &sol1_real, &sol1_imag, &sol2_real, &sol2_imag);

    // Imprimir las soluciones
    print_solutions(sol1_real, sol1_imag, sol2_real, sol2_imag);

    end = omp_get_wtime(); // Captura el tiempo de fin
    cpu_time_used = end - start; // Calcula el tiempo total de ejecución

    printf("Tiempo de ejecucion paralelizado: %f segundos\n", cpu_time_used);

    return 0;
}
