#ifndef FIELD_H
#define FIELD_H

#include "./block.h"

typedef struct Field {
  Vector2 size;
  Vector2 pos;
  int blockCount;
  Block* occupiedBlocks;
  int occupiedBlocksCount;
} Field;

enum { count_rows = 20, count_cols = 10, field_pos_x = 90 };

extern const int field_pos_y;

void drawField(Field field);
Field* initField();
int checkFilledLines(Field* field, int* linesToDelete);
void removeFieldLine(Field* field, int lineY);
void freeField(Field* field);
void changeFieldLineColor(Field* field, int lineY, Color color);

#endif