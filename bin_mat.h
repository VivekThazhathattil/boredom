#include <stdint.h>

typedef struct BIN_MAT_S{
    int rows, cols; 
    uint8_t **mat;
} bin_mat_t;

bin_mat_t* create_bin_matrix(int, int);
bin_mat_t* dupilcate_bin_matrix(bin_mat_t*);
void print_bin_matrix(bin_mat_t*);
void free_bin_matrix(bin_mat_t*);
