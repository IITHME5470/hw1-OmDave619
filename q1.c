#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

void print_to_file(int n, double **A, int format_flag) {

    char filename[50];

    //ascii
    if(format_flag==0) {
        sprintf(filename, "./outputfiles/array_%06d_ascii.out", n);
        FILE *out = fopen(filename, "w");
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                fprintf(out, "%.15lf ", A[i][j]);   
            }
            fprintf(out, "\n");
        }
        fclose(out);
    }   
    else {
        sprintf(filename, "./outputfiles/array_%06d_bin.out", n);
        FILE *out = fopen(filename, "wb");
        fwrite(A, sizeof(double), n*n, out);
        fclose(out);
    } 

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

    double** mat = (double **)malloc(n * sizeof(double *));
    for(int i = 0; i < n; i++) {
        mat[i] = (double *)malloc(n * sizeof(double));
        for(int j = 0; j < n; j++) {
            mat[i][j] = i + j; 
        } 
    }

    // 0 for ascii, 1 for binary
    int format_flag = 0;  
    print_to_file(n, mat, format_flag);  

}