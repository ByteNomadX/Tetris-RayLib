#include <stdlib.h>
#include <raylib.h>
#include <stdio.h>
#include <string.h>

#include "./headers/bag.h"
#include "./headers/figure.h"

static const char* figure_map_i[] = {"1111", "1 1 1 1", NULL};
static const char* figure_map_o[] = {"11 11", NULL};
static const char* figure_map_t[] = {"111 010", "10 11 10", "010 111", "01 11 01", NULL};
static const char* figure_map_s[] = {"011 110", "10 11 01", NULL};
static const char* figure_map_z[] = {"110 011", "01 11 10", NULL};
static const char* figure_map_l[] = {"100 111", "11 10 10", "111 001", "01 01 11", NULL};
static const char* figure_map_j[] = {"001 111", "10 10 11", "111 100", "11 01 01", NULL};

static const int offset_shadow = 3;

int getMapLength(const char* map) {
  char ch = map[0];
  int i = 0;
  int length = 0;

  while((ch = map[i]) != '\0') {
    if(ch == '1') length++;
    i++;
  }

  return length;
}

void setFigureColor(Figure* figure) {
  int i;
  Color color;
  switch (figure->type) {
    case figure_i:
      color = LIME;
      break;
    case figure_o:
      color = DARKPURPLE;
      break;
    case figure_t:
      color = PURPLE;
      break;
    case figure_s:
      color = GREEN;
      break;
    case figure_z:
      color = RED;
      break;
    case figure_l:
      color = ORANGE;
      break;
    case figure_j:
      color = BLUE;
      break;
    default:
      return;
  }

  for(i = 0; i < figure->countBlocks; i++) {
    figure->blocks[i].color = color;
  }
}

Block* getFigureBlocks(const char* map, int length, Vector2 startPos) {
  int i, counter, arrIndex;
  char ch;

  int x; 
	int y = startPos.y;
  
	Block* blocks = MemAlloc(sizeof(Block) * length);

	int len = strlen(map);

  for(i = 0, counter = 0, arrIndex = 0; i < len; i++) {
    x = block_size * counter + startPos.x;

    ch = map[i];
    if(ch == '0') {
      counter++;
      continue;
    }
    else if(ch == '1') {
      blocks[arrIndex].rec.x = x;
      blocks[arrIndex].rec.y = y;

      blocks[arrIndex].rec.width = block_size;
      blocks[arrIndex].rec.height = block_size;

      arrIndex++;
      counter++;
    } else if(ch == ' ') {
      y += block_size;
      counter = 0;
    }

  }
  return blocks;
}

int rotateFigure(Figure* figure, Field field) {
  int i, j;
  int minY, minX;
  int newBlocksLength;
  int prevMapIndex;

  Vector2 oldBlocks[figure->countBlocks];
  Block* newBlocks;

  for(i = 0; i < figure->countBlocks; i++) {
    oldBlocks[i] = (Vector2){figure->blocks[i].rec.x, figure->blocks[i].rec.y};
  }

  prevMapIndex = figure->mapIndex;
  figure->mapIndex++;
  if(figure->mapIndex >= figure->mapLength) {
    figure->mapIndex = 0;
  }

  newBlocksLength = getMapLength(figure->maps[figure->mapIndex]);

	minX = figure->blocks[0].rec.x;
	minY = figure->blocks[0].rec.y;
	for(i = 1; i < figure->countBlocks; i++) {
		if(figure->blocks[i].rec.x < minX) minX = figure->blocks[i].rec.x;
		if(figure->blocks[i].rec.y < minY) minY = figure->blocks[i].rec.y;
	}

	newBlocks = getFigureBlocks(figure->maps[figure->mapIndex], newBlocksLength, (Vector2){minX, minY});
	
  // CHECK OTHER BLOCK COLIISION
  for(i = 0; i < newBlocksLength; i++) {
    for(j = 0; j < field.occupiedBlocksCount; j++) {
      if(CheckCollisionRecs(newBlocks[i].rec, field.occupiedBlocks[j].rec)) {
        MemFree(newBlocks);
        figure->mapIndex = prevMapIndex;
        return 1;
      }
    }
  }

  // CHECK WALL COLLISION
  for (i = 0; i < newBlocksLength; i++) {
    if (newBlocks[i].rec.x >= field.pos.x + field.size.x)
      for (j = 0; j < newBlocksLength; j++) newBlocks[j].rec.x -= block_size;
    if (newBlocks[i].rec.x < field.pos.x)
      for (j = 0; j < newBlocksLength; j++) newBlocks[j].rec.x += block_size;
    if (newBlocks[i].rec.y + block_size > field.pos.y + field.size.y)
      for (j = 0; j < newBlocksLength; j++) newBlocks[j].rec.y -= block_size;
    if (newBlocks[i].rec.y < field.pos.y)
      for (j = 0; j < newBlocksLength; j++) newBlocks[j].rec.y += block_size;
    }

  MemFree(figure->blocks);
  figure->blocks = NULL;

  figure->blocks = newBlocks;
  figure->countBlocks = newBlocksLength;

  setFigureColor(figure);
  return 0;
}


CollisionType checkFigureCollision(Figure figure, Field field) {
  for (int i = 0; i < figure.countBlocks; i++) {
    Rectangle nextPos = figure.blocks[i].rec;
    nextPos.x += figure.dir.x * block_size;
    nextPos.y += figure.dir.y * block_size;

    if (nextPos.x < field.pos.x || nextPos.x + block_size > field.pos.x + field.size.x) {
      return figure.dir.x < 0 ? c_hor_wall_left : c_hor_wall_right;
    }
    if (nextPos.y + block_size > field.pos.y + field.size.y) return c_down_wall;

    for (int j = 0; j < field.occupiedBlocksCount; j++) {
      if (CheckCollisionRecs(nextPos, field.occupiedBlocks[j].rec)) return c_block;
    }
  }

  return c_none;
}

Figure* createFigure(const char** maps, FigureType type, int mapsC, Vector2 fieldPos) {
  int i;

  Figure* figure = MemAlloc(sizeof(Figure));
  if (figure == NULL) {
    return NULL;
  }

  figure->mapLength = mapsC;
  figure->mapsLength = MemAlloc(sizeof(int) * mapsC);

  figure->maps = MemAlloc(sizeof(const char*) * mapsC);
  
  for(i = 0; i < mapsC; i++) {
    figure->mapsLength[i] = getMapLength(maps[i]);
    figure->maps[i] = maps[i];
  }

  figure->type = type;
  figure->countBlocks = figure->mapsLength[0];
  figure->blocks = getFigureBlocks(figure->maps[0], figure->countBlocks, fieldPos);

  setFigureColor(figure);

  return figure;
}


void moveFigure(Figure* figure) {
  int i;

  for(i = 0; i < figure->countBlocks; i++) {
    figure->blocks[i].rec.y += figure->dir.y * block_size;
    figure->blocks[i].rec.x += figure->dir.x * block_size;
  }
}

void drawFigure(Figure figure, int maxY, int maxX) {
  int i;
  Block block;

	int offsetShadowX, offsetShadowY;

  for(i = 0; i < figure.countBlocks; i++) {
    block = figure.blocks[i];

    if(block.rec.y + block_size >= maxY) 
      offsetShadowY = 0;
    else offsetShadowY = offset_shadow;

    if(block.rec.x + block_size >= maxX) 
      offsetShadowX = 0; 
    else offsetShadowX = offset_shadow;
    
    DrawRectangle(block.rec.x + offsetShadowX, block.rec.y + offsetShadowY, block.rec.width, block.rec.height, (Color){0, 0, 0, 60}); // shadow
    DrawRectangle(block.rec.x, block.rec.y, block.rec.width, block.rec.height, block.color);
    DrawRectangleLinesEx(block.rec, 1, WHITE);
  }
}

const char** getFigureMap(FigureType type) {
  switch (type) {
    case figure_i:
      return figure_map_i;
    case figure_o:
      return figure_map_o;
    case figure_t:
      return figure_map_t;
    case figure_s:
      return figure_map_s;
    case figure_z:
      return figure_map_z;
    case figure_l:
      return figure_map_l;
    case figure_j:
      return figure_map_j;
    default:
      return NULL;
  }
}

Figure* getNextFigure(Bag* bag, Vector2 fieldPos) {
	int length = 0;

	FigureType type = getNextFromBag(bag);

	const char** maps = getFigureMap(type);
  while(maps[length] != NULL) {
    length++;
  }

	return createFigure(maps, type, length, fieldPos);
}	

void freeFigure(Figure* figure) {
	if (!figure) return;
	MemFree(figure->blocks);
	MemFree(figure->maps);
	MemFree(figure->mapsLength);
	MemFree(figure);
}

void drawFigurePath(Figure figure, Field field) {
  int i;

  Figure figurePath;
  Block block;

  figurePath.countBlocks = figure.countBlocks;
  figurePath.blocks = MemAlloc(sizeof(Block) * figure.countBlocks);
  figurePath.dir = (Vector2){0, 1};

  Color color = (Color){0,0,0,25};
  
  for(i = 0; i < figure.countBlocks; i++) {
    figurePath.blocks[i].rec = figure.blocks[i].rec;
  }

  while (checkFigureCollision(figurePath, field) == c_none) {
    for(i = 0; i < figurePath.countBlocks; i++) {
      figurePath.blocks[i].rec.y += block_size;
    }      
  }

  for(i = 0; i < figurePath.countBlocks; i++) {
    if(figure.blocks[i].rec.y >= figurePath.blocks[i].rec.y - block_size) {
			MemFree(figurePath.blocks);
      return;
    }
  }

  for(i = 0; i < figurePath.countBlocks; i++) {
    block = figurePath.blocks[i];
    DrawRectangle(block.rec.x, block.rec.y, block.rec.width, block.rec.height, color);
  }

  MemFree(figurePath.blocks);
}