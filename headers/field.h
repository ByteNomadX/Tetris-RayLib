#include "./block.h"

#ifndef FIELD_H
#define FIELD_H

typedef struct Field {
  Vector2 size;
  Vector2 pos;
  int blockCount;
  Block* occupiedBlocks;
  int occupiedBlocksCount;
} Field;

static const int count_rows = 20;
static const int count_cols = 10;

static const int field_pos_x = block_size * 3;
static const int field_pos_y = block_size * 2;

void drawField(Field field);
Field* initField(int width, int height, int blockSize);
int* checkFilledLines(Field* field);
void removeFieldLine(Field* field, int lineY);
void freeField(Field* field);
void changeFieldLineColor(Field* field, int lineY, Color color);

#endif