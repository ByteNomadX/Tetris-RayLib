#include "./headers/block.h"
#include "./headers/config.h"
#include "./headers/figure.h"
#include "./headers/field.h"

#include <math.h>

const static int font_size_big = 52;
const static int font_size_mid = 46;
const static int font_size_norm = 32;
const static int font_size_sm = 24;
const static int font_size_vrysm = 18;


const static Rectangle topPanel = (Rectangle){width - 400, block_size * 2, 300, 300};
const static Rectangle bottomPanel = (Rectangle){width - 400, topPanel.y + topPanel.height + 40, 300, 156};


static const Color background_color = (Color){30, 30, 30, 100};

typedef enum h_alignment {start, end} h_alignment;

void drawText(const char *text, int posX, int posY, int fontSize, Color color) {
	DrawText(text, posX - MeasureText(text, fontSize) / 2, posY, fontSize, color);
}

void drawBackgroundPattern() {
	ClearBackground(RAYWHITE);

	int gap = 60;
	for(int y = 0; y < height; y += gap) {
		for(int x = 0; x < width; x += gap) {
			DrawRectangleLines(x, y, gap, gap, (Color){220, 220, 220, 70});
		}
	}
}

void drawFloatingText(const char *text, int posX, int posY, int fontSize, Color baseColor, float speed) {
	float t = (float)GetTime();
	float alpha = 80 + (sinf(t * speed) * 0.5f + 0.5f) * (255 - 80);
	Color c = {baseColor.r, baseColor.g, baseColor.b, (unsigned char)alpha};
	drawText(text, posX, posY, fontSize, c);
}

void drawWelcomeScreen() {  
  BeginDrawing();
	drawBackgroundPattern();

	drawText("TETRIS", width / 2, 150, font_size_big, (Color){50, 50, 50, 255});
	drawText("The timeless puzzle", width / 2, 150 + font_size_big + 20, font_size_sm, (Color){120, 120, 120, 255});
	
	drawFloatingText("PRESS [ENTER] TO START", width / 2, height / 2, font_size_norm, (Color){100, 150, 220, 255}, 1.5f);

	drawText("By: ByteNomadX", width / 2, height - 60, font_size_vrysm, (Color){150, 150, 150, 255});
	drawText("Press [ESC] to exit", width / 2, height - 35, font_size_vrysm, (Color){150, 150, 150, 255});

  EndDrawing();
}

void drawGameOverScreen(int score) {
  BeginDrawing();

  drawBackgroundPattern(); 
  drawText("GAME OVER!", width / 2, 150, font_size_mid, (Color){50, 50, 50, 255});

	drawFloatingText("PRESS [ENTER] TO RESTART", width / 2, height / 2, font_size_sm, (Color){100, 150, 220, 255}, 1.5f);

	drawText(TextFormat("SCORE: %d", score), width / 2, 230, font_size_mid, (Color){80, 80, 80, 255});

	drawText("Press [ESC] to exit", width / 2, height / 2 + 40, font_size_sm, (Color){120, 120, 120, 255});

  EndDrawing();
} 

void drawPauseScreen() {
  BeginDrawing();
	drawBackgroundPattern();
	drawText("PAUSE", width / 2, 150, font_size_mid, (Color){50, 50, 50, 255});

	
  drawFloatingText("press [ENTER] to continue", width / 2, height / 2 , font_size_sm, 
		(Color){100, 150, 220, 255}, 1.5f);
		
	drawText("Press [R] to restart", width / 2, height / 2 + 40, font_size_sm, (Color){120, 120, 120, 255});
	drawText("Press [ESC] to exit", width / 2, height / 2 + 70, font_size_sm, (Color){120, 120, 120, 255});

  EndDrawing();
}

void drawTopPanelBackground() {
	DrawRectangleRounded(topPanel, 0.02f, 0, WHITE);
	DrawRectangleRoundedLines(topPanel, 0.02f, 0, 2, LIGHTGRAY);
}

void drawBottomPanelBackground() {
	DrawRectangleRounded(bottomPanel, 0.02f, 0, WHITE);
	DrawRectangleRoundedLines(bottomPanel, 0.05f, 0, 2, LIGHTGRAY);
}

void drawInfo() {
	Color textColor = (Color){80, 80, 80, 255};

	drawText("Press [P] to pause", bottomPanel.x + bottomPanel.width / 2, bottomPanel.y + 20, font_size_vrysm, textColor);
	drawText("Press [M] to PLAY/STOP music", bottomPanel.x + bottomPanel.width / 2, bottomPanel.y + 48, font_size_vrysm, textColor);
	drawText("Press [S] to PLAY/STOP sounds", bottomPanel.x + bottomPanel.width / 2, bottomPanel.y + 68, font_size_vrysm, textColor);
	drawText("by ByteNomadX", bottomPanel.x + bottomPanel.width / 2, bottomPanel.y + bottomPanel.height - 26, font_size_vrysm, textColor);
}

void drawNextFigure(Figure figure) {
  int i;
  drawText("Next", (topPanel.x + topPanel.width / 2), topPanel.y + 180, font_size_norm, DARKGRAY);

  float maxX = figure.blocks[0].rec.x;
  for (i = 0; i < figure.countBlocks; i++) {
    if (figure.blocks[i].rec.x > maxX) {
      maxX = figure.blocks[i].rec.x;
    }
  }

  Vector2 pos = (Vector2){(topPanel.x + topPanel.width / 2) - maxX / 2 + block_size / 2, topPanel.y + 200};
  
  for (i = 0; i < figure.countBlocks; i++) {
    DrawRectangle(figure.blocks[i].rec.x - field_pos_x + 2 + pos.x, figure.blocks[i].rec.y + 2 + pos.y, figure.blocks[i].rec.width, figure.blocks[i].rec.height, (Color){0, 0, 0, 30});
    
    DrawRectangle(figure.blocks[i].rec.x - field_pos_x + pos.x, figure.blocks[i].rec.y + pos.y, figure.blocks[i].rec.width, figure.blocks[i].rec.height, figure.blocks[i].color);
    
    DrawRectangleLinesEx((Rectangle){figure.blocks[i].rec.x - field_pos_x + pos.x, figure.blocks[i].rec.y + pos.y, figure.blocks[i].rec.width, figure.blocks[i].rec.height}, 1, WHITE);
  }
}

void drawStats(int score, int lines, int figuresPlaced) {
	drawText(TextFormat("Lines: %d", lines), (topPanel.x + topPanel.width / 2), topPanel.y + 70, font_size_norm, DARKGRAY);
	drawText(TextFormat("Placed: %d", figuresPlaced), (topPanel.x + topPanel.width / 2), topPanel.y + 120, font_size_norm, DARKGRAY);
  drawText(TextFormat("Score: %d", score), (topPanel.x + topPanel.width / 2), topPanel.y + 20, font_size_norm, DARKGRAY);
}

void drawDividers() {
  int line_x_start = topPanel.x + 10;
  int line_x_end = topPanel.x + topPanel.width - 10;

  DrawLine(line_x_start, topPanel.y + 60, line_x_end, topPanel.y + 60, DARKGRAY);  
  DrawLine(line_x_start, topPanel.y + 110, line_x_end, topPanel.y + 110, DARKGRAY);
  DrawLine(line_x_start, topPanel.y + 160, line_x_end, topPanel.y + 160, DARKGRAY);
}

void drawGameplayUI(int score, int lines, int figuresPlaced, Figure nextFigure) {
  drawTopPanelBackground();
  drawBottomPanelBackground();
  drawStats(score, lines, figuresPlaced);
	drawNextFigure(nextFigure);
  drawInfo();
  drawDividers();
}