#include <raylib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE 8

typedef struct Node
{
    int x1;
    int y1;
    int x2;
    int y2;
    struct Node *next;
} Node;

Node *head = NULL;

Node *createNode(int x1, int y1, int x2, int y2)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->x1 = x1;
    newNode->y1 = y1;
    newNode->x2 = x2;
    newNode->y2 = y2;
    newNode->next = NULL;
    return newNode;
}

void addNode(int x1, int y1, int x2, int y2)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->x1 = x1;
    newNode->y1 = y1;
    newNode->x2 = x2;
    newNode->y2 = y2;
    newNode->next = NULL;

    if (head == NULL)
    {
        // The list is empty, so the new node becomes the head
        head = newNode;
    }
    else
    {
        // Traverse the list to find the last node
        Node *current = head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        // Add the new node to the end of the list
        current->next = newNode;
    }
}

void makeListEmpty()
{
    Node *current = head;
    while (current != NULL)
    {
        Node *temp = current;
        current = current->next;
        free(temp); // Free each node of the linked list
    }
    head = NULL; // Set the head pointer to NULL to indicate an empty list
}

void printLinkedList(Node *head)
{
    Node *current = head;

    while (current != NULL)
    {
        printf("(%d, %d, %d, %d) -> ", current->x1, current->y1, current->x2, current->y2);
        current = current->next;
    }

    printf("NULL\n");
}

enum Outcome
{
    LEGAL,
    ILLEGAL,
    CAPTURE
};

struct ChessBoard
{
    int boardGraphics[SIZE][SIZE];
    char *boardState[SIZE][SIZE];
    bool whiteTurn;
};

struct SelectedPiece
{
    int x;
    int y;
    bool isSelected;
};

typedef struct ValidMove
{
    int x1;
    int y1;
    int x2;
    int y2;
} ValidMove;

void freeTempBoard(char *tempBoard[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (tempBoard[i][j] != NULL)
            {
                free(tempBoard[i][j]); // Free the memory allocated for each string
            }
        }
    }
}

void fillBoardGraphics(int arr[][SIZE]);
void initializeBoardState(char *boardState[SIZE][SIZE]);
void drawBoardState(char *boardState[SIZE][SIZE], Texture2D texture);
void swapElements(char *boardState[SIZE][SIZE], int x1, int y1, int x2, int y2);
enum Outcome pawnCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE]);
enum Outcome rookCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE]);
enum Outcome knightCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE]);
enum Outcome bishopCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE]);
enum Outcome kingCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE]);
enum Outcome queenCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE]);
bool isPinned(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE], char turn);
void copyBoardState(char *destination[SIZE][SIZE], char *source[SIZE][SIZE]);
void sudoValidMovesList(char *tempBoard[SIZE][SIZE], char turn);

enum Outcome isValidMove(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE], char turn);

bool doesCheckOccur(int selectedX, int selectedY, int targetX, int targetY, char *tempBoard[SIZE][SIZE], char turn);

void handleMouseInput(struct ChessBoard *board, struct SelectedPiece *selectedPiece, Sound capture, Sound move);
void handleSelectedPiece(struct ChessBoard *board, struct SelectedPiece *selectedPiece, int *redx, int *redy);
void drawBoard(struct ChessBoard *board, int redx, int redy);

int main()
{
    int redx = -1;
    int redy = -1;
    const int WindowWidth = 480;
    const int WindowHeight = 480;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WindowWidth, WindowHeight, "CHESS the Game");
    InitAudioDevice();

    Image ChessPieceArray = LoadImage("resources/ChessPiecesArray.png");
    Texture2D texture = LoadTextureFromImage(ChessPieceArray);
    UnloadImage(ChessPieceArray);

    Sound notify = LoadSound("resources/notify.mp3");
    Sound capture = LoadSound("resources/capture.mp3");
    Sound move = LoadSound("resources/move-self.mp3");

    struct ChessBoard board;
    struct SelectedPiece selectedPiece;

    int x = 0;
    int y = 0;

    SetTargetFPS(60);

    fillBoardGraphics(board.boardGraphics);
    initializeBoardState(board.boardState);
    // swapElements(board.boardState, 0, 0, 1, 1);

    board.whiteTurn = true;

    PlaySound(notify);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_R))
        {
            PlaySound(notify);
            initializeBoardState(board.boardState);
        }
        // PlaySound(notify);

        BeginDrawing();

        handleMouseInput(&board, &selectedPiece, capture, move);
        handleSelectedPiece(&board, &selectedPiece, &redx, &redy);
        drawBoard(&board, redx, redy);
        drawBoardState(board.boardState, texture);

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
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

void copyBoardState(char *destination[SIZE][SIZE], char *source[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (source[i][j] != NULL)
            {
                size_t len = strlen(source[i][j]);
                destination[i][j] = malloc((len + 1) * sizeof(char)); // Allocate memory for the string
                strcpy(destination[i][j], source[i][j]);              // Copy the string
            }
            else
            {
                destination[i][j] = NULL;
            }
        }
    }
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
    char *temp = boardState[y1][x1];         // Swap y1 and x1
    boardState[y1][x1] = boardState[y2][x2]; // Swap y2 and x2
    boardState[y2][x2] = temp;               // Swap y2 and x2
}

enum Outcome pawnCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE])
{
    if (isPinned(selectedX, selectedY, targetX, targetY, boardState, boardState[selectedY][selectedX][0]) == true)
    {
        return ILLEGAL;
    }

    int direction = (boardState[selectedY][selectedX][0] == 'W') ? -1 : 1;

    if (targetY == selectedY + direction && (targetX == selectedX + 1 || targetX == selectedX - 1))
    {

        if (boardState[targetY][targetX] != NULL && boardState[selectedY][selectedX][0] != boardState[targetY][targetX][0])
        {
            return CAPTURE;
        }
    }

    if (targetY == selectedY + direction && targetX == selectedX && boardState[targetY][targetX] == NULL)
    {
        return LEGAL;
    }

    if (targetY == selectedY + 2 * direction && targetX == selectedX && boardState[targetY][targetX] == NULL)
    {

        if ((selectedY == 1 && direction == 1) || (selectedY == 6 && direction == -1))
        {
            return LEGAL;
        }
    }
    return ILLEGAL;
}

enum Outcome rookCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE])
{
    // Check if the target position is in the same row or column as the selected position
    if (selectedX == targetX || selectedY == targetY)
    {
        // Check if there are any pieces between the selected position and the target position
        int stepX = (targetX > selectedX) ? 1 : ((targetX < selectedX) ? -1 : 0);
        int stepY = (targetY > selectedY) ? 1 : ((targetY < selectedY) ? -1 : 0);

        int x = selectedX + stepX;
        int y = selectedY + stepY;

        while (x != targetX || y != targetY)
        {
            if (boardState[y][x] != NULL)
            {
                // There is a piece blocking the path
                return ILLEGAL;
            }

            x += stepX;
            y += stepY;
        }

        if (boardState[targetY][targetX] == NULL)
        {
            return LEGAL;
        }
        else if (boardState[selectedY][selectedX][0] != boardState[targetY][targetX][0])
        {
            return CAPTURE;
        }
    }

    return ILLEGAL;
}

enum Outcome knightCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE])
{
    // Calculate the absolute difference in x and y positions
    int deltaX = abs(targetX - selectedX);
    int deltaY = abs(targetY - selectedY);

    // Check if the absolute differences are valid for a knight move
    if ((deltaX == 1 && deltaY == 2) || (deltaX == 2 && deltaY == 1))
    {
        // Check the target position
        if (boardState[targetY][targetX] == NULL)
        {
            return LEGAL;
        }
        else if (boardState[selectedY][selectedX][0] != boardState[targetY][targetX][0])
        {
            return CAPTURE;
        }
    }

    return ILLEGAL;
}

enum Outcome bishopCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE])
{
    // Check if the target position is on a diagonal from the selected position
    if (abs(targetX - selectedX) == abs(targetY - selectedY))
    {
        // Determine the direction of movement (upward or downward)
        int stepX = (targetX > selectedX) ? 1 : -1;
        int stepY = (targetY > selectedY) ? 1 : -1;

        int x = selectedX + stepX;
        int y = selectedY + stepY;

        while (x != targetX || y != targetY)
        {
            if (boardState[y][x] != NULL)
            {
                // There is a piece blocking the path
                return ILLEGAL;
            }

            x += stepX;
            y += stepY;
        }

        if (boardState[targetY][targetX] == NULL)
        {
            return LEGAL;
        }
        else if (boardState[selectedY][selectedX][0] != boardState[targetY][targetX][0])
        {
            return CAPTURE;
        }
    }

    return ILLEGAL;
}

enum Outcome kingCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE])
{
    // Calculate the absolute difference in x and y positions
    int deltaX = abs(targetX - selectedX);
    int deltaY = abs(targetY - selectedY);

    // Check if the absolute differences are within the valid range for a king move
    if ((deltaX == 1 && deltaY == 0) || (deltaX == 0 && deltaY == 1) || (deltaX == 1 && deltaY == 1))
    {
        // Check the target position
        if (boardState[targetY][targetX] == NULL)
        {
            return LEGAL;
        }
        else if (boardState[selectedY][selectedX][0] != boardState[targetY][targetX][0])
        {
            return CAPTURE;
        }
    }

    return ILLEGAL;
}

enum Outcome queenCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE])
{
    if (targetX == -1)
    {
        return LEGAL;
    }

    if (bishopCheck(selectedX, selectedY, targetX, targetY, boardState) != ILLEGAL)
    {
        return bishopCheck(selectedX, selectedY, targetX, targetY, boardState);
    }
    if (rookCheck(selectedX, selectedY, targetX, targetY, boardState) != ILLEGAL)
    {
        return rookCheck(selectedX, selectedY, targetX, targetY, boardState);
    }
    else
        return ILLEGAL;
}

bool isPinned(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE], char turn)
{
    return false;
}

enum Outcome isValidMove(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE], char turn)
{

    if (boardState[selectedY][selectedX] == NULL)
    {
        return ILLEGAL;
    }
    if (turn == 'W')
    {
        if (boardState[selectedY][selectedX][0] == 'B')
        {
            return ILLEGAL;
        }
    }

    if (turn == 'B')
    {
        if (boardState[selectedY][selectedX][0] == 'W')
        {
            return ILLEGAL;
        }
    }

    switch (boardState[selectedY][selectedX][1])
    {
    case 'P':
    {
        return pawnCheck(selectedX, selectedY, targetX, targetY, boardState);
    }

    case 'R':
    {
        return rookCheck(selectedX, selectedY, targetX, targetY, boardState);
    }
    case 'N':
    {
        return knightCheck(selectedX, selectedY, targetX, targetY, boardState);
    }
    case 'B':
    {
        return bishopCheck(selectedX, selectedY, targetX, targetY, boardState);
    }
    case 'Q':
    {
        return queenCheck(selectedX, selectedY, targetX, targetY, boardState);
    }
    case 'K':
    {
        return kingCheck(selectedX, selectedY, targetX, targetY, boardState);
    }
        return ILLEGAL;
    }
}

// stores all sudo legal moves in a linked list for a given turn.
void sudoValidMovesList(char *tempBoard[SIZE][SIZE], char turn)
{
    // enum Outcome tempResult = isValidMove(selectedX, selectedY, targetX, targetY, tempBoard, turn);
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {

            for (int k = 0; k < SIZE; k++)
            {
                for (int l = 0; l < SIZE; l++)
                {

                    if (isValidMove(i, j, k, l, tempBoard, turn) != ILLEGAL)
                    {
                        addNode(i, j, k, l);
                    }
                }
            }
        }
    }
    // printLinkedList(head);
}

bool doesCheckOccur(int selectedX, int selectedY, int targetX, int targetY, char *tempBoard[SIZE][SIZE], char turn)
{
    int kingX;
    int kingY;

    // Make the suggested move
    swapElements(tempBoard, selectedX, selectedY, targetX, targetY);
    tempBoard[selectedY][selectedX] = NULL;

    // Find the current turn's king
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            // Find own king position
            if (tempBoard[i][j] != NULL && tempBoard[i][j][0] == turn && strcmp(tempBoard[i][j] + 1, "K") == 0)
            {
                kingX = i;
                kingY = j;
                break;
            }
        }
    }

    // Swap turn to check valid moves for the other team
    char otherTeam = (turn == 'W') ? 'B' : 'W';

    // Create a list of possible moves for the other team
    sudoValidMovesList(tempBoard, otherTeam);

    // Compare king position to x2 and y2 at every node
    Node *current = head;
    while (current != NULL)
    {
        if (kingY == current->x2 && kingX == current->y2)
        {
            // The king is under attack

            // Check if the attacking piece can be captured
            if (tempBoard[current->y1][current->x1] != NULL && tempBoard[current->y1][current->x1][0] != turn)
            {
                // The attacking piece can be captured
                if (current->x1 == targetX && current->y1 == targetY)
                {
                   // if(isValidMove(selectedX, selectedY, targetX, targetY, tempBoard, otherTeam) == CAPTURE){
                    return false;
                    //}
                }
                else
                {
                    // The move does not capture the attacking piece
                    return true;
                }
            }
            else
            {
                // The attacking piece cannot be captured
                return true;
            }
        }

        current = current->next;
    }

    return false;
}


void handleMouseInput(struct ChessBoard *board, struct SelectedPiece *selectedPiece, Sound capture, Sound move)
{
    char *tempBoard[SIZE][SIZE];

    Vector2 mouseposition = GetMousePosition();
    char turn;

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

            if (board->whiteTurn)
            {
                turn = 'W';
            }
            else
            {
                turn = 'B';
            }

            copyBoardState(tempBoard, board->boardState);
          
            enum Outcome result = isValidMove(selectedX, selectedY, targetX, targetY, board->boardState, turn);
      
            if (doesCheckOccur(selectedX, selectedY, targetX, targetY, tempBoard, turn) == true)
            {
                printf("CHECK\n");
                result = ILLEGAL;
            }

  
            // printLinkedList(head);

            switch (result)
            {
            case LEGAL:
            {
                PlaySound(move);
                swapElements(board->boardState, selectedX, selectedY, targetX, targetY);
                board->whiteTurn = !board->whiteTurn; // Switch the turn
                break;
            }
            case ILLEGAL:
            {
                selectedPiece->x = -1;
                selectedPiece->y = -1;
                selectedPiece->isSelected = false;
                break;
            }
            case CAPTURE:
            {
                PlaySound(capture);
                board->boardState[targetY][targetX] = NULL;
                swapElements(board->boardState, selectedX, selectedY, targetX, targetY);
                board->whiteTurn = !board->whiteTurn; // Switch the turn
                break;
            }
            }

            // Reset selected piece
            selectedPiece->x = -1;
            selectedPiece->y = -1;
            selectedPiece->isSelected = false;
        }
    }

    // freeTempBoard(tempBoard);

    if (head != NULL)
    {
        makeListEmpty();
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

// cc ChessGame.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 && ./a.out