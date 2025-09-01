#ifndef UI_H
#define UI_H

#include "./figure.h"

void drawGameplayUI(int score, int lines, int figuresPlaced, Figure nextFigure);
void drawGameOverScreen(int score);
void drawWelcomeScreen();
void drawPauseScreen();

#endif