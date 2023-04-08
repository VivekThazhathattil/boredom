#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "grid_gen.h"
#include "colors.h"

#define NUM_COLORS 7

winsize_t get_win_size(){
  winsize_t ws;
  struct winsize win;     
  ioctl(0, TIOCGWINSZ, &win);
  ws.rows = win.ws_row;
  ws.cols = win.ws_col;
  return ws;
}

void place_blocks(char* c, bin_mat_t* bm){
  system("clear");
  for(int i = 0; i < bm->rows; ++i){
    char* random_color = get_random_color();
    for(int j = 0; j < bm->cols; ++j){
      if(!bm->mat[i][j])
        printf(" ");
      else
        printf("%s%s", random_color, c);
    }
  }
}

void init_curr_state(bin_mat_t* bm, pos_t p){
  bm->mat[p.y][p.x] = 1;
}

char* get_random_color(void){
  char* colors[NUM_COLORS] = {RED, GRN, YEL, BLU, MAG, CYN, WHT};
  return colors[rand()%NUM_COLORS];
}

void update_state(bin_mat_t* bm){
  bin_mat_t* bm_temp = dupilcate_bin_matrix(bm);
  for(int i = 0; i < bm_temp->rows; ++i)
    for(int j = 0; j < bm_temp->cols; ++j){
      if(bm_temp->mat[i][j] == 1){
        if(i > 0) /* update N of (i,j) */
            bm->mat[i-1][j] = 1;
        if(i < bm->rows-1) /* update S of (i,j) */
            bm->mat[i+1][j] = 1;
        if(j > 0) /* update W of (i,j) */
            bm->mat[i][j-1] = 1;
        if(j < bm->cols-1) /* update E of (i,j) */
            bm->mat[i][j+1] = 1;

        //if(i > 0 && j > 0) /* update NW of (i,j) */
        //    bm->mat[i-1][j-1] = 1;
        //if(i > 0 && j < bm->cols-1) /* update NE of (i,j) */
        //    bm->mat[i-1][j+1] = 1;
        //if(i < bm->rows-1 && j > 0) /* update SW of (i,j) */
        //    bm->mat[i+1][j-1] = 1;
        //if(i < bm->rows-1 && j < bm->cols-1) /* update SE of (i,j) */
        //    bm->mat[i+1][j+1] = 1;
      }
    }
  free_bin_matrix(bm_temp);
}

int main(void){
  srand(time(NULL));
  winsize_t win_dim = get_win_size();
  char* blk_sym = "■";
  useconds_t frame_update_delay = 5e5; //

  pos_t root_pos = {win_dim.cols/2, win_dim.rows/2};

  bin_mat_t* curr_state = create_bin_matrix(win_dim.rows, win_dim.cols);
  init_curr_state(curr_state, root_pos);

  for(int i = 1; i < 100; ++i){
    place_blocks(blk_sym, curr_state);
    printf("\n");
    usleep(frame_update_delay);
    update_state(curr_state);
  }

  free_bin_matrix(curr_state);
  return 0;
}
