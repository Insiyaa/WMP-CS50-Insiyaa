/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

//blank location...
int blank_r;
int blank_c;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }
    //blank tile location
    blank_r = d - 1;
    blank_c = d - 1;

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    int r,c,value,temp;
    value = (d * d) - 1;
    for(r = 0; r < d; r++)
    {
        for(c = 0; c < d; c++)
        {
            board[r][c] = value;
            value--;
        }
    }
    //swapping last two when d is even
    if(d % 2 == 0)
    {
        temp = board[d - 1][d - 3];
        board[d - 1][d - 3] = board[d - 1][d - 2];
        board[d - 1][d - 2] = temp;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    int r, c;
    printf("\n");
    for(r = 0; r < d; r++)
    {
        for(c = 0; c < d; c++)
        {
            if(r == blank_r && c == blank_c)
                {
                    printf(" _ ");
                }
            else
                printf("%2d ", board[r][c]);
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{
    int row, col, r, c;
    int temp;
    // Finding tile location...
    for(r = 0; r < d; r++)
    {
        for(c = 0; c < d; c++)
        {
            if(board[r][c] == tile)
            {
                row = r;
                col = c;
            }

        }
    }
    // Checking validity of the position, swapping and updating position of blank tile...
    int xi = blank_r + 1;
    int xd = blank_r - 1;
    int ci = blank_c + 1;
    int cd = blank_c - 1;
    if((row == blank_r) && (col == cd))
    {
        //swap
        temp = board[row][col];
        board[row][col] = 0;
        board[blank_r][blank_c] = temp;
        blank_r = row;
        blank_c = col;
        return true;
    }
    else if((row == xi) && (col == blank_c))
    {   temp = board[row][col];
        board[row][col] = 0;
        board[blank_r][blank_c] = temp;
        blank_r = row;
        blank_c = col;
        return true;
    }
    else if((row == blank_r) && (col == ci))
    {
        temp = board[row][col];
        board[row][col] = 0;
        board[blank_r][blank_c] = temp;
        blank_r = row;
        blank_c = col;
        return true;
    }
    else if((row == xd) && (col == blank_c))
    {
        temp = board[row][col];
        board[row][col] = 0;
        board[blank_r][blank_c] = temp;
        blank_r = row;
        blank_c = col;
        return true;
    }
    else
        return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{
    int r, c, value = 1;
    for(r = 0; r < d; r++)
    {
        for(c = 0; c < d; c++)
        {
            if(r == (d - 1) && c == (d - 1))
                break;
            if(!(board[r][c] == value))
                return false;
            value++;
        }
    }
    return true;
}