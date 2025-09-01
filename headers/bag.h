#ifndef BAG_H
#define BAG_H

#include "./figure.h"

enum { count_figures = 7 };

typedef struct bag {
	FigureType figures[count_figures];
	int index;
} Bag;

void initBag(Bag* bag);
FigureType getNextFromBag(Bag* bag);

#endif