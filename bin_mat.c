#include <stdio.h>
#include <stdlib.h>

#include "bin_mat.h"

bin_mat_t* create_bin_matrix(int m, int n){
  bin_mat_t *bin_mat = (bin_mat_t*) malloc(sizeof(bin_mat_t));
  bin_mat->mat  = (uint8_t**)malloc(sizeof(uint8_t*)*m);
  bin_mat->rows = m;
  bin_mat->cols = n;
  
  for(int i = 0; i < m; ++i)
    bin_mat->mat[i] = (uint8_t*)calloc(n, sizeof(uint8_t));
  return bin_mat;
}

bin_mat_t* dupilcate_bin_matrix(bin_mat_t* bm){
    bin_mat_t* bm_dup = create_bin_matrix(bm->rows, bm->cols);
    for(int i = 0; i < bm->rows; ++i)
      for(int j = 0; j < bm->cols; ++j)
          if(bm->mat[i][j])
              bm_dup->mat[i][j] = 1;
    return bm_dup;
}

void print_bin_matrix(bin_mat_t* bm){
  for(int i = 0; i < bm->rows; ++i){
      for(int j = 0; j < bm->cols; ++j)
          printf("%d", bm->mat[i][j]);
      printf("\n");
  }
}

void free_bin_matrix(bin_mat_t* bm){
  for(int i = 0; i < bm->rows; ++i)
      free(bm->mat[i]);
  free(bm->mat);
  free(bm);
}
