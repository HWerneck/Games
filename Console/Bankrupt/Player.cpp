#include "Player.h"
#include "Property.h"
#include <vector>

Player::Player(std::string type)
{
	this->type = type;
	return;
}

std::string Player::getType()
{
	return this->type;
}

void Player::setType(std::string type)
{
	this->type = type;
	return;
}

int Player::getPosition()
{
	return this->position;
}

void Player::setPosition(int position, int die)
{
	this->position = position + die;
	return;
}

int Player::getCoins()
{
	return this->coins;
}

void Player::setCoins(int coins)
{
	this->coins = coins;
	return;
}

void Player::giveCoins(int earned_coins)
{
	this->coins += earned_coins;
	return;
}

std::vector<Property*>* Player::getListOfProperties()
{
	std::vector<Property*>* list = &this->ListOfProperties;
	return list;
}

void Player::appendToListOfProperties(Property* new_property)
{
	this->ListOfProperties.push_back(new_property);
	return;
}

void Player::reset()
{
	this->setPosition(0, 0);
	this->coins = 300;
	this->ListOfProperties.clear();
}