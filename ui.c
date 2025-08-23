#include "./headers/block.h"
#include "./headers/config.h"
#include "./headers/figure.h"
#include "./headers/field.h"

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

static const Color background_color = (Color){30, 30, 30, 100};

typedef enum h_alignment {start, center, end} h_alignment;

void drawText(const char *text, int posX, int posY, int fontSize, Color color, h_alignment alignment) {
	switch (alignment)
	{
	case start:
		DrawText(text, posX, posY, fontSize, color);
		break;
	case center:
		DrawText(text, posX - MeasureText(text, fontSize) / 2, posY, fontSize, color);
		break;
	case end:
		DrawText(text, posX - MeasureText(text, fontSize), posY, fontSize, color);
		break;
	default:
		break;
	}
}

void drawWelcomeScreen() {  
  BeginDrawing();
  ClearBackground(background_color); 
	
  drawText("TETRIS", width / 2, 100, font_size_big, PINK, center);
  drawText("Good game", width - 10, height - 50, font_size_mid, PINK, end);
  drawText("PRESS [ENTER] TO START!", width / 2, height / 2, font_size_norm, GREEN, center);
  EndDrawing();
}

void drawGameOverScreen(int score) {
  BeginDrawing();
  ClearBackground(background_color); 
  drawText("GAME OVER!", width / 2, 150, font_size_mid, RED, center);

  drawText(TextFormat("SCORE: %d", score), width / 2, 230, font_size_mid, GREEN, center);

  drawText("press [ESCAPE] to exit", width / 2, height / 2, font_size_sm, WHITE, center);

  drawText("press [ENTER] to restart", width / 2, height / 2 + 25, font_size_sm, WHITE, center);
  EndDrawing();
} 

void drawPauseScreen() {
  BeginDrawing();
  ClearBackground(background_color); 
  drawText("PAUSE", width / 2, 150, font_size_mid, RED, center);

  drawText("press [ESCAPE] to exit", width / 2, height / 2, font_size_sm, WHITE, center);

  drawText("press [ENTER] to continue", width / 2, height / 2 + 25, font_size_sm, WHITE, center);

  drawText("press [R] to restart", width / 2, height / 2 + 50, font_size_sm, WHITE, center);

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
	drawText("Press [P] to pause", bottom_panel_x + bottom_panel_width / 2, bottom_panel_y + 20, font_size_vrysm, BLACK, center);
	drawText("Press [M] to PLAY/STOP music", bottom_panel_x + bottom_panel_width / 2, bottom_panel_y + 48, font_size_vrysm, BLACK, center);
	drawText("Press [S] to PLAY/STOP sounds", bottom_panel_x + bottom_panel_width / 2, bottom_panel_y + 68, font_size_vrysm, BLACK, center);
	drawText("by ByteNomadX", bottom_panel_x + bottom_panel_width / 2, bottom_panel_y + bottom_panel_height - 26, font_size_vrysm, BLACK, center);
}

void drawNextFigure(Figure figure) {
  int i;
  drawText("Next", (top_panel_x + top_panel_width / 2), top_panel_y + 180, font_size_norm, DARKGRAY, center);

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
  
	drawText(TextFormat("Lines: %d", lines), (top_panel_x + top_panel_width / 2), top_panel_y + 70, font_size_norm, DARKGRAY, center);
	drawText(TextFormat("Placed: %d", figuresPlaced), (top_panel_x + top_panel_width / 2), top_panel_y + 120, font_size_norm, DARKGRAY, center);
  drawText(TextFormat("Score: %d", score), (top_panel_x + top_panel_width / 2), top_panel_y + 20, font_size_norm, DARKGRAY, center);

	drawNextFigure(nextFigure);
  drawInfo();
  drawDividers();
}