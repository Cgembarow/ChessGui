#include <raylib.h>
#include <string.h>

#define SIZE 8

enum Outcome {
    LEGAL,
    ILLEGAL,
    CAPTURE
};

struct ChessBoard
{
    int boardGraphics[SIZE][SIZE];
    char *boardState[SIZE][SIZE];
};

struct SelectedPiece{
    int x;
    int y;
    bool isSelected;
};

 enum Outcome isValidMove(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE]){
    switch (boardState[selectedX][selectedY][1])
    {
        case 'P' : {
            if(selectedY == targetY && targetY == selectedY + 1){
                return LEGAL;
            }
            else return ILLEGAL; 
        }

        default : return LEGAL;
    }
}


void fillBoardGraphics(int arr[][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if ((j - i) % 2 == 0)
                arr[i][j] = 0;
            else
                arr[i][j] = 1;
        }
    }
}

void initializeBoardState(char *boardState[SIZE][SIZE])
{
    char *startingPositions[SIZE][SIZE] = {
        {"BR", "BN", "BB", "BQ", "BK", "BB", "BN", "BR"},
        {"BP", "BP", "BP", "BP", "BP", "BP", "BP", "BP"},
        {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
        {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
        {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
        {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
        {"WP", "WP", "WP", "WP", "WP", "WP", "WP", "WP"},
        {"WR", "WN", "WB", "WQ", "WK", "WB", "WN", "WR"}};

    memcpy(boardState, startingPositions, sizeof(startingPositions));
}

void drawBoardState(char *boardState[SIZE][SIZE], Texture2D texture)
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (boardState[i][j] != NULL)
            {
                int rowOffset = 0;
                int columnOffset = 0;

                // Determine the row and column offsets based on the piece color
                if (boardState[i][j][0] == 'W')
                {
                    rowOffset = 60;
                }

                // Determine the column offset based on the piece type
                switch (boardState[i][j][1])
                {
                case 'Q':
                    columnOffset = 0;
                    break;
                case 'K':
                    columnOffset = 60;
                    break;
                case 'R':
                    columnOffset = 120;
                    break;
                case 'N':
                    columnOffset = 180;
                    break;
                case 'B':
                    columnOffset = 240;
                    break;
                case 'P':
                    columnOffset = 300;
                    break;
                default:
                    // Invalid piece type, skip drawing
                    continue;
                }

                // Draw the piece from the texture
                Rectangle sourceRec = {columnOffset, rowOffset, 60, 60};
                Vector2 position = {j * 60, i * 60};
                DrawTextureRec(texture, sourceRec, position, WHITE);
            }
        }
    }
}

void swapElements(char *boardState[SIZE][SIZE], int x1, int y1, int x2, int y2)
{
    char *temp = boardState[y1][x1]; // Swap y1 and x1
    boardState[y1][x1] = boardState[y2][x2]; // Swap y2 and x2
    boardState[y2][x2] = temp; // Swap y2 and x2
}


void handleMouseInput(struct ChessBoard *board, struct SelectedPiece *selectedPiece)
{
    Vector2 mouseposition = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        int targetX = mouseposition.x / 60;
        int targetY = mouseposition.y / 60;

        if (!selectedPiece->isSelected)
        {
            // First click, select the piece
            selectedPiece->x = targetX;
            selectedPiece->y = targetY;
            selectedPiece->isSelected = true;
        }
        else
        {
            // Second click, swap selected piece with target tile
            int selectedX = selectedPiece->x;
            int selectedY = selectedPiece->y;

            /*enum Outcome result = isValidMove(selectedX, selectedY, targetX, targetY, board);
            switch (result){

                case LEGAL: {
                    swapElements(board->boardState, selectedX, selectedY, targetX, targetY);
                    break;
                }

                case ILLEGAL: {
                    break;
                }

                case CAPTURE: {
                    swapElements(board->boardState, selectedX, selectedY, targetX, targetY);
                    board->boardState[selectedX][selectedY] = NULL;
                    break;
                }
            }*/
            
            swapElements(board->boardState, selectedX, selectedY, targetX, targetY);
            // Reset selected piece
            selectedPiece->x = -1;
            selectedPiece->y = -1;
            selectedPiece->isSelected = false;
        }
    }
}

void handleSelectedPiece(struct ChessBoard *board, struct SelectedPiece *selectedPiece, int *redx, int *redy)
{
    if (selectedPiece->isSelected)
    {
        *redx = selectedPiece->x;
        *redy = selectedPiece->y;
    }
    else
    {
        *redx = -1;
        *redy = -1;
    }
}


void drawBoard(struct ChessBoard *board, int redx, int redy)
{
    int r, g, b, a;

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            // if is red then stay red
            if (i == redx && j == redy)
            {
                DrawRectangle(i * 60, j * 60, 60, 60, RED);
                // if piece on square then piece.pos = mouseposition
                continue;
            }
            if (board->boardGraphics[i][j] == 0)
            {
                r = 232;
                g = 220;
                b = 202;
                a = 255;
            }
            else
            {
                r = 34;
                g = 139;
                b = 34;
                a = 255;
            }
            DrawRectangle(i * 60, j * 60, 60, 60, (Color){r, g, b, a});
        }
    }
}






int main()
{
    int redx = -1;
    int redy = -1;
    const int WindowWidth = 480;
    const int WindowHeight = 480;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WindowWidth, WindowHeight, "CHESS the Game");

    Image ChessPieceArray = LoadImage("resources/ChessPiecesArray.png");
    Texture2D texture = LoadTextureFromImage(ChessPieceArray);
    UnloadImage(ChessPieceArray);

    struct ChessBoard board;
    struct SelectedPiece selectedPiece;

   
   int x = 0;
   int y = 0;


    SetTargetFPS(60);

    fillBoardGraphics(board.boardGraphics);
    initializeBoardState(board.boardState);
    // swapElements(board.boardState, 0, 0, 1, 1);
    
    
    while (!WindowShouldClose())
    {
          
        
        BeginDrawing();

        handleMouseInput(&board, &selectedPiece);
        handleSelectedPiece(&board, &selectedPiece, &redx, &redy);
        drawBoard(&board, redx, redy);
        drawBoardState(board.boardState, texture);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

//cc ChessGame.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11