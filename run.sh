rm -rf obj;
mkdir obj;
cd obj;
gcc -c ../bin_mat.c;
gcc -c ../grid_gen.c;
gcc bin_mat.o grid_gen.o -o ../main;
cd -;
