#ifndef SUDOVALID
#define SUDOVALID

#include "constants.h"



enum Outcome pawnCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE]);
enum Outcome rookCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE]);
enum Outcome knightCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE]);
enum Outcome bishopCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE]);
enum Outcome kingCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE]);
enum Outcome queenCheck(int selectedX, int selectedY, int targetX, int targetY, char *boardState[SIZE][SIZE]);





#endif
