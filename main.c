#include <raylib.h>
#include <string.h>
#include <stdlib.h>

#include "./headers/config.h"
#include "./headers/bag.h"
#include "./headers/figure.h"
#include "./headers/field.h"

#include "./headers/audio.h"

#include "./headers/ui.h"

#if defined(PLATFORM_WEB)
  #include <emscripten/emscripten.h>
#endif

static Field* field;
static int score;
static int lines;
static int figuresPlaced;
static float fallingTime;
static float rotateFigureTime;
static float moveFigureHorTime;
static float moveFigureDownTime;

static bool gameOver;
static bool gameStarted;

static bool pause;
static bool movedDown;
static Figure* figure;
static Figure* nextFigure;
static Bag* bag;

static AudioManager* audioManager;
static RepeatingSound* repeatingLineClearSound;

static bool linesToDelete;
static int lineFadingTime;
static int linesToDeleteArr[count_rows];

static Color fadingColor;

static const int fading_time = 55;
static const float switch_figure_delay = 0.12f;
static const float move_figure_h_delay = 0.051f;
static const float move_figure_down_delay = 0.045f;

void initGame() {
  field = initField();
  score = 0;
	lines = 0;
	figuresPlaced = 0;
  fallingTime = 0.0f;
  rotateFigureTime = 0.0f;
  moveFigureHorTime = 0.0f;
	moveFigureDownTime = 0.0f;

  gameStarted = false;
  gameOver = false;

  pause = false;
	movedDown = false;
  linesToDelete = false;

  lineFadingTime = 0;
  fadingColor = RED;

	bag = MemAlloc(sizeof(Bag));
	initBag(bag);
	
  figure = getNextFigure(bag, field->pos);
  nextFigure = getNextFigure(bag, field->pos);
}

void restartGame() {
  freeFigure(figure);
	freeFigure(nextFigure);
  freeField(field);
	MemFree(bag);

	SeekMusicStream(audioManager->music, 0.0f);

  initGame();
}

void update() {
  if(!gameStarted) {
    drawWelcomeScreen();

    if(IsKeyPressed(KEY_ENTER)) {
      gameStarted = true;
      PlayMusicStream(audioManager->music);
    } else {
      return;
    }
  }
  if(gameOver) {
    drawGameOverScreen(score);
    if(IsKeyPressed(KEY_ESCAPE)) {
      exit(0);
    } else if(IsKeyPressed(KEY_ENTER)) {
      restartGame();
    }

    return;
  }
  if(pause) {
    drawPauseScreen();
    if(IsKeyPressed(KEY_ESCAPE)) {
      exit(0);
    } else if(IsKeyPressed(KEY_ENTER)) {
      pause = false;
			if(IsMusicStreamPlaying(audioManager->music)) {
				ResumeMusicStream(audioManager->music);
			}
    } else if(IsKeyPressed(KEY_R)) {
      restartGame();
    } else {
      return;
    }
  }

	if(IsKeyPressed(KEY_P)) {
		pause = true;
		PauseMusicStream(audioManager->music);
		return;
	}

	updateRepeatingSound(repeatingLineClearSound);
	UpdateMusicStream(audioManager->music);

	movedDown = false;

	figure->dir = (Vector2){0,0};
	
	// TO DO: make function in field.c to delete lines
	if(linesToDelete) {
		BeginDrawing();
		ClearBackground(WHITE); 

		drawField(*field);
		drawGameplayUI(score,lines, figuresPlaced, *nextFigure);
		drawFigure(*figure, field_pos_y + field->size.y, field_pos_x + field->size.x);
		EndDrawing();

		lineFadingTime += 1;

		if(lineFadingTime % 8 == 0) {
			fadingColor = ColorToInt(fadingColor) == ColorToInt(RAYWHITE) ? RED : RAYWHITE;
			for(int i = 0; i < count_rows; i++) {
				if(linesToDeleteArr[i] > 0) {
					changeFieldLineColor(field, linesToDeleteArr[i], fadingColor);
				}
			}
		}

		if(lineFadingTime >= fading_time) {
			lineFadingTime = 0;
			
			for(int i = 0; i < count_rows; i++) {
				if(linesToDeleteArr[i] > 0) {
					removeFieldLine(field, linesToDeleteArr[i]);
				}
			}

			linesToDelete = false;

			int i = 0;
			for(i = 0; i < count_rows; i++) {
				linesToDeleteArr[i] = 0; 
			}

		} else {
			return;
		}
	}

	if(moveFigureDownTime >= move_figure_down_delay) {
		if(IsKeyDown(KEY_DOWN)) {
			figure->dir = (Vector2){0, 1};
			fallingTime = 0;
			movedDown = true;
			moveFigureDownTime = 0;
		} else if(IsKeyReleased(KEY_SPACE)) {
			placeFigureDown(figure, *field);
			movedDown = true;
		}
	}

	if(moveFigureHorTime >= move_figure_h_delay) {
		if(IsKeyDown(KEY_LEFT)) {
			figure->dir = (Vector2){-1, 0};
			moveFigureHorTime = 0;
		} else if(IsKeyDown(KEY_RIGHT)) {
			figure->dir = (Vector2){1, 0};
			moveFigureHorTime = 0;
		}
	}

	if(IsKeyDown(KEY_UP)) {
		if(rotateFigureTime > switch_figure_delay) {
			if(rotateFigure(figure, *field) == 1) {
				playSound(audioManager->figureRotateFail);
			} else {
				playSound(audioManager->figureRotate);
				rotateFigureTime = 0;
			}
		}  
	}

	rotateFigureTime += GetFrameTime();
	moveFigureHorTime += GetFrameTime();
	moveFigureDownTime += GetFrameTime();
	fallingTime += GetFrameTime();

	if(fallingTime >= 1.0f) {
		figure->dir = (Vector2){0, 1};
		fallingTime = 0;
		movedDown = true;
	}

	switch (checkFigureCollision(*figure, *field))
	{
	case c_hor_wall_left:
	case c_hor_wall_right:
		break;
	case c_block:
		if(!movedDown) break;
	case c_down_wall:
		playSound(audioManager->figurePlaced);
		appendFigureToField(figure, field);
		freeFigure(figure); 

		figure = nextFigure;

		nextFigure = getNextFigure(bag, field->pos);

		if(checkFigureCollision(*figure, *field) == c_block) {
			gameOver = true;
			playSound(audioManager->gameOver);
			return;
		}

		int filledLines = 0;
		checkFilledLines(field, linesToDeleteArr);

		for(int i = 0; i < count_rows; i++) {
			if(linesToDeleteArr[i] > 0) {
				filledLines++;
			}
		}    

		if(filledLines > 0) {

			triggerRepeatingSound(repeatingLineClearSound, filledLines);

			linesToDelete = true;
			lines += filledLines;
			score += filledLines * 100;
		}
	
		figuresPlaced++;
		break;

		case c_none:
			moveFigure(figure);
		default:
			break;
	}

	if(IsKeyPressed(KEY_M)) {
		IsMusicStreamPlaying(audioManager->music) 
		? PauseMusicStream(audioManager->music)
		: ResumeMusicStream(audioManager->music); 
	} else if(IsKeyPressed(KEY_S)) {
		toggleSounds();	
	}

	BeginDrawing();
	ClearBackground(WHITE); 

	drawField(*field);
	drawGameplayUI(score,lines, figuresPlaced, *nextFigure);
	drawFigure(*figure, field_pos_y + field->size.y, field_pos_x + field->size.x);
	drawFigurePath(*figure, *field);
	EndDrawing();
}

int main() {
	InitWindow(width, height, title);
	
	audioManager = createAudioManager(0.08);
	repeatingLineClearSound = createRepeatingSound(audioManager->lineClear, 0.1f, 0.2f, 1.0f);

  initGame();
#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
  SetTargetFPS(60);

  while (!WindowShouldClose())
  {
    update();
  }
#endif
}