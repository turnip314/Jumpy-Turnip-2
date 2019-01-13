#pragma once

#include <SFML\Graphics.hpp>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>

#include "Scene.h"
#include "TextureManager.h"
#include "Stats.h"
#include "Player.h"

using namespace std;
using namespace sf;

class Game
{
public:
	Game(TextureManager &textureManager);
	void run();
	static float getWidthRatio();
	static float getHeightRatio();
	static float widthRatio;
	static float heightRatio;

	const int fireballBossLevel = 14;
	const int mrGooseBossLevel = 29;
	const int freeplayNoLoopLevel = 30;
	const int freeplayLoopLevel = 31;
	const int freeplayTimeNoLoopLevel = 32;
	const int freeplayTimeLoopLevel = 33;

	void closeScene();

	void loadMainMenu();
	void openLevelSelection();
	void openUpgradeMenu();
	void openUpgradeMenu(Types::Players playerType);
	void openTeamMenu();
	void openCreditMenu();
	void startNewGame(int level, int difficulty);

	Scene* getCurScene();

	// Upgrade stuff
	void updateUpgrade(Types::Players player, int ID, int newStatus);
	int getMoney();
	bool spendMoney(int cost);

	// Changes page for upgrade
	void nextUpgradePage();
	void prevUpgradePage();

	// Changes page for levels
	void nextLevelPage();
	void prevLevelPage();

	// Turnip selection for team page
	Types::Players getCurSelection();
	void setCurSelection(Types::Players playerType);
	void swapTeamPosition(Types::Players p1, Types::Players p2);
	int getTeamStatus(Types::Players player);
	void buyNewSlot();
	void buyNewPlayer(Types::Players player);
	string getPlayerName(Types::Players playerType);
	int getRank(Types::Players playerType);
	int getTeamSize();

	// Get player texture based on texture map
	Textures::ID getPlayerTextureID(Types::Players playerType, Types::Sizes size);

	// end game conditions
	void updateHighScore(int level, int newScore, int timeElapsed, int difficulty);
	void updateXp(vector<Player*> players, float factor);
	void updateLevelStatus(int level);
	void addMoney(int money);
	void closeGame();

	// File IO stuff
	void loadLevels();
	void loadUpgrades();
	void loadProgress();
	void loadMessages();
	void saveProgress();

private:
	bool closeCurrentScene = false;
	void closeTopScene();
	void addTutorialPanel(Scene* scene, Types::Tutorials tutorialType);

	// Basic processes per frame
	void processEvents();
	void update(Time dt);
	void render();
	void deleteObjects();
	void readjustScale();

	RenderWindow window;
	stack<Scene*> scenes;

	// Texture related stuff
	TextureManager manager;

	//INITIALIING GAME STUFF

	// Texture maps
	void initializeTextureMaps();

	// Tutorials
	map<Types::Tutorials, string> tutorialMessages;
	map<Types::Tutorials, bool> tutorialReadStatus;

	// Stores information about games and objects
	map<Types::Players, string> playerNames;
	map<Types::Players, int[12]> playerUpgrades;
	map<Types::Players, int[12]> upgradeCosts;
	map<Types::Players, string[12]> upgradeNames;
	map<Types::Players, string[12]> upgradeDescriptions;

	// Ranks
	const int numImportantRanks = 17;
	const int importantRanks[17] = { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 60, 70, 80, 90, 100, 110, 120 };
	map<int, string> rankUpNames;

	int numLevels;

	bool refreshLevels = false;

	// Displays page for upgrade
	Types::Players upgradePlayerType = Types::Players::Normie;
	bool refreshUpgrade = false;

	int money;
	map<Types::Players, PlayerRanks> playerRanks;
	LevelScores levelScores[34];

	// Maps level number with level stats (objective, obstacle list, score needed)
	// Might change objectives to a list later
	map<int, Levels> levels;

	// For managing team page
	short teamSize;
	short teamStatus[8]; // -1 if locked, 0 not in team, 1 to teamSize is position
	Types::Players curSelected = Types::Players::PlayersEND;
	bool refreshTeam = false;

	//Displays level page
	short levelPage = 0;

	Vector2i defaultSize;
	Vector2i mainSize;
	Vector2f clickScaleFactor;
	Vector2f imageScaleFactor;

	Sprite cursorSprite;
};