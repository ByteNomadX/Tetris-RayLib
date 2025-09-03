#ifndef FIGURE_H
#define FIGURE_H

#include "./block.h"
#include "./field.h"
#include "raylib.h"

typedef enum CollisionType 
  {c_hor_wall_left, c_hor_wall_right, c_down_wall, c_top_wall, c_block, c_none} 
CollisionType;

typedef enum figureType {
	figure_i, figure_o, figure_t, figure_s, figure_z, figure_l, figure_j
} FigureType;

typedef struct Figure {
  int countBlocks;
  int mapIndex;
  int mapLength;
  int* mapsLength;
  const char** maps;
  Block* blocks;
  Vector2 dir;
  FigureType type;
} Figure;

typedef struct bag Bag;

Figure* getNextFigure(Bag* bag, Vector2 fieldpos);
Figure* createFigure(const char** maps, FigureType type, int mapsC, Vector2 fieldPos);
void drawFigure(Figure figure, int maxY, int maxX);
void moveFigure(Figure* figure);
int rotateFigure(Figure* figure, Field field);
Block* getFigureBlocks(const char* map, int length, Vector2 fieldPos);
CollisionType checkFigureCollision(Figure figure, Field field);
void appendFigureToField(Figure* figure, Field* field);
void freeFigure(Figure* figure);
const char** getFigureMap(FigureType type);
int getMapLength(const char* map);
void drawFigurePath(Figure figure, Field field);
void placeFigureDown(Figure* figure, Field field);

#endif