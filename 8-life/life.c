/* Ah, the game of life.  This is a cellular automation simulator.  The rules
 * are very simple.  The board is a grid.  Each grid square can hold at most one
 * cell.  For a cell to survive from generation to generation it needs two or
 * three neighbors.  Too many neighbors causes the cell to starve.  Too few
 * causes an incurable case of ennui.  All is not lost.  If a grid square has
 * exactly three neighbors, a new cell is born, which is kind of strange when
 * you think about it.
 *
 * There are lots of variations on this game.  Technically, this is Conway's
 * Game of Life and is a B3/S23 game.  That is, cells survive with two or three
 * neighbors and are born with exactly 3 neighbors.  This game can also be
 * extended into the real domain or more diminutions.  We, however, with stick
 * with Conway's original automation.
 *
 * Below is a very simple implementation.  This is your chance to apply some of
 * the loop optimization techniques we've been talking about in class: 
 *  1) Remove loop inefficiencies
 *  2) Reduce procedure calls
 *  3) Reduce unnecessary memory references
 *  4) Loop unrolling
 * There are, of course, more optimizations that you can apply.  For example,
 * moving sequentially through memory is more efficient than jumping around.
 * We'll see why in Chapter 9.  

 * C stores 2D arrays Column Major Order.  That is to say that cell
 * A[0][WIDTH-1] is right next to A[1][0] in memory.  If you notice the nested
 * loops in evolve (below) I'm incrementing the column index, i, more quickly
 * than the row index, j.  Try reversing these and see what happens.
 *
 * The experimental set-up is implemented in a library called liblife.a  Since
 * it has solutions, I'm providing it as a binary in my home directory:
 *   /home/clausoa/lib/liblife.a
 * The provided make files already links in this library.
 *
 * There are two ways to run the test program: with our without a window.
 * Without a window, the program runs the test cases against the provided
 * evolution methods as quickly as possible.  After about 100 generations, it
 * stops and reports the average performance of each method.  
 *
 * If you favor something a little more animated, and I know you do, run it test
 * program with windowing.  This is the default.  This is basically the same
 * set-up, but it displays the current generation and the relative performance
 * of the provided methods.  The scores are jumpy for a little while, but they
 * eventually calm down.  When you close the window, the final scores are
 * displayed on the console.  
 *
 * See how fast you can make your code.
 *
 * I have one final piece of advice.  You are free to add as many test methods
 * as you want.  I would encourage you to do this.  Copy the code, give it a new
 * name, and add it to the library's suite of methods.  This way you can really
 * see if you are making improvements.  Have fun.
 *
 */

#include "liblife.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* This function calculates the number of neighbors for 4 corners, 4 edges, and everything else inside the board, then determine if the next board at the [i][j] position is alive based on the number of neighbors it has. */
void Avery(board prv, board nxt)
{
   int i, j, k;
   int n;

   n = prv[0][1] + prv[1][1] + prv[1][0];
   nxt[0][0] = (prv[0][0] | n) == 3;

   n = prv[0][WIDTH-2] + prv[1][WIDTH-2] + prv[1][WIDTH-1];
   nxt[0][WIDTH-1] = (prv[0][WIDTH-1] | n) == 3;

   n = prv[HEIGHT-2][0] + prv[HEIGHT-2][1] + prv[HEIGHT-1][1];
   nxt[WIDTH-1][0] = (prv[WIDTH-1][0] | n) == 3;

   n = prv[HEIGHT-2][WIDTH-2] + prv[HEIGHT-1][WIDTH-2] + prv[HEIGHT-2][WIDTH-1];
   nxt[HEIGHT-1][WIDTH-1] = (prv[HEIGHT-1][WIDTH-1] | n) == 3;
   
   for (i = 1; i < HEIGHT-1; ++i) {
      for (j = 1; j < WIDTH-1; ++j){
         k = prv[i][j];
         n = prv[i-1][j-1] + prv[i-1][j] + prv[i-1][j+1] 
           + prv[i][j-1]   + prv[i][j+1] + prv[i+1][j-1] 
           + prv[i+1][j]   + prv[i+1][j+1];
         nxt[i][j] = (k | n) == 3;
      }
   }

   for (i = 1; i < HEIGHT-1; ++i) {
      n = prv[i-1][WIDTH-1] + prv[i+1][WIDTH-1] + prv[i+1][WIDTH-2]
         + prv[i-1][WIDTH-2] + prv[i][WIDTH-2];
      nxt[i][WIDTH-1] = (prv[i][WIDTH-1] | n) == 3;      
      
      n = prv[i-1][1] + prv[i+1][1] + prv[i][1]
        + prv[i-1][0] + prv[i+1][0];
      nxt[i][0] = (prv[i][0] | n) == 3;
   }

   for (j = 1; j < WIDTH-1; ++j) {
      n = prv[HEIGHT-1][j-1] + prv[HEIGHT-1][j+1] + prv[HEIGHT-2][j] 
        + prv[HEIGHT-2][j-1] + prv[HEIGHT-2][j+1];
      nxt[HEIGHT-1][j] = (prv[HEIGHT-1][j] | n) == 3;

      n = prv[1][j-1] + prv[1][j+1] + prv[1][j] 
        + prv[0][j-1] + prv[0][j+1];
      nxt[0][j] = (prv[0][j] | n) == 3;
   }
}

/* The program takes one optional arugment: -nw.  This tells the program to skip
 * the GUI components.  The non-GUI form is much more stable and what we will
 * use to measure your code's performance.  
 */
int main(int argc, char* argv[])
{
   if(argc > 2 || (argc == 2 && 0 != strcmp (argv[1], "-nw"))) {
      fprintf(stderr, "Usage: %s [-nw]\n\t-nw  No window\n", argv[0]);
      return EXIT_FAILURE;
   }
   else {
     add_method("Avery", &Avery);
     run_game(argc == 2 && 0 == strcmp (argv[1], "-nw"));
     return EXIT_SUCCESS;
   }
}



