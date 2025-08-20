#include "./headers/structs.h"
#include "./headers/constants.h"

#include <stdio.h>

const static int top_panel_width = 300;
const static int top_panel_height = 300;
const static int top_panel_x = width - top_panel_width - 100;
const static int top_panel_y = block_size * 2;

const static int font_size_big = 52;
const static int font_size_mid = 46;
const static int font_size_norm = 32;
const static int font_size_sm = 24;
const static int font_size_vrysm = 18;

const static int bottom_panel_width = 300;
const static int bottom_panel_height = 156;
const static int bottom_panel_x = width - bottom_panel_width - 100;
const static int bottom_panel_y = top_panel_y + top_panel_height + 20;

void drawWelcomeScreen() {  
  BeginDrawing();
  ClearBackground(bacgkround_color); 
  DrawText("TETRIS", width / 2 - MeasureText("TETRIS", font_size_big) / 2, 100, font_size_big, PINK);
  DrawText("Good game", width - MeasureText("Good game", font_size_mid) - 15, height - 50, font_size_mid, PINK);
  DrawText("PRESS [ENTER] TO START!", width / 2 - MeasureText("PRESS [ENTER] TO START", font_size_norm) / 2, height / 2, font_size_norm, GREEN);
  EndDrawing();
}

void drawGameOverScreen(int score) {
  BeginDrawing();
  ClearBackground(bacgkround_color); 
  DrawText("GAME OVER!", width / 2 - MeasureText("GAME OVER!", font_size_mid) / 2, 150, font_size_mid, RED);

  DrawText(TextFormat("SCORE: %d", score), width / 2 - MeasureText(TextFormat("SCORE: %d", score), font_size_mid) / 2, 230, font_size_mid, GREEN);

  DrawText("press [ESCAPE] to exit", width / 2 - MeasureText("press [ESCAPE] to exit", font_size_sm) / 2, height / 2, font_size_sm, WHITE);

  DrawText("press [ENTER] to restart", width / 2 - MeasureText("press [ENTER] to restart", font_size_sm) / 2, height / 2 + 25, font_size_sm, WHITE);
  EndDrawing();
} 

void drawPauseScreen() {
  BeginDrawing();
  ClearBackground(bacgkround_color); 
  DrawText("PAUSE", width / 2 - MeasureText("PAUSE", font_size_mid) / 2, 150, font_size_mid, RED);

  DrawText("press [ESCAPE] to exit", width / 2 - MeasureText("press [ESCAPE] to exit", font_size_sm) / 2, height / 2, font_size_sm, WHITE);

  DrawText("press [ENTER] to continue", width / 2 - MeasureText("press [ENTER] to continue", font_size_sm) / 2, height / 2 + 25, font_size_sm, WHITE);

  DrawText("press [R] to restart", width / 2 - MeasureText("press [R] to restart", font_size_sm) / 2, height / 2 + 50, font_size_sm, WHITE);

  EndDrawing();
}

void drawTopPanelBackground() {
  DrawRectangle(top_panel_x, top_panel_y, top_panel_width, top_panel_height, (Color){0, 0, 0, 20}); 
  DrawRectangleLines(top_panel_x, top_panel_y, top_panel_width, top_panel_height, (Color){200, 200, 200, 255});
}

void drawBottomPanelBackground() {
  DrawRectangle(bottom_panel_x, bottom_panel_y, bottom_panel_width, bottom_panel_height, (Color){0, 0, 0, 20}); 
  DrawRectangleLines(bottom_panel_x, bottom_panel_y, bottom_panel_width, bottom_panel_height, (Color){200, 200, 200, 255});   
}

void drawInfo() {
   DrawText("Press [P] to pause", bottom_panel_x + bottom_panel_width / 2 - MeasureText("press [P] to pause", font_size_vrysm) / 2, bottom_panel_y + 20, font_size_vrysm, BLACK);
   DrawText("Press [M] to PLAY/STOP music", bottom_panel_x + bottom_panel_width / 2 - MeasureText("Press [M] to PLAY/STOP music", font_size_vrysm) / 2, bottom_panel_y + 48, font_size_vrysm, BLACK);
   DrawText("Press [S] to PLAY/STOP sounds", bottom_panel_x + bottom_panel_width / 2 - MeasureText("Press [S] to PLAY/STOP sounds", font_size_vrysm) / 2, bottom_panel_y + 68, font_size_vrysm, BLACK);
   DrawText("by sHema", bottom_panel_x + bottom_panel_width / 2 - MeasureText("by sHema", font_size_vrysm) / 2, bottom_panel_y + bottom_panel_height - 26, font_size_vrysm, BLACK);
}

void drawNextFigure(Figure figure) {
  int i;
  const char* text = "Next";
  int textSize = MeasureText(text, font_size_mid);

  DrawText(text, (top_panel_x + top_panel_width / 2) - textSize / 2, top_panel_y + 180, font_size_mid, DARKGRAY);

  int maxX = figure.blocks[0].rec.x;
  for (i = 0; i < figure.countBlocks; i++) {
    if (figure.blocks[i].rec.x > maxX) {
      maxX = figure.blocks[i].rec.x;
    }
  }

  Vector2 pos = (Vector2){top_panel_x + top_panel_width / 2 - maxX / 2 + block_size, top_panel_y + 180};
  
  for (i = 0; i < figure.countBlocks; i++) {
    DrawRectangle(figure.blocks[i].rec.x - field_pos_x + 2 + pos.x, figure.blocks[i].rec.y + 2 + pos.y, figure.blocks[i].rec.width, figure.blocks[i].rec.height, (Color){0, 0, 0, 30});
    
    DrawRectangle(figure.blocks[i].rec.x - field_pos_x + pos.x, figure.blocks[i].rec.y + pos.y, figure.blocks[i].rec.width, figure.blocks[i].rec.height, figure.blocks[i].color);
    
    DrawRectangleLinesEx((Rectangle){figure.blocks[i].rec.x - field_pos_x + pos.x, figure.blocks[i].rec.y + pos.y, figure.blocks[i].rec.width, figure.blocks[i].rec.height}, 1, WHITE);
  }
}

void drawFiguresPlaced(int figuresPlaced) {
  const char* text = TextFormat("Placed: %d", figuresPlaced);
  int textSize = MeasureText(text, font_size_norm);

  DrawText(text, (top_panel_x + top_panel_width / 2) - textSize / 2, top_panel_y + 120, font_size_mid, DARKGRAY);
}

void drawScore(int score) {
  const char* text = TextFormat("Score: %d", score);
  int textSize = MeasureText(text, font_size_norm);

  DrawText(text, (top_panel_x + top_panel_width / 2) - textSize / 2, top_panel_y + 20, font_size_mid, DARKGRAY);
}

void drawLines(int lines) {
  const char* text = TextFormat("Lines: %d", lines);
  int textSize = MeasureText(text, font_size_norm);

  DrawText(text, (top_panel_x + top_panel_width / 2) - textSize / 2, top_panel_y + 70, font_size_mid, DARKGRAY);
}

void drawDividers() {
  int line_x_start = top_panel_x + 10;
  int line_x_end = top_panel_x + top_panel_width - 10;

  DrawLine(line_x_start, top_panel_y + 60, line_x_end, top_panel_y + 60, DARKGRAY);  
  DrawLine(line_x_start, top_panel_y + 110, line_x_end, top_panel_y + 110, DARKGRAY);
  DrawLine(line_x_start, top_panel_y + 160, line_x_end, top_panel_y + 160, DARKGRAY);
}

void drawGameplayUI(int score, int lines, int figuresPlaced, Figure nextFigure) {
  drawTopPanelBackground();
  drawBottomPanelBackground();
  
  drawScore(score);
  drawLines(lines);
  drawFiguresPlaced(figuresPlaced);
  drawNextFigure(nextFigure);
  drawInfo();
  drawDividers();
}