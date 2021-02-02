#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Property.h"
#include <string>
#include <vector>

class Property;

class Player
{
public:
	Player(std::string type);
	std::string getType();
	void setType(std::string type);
	int getPosition();
	void setPosition(int position, int die);
	int getCoins();
	void setCoins(int coins);
	void giveCoins(int earned_coins);
	std::vector<Property*>* getListOfProperties();
	void appendToListOfProperties(Property* new_property);
	void reset();

private:
	std::string type;
	int position;
	int coins;
	std::vector<Property*> ListOfProperties;
};

#endif
