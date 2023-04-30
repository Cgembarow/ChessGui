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

    const int WindowWidth = 480;
    const int WindowHeight = 480;
    InitWindow(WindowWidth, WindowHeight, "MyGame");

    //LoadImage("./Chess_blt60.png");
    Image ChessPieceArray = LoadImage("resources/ChessPiecesArray.png");
    
    Rectangle queen = {0, 0, 60, 60};
    Rectangle king = {60, 0, 60, 60};
    Rectangle rook = {120, 0, 60, 60};
    Rectangle knight = {180, 0, 60, 60};
    Rectangle bishop = {240, 0, 60, 60};
    Rectangle pawn = {300, 0, 60, 60};
    Rectangle queenB = {0, 60, 60, 60};
    Rectangle kingB = {60, 60, 60, 60};
    Rectangle rookB = {120, 60, 60, 60};
    Rectangle knightB = {180, 60, 60, 60};
    Rectangle bishopB = {240, 60, 60, 60};
    Rectangle pawnB = {300, 60, 60, 60};
    Texture2D texture = LoadTextureFromImage(ChessPieceArray);


    
  
    struct ChessBoard board;
Vector2 position = {0,0};
    //intializing constants
    int x = 0;
    int y = 0;
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 0;

    //pieces on the board
    int piecesRemaning = 32;

    


   
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
                DrawRectangle(x + i*60, y + j*60, 60, 60, (Color){r,g,b,a});
                
            }
            
        }
       DrawTextureRec(texture,king,position,BLACK );
     
       EndDrawing(); 
    }

    CloseWindow();
}
