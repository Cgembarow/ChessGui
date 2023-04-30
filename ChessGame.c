#include<stdio.h>
#include <raylib.h>
#define size 8

struct ChessBoard{
    int BoardCells[size][size];
    int BoardGraphics[size][size];
    int BoardState[size][size];
};

//determine if cell is green or white
void fillBoardGraphics(int arr[][size], int arr2[][size]){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if((j-i) %2 == 0)arr[i][j] = 0;
            else arr[i][j] = 1; 
        }
    }
};

//intial location for each piece
void initializeBoardState(){
    //B = 0->15
    //W = 16 -> 31





};

int main(){

    const int WindowWidth = 800;
    const int WindowHeight = 800;
    InitWindow(WindowWidth, WindowHeight, "MyGame");

    //LoadImage("./Chess_blt60.png");
    Image pawn = LoadImage("resources/Chess_blt60.png");
    
    Texture2D texture = LoadTextureFromImage(pawn);
    
  
    struct ChessBoard board;

    //intializing constants
    int x = 0;
    int y = 0;
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 0;

    //pieces on the board
    int piecesRemaning = 32;

    


    //const int WindowWidth = 800;
    //const int WindowHeight = 800;
    //InitWindow(WindowWidth, WindowHeight, "MyGame");
    SetTargetFPS(60);

    fillBoardGraphics(board.BoardGraphics, board.BoardCells);
    //initializeBoardState();

   
    while(!WindowShouldClose())
    {
        BeginDrawing();

        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(board.BoardGraphics[i][j] == 0){
                    r = 255;
                    g = 255;
                    b = 255;
                    a = 255;
                }
                else {
                    r = 0;
                    g = 255;
                    b = 0;
                    a = 255;
                }
                DrawRectangle(x + i*100, y + j*100, 100, 100, (Color){r,g,b,a});
                
            }
            
        }
       DrawTexture(texture, 100,100,(Color){0,0,0,255});
     
       EndDrawing(); 
    }

    CloseWindow();
}
