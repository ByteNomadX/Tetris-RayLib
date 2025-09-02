#include <stdlib.h>

#include "./headers/field.h"
#include "./headers/figure.h"
#include "./headers/config.h"

const int field_pos_y = (height - (count_rows * block_size)) / 2;

Field* initField() {
  Field* field = malloc(sizeof(Field));

  field->size.x = count_cols * block_size;
  field->size.y = count_rows * block_size;

  field->pos.x = field_pos_x;
  field->pos.y = field_pos_y;
 
  field->blockCount = count_rows * count_cols;

  field->occupiedBlocks = malloc(sizeof(Block) * field->blockCount);
  field->occupiedBlocksCount = 0;

  return field;
}

void drawField(Field field) {
	int i;
	int blockX, blockY;
	int counter = 0;
	
	blockY = field.pos.y;
	for(i = 0; i < field.blockCount; i++) {
		if(i > 0 && i % count_cols == 0) {
			blockY += block_size;
			counter = 0;
		}

		blockX = field.pos.x + counter * block_size;

		DrawRectangleLinesEx((Rectangle){blockX, blockY, block_size, block_size}, 1, LIGHTGRAY);

		counter++;
	}
	
	for(i = 0; i < field.occupiedBlocksCount; i++) {
		Color color = field.occupiedBlocks[i].color;
		Rectangle rec = field.occupiedBlocks[i].rec;

		DrawRectangle(rec.x, rec.y, rec.width, rec.height, color);
		DrawRectangleLinesEx(rec, 1, LIGHTGRAY);
	}
}

void removeFieldLine(Field* field, int lineY) {
  int i;
  for (i = 0; i < field->occupiedBlocksCount; ) {
    if (field->occupiedBlocks[i].rec.y == lineY) {
      for (int j = i; j < field->occupiedBlocksCount - 1; j++) {
        field->occupiedBlocks[j] = field->occupiedBlocks[j + 1];
      }

      field->occupiedBlocksCount--;
    } else {
      i++;
    }
  }

  for (i = 0; i < field->occupiedBlocksCount; i++) {
    if (field->occupiedBlocks[i].rec.y < lineY) {
      field->occupiedBlocks[i].rec.y += block_size;
    }
  }
}


int checkFilledLines(Field* field, int* linesToDelete) {
  int i, j;
  int countBlocksInRow[count_rows];
  int linesY[count_rows];

  int k = 0;

  for(i = 0; i < count_rows; i++)
    countBlocksInRow[i] = 0;

  for(i = 0; i < count_rows; i++) 
    linesY[i] = i * block_size + field->pos.y;

  for(i = 0; i < field->occupiedBlocksCount; i++) {
    for(j = 0; j < count_rows; j++) {
      if(field->occupiedBlocks[i].rec.y == linesY[j]) {
        countBlocksInRow[j]++;
      }
    }
  }

  for(i = 0, k = 0; i < count_rows; i++) {
    if(countBlocksInRow[i] == count_cols) {
      linesToDelete[k] = linesY[i];
      k++;
    }
  }

  return k;
}

void appendFigureToField(Figure* figure, Field* field) {
  int i;

  for(i = 0; i < figure->countBlocks; i++) {
    figure->blocks[i].color = GRAY;
    field->occupiedBlocks[field->occupiedBlocksCount] = figure->blocks[i];
    field->occupiedBlocksCount++;
  }
}

void changeFieldLineColor(Field* field, int lineY, Color color) {
  int i;

  for(i = 0; i < field->occupiedBlocksCount; i++) {
    if(field->occupiedBlocks[i].rec.y == lineY) {
      field->occupiedBlocks[i].color = color;
    }
  }
}

void freeField(Field* field) {
  free(field->occupiedBlocks);
	free(field);
}