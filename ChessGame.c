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
void initializeBoardState(Texture2D texture){

   

    

    Vector2 positionKB = {240,0};
    Vector2 positionQB = {180,0};
    Vector2 positionBB1 = {120,0};
    Vector2 positionBB2 = {300,0};
    Vector2 positionHB1 = {60,0};
    Vector2 positionHB2 = {360,0};
    Vector2 positionRB1 = {0,0};
    Vector2 positionRB2 = {420,0};

    Vector2 positionKW = {240,420};
    Vector2 positionQW = {180,420};
    Vector2 positionBW1 = {120,420};
    Vector2 positionBW2 = {300,420};
    Vector2 positionHW1 = {60,420};
    Vector2 positionHW2 = {360,420};
    Vector2 positionRW1 = {0,420};
    Vector2 positionRW2 = {420,420};

   


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
    
    //B = 0->15
    //W = 16 -> 31
  //Draw BLack Pieces
    DrawTextureRec(texture,king,positionKB,WHITE );
    DrawTextureRec(texture,queen,positionQB,WHITE );
    DrawTextureRec(texture,bishop,positionBB1,WHITE );
    DrawTextureRec(texture,bishop,positionBB2,WHITE );
    DrawTextureRec(texture,knight,positionHB1,WHITE );
    DrawTextureRec(texture,knight,positionHB2,WHITE );
    DrawTextureRec(texture,rook,positionRB1,WHITE );
    DrawTextureRec(texture,rook,positionRB2,WHITE);
    for(int i = 0; i < 8; i++){
        DrawTextureRec(texture,pawn,(Vector2){i * 60, 60},WHITE );
    }
   //Draw White Pieces
    DrawTextureRec(texture,kingB,positionKW,WHITE );
    DrawTextureRec(texture,queenB,positionQW,WHITE );
    DrawTextureRec(texture,bishopB,positionBW1,WHITE );
    DrawTextureRec(texture,bishopB,positionBW2,WHITE );
    DrawTextureRec(texture,knightB,positionHW1,WHITE );
    DrawTextureRec(texture,knightB,positionHW2,WHITE );
    DrawTextureRec(texture,rookB,positionRW1,WHITE);
    DrawTextureRec(texture,rookB,positionRW2,WHITE );
    for(int i = 0; i < 8; i++){
        DrawTextureRec(texture,pawnB,(Vector2){i * 60, 360},WHITE);
    }



};

int main(){

    const int WindowWidth = 480;
    const int WindowHeight = 480;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WindowWidth, WindowHeight, "MyGame");
    

     //LoadImage("./Chess_blt60.png");
    Image ChessPieceArray = LoadImage("resources/ChessPiecesArray.png");
    Texture2D texture = LoadTextureFromImage(ChessPieceArray);
    UnloadImage(ChessPieceArray);
  
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

    SetTargetFPS(60);

    fillBoardGraphics(board.BoardGraphics, board.BoardCells);
    //initializeBoardState();

    while(!WindowShouldClose()){
        BeginDrawing();

        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(board.BoardGraphics[i][j] == 0){
                    r = 232;
                    g = 220;
                    b = 202;
                    a = 255;
                }
                else {
                    r = 34;
                    g = 139;
                    b = 34;
                    a = 255;
                }
                DrawRectangle(x + i*60, y + j*60, 60, 60, (Color){r,g,b,a});  
            }
        }
      initializeBoardState(texture);
        EndDrawing(); 
    }
    CloseWindow();
}
