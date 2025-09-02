#ifndef BLOCK_H
#define BLOCK_H

#include <raylib.h>

enum { block_size = 26 };

typedef struct block {
  Color color;
  Rectangle rec;
  bool toDelete;
} Block;

#endif