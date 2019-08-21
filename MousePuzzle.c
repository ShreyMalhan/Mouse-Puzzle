#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

#define MAX_DIMENSION 20

// constant definitions for the different cell states
const char WALL    = '1';
const char SPACE   = '0';
const char VISITED = '.';
const char MOUSE   = 'm';
const char EXIT    = 'e';

typedef enum BOOL { false, true } Boolean;



struct CELL
{
    int row;
    int column;
};
typedef struct CELL Cell;

typedef struct CELL_NODE CellNode;

struct CELL_NODE
{
    Cell cell;
    CellNode *next;
};

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------

CellNode *top = NULL;

// a 2D array used to store the maze
char maze[MAX_DIMENSION][MAX_DIMENSION];
int mazeRows;
int mazeCols;

// holds the location of the mouse and escape hatch
Cell mouse;
Cell escape;

//--------------------------------------------------------------------------------------
// Basic Cell Manipulation
//--------------------------------------------------------------------------------------

// returns true if the cells are at the same position in our maze
Boolean equalCells(const Cell cell1, const Cell cell2){
    if(cell1.row == cell2.row && cell1.column == cell2.column){
        return true;
    }
    else{
        return false;
    }
}

// returns a new cell object
Cell makeCell(const int row, const int col){
    Cell newCell;
    newCell.row = row;
    newCell.column = col;
    
    return newCell;
}


// returns true if the cell is within our maze
Boolean validCell(const Cell theCell){
    if(theCell.row >= 0 && theCell.row-1 < mazeRows && theCell.column >=0 && theCell.column < mazeCols-1){
        return true;
    }else{
        return false;
    }
}

// routines for managing our backtracking

// returns true if there are no more cells to try
Boolean noMoreCells(){
    if(top->next == NULL){
        return true;
    }
    else{
        return false;
    }
}


// returns the next cell to try for a path out of the maze
Cell nextCell(){
    Cell temp = {0,0};
    CellNode *curr = top;
    if(curr != NULL){
        temp.row = top->cell.row;
        temp.column = top -> cell.column;
        curr = curr->next;
        top = curr;
    }
    
    return temp;
}


// introduces a new cell to try
void addCell(const Cell cell){
    CellNode *newCell;
    newCell = malloc(sizeof(CellNode));
    (*newCell).cell.row = cell.row;
    newCell->cell.column = cell.column;
    
    newCell-> next = top;
    top = newCell;
}

//printing out the maze
void printMaze(){
    for(int i=0; i < mazeRows; i++){
        for(int j=0; j<mazeCols;j++){
            printf("%c ", maze[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
}

//initializing the maze
void loadMaze(){
    char r;
    char c;
    char m;
    FILE *inputfile = NULL;
    char name[20];
        
    inputfile = freopen("put the path of the inputFile", "r");
    
    //printing out an error message if the inputfile is not //invalid	    
	if (inputfile == NULL){
        printf("Unable to open input file.\n");
        exit(0);
    }
    
	else{
        r = fgetc(inputfile);   //reading the 1st character of the file to determine the number of rows
        mazeRows = r - 48;	    //to convert it into ASCII character
        m = fgetc(inputfile);   
        c = fgetc(inputfile);   // reading the 3rd character of the file to determine the number of coloums
        mazeCols = c - 48;      //to convert it into ASCII character
        
        // setting up the maze
        for(int i =0; i<mazeRows; i++){
            for(int j = 0; j<mazeCols; j++){
                if((fgetc(inputfile) != '1') || (fgetc(inputfile) != '0') || (fgetc(inputfile) != 'e') || (fgetc(inputfile) != 'm')){
                    maze[i][j] = fgetc(inputfile);
                }
            }
        }
    }
    fclose(inputfile);
}

// returns true if there's a solution to the maze
Boolean solveMaze(){
    if((maze[mouse.row][mouse.column -1] == EXIT) || (maze[mouse.row -1][mouse.column]) == EXIT || (maze[mouse.row][mouse.column +1] == EXIT) || (maze[mouse.row+1][mouse.column] == EXIT)){
        return true;
    }
    else{
        return false;
    }
}

//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------

int main( int argc, char *argv[] )
{
    loadMaze();

    Cell startCell, currentCell, goalCell,a1,b2,c3,d4;
    for(int i =0; i<mazeRows; i++){
        for(int j = 0; j<mazeCols; j++){
            //Finding the goalCell
            if(maze[i][j] == EXIT){             
                goalCell.row = i;
                goalCell.column = j;
                escape = goalCell;
            }
            //Finding the location of the mouse 
            if(maze[i][j] == MOUSE){            
                startCell.row = i;
                startCell.column = j;
                currentCell = startCell;
                mouse = currentCell;
            }
        }
    }
    
    printMaze();

    while(!(equalCells(currentCell, goalCell))){

        //Converting not visited cells to visited so that mouse don't go back to those cells
        maze[currentCell.row][currentCell.column] = VISITED;    
        
        // initializing the neighbouring cells
        a1 = makeCell(currentCell.row, currentCell.column - 1);
        b2 = makeCell(currentCell.row - 1, currentCell.column);
        c3 = makeCell(currentCell.row, currentCell.column + 1);
        d4 = makeCell(currentCell.row + 1, currentCell.column);
        
        //checks the neighbouring cells if they are valid and for space and adding them to the list
        if(validCell(a1) && (maze[a1.row][a1.column] == SPACE || maze[a1.row][a1.column] == EXIT)){
            addCell(a1);
        }
        
        if(validCell(b2) && (maze[b2.row][b2.column] == SPACE || maze[b2.row][b2.column] == EXIT)){
            addCell(b2);
        }
        
        if(validCell(c3) && (maze[c3.row][c3.column] == SPACE || maze[c3.row][c3.column] == EXIT)){
            addCell(c3);
        }
        
        if(validCell(d4) && (maze[d4.row][d4.column] == SPACE || maze[d4.row][d4.column] == EXIT)){
            addCell(d4);
        }
        
        if (top == NULL){
            printMaze();
            mouse = currentCell;
            break;
        }else{
            currentCell = nextCell();
            mouse = currentCell;
            printMaze();
        }
    }
    
    //Printing out the result of the puzzle
    if ( solveMaze() )
        printf( "The mouse is free!!!!\n" );
    else
        printf( "The mouse is trapped!!!!\n" );
    
    printf( "\nEnd of processing\n" );
    
    return EXIT_SUCCESS;
}