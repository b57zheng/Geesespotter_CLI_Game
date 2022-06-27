/////////////////////////////////////////////////////////////////////////////////////
// Project: Geesespotter
// Authors: Bowen Zheng
// Compile and run with: g++ -o main geesespotter.cpp geesespotter_lib.cpp -std=c++11
/////////////////////////////////////////////////////////////////////////////////////

#include "geesespotter_lib.h"

char * createBoard(std::size_t xdim, std::size_t ydim);
void computeNeighbors(char * board, std::size_t xdim, std::size_t ydim);
void hideBoard(char * board, std::size_t xdim, std::size_t ydim);
void cleanBoard(char * board);
void printBoard(char * board, std::size_t xdim, std::size_t ydim);
int reveal(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
int mark(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
bool isGameWon(char * board, std::size_t xdim, std::size_t ydim);

/*int main()
{
    std::cout << "Hello" << std::endl;
    char *board = createBoard(5, 5);
    printBoard(board, 5,5);
}
*/


char * createBoard(std::size_t xdim, std::size_t ydim)
{
    char *board{ new char [xdim*ydim]{} }; //new is a key word for allocating memory.
    //declare array
    for (int i{0}; i <= xdim*ydim - 1; ++i) //index (board size - 1)
    {
        board[i] = 0;
    }
    return board;
    //return array

    //make a character arry, dynamically allocated, it is a pointer, size = xdim*ydim;
    //make every element = 0
    //return charater array;
    //return nullptr;
}

void computeNeighbors(char * board, std::size_t xdim, std::size_t ydim)
{
    for (int row{0}; row <= ydim - 1; ++row)
    {
        for (int col{0}; col <= xdim - 1; ++col)
        {
            //board [width*row# + col#] index: 0,1,2,3,4,5,6,7,8,9 , 
            if ( board[ xdim*row + col ] == 0 )
            {
                if (row > 0 && col > 0 && board[ xdim*(row - 1) + (col - 1) ] == 9) //if board = geese (9)
                {
                    board[ xdim*row + col ] =  board[ xdim*row + col ] + 1; //surrending number(board[ xdim*row + col ]) + 1
                }
                if (row > 0 && board[ xdim*(row - 1) + col ] == 9)
                {
                    board[ xdim*row + col ] =  board[ xdim*row + col ] + 1;
                }
                if (row > 0 && col < xdim -1 && board[ xdim*(row - 1) + (col + 1) ] == 9)
                {
                    board[ xdim*row + col ] =  board[ xdim*row + col ] + 1;
                }
                if (col > 0 && board[ xdim*row + (col - 1) ] == 9)
                {
                    board[ xdim*row + col ] =  board[ xdim*row + col ] + 1;
                }
                if (col < xdim - 1 && board[ xdim*row + (col + 1) ] == 9)
                {
                    board[ xdim*row + col ] =  board[ xdim*row + col ] + 1;
                }
                if (row < ydim - 1 && col > 0 && board[ xdim*(row + 1) + (col - 1) ] == 9)
                {
                    board[ xdim*row + col ] =  board[ xdim*row + col ] + 1;
                }
                if (row < ydim - 1 && board[ xdim*(row + 1) + col ] == 9)
                {
                    board[ xdim*row + col ] =  board[ xdim*row + col ] + 1;
                }
                if (row < ydim - 1 && col < xdim - 1 && board[ xdim*(row + 1) + (col + 1) ] == 9)
                {
                    board[ xdim*row + col ] =  board[ xdim*row + col ] + 1;
                }
            }
        }
    }
   
}

void hideBoard(char * board, std::size_t xdim, std::size_t ydim)
{
    for (int row{0}; row <= ydim - 1; ++row)
    {
        for (int col{0}; col <= xdim - 1; ++col)
        {
            board[ xdim*row + col ] = board[ xdim*row + col ] + hiddenBit();
        }
    }
    //hide all board elements
    //board is a character array --> hidden fields
}

void cleanBoard(char * board)
{
//deallocate the hidden board (deallocate array)
    delete[] board;
    board = nullptr;
}

void printBoard(char * board, std::size_t xdim, std::size_t ydim)
{
    for (int row{0}; row <= ydim - 1; ++row)
    {
        for (int col{0}; col <= xdim - 1; ++col)
        {
            if (board[ xdim*row + col ] >= markedBit() + hiddenBit() ) // 0x10 + 0x20 
            {
                std::cout << "M";
            }
            else if (board[ xdim*row + col ] >= hiddenBit() ) //0x20
            {
                std::cout << "*";
            }
            else //reveal
            {
                std::cout << board[ xdim*row + col ] + 0; // ASCII value print number (char 47-57)
            }
        }
        std::cout << std::endl; //print new line 
    }


}

int reveal(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc)
{
    if ( board[ xdim*yloc + xloc ] >= markedBit() + hiddenBit() ) //check if it is marked 10 +20
    {
        return 1;
    }
    else if ( board[ xdim*yloc + xloc ] >= hiddenBit() + 9) //reveal a gossse
    {
        board[ xdim*yloc + xloc ] =  board[ xdim*yloc + xloc ] - hiddenBit();
        return 9;
    }
    else if ( board[ xdim*yloc + xloc ] <= 8 ) // reveal a revealed field
    {
        return 2;
    }
    else
    {
        board[ xdim*yloc + xloc ] = board[ xdim*yloc+ xloc ] - hiddenBit(); //reveal a hidden field
        if ( board[ xdim*yloc + xloc ] == 0 )
        {
            if (yloc > 0 && xloc > 0 && board[ xdim*(yloc - 1) + (xloc - 1) ] >= hiddenBit()) //check boarder
                {
                    board[ xdim*(yloc - 1) + (xloc - 1) ] = board[ xdim*(yloc - 1) + (xloc - 1) ] - hiddenBit(); 
                }
                if (yloc > 0 && board[ xdim*(yloc - 1) + xloc ] >= hiddenBit())
                {
                    board[ xdim*(yloc - 1) + xloc ] =board[ xdim*(yloc - 1) + xloc ] - hiddenBit();
                }
                if (yloc > 0 && xloc < xdim -1 && board[ xdim*(yloc - 1) + (xloc + 1) ] >= hiddenBit())
                {
                    board[ xdim*(yloc - 1) + (xloc + 1) ] = board[ xdim*(yloc - 1) + (xloc + 1) ] - hiddenBit();
                }
                if (xloc > 0 && board[ xdim*yloc + (xloc - 1) ] >= hiddenBit())
                {
                    board[ xdim*yloc + (xloc - 1) ] = board[ xdim*yloc + (xloc - 1) ] - hiddenBit();
                }
                if (xloc < xdim - 1 && board[ xdim*yloc + (xloc + 1) ] >= hiddenBit())
                {
                    board[ xdim*yloc + (xloc + 1) ] = board[ xdim*yloc + (xloc + 1) ] - hiddenBit();
                }
                if (yloc < ydim - 1 && xloc > 0 && board[ xdim*(yloc + 1) + (xloc - 1) ] >= hiddenBit())
                {
                    board[ xdim*(yloc + 1) + (xloc - 1) ] = board[ xdim*(yloc + 1) + (xloc - 1) ] - hiddenBit();
                }
                if (yloc < ydim - 1 && board[ xdim*(yloc + 1) + xloc ] >= hiddenBit())
                {
                    board[ xdim*(yloc + 1) + xloc ] = board[ xdim*(yloc + 1) + xloc ] - hiddenBit();
                }
                if (yloc < ydim - 1 && xloc < xdim - 1 && board[ xdim*(yloc + 1) + (xloc + 1) ] >= hiddenBit())
                {
                    board[ xdim*(yloc + 1) + (xloc + 1) ] = board[ xdim*(yloc + 1) + (xloc + 1) ] - hiddenBit();
                }
        }
    }
    return 0;
}

int mark(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc)
{
    //mark a particular coordinate on our board (1d array)
    //find the location value for board (from 2d array to 1d array)
    // *array = {0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00}
    /* 000
       090
       000
       xdim = 3
       ydim = 3
       xloc = 2
       yloc = 2
    yloc*xdim + xloc = element
    how to mark at the element?
    :
    make it a mark field (0x10) 0001
    if not hidden (it is revealed), return 2;
    if its marked already, unmark it
    */
    if (board[ xdim*yloc + xloc ] >= markedBit() + hiddenBit() ) // 0x10 + 0x20 
    {
        board[ xdim*yloc + xloc ] = board[ xdim*yloc + xloc ] - markedBit();
    }
    else if (board[ xdim*yloc + xloc ] >= hiddenBit() ) //0x20
    {
        board[ xdim*yloc + xloc ] = board[ xdim*yloc + xloc ] + markedBit();
    }
    else //reveal
    {
        return 2;
    }
    return 0;
}

bool isGameWon(char * board, std::size_t xdim, std::size_t ydim)
{
    for (int row{0}; row <= ydim - 1; ++row)
    {
        for (int col{0}; col <= xdim - 1; ++col)
        {
            if ( board[ xdim*row + col ] >= hiddenBit() ) // check if the field is hidden
            {
                if ( board[ xdim*row + col ] != hiddenBit() + 9 && board[ xdim*row + col ] != markedBit() + hiddenBit() + 9 ) // check if there isn't a geese in hidden field
                {
                    return false;
                }
            }
        }
    }
    return true;
}
