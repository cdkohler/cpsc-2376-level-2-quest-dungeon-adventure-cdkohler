#include<iostream>
#include<vector>
#include<random>
#include<map>
#include<algorithm>

struct Room
{
	enum class Name { cell, gate, armory, jailers, exit };
	enum class Direction { N, S, E, W, none };
	std::string message;
	std::map<Direction, Name> doorways;
	bool hasKey{ false };
};

struct Player
{
	Room::Name currentRoom{ Room::Name::cell };
	int health{ 10 };
	bool hasKey{ false };
};
/* GUI Functions */

void clearConsole();
void pauseConsole();
void splashScreen();
void displayGameState(const Player& player, const std::vector<Room>& map);
void displayGameDone(const Player& player, const std::vector<Room>& map);
void displayIllegalMove(const Player& player, Room::Direction action);
Room::Direction getAction();

/* Engine Functions*/
std::vector<Room> buildMap();
void randomizeKey(std::map<Room::Name, Room> dungeonMap);
bool changeGameState(Room::Direction action, Player& player, std::vector<Room>& map);
bool gameIsNotDone(const Player& player);

int main() {

	/*State variables*/

	/*Splash Screen*/
	clearConsole();
	splashScreen();

	/*set up game*/
	std::vector<Room> map{ buildMap() };
	Player player;

	/*Until Game Termination Condition Do: */
	while (gameIsNotDone(player))
	{

		/*Display Game State*/
		clearConsole();
		displayGameState(player, map);

		/*Collect Player Action*/
		Room::Direction action{ getAction() };

		/*Update Game State*/
		if (!changeGameState(action, player, map))
		{
			displayIllegalMove(player, action);
		}
		/*Check Game Termination*/
	}

	/*Display Termination Game State*/
	displayGameDone(player, map);

	return 0;

}


std::vector<Room> buildMap()
{
	std::map<Room::Name, Room> dungeonMap;
	dungeonMap.push_back(
		{
			"A small, dark prison cell with doors South and East.",
			{Room::Direction::S, Room::Direction::E},
			{Room::Name::armory, Room::Name::gate},
			false
		});
	dungeonMap.push_back(
		{
			"A large, torchlit room with doors West, South, and East. \n There is daylight entering under the door to the East.",
			{Room::Direction::W, Room::Direction::S, Room::Direction::E},
			{Room::Name::cell, Room::Name::jailers, Room::Name::exit},
			false
		});

	dungeonMap.push_back(
		{
			"A store room with doors North and East.",
			{Room::Direction::N, Room::Direction::E},
			{Room::Name::cell, Room::Name::jailers},
			false
		});
	dungeonMap.push_back(
		{
			"A jailer's barracks with doors West and North.",
			{Room::Direction::W, Room::Direction::N},
			{Room::Name::armory, Room::Name::gate},
			false
		});
	dungeonMap.push_back(
		{
			"YOU FOUND THE KEY AND ESCAPED!",
			{},
			{},
			false
		});
	randomizeKey(dungeonMap);
	return dungeonMap;
}

void randomizeKey(std::vector<Room>& map)
{
	std::random_device seed;
	std::default_random_engine e(seed());
	std::uniform_int_distribution<int> randomRoom((int)Room::Name::cell, (int)Room::Name::jailers);
	map[randomRoom(e)].hasKey = true;

}

void clearConsole() {
	system("cls");
}

void pauseConsole() {
	system("PAUSE");
}

void splashScreen() {
	std::cout << "DUNGEON ADVENTURE" << std::endl;
	std::cout << std::endl;
	std::cout << "Davis (2020)" << std::endl;
	std::cout << "CPSC 2376, Programming II, Level 1 Quest" << std::endl;
	std::cout << "UA Little Rock, Computer Science Dept." << std::endl;
	std::cout << std::endl;
	std::cout << "INSTRUCTIONS:" << std::endl;
	std::cout << std::endl;
	std::cout << "Find the key and get out of the dungeon!" << std::endl;
	std::cout << std::endl;
	std::cout << "        (North)          " << std::endl;
	std::cout << "           w             " << std::endl;
	std::cout << "           |             " << std::endl;
	std::cout << "(West) a --+-- d (East)  " << std::endl;
	std::cout << "           |             " << std::endl;
	std::cout << "           s             " << std::endl;
	std::cout << "        (South)          " << std::endl;
	std::cout << std::endl;
	pauseConsole();
}

void displayGameState(const Player& player, const std::vector<Room>& map) {

	std::cout << "View:   ";
	std::cout << map[(int)player.currentRoom].message << std::endl;
	if (map[(int)player.currentRoom].hasKey)
	{
		std::cout << "        There is a key on the floor.You pick it up." << std::endl;
	}
	std::cout << "Health: " << player.health << std::endl;
	std::cout << "Equip:  ";
	if (player.hasKey) {
		std::cout << "1 jailer's key";
	}
	std::cout << std::endl;
}

void displayGameDone(const Player& player, const std::vector<Room>& map) {

	clearConsole();
	if (player.health > 0)
	{
		if (player.currentRoom == Room::Name::exit)
		{
			std::cout << map[(int)player.currentRoom].message << std::endl;
		}
	}
	else
	{
		std::cout << "YOU DIED...RIP." << std::endl;
	}

	std::cout << std::endl;
	pauseConsole();
}


void displayIllegalMove(const Player& player, Room::Direction action)
{

	clearConsole();

	if (player.currentRoom == Room::Name::gate && !player.hasKey && action == Room::Direction::E)
	{
		std::cout << "The door is locked. \n Perhaps if you had a key???" << std::endl;
	}
	else
	{
		std::cout << "You cannot go that way." << std::endl;
	}
	std::cout << std::endl;

	pauseConsole();

}

Room::Direction getAction() {
	char action;
	std::cout << std::endl;
	std::cout << "Select action: ";
	std::cin >> action;
	switch (action)
	{
	case 'w':
		return Room::Direction::N;
		break;
	case 'a':
		return Room::Direction::W;
		break;
	case 'd':
		return Room::Direction::E;
		break;
	case 's':
		return Room::Direction::S;
		break;

	default:
		return Room::Direction::none;
		break;
	}
}

//Apply the game's logical rules to the current location, possession of the
//key, move legality, and health.
bool changeGameState(Room::Direction action, Player& player, std::vector<Room>& map) {

	//pick up the key if it is in this room.
	if (map[(int)player.currentRoom].hasKey)
	{
		player.hasKey = true;
		map[(int)player.currentRoom].hasKey = false;
	}

	//if we are trying to go to the exit, but we don't have the key, it is illegal.
	if (player.currentRoom == Room::Name::gate && !player.hasKey && action == Room::Direction::E)
	{
		return false;
	}
	//check for door;
	for (int i{ 0 }; i < map[(int)player.currentRoom].doorways.size(); i++)
	{
		if (map[(int)player.currentRoom].doorways[i] == action)
		{
			//found a door in the direction we are trying to go, so go that way.
			player.currentRoom = map[(int)player.currentRoom].connectedRoom[i];
			player.health--;
			return true;
		}
	}
	return false; //if there is not a connected room in that direction, it is illegal.

}



//Check the end-of-game conditions.  If the player is out
//of health or the player has reached the exit
//then the game is over.
bool gameIsNotDone(const Player& player)
{
	return !(player.health <= 0 || player.currentRoom == Room::Name::exit);
}

