#include "Property.h"
#include "Player.h"

Property::Property()
{
	this->property_id = -1;
	this->buy_value = 99;
	this->rent_value = 19;
	this->owner = NULL;
	return;
}

Property::Property(int property_id, int buy_value, int rent_value)
{
	this->property_id = property_id;
	this->buy_value = buy_value;
	this->rent_value = rent_value;
	this->owner = NULL;
	return;
}

int Property::getPropertyID()
{
	return this->property_id;
}

int Property::getBuyValue()
{
	return this->buy_value;
}

int Property::getRentValue()
{
	return this->rent_value;
}

Player* Property::getOwner()
{
	return this->owner;
}

void Property::setOwner(Player* owner)
{
	this->owner = owner;
	return;
}