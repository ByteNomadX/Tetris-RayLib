#include "./headers/bag.h"

void initBag(Bag* bag) {
	int i;
	int value;
	int reduce = 1;
	int lastIndex;
	int temp;
	int index;

	int arr[] = {0,1,2,3,4,5,6};
	
	for(i = 0; i < count_figures; i++) {
		index = GetRandomValue(0, count_figures - reduce);
		value = arr[index];
		lastIndex = count_figures - reduce;
		
		temp = arr[lastIndex];
		arr[lastIndex] = value;
		arr[index] = temp;
		reduce++;

		bag->figures[i] = value;
	}

	bag->index = 0;
}

FigureType getNextFromBag(Bag* bag) {
	if((bag->index + 1) >= count_figures) {
		initBag(bag);
	}

	return bag->figures[bag->index++];
}