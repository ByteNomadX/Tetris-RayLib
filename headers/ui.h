#include "./structs.h"

#ifndef UI_H
#define UI_H
void drawGameplayUI(int score, int lines, int figuresPlaced, Figure nextFigure);
void drawGameOverScreen(int score);
void drawWelcomeScreen();
void drawPauseScreen();

#endif