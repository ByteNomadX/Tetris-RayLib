#include <raylib.h>

#ifndef BLOCK_H
#define BLOCK_H

static const int block_size = 24;

typedef struct block {
  Color color;
  Rectangle rec;
  bool toDelete;
} Block;

#endif