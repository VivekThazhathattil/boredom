#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

#include "grid_gen.h"
#include "colors.h"
#include "consts.h"

winsize_t get_win_size(){
  winsize_t ws;
  struct winsize win;     
  ioctl(0, TIOCGWINSZ, &win);
  ws.rows = win.ws_row;
  ws.cols = win.ws_col;
  return ws;
}

void place_blocks(char* c, bin_mat_t* bm){
  //system("clear");
  for(int i = 0; i < bm->rows; ++i){
    //char* random_color = get_random_color();
    for(int j = 0; j < bm->cols; ++j){
      if(!bm->mat[i][j]){
        //printf(" ");
      }
      else{
        gotoxy(j, i);
        char* spec_color = get_specific_color(bm->mat[i][j]);
        printf("%s%s", spec_color, c);
        //printf("%s", c);
      }
    }
  }
}

void init_curr_state_center(bin_mat_t* bm, pos_t p){
  bm->mat[p.y][p.x] = 1;
}

char* get_random_color(void){
  char* colors[NUM_COLORS] = {RED, GRN, YEL, BLU, MAG, CYN, WHT};
  return colors[rand()%NUM_COLORS];
}

char* get_specific_color(int num){
  char* colors[NUM_COLORS] = {RED, GRN, YEL, BLU, MAG, CYN, WHT};
  return colors[num%7];
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

void init_curr_state_plants(bin_mat_t* bm){
  // assign a tree number instead of simply 1s and 0s
  uint8_t tree_num = 1;
  for(int i = 0; i < bm->cols; ++i){
    if(!(i%4)){
      bm->mat[bm->rows-1][i] = tree_num;
      ++tree_num;
    }
  }
}

int can_grow(bin_mat_t* bm, int i, int j){
  int can_grow_up = 0,
      can_grow_left = 0,
      can_grow_right = 0;
  if(i > 0 && !bm->mat[i-1][j]) can_grow_up = 1;
  if(j > 0 && !bm->mat[i][j-1]) can_grow_left = 1;
  if(j < bm->cols-1 && !bm->mat[i][j+1]) can_grow_right = 1;
  return can_grow_up || can_grow_left || can_grow_right;
}

void update_state_plants(bin_mat_t* bm){
  uint8_t* marked_trees = (uint8_t*) calloc(bm->cols/2 + 1, sizeof(uint8_t));
  for(int i = 0 ; i < bm->rows; ++i){
    for(int j = 0; j < bm->cols; ++j){
      uint8_t elm = bm->mat[i][j];
      if(elm && marked_trees[elm-1] < NUM_BRANCHES){
        //the tree can only grow in 3 dirs (L, R, U)
        while(1){
          if(!can_grow(bm, i, j))
            break;
          int rand_dir = rand()%3;
          int flag = 0;
          switch(rand_dir){
            case L:
              if(j > 0 && !bm->mat[i][j-1]){
                bm->mat[i][j-1] = elm;
                ++marked_trees[elm-1]; 
                flag = 1;
              }
              break;
            case R:
              if(j < bm->cols-1 && !bm->mat[i][j+1]){
                bm->mat[i][j+1] = elm;
                ++marked_trees[elm-1];
                flag = 1;
              }
              break;
            case U:
              if(i > 0 && !bm->mat[i-1][j]){
                bm->mat[i-1][j] = elm;
                ++marked_trees[elm-1];
                flag = 1;
              }
              break;
            default:
              flag = 1;
          }
          if(flag)
            break;
        }
      }
    }
  }
  free(marked_trees);
}

int main(void){
  srand(time(NULL));
  winsize_t win_dim = get_win_size();
  char* blk_sym = "■";
  useconds_t frame_update_delay = 5e5; //

  pos_t root_pos = {win_dim.cols/2, win_dim.rows/2};

  bin_mat_t* curr_state = create_bin_matrix(win_dim.rows, win_dim.cols);
  //init_curr_state_center(curr_state, root_pos);
  
  init_curr_state_plants(curr_state);

  system("clear");
  for(int i = 0; i < 1000; ++i){
    place_blocks(blk_sym, curr_state);
    printf("\n");
    //usleep(frame_update_delay);
    update_state_plants(curr_state);
  }

  free_bin_matrix(curr_state);
  return 0;
}
