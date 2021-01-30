#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#include "Player.h"
#include "Board.h"
#include "Property.h"

#define MAX_MATCHES 300
#define MAX_ROUNDS 1000
#define MAX_PLAYERS 4

struct Data
{
	std::string winner;
	int round;
};

bool isFileEmpty (std::ifstream& file)
{
	return (file.peek() == std::ifstream::traits_type::eof());
}

void createProperties(std::vector<Property>* property)
{
	std::string line;
	std::ifstream gameConfig("gameConfig.txt");
	if (gameConfig.is_open())
	{
		int iteration = 0;
		while (std::getline(gameConfig, line))
		{
			int buy_value = 0;
			int rent_value = 0;

			std::istringstream split(line);
			char split_char = ' ';
			std::vector<std::string> tokens;
			std::string token = "";
			while (std::getline(split, token, split_char))
			{
				if (token != "")
				{
					tokens.push_back(token);
				}
			}
			
			std::stringstream value1 (tokens.at(0));
			value1 >> buy_value;
			std::stringstream value2(tokens.at(1));
			value2 >> rent_value;

			Property new_property = Property(iteration, buy_value, rent_value);
			property->push_back(new_property);
		}
	}
	gameConfig.close();
	return;
}

void sort_players(std::vector<Player*>* player)
{
	int numberOfPlayers = player->size();
	std::vector<Player*> support_vector;
	for (int iii = 0; iii < numberOfPlayers; ++iii)
	{
		support_vector.push_back(player->at(iii));
	}
	player->clear();
	for (int iteration = 0; iteration <	numberOfPlayers; ++iteration)
	{
		int x = (rand() % support_vector.size());
		player->push_back(support_vector.at(x));
		support_vector.erase(support_vector.begin() + x);
	}
	return;
}

int rollTheDice()
{
	int die = (rand() % 6) + 1;
	return die;
}

void makePlayerMove(Player* player, int die)
{
	player->setPosition(player->getPosition(), die);
	if (player->getPosition() > 20)
	{
		int new_position = ((player->getPosition()) - 20);
		player->setPosition(new_position, 0);
		player->giveCoins(100);
	}
	return;
}

bool buyOrPass(Player* player, Property* property)
{
	if (player->getType() == "impulsivo")
	{
		return true;
	}
	else if (player->getType() == "exigente")
	{
		if (property->getRentValue() > 50)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (player->getType() == "cauteloso")
	{
		if ((player->getCoins() - property->getBuyValue()) >= 80)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (player->getType() == "aleatorio")
	{
		int coin = (rand() % 100) + 1;
		if (coin > 50)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

void removePlayer(std::vector<Player*>* player, int turn)
{
	player->at(turn)->setCoins(0);
	for (int ppp = 0; ppp < player->at(turn)->getListOfProperties()->size(); ++ppp)
	{
		player->at(turn)->getListOfProperties()->at(ppp)->setOwner(NULL);
	}
	player->erase(player->begin() + turn);
	return;
}

bool isGameFinished(std::vector<Player*>* player, int round)
{
	if (player->size() < 2)
	{
		return true;
	}
	else if (round > MAX_ROUNDS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Player* determineWinner(std::vector<Player*>* player, int round)
{
	Player* winner = NULL;
	if (player->size() < 2)
	{
		winner = player->at(0);
		return winner;
	}
	else
	{
		for (int www = 0; www < player->size(); ++www)
		{
			if (winner == NULL)
			{
				winner = player->at(www);
			}
			else if (winner->getCoins() < player->at(www)->getCoins())
			{
				winner = player->at(www);
			}
		}
		return winner;
	}
}

void printData(std::vector<Data>* dataVector)
{
	std::cout << "\nGame data:\n";

	int time_out = 0;
	int mean_round = 0;
	int impulsive_wins = 0;
	int demanding_wins = 0;
	int cautious_wins = 0;
	int random_wins = 0;
	for (int ttt = 0; ttt < dataVector->size(); ++ttt)
	{
		if (dataVector->at(ttt).round > MAX_ROUNDS)
		{
			time_out += 1;
			mean_round += MAX_ROUNDS;
		}
		else
		{
			mean_round += dataVector->at(ttt).round;
		}
		if (dataVector->at(ttt).winner == "impulsivo")
		{
			impulsive_wins += 1;
		}
		else if (dataVector->at(ttt).winner == "exigente")
		{
			demanding_wins += 1;
		}
		else if (dataVector->at(ttt).winner == "cauteloso")
		{
			cautious_wins += 1;
		}
		else if (dataVector->at(ttt).winner == "aleatorio")
		{
			random_wins += 1;
		}
	}
	std::cout << "Matches that ended by time out: " << time_out << ";\n";
	mean_round = mean_round / dataVector->size();
	std::cout << "Average round that matches end: " << mean_round << ";\n";

	std::cout << "The number of wins per player is:\n";
	std::cout << "    - Impulsivo: " << impulsive_wins << ";\n";
	std::cout << "    - Exigente: " << demanding_wins << ";\n";
	std::cout << "    - Cauteloso: " << cautious_wins << ";\n";
	std::cout << "    - Aleatorio: " << random_wins << ";\n";

	std::cout << "The percentage of wins by player is:\n";
	double impulsiveWinsPercentage = (impulsive_wins*100) / MAX_MATCHES;
	double demandingWinsPercentage = (demanding_wins*100) / MAX_MATCHES;
	double cautiousWinsPercentage = (cautious_wins*100) / MAX_MATCHES;
	double randomWinsPercentage = (random_wins*100) / MAX_MATCHES;
	std::cout << "    - Impulsivo: " << impulsiveWinsPercentage << "%;\n";
	std::cout << "    - Exigente: " << demandingWinsPercentage << "%;\n";
	std::cout << "    - Cauteloso: " << cautiousWinsPercentage << "%;\n";
	std::cout << "    - Aleatorio: " << randomWinsPercentage << "%;\n";

	int mostWins = 0;
	int players_with_most_wins = 0;

	if (mostWins < impulsive_wins)
	{ mostWins = impulsive_wins; players_with_most_wins = 1; }

	if (mostWins < demanding_wins)
	{ mostWins = demanding_wins; players_with_most_wins = 1; }
	else if (mostWins == demanding_wins) { players_with_most_wins += 1; }

	if (mostWins < cautious_wins)
	{ mostWins = cautious_wins; players_with_most_wins = 1;	}
	else if (mostWins == cautious_wins) { players_with_most_wins += 1; }

	if (mostWins < random_wins)
	{ mostWins = random_wins; players_with_most_wins = 1; }
	else if (mostWins == random_wins) { players_with_most_wins += 1; }

	if (players_with_most_wins == 1)
	{
		std::cout << "The player with most wins is the player ";
	}
	else if (players_with_most_wins > 1)
	{
		std::cout << "The players with most wins are the players ";
	}

	std::vector<std::string> mostTimeWinners;
	if (impulsive_wins == mostWins) { mostTimeWinners.push_back("impulsivo"); }
	if (demanding_wins == mostWins) { mostTimeWinners.push_back("exigente"); }
	if (cautious_wins == mostWins) { mostTimeWinners.push_back("cauteloso"); }
	if (random_wins == mostWins) { mostTimeWinners.push_back("aleatorio"); }

	for (int www = 0; www < players_with_most_wins; ++www)
	{
		if (players_with_most_wins == 1)
		{
			std::cout << mostTimeWinners.at(www);
			break;
		}
		if (players_with_most_wins > 1 && ((www + 1) != players_with_most_wins))
		{
			std::cout << mostTimeWinners.at(www) << ", ";
		}
		else if (players_with_most_wins > 1 && ((www + 1) == players_with_most_wins))
		{
			std::cout << mostTimeWinners.at(www);
		}
	}
	std::cout << "." << std::endl;
	return;
}

void createCSVFile(std::vector<Data>* dataVector)
{
	std::ofstream csvFile;
	csvFile.open("gameData.csv", std::ofstream::out | std::ofstream::ate | std::ofstream::app);
	if (csvFile.is_open())
	{
		for (int ddd = 0; ddd < dataVector->size(); ++ddd)
		{
			csvFile << dataVector->at(ddd).winner << ",";
			if (dataVector->at(ddd).round > MAX_ROUNDS)
			{
				csvFile << MAX_ROUNDS << "\n";
			}
			else
			{
				csvFile << dataVector->at(ddd).round << "\n";
			} 
		}
		csvFile.close();
	}
	return;
}

int main()
{
	Player impulsivo ("impulsivo");
	Player exigente ("exigente");
	Player cauteloso ("cauteloso");
	Player aleatorio ("aleatorio");
	std::vector<Player*> player;

	std::vector<Property> property;
	createProperties(&property);
	Board board (&property);
	int numberOfMatches = 0;

	std::vector<Data> dataVector;

	//Welcome to Bankrupt message
	std::cout << "\n\nWelcome to Bankrupt!\n";

	//Random seed
	srand(time(NULL));

	//Bankrupt game loop
	while (numberOfMatches < MAX_MATCHES)
	{
		//Set variables to start a game
		
		//First, we put the players in a vector, and then we sort them randomly
		player.clear();
		player.push_back(&impulsivo);
		player.push_back(&exigente);
		player.push_back(&cauteloso);
		player.push_back(&aleatorio);
		sort_players(&player);

		//Reset players (put all players in a position zero, reset their coins to 300 and properties to none)
		for (int ppp = 0; ppp < player.size(); ++ppp)
		{
			player.at(ppp)->reset();
		}

		//Reset winner
		Player* winner = NULL;

		//Reset properties to no owner
		for (int ppp = 1; ppp <= board.getSpaces(); ++ppp)
		{
			board.getProperty(ppp)->setOwner(NULL);
		}

		//Start the game at round 1.
		int round = 0;
		bool gameFinished = false;

		//Match start message
		//std::cout << "\nMatch " << (numberOfMatches+1) << "\n" ;

		//Main game loop
		while (!gameFinished)
		{
			round += 1;

			//Check if game has ended at the start of each round
			gameFinished = isGameFinished(&player, round);

			/*
			If positive, the game ends. This will be an end by time out when the round reaches (MAX_ROUNDS+1)
			and there are more than one player in the game.
			I'd like to highlight that the game will end at (MAX_ROUNDS+1) intentionally. The game could very well
			end naturally, but in the last round. Allowing the game to reach (MAX_ROUNDS+1) proves that without the
			need to create additional variables. Plus it is accounted correctly in the "determineWinner" function.
			*/
			if (gameFinished)
			{
				winner = determineWinner(&player, round);
				dataVector.push_back({ winner->getType(), round });
			}
			else
			{
				//Round start message
				//std::cout << "\nRound " << round << std::endl;
				for (int turn = 0; turn < player.size(); ++turn)
				{
					int die = rollTheDice();
					makePlayerMove(player.at(turn), die);

					//Commands to print die rolled, player position and number coin count
					//std::cout << "Player " << player.at(turn)->getType() << " got " << die << " and is at " << player.at(turn)->getPosition() << ".\n";
					//std::cout << "Player has " << player.at(turn)->getCoins() << " coins." << std::endl;

					//These two instances create pointers to the property and player being analysed.
					//This was done to improve the readability of the code in the next few steps.
					Property* currentProperty = board.getProperty(player.at(turn)->getPosition());
					Player* currentPropertyOwner = currentProperty->getOwner();

					//Check property the player is standing on

					//Property has an owner different than the current player
					if (currentProperty->getOwner() != NULL && currentPropertyOwner != player.at(turn))
					{
						//Pay Rent

						//Player can afford the rent
						if (player.at(turn)->getCoins() >= currentProperty->getRentValue())
						{
							currentPropertyOwner->giveCoins(currentProperty->getRentValue());
							player.at(turn)->setCoins(player.at(turn)->getCoins() - currentProperty->getRentValue());
						}

						//Player pays how much it can of the rent and goes broke
						else
						{
							currentPropertyOwner->giveCoins(player.at(turn)->getCoins());
							removePlayer(&player, turn);
						}
					}

					//Property has no owner
					else if (currentPropertyOwner == NULL)
					{
						if (player.at(turn)->getCoins() >= currentProperty->getBuyValue())
						{
							bool buyProperty = false;
							buyProperty = buyOrPass(player.at(turn), currentProperty);
							if (buyProperty)
							{
								player.at(turn)->setCoins(player.at(turn)->getCoins() - currentProperty->getBuyValue());
								currentProperty->setOwner(player.at(turn));
								player.at(turn)->appendToListOfProperties(currentProperty);
							}
						}
					}

					//Commands to print player coin count at the end of its turn
					//std::cout << "Player has " << player.at(turn)->getCoins() << " coins." << std::endl;

					//Check if game has ended at the end of each player's turn
					gameFinished = isGameFinished(&player, round);
					if (gameFinished)
					{
						winner = determineWinner(&player, round);
						dataVector.push_back({ winner->getType(), round });
					}
				}
			}
		}
		//Increase match count
		numberOfMatches += 1;
	}
	printData(&dataVector);
	createCSVFile(&dataVector);
	std::cout << "\nThanks for playing Bankrupt! See ya!\n";
	return 0;
}