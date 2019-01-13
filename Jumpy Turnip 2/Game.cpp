#include <SFML\Graphics.hpp>

#include <string>
#include "Game.h"
#include "Menu.h"
#include "MenuButton.h"
#include "Bird.h"
#include "GameScene.h"
#include "Flappy.h"
#include "Fireball.h"
#include "Plane.h"
#include "SupplyCrate.h"
#include "LevelButton.h"
#include "UpgradeButton.h"
#include "PlayerButton.h"
#include "MrGoose.h"

using namespace sf;

float Game::widthRatio = VideoMode::getDesktopMode().width / 1920.0;
float Game::heightRatio = VideoMode::getDesktopMode().height / 1080.0;

Game::Game(TextureManager &textureManager) : window(VideoMode(1280 * widthRatio, 720 * heightRatio), "RPG Game")
{
	loadLevels();
	loadUpgrades();
	loadProgress();
	loadMessages();

	// Initializes texture map
	initializeTextureMaps();

	// Initializes windows
	window.setFramerateLimit(360);
	window.setKeyRepeatEnabled(false);
	window.setTitle("Jumpy Turnip");
	window.setMouseCursorVisible(false);

	View scaledView(FloatRect(0, 0, 1280, 720));
	scaledView.setViewport(sf::FloatRect(0, 0, 1, 1));
	window.setView(scaledView);

	Image icon;
	icon.loadFromFile("Images/turnip.png");
	window.setIcon(50, 55, icon.getPixelsPtr());
	manager = textureManager;
	loadMainMenu();

	defaultSize = Vector2i(1280, 720);
	mainSize = Vector2i(1280*Game::getWidthRatio(), 720*Game::getHeightRatio());
	clickScaleFactor = Vector2f(Game::getWidthRatio(), Game::getHeightRatio());
	imageScaleFactor = Vector2f(Game::getWidthRatio(), Game::getHeightRatio());

	cursorSprite.setTexture(*manager.getTexture(Textures::Cursor));
}

void Game::run()
{
	Clock clock;
	while (window.isOpen())
	{
		processEvents();
		update(clock.restart());
		render();
		deleteObjects();
	}
}

void Game::processEvents()
{
	
	Vector2i mousePos = Mouse::getPosition(window);


	Event windowEvent;
	while (window.pollEvent(windowEvent))
	{
		switch (windowEvent.type)
		{
		case Event::KeyPressed:
			scenes.top()->processInput(windowEvent.key.code, true);
			break;
		case Event::KeyReleased:
			scenes.top()->processInput(windowEvent.key.code, false);
			break;
		case Event::Closed:
			saveProgress();
			window.close();
			break;
		case Event::MouseButtonPressed:
			scenes.top()->processInput((Vector2i) Math::scaleBack(Mouse::getPosition(window), clickScaleFactor), true);
			break;
		case Event::LostFocus:
			scenes.top()->processInput(Keyboard::Escape, true);
			break;
		case Event::Resized:
			readjustScale();
			break;
		}
	}
	scenes.top()->processInput((Vector2i) Math::scaleBack(Mouse::getPosition(window), clickScaleFactor), false);
	cursorSprite.setPosition(Math::scaleBack(Mouse::getPosition(window), clickScaleFactor));
}

void Game::update(Time dt)
{
	scenes.top()->update(dt);
}

void Game::render()
{
	window.clear();
	scenes.top()->render(&window, imageScaleFactor);
	window.draw(cursorSprite);
	window.display();
}

void Game::deleteObjects()
{
	scenes.top()->deleteObjects();

	if (closeCurrentScene)
	{
		closeTopScene();
	}
	else if (refreshTeam)
	{
		closeTopScene();
		openTeamMenu();
		refreshTeam = false;
	}
	else if (refreshUpgrade)
	{
		closeTopScene();
		openUpgradeMenu();
		refreshUpgrade = false;
	}
	else if (refreshLevels)
	{
		closeTopScene();
		openLevelSelection();
		refreshLevels = false;
	}
}

void Game::readjustScale()
{
	clickScaleFactor.x = window.getSize().x / 1280.f;
	clickScaleFactor.y = window.getSize().y / 720.f;
}

float Game::getWidthRatio()
{
	return widthRatio;
}

float Game::getHeightRatio()
{
	return heightRatio;
}

Scene* Game::getCurScene()
{
	return scenes.top();
}

Textures::ID Game::getPlayerTextureID(Types::Players playerType, Types::Sizes size)
{
	if (playerType == Types::Players::PlayersEND)
	{
		if (size == Types::Sizes::MediumScaled)
		{
			return Textures::SlotLockS;
		}
		else
		{
			return Textures::SlotLock;
		}
	}
	else if (size == Types::Sizes::BigScaled)
	{
		return static_cast<Textures::ID>(Textures::NormieBigS + playerType);
	}
	else if (size == Types::Sizes::Big)
	{
		return static_cast<Textures::ID>(Textures::NormieBig + playerType);
	}
	else if (size == Types::Sizes::MediumScaled)
	{
		return static_cast<Textures::ID>(Textures::NormieMedS + playerType);
	}
	else if (size == Types::Sizes::Medium)
	{
		return static_cast<Textures::ID>(Textures::NormieMed + playerType);
	}
	else if (size == Types::Sizes::Small)
	{
		// placeholder
		return static_cast<Textures::ID>(Textures::NormieMed + playerType);
	}
}

void Game::loadMainMenu()
{
	// Initializes Main Menu
	int i = 1;
	Menu* mainMenu = new Menu(&manager);

	MenuButton* playButton = new MenuButton(Types::Buttons::AdventureButton, &manager, this,
		Vector2f(200, 80), Vector2f(440, 180));
	playButton->setText(Fonts::Atarian, 65, "Adventure");
	mainMenu->addButton(playButton);

	MenuButton* upgradeMenuButton = new MenuButton(Types::Buttons::UpgradeMenuButton, &manager, this,
		Vector2f(200, 230), Vector2f(440, 330));
	upgradeMenuButton->setText(Fonts::Atarian, 65, "Upgrades");
	mainMenu->addButton(upgradeMenuButton);

	MenuButton* teamButton = new MenuButton(Types::Buttons::TeamButton, &manager, this,
		Vector2f(200, 380), Vector2f(440, 480));
	teamButton->setText(Fonts::Atarian, 65, "Turnips");
	mainMenu->addButton(teamButton);

	MenuButton* creditButton = new MenuButton(Types::Buttons::CreditButton, &manager, this,
		Vector2f(200, 530), Vector2f(440, 630));
	creditButton->setText(Fonts::Atarian, 65, "About");
	mainMenu->addButton(creditButton);

	

	scenes.push(mainMenu);
}

void Game::closeScene()
{
	closeCurrentScene = true;
}

void Game::closeTopScene()
{
	// delete all pointers created in scene
	if (scenes.size() == 1)
	{
		window.close();
	}
	else
	{
		Scene* scene = scenes.top();
		delete scene;
		scenes.pop();
	}
	closeCurrentScene = false;
}

void Game::prevLevelPage()
{
	levelPage = (levelPage + 3) % 4;
	refreshLevels = true;
}

void Game::nextLevelPage()
{
	levelPage = (levelPage + 1) % 4;
	refreshLevels = true;
}

void Game::openLevelSelection()
{
	Menu* lvlSelectionMenu = new Menu(&manager);

	if (levelPage <= 2)
	{
		for (int i = 0; i < 10; i++)
		{
			float xPos = 200 + 180 * (i % 5);
			float yPos = 150 + 220 * (i / 5);
			int levelNum = i + 10 * levelPage;
			LevelButton* lvlButton = new LevelButton(levelNum, levelScores[levelNum], this, &manager,
				Vector2f(xPos, yPos), Vector2f(xPos + 150, yPos + 150));

			lvlSelectionMenu->addButton(lvlButton);
		}
	}
	else
	{
		LevelButton* noLoopButton = new LevelButton(30, levelScores[30], this, &manager,
			Vector2f(280, 100), Vector2f(280 + 300, 100 + 200));

		lvlSelectionMenu->addButton(noLoopButton);

		LevelButton* loopButton = new LevelButton(31, levelScores[31], this, &manager,
			Vector2f(700, 100), Vector2f(700 + 300, 100 + 200));

		lvlSelectionMenu->addButton(loopButton);

		LevelButton* noLoopTimeButton = new LevelButton(32, levelScores[32], this, &manager,
			Vector2f(280, 350), Vector2f(280 + 300, 350 + 200));

		lvlSelectionMenu->addButton(noLoopTimeButton);

		LevelButton* loopTimeButton = new LevelButton(33, levelScores[33], this, &manager,
			Vector2f(700, 350), Vector2f(700 + 300, 350 + 200));

		lvlSelectionMenu->addButton(loopTimeButton);
	}

	MenuButton* prevPageButton = new MenuButton(Types::Buttons::PrevLevelsButton, &manager, this,
		Vector2f(200, 600), Vector2f(300, 700));
	MenuButton* nextPageButton = new MenuButton(Types::Buttons::NextLevelsButton, &manager, this,
		Vector2f(950, 600), Vector2f(1050, 700));
	prevPageButton->setText(Fonts::Calibri, 25, "Previous");
	nextPageButton->setText(Fonts::Calibri, 25, "Next");
	lvlSelectionMenu->addButton(prevPageButton);
	lvlSelectionMenu->addButton(nextPageButton);

	MenuButton* backButton = new MenuButton(Types::Buttons::BackButton, &manager, this,
		Vector2f(20, 40), Vector2f(140, 90));
	backButton->setText(Fonts::Calibri, 35, "Back");
	backButton->centerText();

	lvlSelectionMenu->addButton(backButton);

	scenes.push(lvlSelectionMenu);
}

void Game::prevUpgradePage()
{
	int curPlayer = upgradePlayerType;
	int nextPlayer = (curPlayer + 7) % (Types::Players::PlayersEND);
	upgradePlayerType = static_cast<Types::Players>(nextPlayer);
	refreshUpgrade = true;
}

void Game::nextUpgradePage()
{
	int curPlayer = upgradePlayerType;
	int nextPlayer = (curPlayer + 1) % (Types::Players::PlayersEND);
	upgradePlayerType = static_cast<Types::Players>(nextPlayer);
	refreshUpgrade = true;
}

void Game::openUpgradeMenu()
{
	openUpgradeMenu(upgradePlayerType);
}

void Game::openUpgradeMenu(Types::Players playerType)
{
	// How upgrade buttons work:
	// Each turnip has 12 upgrades (3 paths of 4)
	// The upgrades are numbered 0 to 11
	// Upgrade Menu defaults to Normie Turnip
	Menu* upgradeMenu = new Menu(&manager);
	upgradeMenu->setText(playerNames[playerType], Vector2f(40, 500), Fonts::Calibri, 30, Color::White);
	upgradeMenu->setText("Rank: " + to_string(playerRanks[playerType].rank), Vector2f(40, 535), Fonts::Calibri, 30, Color::White);
	
	Textures::ID displayTexture = static_cast<Textures::ID>(Textures::NormieHuge + playerType);
	upgradeMenu->addImage(displayTexture, Vector2f(20.f, 140.f));

	for (int i = 0; i < 12; i++)
	{
		float xPos = 320 + 220 * (i % 4);
		float yPos = 100 + 200 * (i / 4);
		UpgradeButton* upgradeButton = new UpgradeButton(playerType, i, this, &manager,
			Vector2f(xPos, yPos), Vector2f(xPos + 160, yPos + 130));
		upgradeButton->setUpgradeStatus(playerUpgrades[playerType][i], upgradeCosts[playerType][i]);
		// Last input to be changed
		upgradeButton->setText(Fonts::Calibri, 25, upgradeNames[playerType][i], upgradeDescriptions[playerType][i]);
		upgradeMenu->addButton(upgradeButton);
	}

	MenuButton* backButton = new MenuButton(Types::Buttons::BackButton, &manager, this,
		Vector2f(20, 40), Vector2f(140, 90));
	backButton->setText(Fonts::Calibri, 35, "Back");
	backButton->centerText();

	upgradeMenu->addButton(backButton);

	// Next and Previous page buttons
	MenuButton* prevButton = new MenuButton(Types::Buttons::PrevUpgradesButton, &manager, this,
		Vector2f(100, 650), Vector2f(220, 700));
	MenuButton* nextButton = new MenuButton(Types::Buttons::NextUpgradesButton, &manager, this,
		Vector2f(1100, 650), Vector2f(1220, 700));
	prevButton->setText(Fonts::Calibri, 25, "Previous");
	nextButton->setText(Fonts::Calibri, 25, "Next");

	upgradeMenu->addButton(prevButton);
	upgradeMenu->addButton(nextButton);

	addTutorialPanel(upgradeMenu, Types::Tutorials::UpgradesIntro);

	scenes.push(upgradeMenu);
}

string Game::getPlayerName(Types::Players playerType)
{
	return playerNames[playerType];
}

void Game::setCurSelection(Types::Players playerType)
{
	curSelected = playerType;
}

Types::Players Game::getCurSelection()
{
	return curSelected;
}

void Game::swapTeamPosition(Types::Players p1, Types::Players p2)
{
	// This is equivalent to nothing being selected
	if (p1 == Types::Players::PlayersEND)
	{
		return;
	}

	int p1Status = teamStatus[p1];
	teamStatus[p1] = teamStatus[p2];
	teamStatus[p2] = p1Status;

	refreshTeam = true;
	curSelected = Types::Players::PlayersEND;
}

int Game::getTeamStatus(Types::Players player)
{
	return teamStatus[player];
}

void Game::buyNewSlot()
{
	// Unlock size is number of turnips you have unlocked
	int unlockSize = Types::Players::PlayersEND;
	for (int i = 0; i < Types::Players::PlayersEND; i++)
	{
		if (teamStatus[i] == -1)
		{
			unlockSize -= 1;
		}
	}
	if (unlockSize <= teamSize)
	{
		// Can't buy new slot - not enough team members
		MessagePanel* noSlotPanel = new MessagePanel(MessagePanel::CANCEL, scenes.top(), &manager, Vector2f(100, 100));
		noSlotPanel->setMessage("Unlock more turnips before buying this slot");
		scenes.top()->addPanel(noSlotPanel);
	}
	else if (money < 10 * ((int) (pow((teamSize + 2), 5) / 10)))
	{
		MessagePanel* noMoneyPanel = new MessagePanel(MessagePanel::CANCEL, scenes.top(), &manager, Vector2f(100, 100));
		noMoneyPanel->setMessage("You can't afford this");
		scenes.top()->addPanel(noMoneyPanel);
	}
	else
	{
		refreshTeam = true;
		teamSize++;
		int newMember;
		for (int i = 0; i < Types::Players::PlayersEND; i++)
		{
			if (teamStatus[i] == 0)
			{
				teamStatus[i] = teamSize;
				break;
			}
		}
		money -= 10 * ((int)(pow((teamSize + 1), 5) / 10));
	}
}

void Game::buyNewPlayer(Types::Players playerType)
{
	if (spendMoney(500))
	{
		teamStatus[playerType] = 0;
		for (int i = 0; i < 3; i++)
		{
			playerUpgrades[playerType][4*i] = 1;
		}
		refreshTeam = true;
	}
	else
	{
		MessagePanel* noMoneyPanel = new MessagePanel(MessagePanel::CANCEL, scenes.top(), &manager, Vector2f(100, 100));
		noMoneyPanel->setMessage("You can't afford this");
		scenes.top()->addPanel(noMoneyPanel);
	}
}

int Game::getTeamSize()
{
	return teamSize;
}

void Game::openTeamMenu()
{
	curSelected = Types::Players::PlayersEND;

	Menu* teamMenu = new Menu(&manager);

	MenuButton* backButton = new MenuButton(Types::Buttons::BackButton, &manager, this,
		Vector2f(20, 40), Vector2f(140, 90));
	backButton->setText(Fonts::Calibri, 35, "Back");
	backButton->centerText();
	teamMenu->addButton(backButton);

	// The eight slots for turnips displayed here
	for (int i = 0; i < 8; i++)
	{
		// Make eight player buttons, initialize with player use/unlock status
		Types::Players playerType = static_cast<Types::Players>(i);
		float xPos = 250 + 200 * (i % 4);
		float yPos = 50 + 200 * (i / 4);
		PlayerButton* playerButton = new PlayerButton(this, &manager,
			Vector2f(xPos, yPos), Vector2f(xPos + 150, yPos + 150));
		playerButton->initialize(Types::Buttons::PlayerButton, playerType, teamStatus[i]);
		teamMenu->addButton(playerButton);
	}

	for (int i = 1; i <= teamSize; i++)
	{
		// Makes buttons for team member slots
		int teamMember;
		for (int j = 0; j < Types::Players::PlayersEND; j++)
		{
			if (teamStatus[j] == i)
			{
				teamMember = j;
				break;
			}
		}
		
		Types::Players playerType = static_cast<Types::Players>(teamMember);

		float xPos = 640 - 75 * (min((int) Types::Players::PlayersEND, (teamSize + 1))) + 150 * (i-1);
		float yPos = 500;
		PlayerButton* teamSlotButton = new PlayerButton(this, &manager,
			Vector2f(xPos, yPos), Vector2f(xPos + 125, yPos + 125));
		teamSlotButton->initialize(Types::Buttons::TeamSlotButton, playerType, teamStatus[teamMember]);
		teamMenu->addButton(teamSlotButton);
	}

	if (teamSize < Types::Players::PlayersEND)
	{
		float xPos = 640 - 75 * (min((int)Types::Players::PlayersEND, (teamSize + 1))) + 150 * teamSize;
		float yPos = 500;
		PlayerButton* teamSlotButton = new PlayerButton(this, &manager,
			Vector2f(xPos, yPos), Vector2f(xPos + 125, yPos + 125));
		teamSlotButton->initialize(Types::Buttons::TeamSlotButton, Types::Players::PlayersEND, -1);
		teamMenu->addButton(teamSlotButton);
	}

	addTutorialPanel(teamMenu, Types::Tutorials::TeamIntro);

	scenes.push(teamMenu);
	
}

void Game::openCreditMenu()
{
	Menu* creditMenu = new Menu(&manager);

	MenuButton* backButton = new MenuButton(Types::Buttons::BackButton, &manager, this,
		Vector2f(20, 40), Vector2f(140, 90));
	backButton->setText(Fonts::Calibri, 35, "Back");
	backButton->centerText();
	creditMenu->addButton(backButton);

	creditMenu->addImage(Textures::CreditPanel, Vector2f(20, 130), Color(255, 255, 255, 128));

	creditMenu->setText("About", Vector2f(80, 160), Fonts::Atarian, 80, Color(185, 242, 255));
	creditMenu->setText("Jumpy Turnip 2 is a third-person shooter game with flappy bird", 
		Vector2f(80, 260), Fonts::Calibri, 40, Color(185, 242, 255));
	creditMenu->setText("movement mechanics. The game is programmed by Andrew Luo.",
		Vector2f(80, 310), Fonts::Calibri, 40, Color(185, 242, 255));
	creditMenu->setText("Special thanks to Kayla Estacio for creating the amazing artwork", 
		Vector2f(80, 390), Fonts::Calibri, 40, Color(185, 242, 255));
	creditMenu->setText("for this game, from the turnips to the backgrounds to all the buttons.",
		Vector2f(80, 440), Fonts::Calibri, 40, Color(185, 242, 255));
	creditMenu->setText("The game wouldn't have been nearly as successful without her help!", 
		Vector2f(80, 490), Fonts::Calibri, 40, Color(185, 242, 255));
	creditMenu->setText("Lastly, I would like to thank Mr. Goose. Always thank Mr. Goose.",
		Vector2f(80, 570), Fonts::Calibri, 40, Color(185, 242, 255));

	creditMenu->addImage(Textures::Goose, Vector2f(350, 30));
	scenes.push(creditMenu);

}

void Game::startNewGame(int level, int difficulty)
{
	Texture *backgroundTexture = manager.getTexture(Textures::Background1);
	GameScene* gameScene = new GameScene(this, &manager);

	MenuButton* pauseButton = new MenuButton(Types::Buttons::PauseButton, &manager, this,
		Vector2f(1140, 640), Vector2f(1240, 710));
	pauseButton->setText(Fonts::Calibri, 35, "_Pause");
	gameScene->addButton(pauseButton);

	// Set team
	vector<Player*> playerTeam;
	for (int i = 1; i <= teamSize; i++)
	{
		// Finds team member in nth slot
		int teamMember;
		for (int j = 0; j < Types::Players::PlayersEND; j++)
		{
			if (teamStatus[j] == i)
			{
				teamMember = j;
				break;
			}
		}
		Types::Players playerType = static_cast<Types::Players>(teamMember);
		Player *player = new Player(&manager, gameScene);
		player->initializeStats(playerType, playerUpgrades[playerType],
			playerRanks[playerType].rank);
		playerTeam.push_back(player);

		if (playerUpgrades[playerType][7] == 2 ||
			((playerUpgrades[playerType][3] == 2 || playerUpgrades[playerType][11] == 2) && playerType == Types::Players::Nature))
		{
			addTutorialPanel(gameScene, Types::Tutorials::AbilityIntro);
		}
	}
	gameScene->makeTeam(teamSize, playerTeam);
	gameScene->setPlayer(playerTeam.at(0));

	// Gets levelInfo from level -> objective and obstacle pair
	Levels levelInfo = levels[level];

	// Decides to loop enemies or not
	// Everything except survive and survive get score will loop
	bool doLoop = levelInfo.loops;

	// Passes highscore to gamescene
	int highScore;
	if (difficulty == 1)
	{
		highScore = levelScores[level].easyScore;
	}
	else if (difficulty == 2)
	{
		highScore = levelScores[level].mediumScore;
	}
	else if (difficulty == 3)
	{

		highScore = levelScores[level].hardScore;
	}
	else if (difficulty == 4)
	{
		highScore = levelScores[level].impossibleScore;
	}

	// Initializes info about the current game to be played
	gameScene->initializeGame(level, levelScores[level].levelStatus, levelInfo.objective,
		doLoop, levelInfo.scoreNeeded, difficulty, highScore);

	// Get Score and None Objectives have auto-generated obstacles
	if (levelInfo.objective == Types::Objectives::GetScoreEndless || levelInfo.objective == Types::Objectives::None)
	{
		for (ObstacleStats stat : levelInfo.obstacleList)
		{
			gameScene->addObstacleToChoose(stat);
		}
	}
	else
	{
		for (ObstacleStats stat : levelInfo.obstacleList)
		{
			gameScene->addObstacle(stat);
		}
	}

	// Game objective message and tutorial
	MessagePanel* objectivePanel = new MessagePanel(MessagePanel::OK, gameScene, &manager, Vector2f(200, 100));
	if (level <= mrGooseBossLevel)
	{
		objectivePanel->setMessage("Level " + to_string(level + 1) + ":");
	}
	if (levelInfo.objective == Types::Objectives::Survive)
	{
		objectivePanel->setMessage("Survive to win the round.");
	}
	else if (levelInfo.objective == Types::Objectives::SurviveGetScore)
	{
		objectivePanel->setMessage("Survive and obtain a score of " + to_string(levelInfo.scoreNeeded) + " to win the round.");
	}
	else if (levelInfo.objective == Types::Objectives::GetScoreEndless)
	{
		objectivePanel->setMessage("Obtain a score of " + to_string(levelInfo.scoreNeeded) + " to win the round. Survival is not necessary.");
	}
	else if (levelInfo.objective == Types::Objectives::None)
	{
		objectivePanel->setMessage("Freeplay mode. Survive as long as you can. Good luck!");
	}

	if (level == 0)
	{
		addTutorialPanel(gameScene, Types::Tutorials::Intro1);
		addTutorialPanel(gameScene, Types::Tutorials::Intro2);
		addTutorialPanel(gameScene, Types::Tutorials::Intro3);
		addTutorialPanel(gameScene, Types::Tutorials::Intro4);
	}
	else if (level == 2)
	{
		addTutorialPanel(gameScene, Types::Tutorials::FireballIntro);
	}
	else if (level == 3)
	{
		addTutorialPanel(gameScene, Types::Tutorials::LoopIntro);
	}
	else if (level == 5)
	{
		addTutorialPanel(gameScene, Types::Tutorials::EndlessIntro);
	}
	else if (level == 8)
	{
		addTutorialPanel(gameScene, Types::Tutorials::FlappyIntro);
	}
	else if (level == 9)
	{
		addTutorialPanel(gameScene, Types::Tutorials::RegenIntro);
	}
	else if (level == 11)
	{
		addTutorialPanel(gameScene, Types::Tutorials::FadeIntro);
	}
	else if (level == 12)
	{
		addTutorialPanel(gameScene, Types::Tutorials::FlappyWarning);
	}
	else if (level == fireballBossLevel)
	{
		addTutorialPanel(gameScene, Types::Tutorials::FireballBossWarning);
	}
	else if (level == 15)
	{
		addTutorialPanel(gameScene, Types::Tutorials::CloudIntro);
	}
	else if (level == 17)
	{
		addTutorialPanel(gameScene, Types::Tutorials::PlaneIntro);
	}
	else if (level == 19)
	{
		addTutorialPanel(gameScene, Types::Tutorials::UFOIntro);
	}
	else if (level == 22)
	{
		addTutorialPanel(gameScene, Types::Tutorials::FireballDodgeWarning);
	}
	else if (level == 28)
	{
		addTutorialPanel(gameScene, Types::Tutorials::MigratingBirdsWarning);
	}
	else if (level == mrGooseBossLevel)
	{
		addTutorialPanel(gameScene, Types::Tutorials::FinalLevelWarning);
	}
	else if (level > mrGooseBossLevel)
	{
		addTutorialPanel(gameScene, Types::Tutorials::FreeplayLevelIntro);
	}

	if (teamSize > 1)
	{
		addTutorialPanel(gameScene, Types::Tutorials::TeamPlayIntro);
	}
	// For boss level
	if (level == 29)
	{
		MrGoose* goose = new MrGoose(Types::Obstacles::MrGoose,
			manager.getTexture(Textures::Goose),
			manager.getTexture(Textures::GooseAttack),
			manager.getTexture(Textures::Fireball1),
			manager.getTexture(Textures::Fireball2), gameScene);
		goose->setStats(1.f + difficulty / 10.f, 1.f + difficulty / 6.f, 1.f, false, false);
		gameScene->pushObstacle(goose);
	}

	gameScene->addPanel(objectivePanel);
	scenes.push(gameScene);

}

void Game::addTutorialPanel(Scene* scene, Types::Tutorials tutorialType)
{
	if (!tutorialReadStatus[tutorialType])
	{
		string message = tutorialMessages[tutorialType];
		MessagePanel* tutorialPanel = new MessagePanel(MessagePanel::OK, scene, &manager, Vector2f(200, 100));
		tutorialPanel->setMessage(message);
		scene->addPanel(tutorialPanel);
		tutorialReadStatus[tutorialType] = true;
	}
}

int Game::getRank(Types::Players playerType)
{
	return playerRanks[playerType].rank;
}

void Game::loadLevels()
{
	// Load level file
	ifstream lvlFile;
	lvlFile.open("Saves/levels.txt");

	// Check For Error
	if (lvlFile.fail())
	{
		cerr << "File does not exist";
		exit(1);
	}

	// First number in file is number of levels
	lvlFile >> numLevels;

	// Loops through each level
	for (unsigned i = 0; i < numLevels; i++)
	{
		// For each level, file will specify game objective,
		// how many objects to add, and objective score
		vector<ObstacleStats> obstacleStats;
		int gameObjective;
		int numObstacles;
		int scoreNeeded;
		bool ifLoops;
		lvlFile >> gameObjective;
		lvlFile >> numObstacles;
		lvlFile >> scoreNeeded;
		lvlFile >> ifLoops;

		// Loops through number of objectives and stores of stat of each
		// Speed, health, and damage are stored as fractions
		for (unsigned i = 0; i < numObstacles; i++)
		{
			int stats[12];
			for (unsigned j = 0; j < 12; j++)
			{
				lvlFile >> stats[j];
			}
			ObstacleStats curStat;
			curStat.type = static_cast<Types::Obstacles>(stats[0]);
			curStat.time = float(stats[1]) / stats[2];
			curStat.speedMul = float(stats[3]) / stats[4];
			curStat.healthMul = float(stats[5]) / stats[6];
			curStat.damageMul = float(stats[7]) / stats[8];
			curStat.fade = stats[9];
			curStat.regen = stats[10];
			curStat.startYPos = stats[11];
			obstacleStats.push_back(curStat);
		}
		// Creates a new level struct, and stores level info
		// Adds to levels array
		Levels level;
		Types::Objectives objective = static_cast<Types::Objectives>(gameObjective);
		level.objective = objective;
		level.obstacleList = obstacleStats;
		level.scoreNeeded = scoreNeeded;
		level.loops = ifLoops;
		levels[i] = level;
	}

	lvlFile.close();

}

void Game::loadUpgrades()
{
	// Load level file
	ifstream upgradeFile;
	upgradeFile.open("Saves/upgrades.txt");

	// Check For Error
	if (upgradeFile.fail())
	{
		cerr << "File does not exist";
		exit(1);
	}

	// Gets saved upgrade information and updates player stats for each player
	for (int i = 0; i < Types::Players::PlayersEND; i++)
	{
		Types::Players type = static_cast<Types::Players>(i);
		// 3 paths of 4 upgrades
		for (int j = 0; j < 12; j++)
		{
			upgradeFile >> playerUpgrades[type][j];
		}
	}

	upgradeFile.close();

	upgradeFile.open("Saves/upgradeCosts.txt");

	// Check For Error
	if (upgradeFile.fail())
	{
		cerr << "File does not exist";
		exit(1);
	}

	// Gets saved upgrade information and updates player stats for each player
	for (int i = 0; i < Types::Players::PlayersEND; i++)
	{
		Types::Players type = static_cast<Types::Players>(i);
		// 3 paths of 4 upgrades
		for (int j = 0; j < 12; j++)
		{
			upgradeFile >> upgradeCosts[type][j];
		}
	}

	upgradeFile.close();
	upgradeFile.open("Saves/upgradeNames.txt");

	// Check For Error
	if (upgradeFile.fail())
	{
		cerr << "File does not exist";
		exit(1);
	}

	upgradeFile >> noskipws;

	// Gets descriptions of the upgrades
	string allNames;
	while (true) {
		char x;
		upgradeFile >> x;
		if (x != '\n')
		{
			allNames += x;
		}
		if (upgradeFile.eof()) break;
	}
	vector<string> nameList = TextBox::splitString(allNames, '#');

	for (int i = 0; i < Types::Players::PlayersEND; i++)
	{
		Types::Players playerType = static_cast<Types::Players>(i);
		for (int j = 0; j < 12; j++)
		{
			upgradeNames[playerType][j] = nameList.at(0);
			nameList.erase(nameList.begin());
		}
	}
	
	upgradeFile.close();
	upgradeFile.open("Saves/upgradeDescriptions.txt");

	// Check For Error
	if (upgradeFile.fail())
	{
		cerr << "File does not exist";
		exit(1);
	}

	upgradeFile >> noskipws;

	// Gets descriptions of the upgrades
	string allDescriptions;
	while (true) {
		char x;
		upgradeFile >> x;
		if (x != '\n')
		{
			allDescriptions += x;
		}
		if (upgradeFile.eof()) break;
	}
	vector<string> descriptionList = TextBox::splitString(allDescriptions, '#');

	for (int i = 0; i < Types::Players::PlayersEND; i++)
	{
		Types::Players playerType = static_cast<Types::Players>(i);
		for (int j = 0; j < 12; j++)
		{
			upgradeDescriptions[playerType][j] = descriptionList.at(0);
			descriptionList.erase(descriptionList.begin());
		}
	}

	upgradeFile.close();
}

void Game::loadProgress()
{
	// Load level file
	ifstream progressFile;
	progressFile.open("Saves/progress.txt");

	// Check For Error
	if (progressFile.fail())
	{
		cerr << "File does not exist";
		exit(1);
	}

	// In order, saves will be
	// money, rank, xp
	// level progress = { win/lock status, high scores for
	// easy, med, hard, impossible }
	progressFile >> money;
	for (int i = 0; i < Types::Players::PlayersEND; i++)
	{
		PlayerRanks curRank;
		progressFile >> curRank.rank;
		progressFile >> curRank.xp;
		Types::Players playerType = static_cast<Types::Players>(i);
		playerRanks[playerType] = curRank;
	}

	// Turnip team and unlock status (add later)
	// First entry is team size
	// Make : -1 locked, 0 not in team, then 1, 2, 3... for position in team
	progressFile >> teamSize;

	for (int i = 0; i < 8; i++)
	{
		progressFile >> teamStatus[i];
	}

	for (int i = 0; i < numLevels; i++)
	{
		LevelScores curLevel;
		progressFile >> curLevel.levelStatus;
		progressFile >> curLevel.easyScore;
		progressFile >> curLevel.mediumScore;
		progressFile >> curLevel.hardScore;
		progressFile >> curLevel.impossibleScore;
		levelScores[i] = curLevel;
	}

	for (int i = 0; i < Types::Tutorials::TutorialsEND; i++)
	{
		Types::Tutorials tutorialType = static_cast<Types::Tutorials>(i);
		progressFile >> tutorialReadStatus[tutorialType];
	}

	progressFile.close();
}

void Game::loadMessages()
{
	// Tutorial messages should be same order in enum as they are in the file
	ifstream messageFile;
	messageFile.open("Saves/tutorials.txt");

	// Check For Error
	if (messageFile.fail())
	{
		cerr << "File does not exist";
		exit(1);
	}

	messageFile >> noskipws;

	string allTutorials;
	while (true) {
		char x;
		messageFile >> x;
		if (x == '@')
		{
			break;
		}
		if (x != '\n')
		{
			allTutorials += x;
		}
	}
	vector<string> tutorialList = TextBox::splitString(allTutorials, '#');

	for (int i = 0; i < Types::Tutorials::TutorialsEND; i++)
	{
		Types::Tutorials tutorialType = static_cast<Types::Tutorials>(i);
		tutorialMessages[tutorialType] = tutorialList[i];
	}

	// Rank up messages for turnips
	string allRankUps;
	while (true) {
		char x;
		messageFile >> x;
		if (x == '@')
		{
			break;
		}
		if (x != '\n')
		{
			allRankUps += x;
		}
	}

	vector<string> rankUpList = TextBox::splitString(allRankUps, '#');
	for (int i = 0; i < numImportantRanks; i++)
	{
		rankUpNames[importantRanks[i]] = rankUpList.at(i);
	}

	messageFile.close();
}

void Game::saveProgress()
{
	// Saves upgrades
	ofstream upgradeFile;
	upgradeFile.open("Saves/upgrades.txt");

	for (int i = 0; i < Types::Players::PlayersEND; i++)
	{
		Types::Players type = static_cast<Types::Players>(i);
		// 3 paths of 4 upgrades
		for (int j = 0; j < 12; j++)
		{
			if (j % 4 == 3)
			{
				upgradeFile << playerUpgrades[type][j] << endl;
			}
			else
			{
				upgradeFile << playerUpgrades[type][j] << " ";
			}
		}

		upgradeFile << endl;
	}

	upgradeFile.close();
	
	ofstream progressFile;
	progressFile.open("Saves/progress.txt");
	progressFile << money << endl;
	
	for (int i = 0; i < Types::Players::PlayersEND; i++)
	{
		Types::Players playerType = static_cast<Types::Players>(i);
		progressFile << playerRanks[playerType].rank << " ";
		progressFile << playerRanks[playerType].xp << endl;
	}
	
	while (teamSize == 11);
	progressFile << teamSize << " ";

	for (int i = 0; i < 8; i++)
	{
		progressFile << teamStatus[i] << " ";
	}

	progressFile << endl;

	for (int i = 0; i < numLevels; i++)
	{
		LevelScores curLevel = levelScores[i];
		progressFile << curLevel.levelStatus << " ";
		progressFile << curLevel.easyScore << " ";
		progressFile << curLevel.mediumScore << " ";
		progressFile << curLevel.hardScore << " ";
		progressFile << curLevel.impossibleScore << endl;
	}

	for (int i = 0; i < Types::Tutorials::TutorialsEND; i++)
	{
		Types::Tutorials tutorialType = static_cast<Types::Tutorials>(i);
		progressFile << tutorialReadStatus[tutorialType] << " ";
	}

	progressFile.close();

}

// Upgrade Button Stuff
void Game::updateUpgrade(Types::Players player, int ID, int newStatus)
{
	playerUpgrades[player][ID] = newStatus;
	if (ID % 4 != 3)
	{
		playerUpgrades[player][ID + 1] = 1;
	}
	refreshUpgrade = true;
}

int Game::getMoney()
{
	return money;
}

bool Game::spendMoney(int cost)
{
	// If enough money subtracts cost from total money
	// Assumes any function that calls this function will spend
	// the money if allowed
	if (money >= cost)
	{
		money -= cost;
		return true;
	}
	else
	{
		return false;
	}
}

void Game::initializeTextureMaps()
{
	playerNames[Types::Players::Normie] = "Normie Turnip";
	playerNames[Types::Players::Math] = "Math Turnip";
	playerNames[Types::Players::Physics] = "Physics Turnip";
	playerNames[Types::Players::Chemistry] = "Chemistry Turnip";
	playerNames[Types::Players::Biology] = "Biology Turnip";
	playerNames[Types::Players::Geology] = "Geology Turnip";
	playerNames[Types::Players::Engineer] = "Engineer Turnip";
	playerNames[Types::Players::Nature] = "Nature Turnip";
	playerNames[Types::Players::PlayersEND] = "Locked";
}

// End game
void Game::updateHighScore(int level, int newScore, int timeElapsed, int difficulty)
{
	if (level == freeplayTimeLoopLevel || level == freeplayTimeNoLoopLevel)
	{
		newScore = timeElapsed;
	}
	if (difficulty == 1)
	{
		levelScores[level].easyScore = newScore;
	}
	else if (difficulty == 2)
	{
		levelScores[level].mediumScore = newScore;
	}
	else if (difficulty == 3)
	{
		levelScores[level].hardScore = newScore;
	}
	else if (difficulty == 4)
	{
		levelScores[level].impossibleScore = newScore;
	}
}

void Game::updateXp(vector<Player*> players, float factor)
{
	for (int i = 0; i < players.size(); i++)
	{
		Types::Players playerType = players.at(i)->getType();
		playerRanks[playerType].xp += players.at(i)->getXp() * factor;
		while (playerRanks[playerType].xp > 50 + 10 * pow(playerRanks[playerType].rank, 2))
		{
			playerRanks[playerType].xp -= 50 + 10 * pow(playerRanks[playerType].rank, 2);
			playerRanks[playerType].rank++;
			
			// check if important rank
			for (int i = 0; i < numImportantRanks; i++)
			{
				if (importantRanks[i] == playerRanks[playerType].rank)
				{
					string message = rankUpNames[importantRanks[i]];
					MessagePanel* rankUpPanel = new MessagePanel(MessagePanel::OK, scenes.top(), &manager, Vector2f(300, 100));
					rankUpPanel->setMessage(playerNames[playerType] + " is now rank " + to_string(importantRanks[i]) + "!");
					rankUpPanel->skipLine(10);
					rankUpPanel->setMessage(message);
					scenes.top()->addPanel(rankUpPanel);
				}
			}
		}
	}
	
}

void Game::addMoney(int moneyGain)
{
	money += moneyGain;
}

void Game::updateLevelStatus(int level)
{
	levelScores[level].levelStatus++;
	if (level < freeplayNoLoopLevel && levelScores[level + 1].levelStatus == 0)
	{
		levelScores[level + 1].levelStatus = 1;
	}
	if (level == fireballBossLevel && levelScores[freeplayNoLoopLevel].levelStatus == 0)
	{
		levelScores[freeplayNoLoopLevel].levelStatus = 5;
		levelScores[freeplayTimeNoLoopLevel].levelStatus = 5;
	}
	if (level == mrGooseBossLevel && levelScores[freeplayLoopLevel].levelStatus == 0)
	{
		levelScores[freeplayLoopLevel].levelStatus = 5;
		levelScores[freeplayTimeLoopLevel].levelStatus = 5;
	}
	
}

void Game::closeGame()
{
	closeCurrentScene = true;
	refreshLevels = true;
}