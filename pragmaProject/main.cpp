#include <omp.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

double result[256];
int n;

//// version2
//int a = 0;
//int b = 1;
//double dx = 0.000000002;
//long m = (b - a) / dx;
//double suma = 0;
//// end of version2
const int ITERATIONS = 10000000;
int sum_points = 0;


double f1(double x) {
    return 4 / (1 + x * x);
}

double integral_value() {
    int a = 0;
    int b = 1;

    double dx = 0.000000002;

    long m = (b - a) / dx;

    double w = 0;

    for (long i = 0; i < m; i++) {
        w += f1(a + i * dx) * dx;
    }

    return w;
}

void integral_parallel() {
#pragma omp parallel
    {
        int k;

        n = omp_get_num_threads();
        k = omp_get_thread_num();

        int a = 0;
        int b = 1;

        double dx = 0.000000002;

        long m = (b - a) / dx;

        double w = 0;

        for (long i = k; i < m; i += n) {
            w += f1(a + i * dx) * dx;
        }

        result[k] = w;
    }
}
//
//void integral_parallel_reduction() {
//#pragma omp parallel for reduction(+ : suma)
//    for (long i = 0; i < m; i++) {
//        suma += f1(a + i * dx) * dx;
//    }
//}

double monte_carlo() {
    srand(time(NULL));

    int how_many_points_in_circle = 0;

    for (int i = 0; i < ITERATIONS; i++) {
        double random_x = rand() / (RAND_MAX + 1.);
        double random_y = rand() / (RAND_MAX + 1.);

        bool is_point_in_circle = (random_x - 0.5) * (random_x - 0.5) + (random_y - 0.5) * (random_y - 0.5) <=0.5 * 0.5;
        if (is_point_in_circle) {
            how_many_points_in_circle++;
        }
    }
    return (double) ITERATIONS/how_many_points_in_circle;
}

void monte_carlo_parallel() {
#pragma omp parallel for reduction(+ : sum_points)
    for (long i = 0; i < ITERATIONS; i++) {
        double random_x = rand() / (RAND_MAX + 1.);
        double random_y = rand() / (RAND_MAX + 1.);

        bool is_point_in_circle = (random_x - 0.5) * (random_x - 0.5) + (random_y - 0.5) * (random_y - 0.5) <= 0.5 * 0.5;
        if (is_point_in_circle) {
            sum_points += 1;
        }
    }
}

int main() {
    // START integral_value
    if (true) {
        double wtime = omp_get_wtime();
        double result = integral_value();
        printf("Integral value result: %f. Time: %f\n\n", result, omp_get_wtime() - wtime);
    }
    // END integral_value

    // START - integral_parallel
    if (true) {
        double p_wtime = omp_get_wtime();
        integral_parallel();
        double result_sum = 0;

        for (int i = 0; i < n; i++) {
            result_sum += result[i];
        }

        printf("Integral value (PARALLEL) result: %f.  Time: %f\n\n", result_sum, omp_get_wtime() - p_wtime);
    }
    // END - integral_parallel

    // START monte_carlo
    if (true) {
        double mc_wtime = omp_get_wtime();
        double mc_result = monte_carlo();
        printf("4/PI result: %f.  Time: %f\n\n", mc_result, omp_get_wtime() - mc_wtime);
    }
    // END monte_carlo

    // START monte_carlo_parallel
    if (true) {
        srand(time(NULL));
        double mcp_wtime = omp_get_wtime();
        monte_carlo_parallel();
        double mcp_result = (double) ITERATIONS/sum_points;
        printf("4/PI (PARALLEL) result: %f. Time: %f\n\n", mcp_result, omp_get_wtime() - mcp_wtime);
    }
    // END monte_carlo_parallel

    return 0;
}