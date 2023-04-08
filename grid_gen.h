#include <sys/ioctl.h>
#include "bin_mat.h"

typedef struct WINSIZE_S{
    int rows, cols;
} winsize_t;

typedef struct POS_S{
    int x, y;
} pos_t;

winsize_t get_win_size(void);
void place_blocks(char*, bin_mat_t*);
void init_curr_state(bin_mat_t*, pos_t);
void update_state(bin_mat_t*);
char* get_random_color(void);
