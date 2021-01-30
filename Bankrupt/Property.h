#pragma once
#ifndef PROPERTY_H
#define PROPERTY_H

#include "Player.h"

class Player;

class Property
{
public:
	Property();
	Property(int property_id, int buy_value, int rent_value);
	int getPropertyID();
	int getBuyValue();
	int getRentValue();
	Player* getOwner();
	void setOwner(Player* owner);

private:
	int property_id;
	int buy_value;
	int rent_value;
	Player* owner;
};

#endif