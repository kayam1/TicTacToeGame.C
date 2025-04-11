#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define SIZE 3
#define MAX_IN 10

void initXO(char xo[SIZE][SIZE])    //Fills the array with " - " to start the game
{
    int i,j;
    for(i=0;i<SIZE;i++)
        for(j=0;j<SIZE;j++)
            xo[i][j] = '-';
}

void printGrid(char xo[SIZE][SIZE]) //Prints the grid either at the start of the game or every time someone makes a move to show it to the players
{ 
    int i,j;
    printf("TRILIZA\n------------------------------------------\n");
    for(i=0;i<SIZE;i++)
    {
        for(j=0;j<SIZE;j++)
        {
            if (j == (SIZE-1))
                printf(" %c",xo[i][j]);
            else
                printf(" %c |",xo[i][j]);
        }
        if (i < (SIZE-1))
        printf("\n---+---+---\n");
    }
    printf("\n");
}

void pauseCls() //Pause custom message and clear screen after user input
{
    printf("\nPress any key to try again...");
    system("pause > nul");
    system("cls");
}

void clearBuffer()  //Clears buffer before string input
{
    while ( getchar() != '\n' );
}

void getCoordinates(char coords[MAX_IN], int p, int turn)   //Gets coordinates for each player's move each turn
{
    if (turn == 1)
        printf("\nPlayer1, you play with: %c", p);
    else if (turn == 2)
        printf("\nPlayer2, you play with: %c", p);
    
    printf("\nWhich row and column to put %c in? ", p);
    printf("\nType two numbers from 0 to %d with a space between them (example: 0 0): ",SIZE-1);
    if (fgets(coords,MAX_IN,stdin) == NULL) 
        coords[strcspn(coords, "\n\r")] = '\0';  //clear trailing \n from string 
}

bool stringToInt(char coords[MAX_IN], int *row, int *column)    //Input type string gets turned into 2 ints to use as indexes for the xo array (with defense)
{
    char *str_num1, *str_num2, *extra_num, *end;
    
    coords[strcspn(coords, "\n\r")] = '\0'; //trim whitespaces 

    str_num1 = strtok(coords, " ");
    str_num2 = strtok(NULL, " ");
    
    extra_num = strtok(NULL, " ");

    if (isdigit(*str_num1) == false || isdigit(*str_num2) == false)     //check if input is character 
        return printf("Error: input must consist of numbers only"), true;

    if  (str_num1 == NULL || str_num2 == NULL)      //check if there is 0 or 1 input instead of 2 
        return printf("Error: please type exactly two numbers"), true;

    if (extra_num != NULL)  //check if there are 3 inputs instead of 2
        return printf("Error: please type exactly two numbers"), true;
       
    *row = strtol(str_num1, &end, 10);
    *column = strtol(str_num2, &end, 10);

    if (*row < 0 || *row > SIZE-1 || *column < 0 || *column > SIZE-1)   //check for out of grid input
        return  printf("\nError entering coordinates \nCoordinates must be from 0 to %d", SIZE-1), true;

    return false;
}

bool duplicateInput(char xo_value)  //Duplicate input defense
{    
    if (xo_value == 'X' || xo_value == 'O')
        return printf("\nError: Someone already picked those coordinates. Try something new"), true;

    return false;
}

void changeGrid(char xo[SIZE][SIZE], int i, int j, char p) //Puts player1's or player2's symbol at the coordinates they chose
{
        xo[i][j] = p;
} 

void playerTurn(char xo[SIZE][SIZE], int p, int turn)//Goes through all the steps for each player to play his turn by calling other functions and restarting on errors
{
    char coords[MAX_IN];    //string to store initial user input ex. (0 0)
    int row, column;    //row = first number of user input, column = second number of user input
    bool error;     

    do {
    error = false;
    printGrid(xo);  

    getCoordinates(coords, p, turn);    

    error = stringToInt(coords, &row, &column); 
    if (error == true)
    {
        pauseCls();
        continue;
    }

    error = duplicateInput(xo[row][column]);
    if (error == true)
    {
        pauseCls();
        continue;
    }

    } while(error == true);  
    changeGrid(xo, row, column, p);
    system("cls");
}

bool Win (char xo[SIZE][SIZE], int p1count, int p2count)     //Checks if 3 X's or 3 O's are next to eachother and returns true if they are and false if they are not
{
    if (p1count == SIZE)    
    {
        printGrid(xo);
        printf("\nPlayer 1 Wins!");
        return true;
    }
    else if (p2count == SIZE)
    {
        printGrid(xo);
        printf("\nPlayer 2 Wins!");
        return true;
    }
    return false;
}

void countP1P2 (char xo_value, int *p1count, int *p2count, char p1, char p2)    //Counts X's and O's in specific spots
{ 
    if (xo_value == p1)
        (*p1count)++;
    if (xo_value == p2)
        (*p2count)++;
}

bool gameOver(char xo[SIZE][SIZE], char p1, char p2)//Checks if someone made the winning move and returns true if the game should be over or false if it should continue 
{
    int i,j, draw = 0, p1count, p2count;

    for(i=0;i<SIZE;i++)//HORIZONTAL WIN + DRAW CALCULATION
    {
        p1count = 0;
        p2count = 0;
        for(j=0;j<SIZE;j++)
        {
            countP1P2(xo[i][j], &p1count, &p2count, p1, p2);
            if (xo[i][j] != '-')
                draw++;
        }
        if (Win(xo, p1count, p2count) == true)
            return true;
    }

    if (draw == SIZE*SIZE) //DRAW CONDITION
    {
        printGrid(xo);
        printf("\nIt's a Draw...");
        return true;
    }

    for(j=0; j<SIZE; j++) //VERTICAL WIN
    {
        p1count = 0;
        p2count = 0;
        for (i=0; i<SIZE; i++)
            countP1P2(xo[i][j], &p1count, &p2count, p1, p2);
        if (Win(xo, p1count, p2count) == true)
            return true;
    }

    p1count = 0;
    p2count = 0;
    for(i=0;i<SIZE;i++) //MAIN DIAGONAL WIN
        countP1P2(xo[i][i], &p1count, &p2count, p1, p2);
    if (Win(xo, p1count, p2count) == true)
        return true;
    
    p1count = 0;
    p2count = 0;
    for(i=0;i<SIZE;i++) //REVERSE DIAGONAL WIN
        countP1P2(xo[i][SIZE-1-i], &p1count, &p2count, p1, p2);    
    if (Win(xo, p1count, p2count) == true)
        return true;

    return false;
}

int main()
{   
    int i;
    char p1, p2, xo[SIZE][SIZE];

    initXO(xo); 
    //X or O selection from player1 
    do 
    {
        printGrid(xo);  
        printf("\nplayer1: X or O? ");
        scanf(" %c",&p1);
        if (p1 != 'x' && p1 != 'X' && p1 != 'o' && p1 != 'O')  
        {
            printf("\nError: invalid input");
            pauseCls(); 
        }

    }while (p1 != 'x' && p1 != 'X' && p1 != 'o' && p1 != 'O');  //player 1 must select one of the 2 symbols (upper or lower case)
    
    //we give player2 the symbol that player1 did not select and give player 1 the one he chose in upper case
    switch (p1) 
    {
    case 'x':
    case 'X':
        p1 = 'X';
        p2 = 'O';
        break;
    case 'o':
    case 'O':
        p1 = 'O';
        p2 = 'X';
        break;
    }
    //printing results of above code for clarity to the player
    printf("\nPlayer1: %c",p1); 
    printf("\nPlayer2: %c\n",p2);
    printf("Press any key to start the game!!!");
    system("pause > nul");
    system("cls");
    
    clearBuffer();  
    i=1;    //turn 1 == player1 plays first
    do
    {   
        if (i%2 == 1) 
            playerTurn(xo, p1, 1);  
        else if (i%2 == 0)
            playerTurn(xo, p2, 2);
        i++;    //change turn
    }while (gameOver(xo, p1, p2) == false);

    printf("\n\n\n");
    system("Pause");
    return 0;
}