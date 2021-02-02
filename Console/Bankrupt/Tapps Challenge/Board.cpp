#include "Board.h"
#include "Property.h"

Board::Board(std::vector<Property>* property)
{
	this->spaces = property->size();
	for (int iii = 0; iii < this->spaces; ++iii)
	{
		this->property.push_back(&property->at(iii));
	}
	return;
}

int Board::getSpaces()
{
	return this->spaces;
}

Property* Board::getProperty(int position)
{
	return this->property.at(position - 1);
}
