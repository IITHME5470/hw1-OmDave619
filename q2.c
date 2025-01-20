#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define eps 1e-10

double* is_eigenvector(int n, double** A, double* x) {
    // To check if vector x is eigen vector for matrix A
    // Ax = cx

    // finding prod = Ax
    double* prod = (double*)malloc(n * sizeof(double));
    if (prod == NULL) {
        fprintf(stderr, "Memory allocation failed for prod.\n");
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            sum += A[i][j] * x[j];
        }
        prod[i] = sum;
    }

    bool null_vector = true;    //to Handle edge case for null vector    

    // check if prod is multiple of x
    for (int i = 0; i < n - 1; i++) {
        if (fabs(prod[i] * x[i + 1] - prod[i + 1] * x[i]) > eps) {
            free(prod);
            return NULL;
        }
        if (fabs(x[i]) > eps) null_vector = false;
    }

    if (null_vector) {
        free(prod);
        return NULL;
    }

    return prod;    
}

double** read_matrix(int n, char* filename) {
    double** mat = (double**)malloc(n * sizeof(double*));

    FILE* input_mat = fopen(filename, "r");
    if (input_mat == NULL) {
        fprintf(stderr, "Failed to open matrix file %s.\n", filename);
        free(mat);
        return NULL;
    }

    printf("Reading matrix from file %s\n", filename);
    for (int i = 0; i < n; i++) {
        mat[i] = (double*)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            fscanf(input_mat, "%lf", &mat[i][j]);
            //ignore the comma
            fgetc(input_mat);
        }
    }
    fclose(input_mat);
    return mat;
}

double* read_vector(int n, char* filename) {
    double* vec = (double*)malloc(n * sizeof(double));

    FILE* input_vec = fopen(filename, "r");
    if (input_vec == NULL) {
        fprintf(stderr, "Failed to open vector file %s.\n", filename);
        free(vec);
        return NULL;
    }

    for (int j = 0; j < n; j++) {
        fscanf(input_vec, "%lf", &vec[j]);
        //ignore the comma
        fgetc(input_vec);
    }
    fclose(input_vec);
    return vec;
}

int main() {
    // read n from input file 
    FILE* input_n = fopen("./inputfiles/input.in", "r");
    if (input_n == NULL) {
        fprintf(stderr, "Failed to open input file ./inputfiles/input.in.\n");
        return EXIT_FAILURE;
    }

    int n;
    if (fscanf(input_n, "%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Invalid matrix size read from input.in.\n");
        fclose(input_n);
        return EXIT_FAILURE;
    }
    fclose(input_n);

    // read n by n matrix from input file
    char filename[50];
    sprintf(filename, "./inputfiles/mat_%06d.in", n);
    double** mat = read_matrix(n, filename);
    if (mat == NULL) {
        return EXIT_FAILURE;
    }

    // Read vectors and check if they are eigen vector for the matrix 
    int vecnum = 4;
    for (int i = 1; i <= vecnum; i++) {
        sprintf(filename, "./inputfiles/vec_%06d_%06d.in", n, i);
        double* vec = read_vector(n, filename);

        double* prod = is_eigenvector(n, mat, vec); // returns prod = Ax if it is eigenvector, else returns NULL
        if (prod) {
            double eigen_value = 0;
            for (int k = 0; k < n; k++) {
                if (vec[k] == 0.0) continue;
                eigen_value = prod[k] / vec[k];
                break;
            }
            printf("vec_%06d_%06d.in : Yes : %lf\n", n, i, eigen_value);
            free(prod);
        }
        else {
            printf("vec_%06d_%06d.in : Not an eigenvector\n", n, i);
        }

        free(vec);
    }

    // Free matrix
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);

}
