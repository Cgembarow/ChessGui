
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

#include "sudo_valid.h"

enum Outcome pawnCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE])
{
    

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