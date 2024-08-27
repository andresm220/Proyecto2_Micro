#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

// Esta función resuelve la ecuación cuadrática (ax^2 + bx + c = 0) de manera paralela.
// Básicamente, calcula las raíces de la ecuación, ya sean reales o complejas.
void quadratic_solver_parallel(double a, double b, double c, double *sol1_real, double *sol1_imag, double *sol2_real, double *sol2_imag) {
    // Calculamos el discriminante, que nos dice qué tipo de raíces tiene la ecuación.
    double discriminant = b * b - 4 * a * c;

    if (discriminant >= 0) {
        // Si el discriminante es mayor o igual a 0, las raíces son reales.
        // Usamos OpenMP para paralelizar el cálculo de las dos raíces.
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                // Calculamos la primera raíz (la de "+")
                *sol1_real = (-b + sqrt(discriminant)) / (2 * a);
                *sol1_imag = 0; // Como es una raíz real, la parte imaginaria es 0.
            }
            #pragma omp section
            {
                // Calculamos la segunda raíz (la de "-")
                *sol2_real = (-b - sqrt(discriminant)) / (2 * a);
                *sol2_imag = 0; // También es una raíz real.
            }
        }
    } else {
        // Si el discriminante es negativo, tenemos raíces complejas.
        // Aquí no hay mucho que paralelizar, así que hacemos los cálculos de forma normal.
        *sol1_real = *sol2_real = -b / (2 * a); // Parte real común para ambas raíces.
        *sol1_imag = sqrt(-discriminant) / (2 * a); // Parte imaginaria para la primera raíz.
        *sol2_imag = -(*sol1_imag); // La segunda raíz tiene la misma parte imaginaria pero negativa.
    }
}

// Esta función simplemente lee un número desde la entrada del usuario.
// Tiene algo de validación para asegurarse de que el usuario ingrese un número válido.
double read_number(const char *prompt) {
    double value;
    char term;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf%c", &value, &term) != 2 || term != '\n') {
            printf("Entrada inválida. Por favor, ingrese un número válido.\n");
            while (getchar() != '\n'); // Limpiamos el buffer de entrada para que no cause problemas.
        } else {
            return value;
        }
    }
}

// Esta función imprime las soluciones calculadas.
// Maneja tanto los casos de soluciones reales como complejas.
void print_solutions(double sol1_real, double sol1_imag, double sol2_real, double sol2_imag) {
    if (sol1_imag == 0) {
        printf("Solución 1: %.2lf\n", sol1_real); // Imprimimos la primera solución si es real.
    } else {
        printf("Solución 1: %.2lf + %.2lfi\n", sol1_real, sol1_imag); // Imprimimos la primera solución si es compleja.
    }

    if (sol2_imag == 0) {
        printf("Solución 2: %.2lf\n", sol2_real); // Imprimimos la segunda solución si es real.
    } else {
        printf("Solución 2: %.2lf + %.2lfi\n", sol2_real, sol2_imag); // Imprimimos la segunda solución si es compleja.
    }
}

int main() {
    double a, b, c;
    double sol1_real, sol1_imag, sol2_real, sol2_imag;
    double start, end;
    double cpu_time_used;

    // Aquí pedimos al usuario que ingrese los coeficientes a, b, y c de la ecuación cuadrática.
    a = read_number("Ingrese el coeficiente a: ");
    b = read_number("Ingrese el coeficiente b: ");
    c = read_number("Ingrese el coeficiente c: ");

    // Verificamos que 'a' no sea cero, porque si lo es, no sería una ecuación cuadrática.
    if (a == 0) {
        printf("El coeficiente 'a' no puede ser cero.\n");
        return 1; // Salimos del programa si 'a' es cero.
    }

    // Medimos cuánto tiempo toma resolver la ecuación.
    start = omp_get_wtime(); // Guardamos el tiempo de inicio.
    cpu_time_used = end - start; // Calcula el tiempo total de ejecución

    printf("Tiempo de ejecucion paralelizado: %f segundos\n", cpu_time_used);

    return 0;
}
