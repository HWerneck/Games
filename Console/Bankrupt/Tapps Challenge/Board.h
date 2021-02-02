#pragma once
#ifndef BOARD_H
#define BOARD_H

#include "Property.h"

class Board
{
public:
	Board(std::vector<Property>* property);
	int getSpaces();
	Property* getProperty(int position);

private:
	int spaces;
	std::vector<Property*> property;
};

#endif
