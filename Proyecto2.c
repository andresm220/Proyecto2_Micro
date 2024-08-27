#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

// Esta función resuelve la ecuación cuadrática de forma paralela.
// Básicamente, toma los coeficientes a, b, c de la ecuación ax^2 + bx + c = 0 y calcula las raíces.
void quadratic_solver_parallel(double a, double b, double c, double *sol1_real, double *sol1_imag, double *sol2_real, double *sol2_imag) {
    // Primero, calculamos el discriminante, que nos dice si las soluciones son reales o complejas.
    double discriminant = b * b - 4 * a * c;

    // Si el discriminante es positivo o cero, tenemos soluciones reales.
    if (discriminant >= 0) {
        // Usamos OpenMP para calcular ambas soluciones reales en paralelo.
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                // Calculamos la primera raíz real.
                *sol1_real = (-b + sqrt(discriminant)) / (2 * a);
                *sol1_imag = 0;  // Como es una solución real, la parte imaginaria es 0.
            }
            #pragma omp section
            {
                // Calculamos la segunda raíz real.
                *sol2_real = (-b - sqrt(discriminant)) / (2 * a);
                *sol2_imag = 0;  // Parte imaginaria es 0 también.
            }
        }
    } else {
        // Si el discriminante es negativo, las soluciones son complejas.
        // Aquí no paralelizamos porque los cálculos están interrelacionados.
        *sol1_real = *sol2_real = -b / (2 * a);  // La parte real es la misma para ambas raíces.
        *sol1_imag = sqrt(-discriminant) / (2 * a);  // Parte imaginaria de la primera raíz.
        *sol2_imag = -(*sol1_imag);  // Parte imaginaria de la segunda raíz (es el negativo de la primera).
    }
}

// Esta función simplemente lee un número del usuario, asegurándose de que sea un número válido.
double read_number(const char *prompt) {
    double value;
    char term;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf%c", &value, &term) != 2 || term != '\n') {
            // Si la entrada no es válida, avisamos al usuario y pedimos que lo intente de nuevo.
            printf("Entrada inválida. Por favor, ingrese un número válido.\n");
            while (getchar() != '\n'); // Limpiamos el buffer de entrada para evitar errores en el próximo intento.
        } else {
            return value;  // Si todo está bien, devolvemos el número ingresado.
        }
    }
}

// Esta función muestra las soluciones de la ecuación, manejando tanto raíces reales como complejas.
void print_solutions(double sol1_real, double sol1_imag, double sol2_real, double sol2_imag) {
    // Si la parte imaginaria es cero, mostramos solo la parte real.
    if (sol1_imag == 0) {
        printf("Solucion 1: %.2lf\n", sol1_real);
    } else {
        // Si no, mostramos la parte real junto con la parte imaginaria.
        printf("Solucion 1: %.2lf + %.2lfi\n", sol1_real, sol1_imag);
    }

    // Lo mismo para la segunda solución.
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

    // Pedimos al usuario que ingrese los coeficientes de la ecuación cuadrática.
    a = read_number("Ingrese el coeficiente a: ");
    b = read_number("Ingrese el coeficiente b: ");
    c = read_number("Ingrese el coeficiente c: ");

    // Nos aseguramos de que 'a' no sea cero, porque si lo es, no es una ecuación cuadrática.
    if (a == 0) {
        printf("El coeficiente 'a' no puede ser cero.\n");
        return 1;  // Salimos del programa con un error.
    }

    // Comenzamos a medir el tiempo de ejecución para ver cuánto tarda el proceso.
    start = omp_get_wtime(); // Tomamos el tiempo de inicio.

    // Llamamos a la función que resuelve la ecuación cuadrática.
    quadratic_solver_parallel(a, b, c, &sol1_real, &sol1_imag, &sol2_real, &sol2_imag);

    // Mostramos las soluciones calculadas.
    print_solutions(sol1_real, sol1_imag, sol2_real, sol2_imag);

    // Terminamos la medición de tiempo.
    end = omp_get_wtime(); // Tomamos el tiempo de fin.
    cpu_time_used = end - start; // Calculamos cuánto tiempo tomó todo el proceso.

    // Mostramos el tiempo de ejecución.
    printf("Tiempo de ejecucion paralelizado: %f segundos\n", cpu_time_used);

    return 0;  // Terminamos el programa sin errores.
}
