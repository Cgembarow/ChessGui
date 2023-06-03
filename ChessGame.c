#include <raylib.h>
#include <string.h>

#define size 8

struct ChessBoard {
    int boardGraphics[size][size];
    char* boardState[size][size];
};



void fillBoardGraphics(int arr[][size]) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if ((j - i) % 2 == 0)
                arr[i][j] = 0;
            else
                arr[i][j] = 1;
        }
    }
}

void initializeBoardState(char* boardState[size][size]) {
    char* startingPositions[size][size] = {
        {"BR", "BN", "BB", "BQ", "BK", "BB", "BN", "BR"},
        {"BP", "BP", "BP", "BP", "BP", "BP", "BP", "BP"},
        {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
        {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
        {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
        {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
        {"WP", "WP", "WP", "WP", "WP", "WP", "WP", "WP"},
        {"WR", "WN", "WB", "WQ", "WK", "WB", "WN", "WR"}
    };

    memcpy(boardState, startingPositions, sizeof(startingPositions));
}

void drawBoardState(char* boardState[size][size], Texture2D texture) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (boardState[i][j] != NULL) {
                int rowOffset = 0;
                int columnOffset = 0;

                // Determine the row and column offsets based on the piece color
                if (boardState[i][j][0] == 'W') {
                    rowOffset = 60;
                }

                // Determine the column offset based on the piece type
                switch (boardState[i][j][1]) {
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
                Rectangle sourceRec = { columnOffset, rowOffset, 60, 60 };
                Vector2 position = { j * 60, i * 60 };
                DrawTextureRec(texture, sourceRec, position, WHITE);
            }
        }
    }
}

void handleMouseInput(struct ChessBoard* board, int* redx, int* redy) {
    Vector2 mouseposition = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        bool clickedOnTile = false;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (mouseposition.x >= i * 60 && mouseposition.x < (i + 1) * 60 &&
                    mouseposition.y >= j * 60 && mouseposition.y < (j + 1) * 60) {

                    // if the rectangle is not shown, show it on click
                    if (*redx == i && *redy == j) {
                        *redx = -1;
                        *redy = -1;
                        continue;
                    }
                    *redx = i;
                    *redy = j;
                }
            }
        }
    }
}

void drawBoard(struct ChessBoard* board, int redx, int redy) {
    int r, g, b, a;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // if is red then stay red
            if (i == redx && j == redy) {
                DrawRectangle(i * 60, j * 60, 60, 60, RED);
                //if piece on square then piece.pos = mouseposition
                continue;
            }
            if (board->boardGraphics[i][j] == 0) {
                r = 232;
                g = 220;
                b = 202;
                a = 255;
            } else {
                r = 34;
                g = 139;
                b = 34;
                a = 255;
            }
            DrawRectangle(i * 60, j * 60, 60, 60, (Color){ r, g, b, a });
        }
    }
}


    
void swapElements(char* boardState[size][size], int x1, int y1, int x2, int y2) {
    char* temp = boardState[x1][y1];
    boardState[x1][y1] = boardState[x2][y2];
    boardState[x2][y2] = temp;
}

void updateBoardState(char* boardState[size][size]){
    /*find first mouse click
    if mouse click on a piece and not null then piece location = mouse location
    find second mouse click
    check if piece is on the square
    if true replace
    if false piece location = square clicked on
    */
   
}

int main() {
    int redx = -1;
    int redy = -1;
    const int WindowWidth = 480;
    const int WindowHeight = 480;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WindowWidth, WindowHeight, "SUPER AWESOME JOE BIDEN CHESS");

    Image ChessPieceArray = LoadImage("resources/ChessPiecesArray.png");
    Texture2D texture = LoadTextureFromImage(ChessPieceArray);
    UnloadImage(ChessPieceArray);

    struct ChessBoard board;

    // intializing constants
    int x = 0;
    int y = 0;

    SetTargetFPS(60);

    fillBoardGraphics(board.boardGraphics);
    initializeBoardState(board.boardState);
    //swapElements(board.boardState, 0, 0, 1, 1);

    while (!WindowShouldClose()) {

        //updateBoard(board.boardState);
        BeginDrawing();

        handleMouseInput(&board, &redx, &redy);
        drawBoard(&board, redx, redy);
        drawBoardState(board.boardState, texture);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}