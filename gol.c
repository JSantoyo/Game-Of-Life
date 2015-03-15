/*******************************************************************************

    PURPOSE: The purpose of this program is to display a 64x32 grid of Conway's
             game of life whereby each cell lives, dies, or comes back to life 
             based on the implementation of the rules.

    HISTORY: Created by Joseph Santoyo, March 6, 2015

    INPUTS: Any key to continue, 'x' to exit.

    OUTPUTS: The 64x32 randomly generated grid of cells

    ALGORITHM(S): Loops through each row calling an init() function which initi-
                  alizes the grid with random cells. Each row in the grid is th-
                  en displayed using a displayBinary() function. 

                  The program then retrieves the next generation by calling a 
                  sumNeighbours() function twice. One for the NORMAL condition, 
                  and again for the ZOMBIE condition, bitwise OR-ing the return 
                  values in order to bring back to life the zombie cells, stor-
                  ing the results for each row in the grid in a global 
                  next_generation[] array.

                  The next_generation[] array then replaces the old values in-
                  side of the global grid[] array and the process starts all
                  over again.

*******************************************************************************/

/*******************************************************************************
    Begin #include statements
*******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/*******************************************************************************
    Begin #define statements
*******************************************************************************/


// LMASK checks the left-most bit RMASK checks the right-most bit
#define LMASK 0x8000000000000000
#define RMASK 0x0000000000000001

// size of grid
#define SIZE   32      

// cell conditions
#define NORMAL 0    
#define ZOMBIE 1   

// compass directions
#define TOP          0
#define BOTTOM       1
#define LEFT         2
#define RIGHT        3
#define TOP_LEFT     4
#define TOP_RIGHT    5
#define BOTTOM_LEFT  6
#define BOTTOM_RIGHT 7

// number of compass directions
#define DIRECTIONS   8


/*******************************************************************************
    Begin declarations
*******************************************************************************/


// data type used for the cells to create 64 cells per row
typedef unsigned long long int uLLInt;

// directional function prototypes
uLLInt TL(int, int);
uLLInt T(int, int);
uLLInt TR(int, int);
uLLInt L(int, int);
uLLInt R(int, int);
uLLInt BL(int, int);
uLLInt B(int, int);
uLLInt BR(int, int);

// checks the number of neighbours a cell has for each cell in a row
uLLInt sumNeighbours(int, int);

// outputs the cells
void displayBinary(uLLInt);

// initializes the grid with random cells
uLLInt init(void);

// globally declared playing grid
uLLInt grid[SIZE] = {0};
// next generation of cells to display
uLLInt next_generation[SIZE] = {0};


/*******************************************************************************
    Begin main()
*******************************************************************************/


int main(void)
{   

    //initialize grid with random cell states
    srand(time(NULL));
    int i;
    for(i = 0; i < SIZE; i++) {
        grid[i] = init();
    }

    // output the grid
    char c;
    do {
        system("clear");

        // display current generation of grid
        for (i = 0; i < SIZE; i++) {
            displayBinary(grid[i]);
            printf("\n");
        }
        // get the next generation
        for (i = 0; i < SIZE; i++) {
            next_generation[i] = (sumNeighbours(i, NORMAL) |
                sumNeighbours(i, ZOMBIE));
        }
        // replace the current generation with the next generation
        for (i = 0; i < SIZE; i++) {
            grid[i] = next_generation[i];
        }


    } while((c = getchar()) != 'x');

    return EXIT_SUCCESS;
}

/*******************************************************************************

    PURPOSE: To return a randomized row

    HISTORY: Created by D. Houtman

    INPUTS: NONE

    OUTPUTS: A random 64-bit uLLInt

    ALGORITHM(S): Provided by D. Houtman

*******************************************************************************/
    
uLLInt init() {
    uLLInt init64;

    init64 = rand();
    init64 <<= 2;
    init64 |= (0x00000003 & rand());
    init64 <<= 31;
    init64 |= rand();

    return init64;
}

/*******************************************************************************

    PURPOSE: To visually display rows of cells as the ASCII values 'x' and ' '

    HISTORY: Created by D. Houtman

    INPUTS: An uLLInt row.

    OUTPUTS: NONE

    ALGORITHM(S): Assign MASK to the value 0x80000000000000000 which is just an
                  left-bit-mask that has a 1 on it's left most bit. Bitwise and
                  the row with the mask, if that returns true (1), output an x
                  otherwise output a space. Shift the bit in the mask over by 1,
                  repeat.

    NOTES: Though the function doesn't 'OUTPUT' anything per say, it does displa
           y a row using the characters 'x' and ' ' where 'x' represents a live
           cell and ' ' a dead cell. 

*******************************************************************************/

void displayBinary(uLLInt row) {
    uLLInt MASK = LMASK;

    do {
        // output an X if the bit is 1, otherwise a space if it is 0
        printf("%c", (row & MASK) ? 'x' : 0x20);
    } while((MASK >>=1) != 0);
}

/*******************************************************************************

    PURPOSE: To count the number of neighbours surrounding a cell

    HISTORY: Created by D. Houtman, Modified by Joseph Santoyo, March 6, 2015

    INPUTS: An int row, and an int condition (NORMAL OR ZOMBIE)

    OUTPUTS: An uLLInt next_generation which represents the next generation of
             the row being passed in.

    ALGORITHM(S): Initialize an array which represents the neighbours surroundi-
                  ng a cell. For each of the 8 directional positions around a 
                  cell, call the functions corresponding to those positions.

                  Set a mask corresponding to a left bit-mask, initialize a 
                  uLLInt next_generation to 0.

                  While the mask has not shifted off the end, iterate through
                  each of the 8 directional positions and sum up all the 1's
                  in the column of wherever the mask is currently positioned,
                  store the total in total_alive_neighbours.

                  If the condition provided is NORMAL and the number of alive
                  neighbours is either 2 or 3, keep that cell alive.

                  If the condition is ZOMBIE and the number of total alive neigh
                  bours is 3, revive the zombie cell.

                  Shift the mask of a column and repeat if the mask has not 
                  shifted off the end.

*******************************************************************************/

uLLInt sumNeighbours(int row, int condition) {
    uLLInt neighbours[DIRECTIONS] = {0};

    neighbours[TOP] = T(row, condition);
    neighbours[RIGHT] = R(row, condition);
    neighbours[BOTTOM] = B(row, condition);
    neighbours[LEFT] = L(row, condition);
    neighbours[TOP_RIGHT] = TR(row, condition);
    neighbours[TOP_LEFT] = TL(row, condition);
    neighbours[BOTTOM_LEFT] = BL(row, condition);
    neighbours[BOTTOM_RIGHT] = BR(row, condition);

    uLLInt MASK = LMASK;
    uLLInt next_generation = 0;
    while (MASK) {
        int total_alive_neighbours = 0;
        
        for (unsigned int compass = 0; compass < DIRECTIONS; compass++) {
            total_alive_neighbours += ((MASK & neighbours[compass]) > 0);
        }

        if ((condition == NORMAL) &&  
            (total_alive_neighbours == 2 || total_alive_neighbours == 3)) {
            // keep that cell alive
            next_generation |= MASK;
        }
        if ((condition == ZOMBIE) && total_alive_neighbours == 3) {
            // revive the cell
            next_generation |= MASK;
        }

        MASK >>= 1;
    }

    return next_generation;
}

/*******************************************************************************

    PURPOSE: To find out how many alive cells a row of cells has above it

    HISTORY: Created by Joseph Santoyo, March 6, 2015

    INPUTS: A row number and condition, both ints.

    OUTPUTS: The uLLInt representing the alive cells above the row provided

    ALGORITHM(S): Set upper to the row above the one being passed, adding the
                  size of the array due to C's peculiar functionality with the % 
                  operator. Wrap the upper row around if it goes beyond the 
                  top of the grid.

                  Set lower to the row being passed.

                  If the condition is a NORMAL condition bit-wise and the upper
                  row with the lower row and return the result.

                  If the condition is a ZOMBIE condition bit-wise and the upper
                  row with a negated lower row (effectively turning on the 
                  Zombie cells temporarily). Return that result.


*******************************************************************************/

uLLInt T(int row, int condition) {
    // upper and lower rows being compared
    int upper, lower;

    // set upper to the row above the one being passed
    upper = (row + SIZE - 1);
    // wrap the upper row around if it goes beyond the top of the grid
    upper %= SIZE;

    // set lower to the row being passed
    lower = row;
    
    if (condition == NORMAL) {
        return grid[upper] & grid[lower];
    }

    if (condition == ZOMBIE) {
        // make the dead cells alive and the alive cells dead so you can check
        // whether or not zombie cells have any live neighbours above
        return grid[upper] & (~grid[lower]);
    }
}

/*******************************************************************************

    PURPOSE: To determine the live cells below a row being passed in.

    HISTORY: Created by Joseph Santoyo, March 6, 2015

    INPUTS: A row number and condition, both ints.

    OUTPUTS: A uLLInt representing the live cells below a row

    ALGORITHM(S): Set upper to the row being passed and lower to the row below
                  the one being passed. Wrap the lower row around if it goes 
                  beyond the bottom of the grid.

                  If the condition is NORMAL bit-wise and the upper and lower
                  rows. Return the result.

                  If the condition is ZOMBIE, negate the upper row (temporarily
                  turning on the zombie cells) and bit-wise and that with the
                  lower row. Return the result.

*******************************************************************************/

uLLInt B(int row, int condition) {
    // upper and lower rows being compared
    int upper, lower;

    // set upper to the row being passed
    upper = row;

    // set lower to the row below the one being passed
    lower = row + 1;

    // wrap the lower row around if it goes beyond the bottom of the grid
    lower %= SIZE;
    
    if (condition == NORMAL) {
        return grid[upper] & grid[lower];
    }

    if (condition == ZOMBIE) {
        return (~grid[upper]) & grid[lower];
    }
}

/*******************************************************************************

    PURPOSE: To determine which cells left of the row being passed, are alive.

    HISTORY: Created by Joseph Santoyo, March 6, 2015

    INPUTS: A row number and condition, both ints.

    OUTPUTS: A row representing the live cells to the left of the row being
             passed.

    ALGORITHM(S): Set the row you want to shift to a variable called
                  shifted_row.

                  If the condition is NORMAL, check if there is a live cell on
                  the rightmost bit, if there is, shift the row over to the right 
                  by 1, and set the leftmost bit on, effectively wrapping the 
                  row's cells.

                  Bit-wise AND this shifted row with the row you want to check. 
                  Return the result.

                  If there is no live cell on the right most bit, then shift
                  the row to the right without accounting for wrapping and bit-w
                  ise AND this shifted row with the row you want to check.

                  The same will be done for zombie rows except you will be temp-
                  orarily turning on the zombies in the row you are checking be-
                  fore bit-wise ANDing the shifted row with the zombie row. 

*******************************************************************************/

uLLInt L(int row, int condition) {
    // the row being shifted
    uLLInt shifted_row = grid[row];

    if (condition == NORMAL) {
        // check for a bit that needs to be wrapped around
        if (grid[row] & RMASK) {
            // shift the row by 1
            shifted_row = (shifted_row >> 1) | LMASK;
            // check if there is a left neighbour after shifting it over
            return grid[row] & shifted_row;
        } else {
            return grid[row] & (shifted_row >> 1);
        }
        
    }

    if (condition == ZOMBIE) {
        // check for a bit that needs to be wrapped around
        if (grid[row] & RMASK) {
            // shift the row by 1
            shifted_row = (shifted_row >> 1) | LMASK;
            // check if there is a left neighbour after shifting it over
            return (~grid[row]) & shifted_row;
        } else {
            return (~grid[row]) & (shifted_row >> 1);
        }
    }
}

/*******************************************************************************

    PURPOSE: To determine which cells right of the row being passed, are alive.

    HISTORY: Created by Joseph Santoyo, March 6, 2015

    INPUTS: A row number and condition, both ints.

    OUTPUTS: A row representing the live cells to the right of the row being
             passed.

    ALGORITHM(S): Set the row you want to shift to a variable called
                  shifted_row.

                  If the condition is NORMAL, check if there is a live cell on
                  the leftmost bit, if there is, shift the row over to the left 
                  by 1, and set the rightmost bit on, effectively wrapping the 
                  row's cells.

                  Bit-wise AND this shifted row with the row you want to check. 
                  Return the result.

                  If there is no live cell on the left most bit, then shift
                  the row to the left without accounting for wrapping and bit-w
                  ise AND this shifted row with the row you want to check.

                  The same will be done for zombie rows except you will be temp-
                  orarily turning on the zombies in the row you are checking be-
                  fore bit-wise ANDing the shifted row with the zombie row. 


*******************************************************************************/

uLLInt R(int row, int condition) {
    // the row being shifted
    uLLInt shifted_row = grid[row];
    if (condition == NORMAL) {
        // check for a bit that needs to be wrapped around
        if (grid[row] & LMASK) {
            // shift the row by 1
            shifted_row = ((shifted_row << 1) | RMASK);
            // check if there is a left neighbour after shifting it over
            return (grid[row] & shifted_row);
        } else {
            return (grid[row] & (shifted_row << 1));
        }
    }

    if (condition == ZOMBIE) {
        // check for a bit that needs to be wrapped around
        if (grid[row] & LMASK) {
            // shift the row by 1
            shifted_row = ((shifted_row << 1) | RMASK);
            // check if there is a left neighbour after shifting it over
            return ((~grid[row]) & shifted_row);
        } else {
            return ((~grid[row]) & (shifted_row << 1));
        }
    }
}

/*******************************************************************************

    PURPOSE: To determine which cells in the row being passed have live cells
             to their top left.

    HISTORY: Created by Joseph Santoyo, March 6, 2015

    INPUTS: A row number and condition, both ints.

    OUTPUTS: A row representing the live cells to the top left of the row being
             passed.

    ALGORITHM(S): This function is a combination of the logic contained within
                  both T() and L() functions. Essentially, You are shifting
                  the upper row to the right and bitwise AND-ing it with the
                  upper row.

                  For the ZOMBIE condition you are negating the
                  lower row to flip the cells from dead to alive temporarily
                  and bitwise ANDing it with the shifted row.

                  The result of this is being returned.

*******************************************************************************/

uLLInt TL(int row, int condition) {
    // upper and lower rows being compared
    int upper, lower;

    // set upper to the row above the one being passed
    upper = row + SIZE - 1;
    // wrap the upper row around if it goes beyond the top of the grid
    upper %= SIZE;

    // set lower to the row being passed
    lower = row;

    // the row being shifted
    uLLInt shifted_row = grid[upper];

    if (condition == NORMAL) {
        // check for a bit that needs to be wrapped around
        if (grid[upper] & RMASK) {
            // shift the row by 1
            shifted_row = ((shifted_row >> 1) | LMASK);
            // check if there is a left neighbour after shifting it over
            return (grid[lower] & shifted_row);
        } else {
            return (grid[lower] & (shifted_row >> 1));
        }
        
    }

    if (condition == ZOMBIE) {
        // check for a bit that needs to be wrapped around
        if (grid[upper] & RMASK) {
            // shift the upper by 1
            shifted_row = ((shifted_row >> 1) | LMASK);
            // check if there is a left neighbour after shifting it over
            return ((~grid[lower]) & shifted_row);
        } else {
            return ((~grid[lower]) & (shifted_row >> 1));
        }
    }

}

/*******************************************************************************

    PURPOSE: To determine which cells in the row being passed have live cells
             to their top right.

    HISTORY: Created by Joseph Santoyo, March 6, 2015

    INPUTS: A row number and condition, both ints.

    OUTPUTS: A row representing the live cells to the top right of the row being
             passed.

    ALGORITHM(S): This function is a combination of the logic contained within
                  both T() and R() functions. Essentially, You are shifting
                  the upper row to the left and bitwise AND-ing it with the
                  lower row.

                  For the ZOMBIE condition you are negating the
                  lower row to flip the cells from dead to alive temporarily
                  and bitwise ANDing it with the shifted row.

                  The result of this is being returned.

*******************************************************************************/

uLLInt TR(int row, int condition) {
    // upper and lower rows being compared
    int upper, lower;

    // set upper to the row above the one being passed
    upper = row + SIZE - 1;
    // wrap the upper row around if it goes beyond the top of the grid
    upper %= SIZE;

    // set lower to the row being passed
    lower = row;

    // the row being shifted
    uLLInt shifted_row = grid[upper];

    if (condition == NORMAL) {
        // check for a bit that needs to be wrapped around
        if (grid[upper] & LMASK) {
            // shift the row by 1
            shifted_row = ((shifted_row << 1) | RMASK);
            // check if there is a left neighbour after shifting it over
            return (grid[lower] & shifted_row);
        } else {
            return (grid[lower] & (shifted_row << 1));
        }
        
    }

    if (condition == ZOMBIE) {
        // check for a bit that needs to be wrapped around
        if (grid[upper] & LMASK) {
            // shift the upper by 1
            shifted_row = ((shifted_row << 1) | RMASK);
            // check if there is a left neighbour after shifting it over
            return ((~grid[lower]) & shifted_row);
        } else {
            return ((~grid[lower]) & (shifted_row << 1));
        }
    }

}

/*******************************************************************************

    PURPOSE: To determine which cells in the row being passed have live cells
             to their bottom left.

    HISTORY: Created by Joseph Santoyo, March 6, 2015

    INPUTS: A row number and condition, both ints.

    OUTPUTS: A row representing the live cells to the bottom left of the row 
             being passed.

    ALGORITHM(S): This function is a combination of the logic contained within
                  both B() and L() functions. Essentially, You are shifting
                  the lower row to the right and bitwise AND-ing it with the
                  upper row. 

                  For the ZOMBIE condition you are negating the
                  upper row to flip the cells from dead to alive temporarily
                  and bitwise ANDing it with the shifted row.

                  The result of this is being returned.


*******************************************************************************/

uLLInt BL(int row, int condition) {
    // upper and lower rows being compared
    int upper, lower;

    // set upper to the row being passed
    upper = row;

    // set lower to the row below the one being passed
    lower = row + 1;

    // wrap the lower row around if it goes beyond the bottom of the grid
    lower %= SIZE;

    // the row being shifted
    uLLInt shifted_row = grid[lower];

    if (condition == NORMAL) {
        // check for a bit that needs to be wrapped around
        if (grid[lower] & RMASK) {
            // shift the row by 1
            shifted_row = ((shifted_row >> 1) | LMASK);
            // check if there is a left neighbour after shifting it over
            return (grid[upper] & shifted_row);
        } else {
            return (grid[upper] & (shifted_row >> 1));
        }
        
    }

    if (condition == ZOMBIE) {
        // check for a bit that needs to be wrapped around
        if (grid[lower] & RMASK) {
            // shift the upper by 1
            shifted_row = ((shifted_row >> 1) | LMASK);
            // check if there is a left neighbour after shifting it over
            return ((~grid[upper]) & shifted_row);
        } else {
            return ((~grid[upper]) & (shifted_row >> 1));
        }
    }

}

/*******************************************************************************

    PURPOSE: To determine which cells in the row being passed have live cells
             to their bottom right.

    HISTORY: Created by Joseph Santoyo, March 6, 2015

    INPUTS: A row number and condition, both ints.

    OUTPUTS: A row representing the live cells to the bottom right of the row 
             being passed.


    ALGORITHM(S): This function is a combination of the logic contained within
                  both B() and R() functions. Essentially, You are shifting
                  the lower row to the left and bitwise AND-ing it with the
                  upper row.

                  For the ZOMBIE condition you are negating the
                  upper row to flip the cells from dead to alive temporarily
                  and bitwise ANDing it with the shifted row.

                  The result of this is being returned.


*******************************************************************************/

uLLInt BR(int row, int condition) {
    // upper and lower rows being compared
    int upper, lower;

    // set upper to the row being passed
    upper = row;

    // set lower to the row below the one being passed
    lower = row + 1;

    // wrap the lower row around if it goes beyond the bottom of the grid
    lower %= SIZE;

    // the row being shifted
    uLLInt shifted_row = grid[lower];

    if (condition == NORMAL) {
        // check for a bit that needs to be wrapped around
        if (grid[lower] & LMASK) {
            // shift the row by 1
            shifted_row = ((shifted_row << 1) | RMASK);
            // check if there is a left neighbour after shifting it over
            return (grid[upper] & shifted_row);
        } else {
            return (grid[upper] & (shifted_row << 1));
        }
        
    }

    if (condition == ZOMBIE) {
        // check for a bit that needs to be wrapped around
        if (grid[lower] & LMASK) {
            // shift the upper by 1 and set the right most bit
            shifted_row = (shifted_row << 1) | RMASK;
            // check if there is a left neighbour after shifting it over
            return (~grid[upper]) & shifted_row;
        } else {
            return (~grid[upper]) & (shifted_row << 1);
        }
    }

}

