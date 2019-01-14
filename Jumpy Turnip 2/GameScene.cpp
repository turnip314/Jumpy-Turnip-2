#include "GameScene.h"

#include "Player.h"
#include "Projectile.h"
#include "Obstacle.h"
#include "Bird.h"
#include "MenuButton.h"
#include "Flappy.h"
#include "Fireball.h"
#include "Plane.h"
#include "SupplyCrate.h"
#include "Plane.h"
#include "Cloud.h"
#include "UFO.h"
#include "Game.h"
#include "MrGoose.h"
#include "Coin.h"

GameScene::GameScene(Game* thisGame, TextureManager* manager)
{
	game = thisGame;
	textureManager = manager;
	loop = true;
}


GameScene::~GameScene()
{
	while (buttons.size() > 0)
	{
		// ADD destructor of object
		delete buttons.at(0);
		buttons.erase(buttons.begin());
	}
	while (panels.size() > 0)
	{
		// ADD destructor of object
		delete panels.at(0);
		panels.erase(panels.begin());
	}
	while (entities.size() > 0)
	{
		// ADD destructor of object
		delete entities.at(0);
		entities.erase(entities.begin());
	}
	while (team.size() > 0)
	{
		// ADD destructor of object
		delete team.at(0);
		team.erase(team.begin());
	}
	while (obstacles.size() > 0)
	{
		// ADD destructor of object
		delete obstacles.at(0);
		obstacles.erase(obstacles.begin());
	}
	while (playerProjectiles.size() > 0)
	{
		// ADD destructor of object
		delete playerProjectiles.at(0);
		playerProjectiles.erase(playerProjectiles.begin());
	}
}

void GameScene::initializeGame(int thisLevel, int thisLevelStatus, Types::Objectives objective, 
	bool doLoop, int scoreNeeded, int thisDifficulty, int thisHighScore)
{
	highScore = thisHighScore;
	difficulty = thisDifficulty;
	level = thisLevel;
	levelStatus = thisLevelStatus;
	gameObjective = objective;
	loop = doLoop;
	goalScore = scoreNeeded;
	if (objective == Types::Objectives::GetScoreEndless || objective == Types::Objectives::None)
	{
		autoGenerate = true;
	}
	else
	{
		autoGenerate = false;
	}
	scoreDisplay.setPosition(0, 52);
	scoreDisplay.setFont(*textureManager->getFont(Fonts::Calibri));
	scoreDisplay.setCharacterSize(30);

	moneyDisplay.setPosition(0, 84);
	moneyDisplay.setFont(*textureManager->getFont(Fonts::Calibri));
	moneyDisplay.setCharacterSize(30);

	if (difficulty >= 2)
	{
		// each difficulty increase, player starts with less health and damage
		for (Player* player : team)
		{
			player->setHealth(player->getHealth() * 0.75);
			player->setDamage(player->getDamage() * 0.75);
		}
		if (difficulty >= 3)
		{
			for (Player* player : team)
			{
				player->setHealth(player->getHealth() * 0.75);
				player->setDamage(player->getDamage() * 0.75);
			}
		}
		if (difficulty == 4)
		{
			// at impossible difficulty, player's health is drained over time
			for (Player* player : team)
			{
				player->setHealth(player->getHealth() * 0.75);
				player->setDamage(player->getDamage() * 0.75);
			}
			drain = true;
		}
	}

	player->resetInitHealth();

	if (level < 10)
	{
		background.setTexture(*textureManager->getTexture(Textures::Background1));
	}
	else if (level < 20)
	{
		background.setTexture(*textureManager->getTexture(Textures::Background3));
	}
	else if (level < 30)
	{
		background.setTexture(*textureManager->getTexture(Textures::Background4));
	}
	else
	{
		float randNum = Math::random();
		if (randNum < 0.25f)
		{
			background.setTexture(*textureManager->getTexture(Textures::Background1));
		}
		else if (randNum < 0.5f)
		{
			background.setTexture(*textureManager->getTexture(Textures::Background3));
		}
		else if (randNum < 0.75f)
		{
			background.setTexture(*textureManager->getTexture(Textures::Background4));
		}
		else
		{
			background.setTexture(*textureManager->getTexture(Textures::Background2));
		}
	}
}

void GameScene::update(Time dt)
{
	float time = dt.asSeconds();

	if (panels.size() > 0)
	{
		for (unsigned i = 0; i < panels.size(); i++)
		{
			panels[i]->update(dt);
		}
		return;
	}
	else if (close)
	{
		game->closeGame();
		return;
	}

	// handles score
	if (gameObjective == Types::Objectives::GetScoreEndless || gameObjective == Types::Objectives::None)
		addScore(3.f * time);

	timeElapsed += time;
	if (level == game->freeplayTimeLoopLevel || level == game->freeplayTimeNoLoopLevel)
	{
		player->gainXp(6 * time);
	}

	// Adds item from deploy list into game if time = 0
	if (obstaclesToDeploy.size() == 0)
	{

	}
	else if (obstaclesToDeploy.at(0).time > 0)
	{
		obstaclesToDeploy.at(0).time -= time;
	}
	else
	{
		addObstacle(obstaclesToDeploy.at(0).obstacle);
		obstaclesToDeploy.erase(obstaclesToDeploy.begin());
	}

	// Changes position of scrolling background
	scroll = fmod(scroll - 200.f*time, 2560);

	// Updates all GameObjects
	player->update(dt);
	for (unsigned i = 0; i < playerProjectiles.size(); i++) {
		playerProjectiles[i]->update(dt);
	}

	enemiesLeft = false;
	for (unsigned i = 0; i < obstacles.size(); i++) {
		obstacles[i]->update(dt);
		if (obstacles[i]->getType() != Types::Obstacles::Cloud)
		{
			enemiesLeft = true;
		}
	}

	for (unsigned i = 0; i < buttons.size(); i++) {
		buttons[i]->update(dt);
	}
	for (unsigned i = 0; i < entities.size(); i++) {
		entities[i]->update(dt);
	}
	for (unsigned i = 0; i < panels.size(); i++) {
		panels[i]->update(dt);
	}

	// Does auto-generation of obstacles for levels where it's needed
	if (autoGenerate)
	{
		autoGenerateObstacle();
	}

	// drain player's health for impossible difficulty
	if (drain)
	{
		player->drain(dt);
	}

	// Check game end condition
	if (gameOverTimer > 0)
	{
		gameOverTimer -= time;
	}
	else if (gameOver)
	{
		endGame();
	}
	else if (obstaclesToDeploy.size() == 0 && (obstacles.size() == 0 || !enemiesLeft))
	{
		gameOver = true;
		gameOverTimer = 4;
	}
}

void GameScene::render(RenderWindow* handle, Vector2f scale)
{
	background.setPosition(Vector2f(scroll, 0));
	handle->draw(background);

	for (unsigned i = 0; i < entities.size(); i++) {
		entities[i]->render(handle, scale);
	}
	for (unsigned i = 0; i < playerProjectiles.size(); i++) {
		playerProjectiles[i]->render(handle, scale);
	}
	for (unsigned i = 0; i < obstacles.size(); i++) {
		obstacles[i]->render(handle, scale);
	}
	player->render(handle, scale);
	for (unsigned i = 0; i < teamSize; i++)
	{
		team[i]->renderFrontView(handle, scale, Vector2f(50.f + 60 * i, 650.f));
	}
	for (unsigned i = 0; i < buttons.size(); i++) {
		buttons[i]->render(handle, scale);
	}
	for (unsigned i = 0; i < panels.size(); i++) {
		panels[i]->render(handle, scale);
	}

	if (level == game->freeplayTimeLoopLevel || level == game->freeplayTimeNoLoopLevel)
	{
		scoreDisplay.setString("Time: " + to_string((int)timeElapsed) + " s");
	}
	else
	{
		scoreDisplay.setString("Score: " + to_string((int)score));
	}
	
	handle->draw(scoreDisplay);
	moneyDisplay.setString("Money: " + to_string(moneyGain));
	handle->draw(moneyDisplay);
}

void GameScene::processInput(Keyboard::Key key, bool isPressed)
{
	if (!isPressed)
	{
		return;
	}

	if (panels.size() > 0)
	{
		for (unsigned i = 0; i < panels.size(); i++)
		{
			panels[i]->processInput(key, isPressed);
		}
		return;
	}

	if (key == Keyboard::Num1)
	{
		if (team.at(0)->isAlive())
		{
			player->doSwitchDamage();
			player = team.at(0);
			player->doSwitchDamage();
			player->resetMovement();
		}
	}
	else if (key == Keyboard::Num2)
	{
		if (teamSize >= 2 && team.at(1)->isAlive())
		{
			player->doSwitchDamage();
			player = team.at(1);
			player->doSwitchDamage();
			player->resetMovement();
		}
	}
	else if (key == Keyboard::Num3)
	{
		if (teamSize >= 3 && team.at(2)->isAlive())
		{
			player->doSwitchDamage();
			player = team.at(2);
			player->doSwitchDamage();
			player->resetMovement();
		}
	}
	else if (key == Keyboard::Num4)
	{
		if (teamSize >= 4 && team.at(3)->isAlive())
		{
			player->doSwitchDamage();
			player = team.at(3);
			player->doSwitchDamage();
			player->resetMovement();
		}
	}
	else if (key == Keyboard::Num5)
	{
		if (teamSize >= 5 && team.at(4)->isAlive())
		{
			player->doSwitchDamage();
			player = team.at(4);
			player->doSwitchDamage();
			player->resetMovement();
		}
	}
	else if (key == Keyboard::Num6)
	{
		if (teamSize >= 6 && team.at(5)->isAlive())
		{
			player->doSwitchDamage();
			player = team.at(5);
			player->doSwitchDamage();
			player->resetMovement();
		}
	}
	else if (key == Keyboard::Num7)
	{
		if (teamSize >= 7 && team.at(6)->isAlive())
		{
			player->doSwitchDamage();
			player = team.at(6);
			player->doSwitchDamage();
			player->resetMovement();
		}
	}
	else if (key == Keyboard::Num8)
	{
		if (teamSize >= 8 && team.at(7)->isAlive())
		{
			player->doSwitchDamage();
			player = team.at(7);
			player->doSwitchDamage();
			player->resetMovement();
		}
	}

	player->processInput(key, isPressed);
	for (unsigned i = 0; i < playerProjectiles.size(); i++) {
		playerProjectiles[i]->processInput(key, isPressed);
	}
	// These are probably never needed, but I'll leave them in here
	// For now just in case
	for (unsigned i = 0; i < obstacles.size(); i++) {
		obstacles[i]->processInput(key, isPressed);
	}
	for (unsigned i = 0; i < buttons.size(); i++) {
		buttons[i]->processInput(key, isPressed);
	}
	for (unsigned i = 0; i < entities.size(); i++) {
		entities[i]->processInput(key, isPressed);
	}
	for (unsigned i = 0; i < panels.size(); i++) {
		panels[i]->processInput(key, isPressed);
	}
}

void GameScene::processInput(Vector2i pos, bool isPressed)
{
	if (panels.size() > 0)
	{
		panels[panels.size() - 1]->processInput(pos, isPressed);
		return;
	}

	player->processInput(pos, isPressed);
	for (unsigned i = 0; i < playerProjectiles.size(); i++) {
		playerProjectiles[i]->processInput(pos, isPressed);
	}
	for (unsigned i = 0; i < buttons.size(); i++) {
		buttons[i]->processInput(pos, isPressed);
	}
	// These are probably never needed, but I'll leave them in here
	// For now just in case
	for (unsigned i = 0; i < obstacles.size(); i++) {
		obstacles[i]->processInput(pos, isPressed);
	}
	for (unsigned i = 0; i < entities.size(); i++) {
		entities[i]->processInput(pos, isPressed);
	}
	for (unsigned i = 0; i <panels.size(); i++) {
		panels[i]->processInput(pos, isPressed);
	}
}

void GameScene::deleteObjects()
{
	while (obstacleRemoveList.size() > 0)
	{
		auto it = find(obstacles.begin(), obstacles.end(), obstacleRemoveList.at(0));
		if (it != obstacles.end())
		{
			obstacles.erase(it);
			delete obstacleRemoveList.at(0);
			obstacleRemoveList.erase(obstacleRemoveList.begin());
		}
		else
		{
			throw runtime_error("Obstacle does not exist");
		}
	}
	while (projectileRemoveList.size() > 0)
	{
		auto it = find(playerProjectiles.begin(), playerProjectiles.end(), projectileRemoveList.at(0));
		if (it != playerProjectiles.end())
		{
			playerProjectiles.erase(it);
			delete projectileRemoveList.at(0);
			projectileRemoveList.erase(projectileRemoveList.begin());
		}
		else
		{
			throw runtime_error("Projectile does not exist");
		}
	}
	
	// Only one panel will be displayed at a time anyways
	while (panelRemoveList.size() > 0)
	{
		auto it = find(panels.begin(), panels.end(), panelRemoveList.at(0));
		if (it != panels.end())
		{
			panels.erase(it);
			delete panelRemoveList.at(0);
			panelRemoveList.erase(panelRemoveList.begin());
		}
		else
		{
			throw runtime_error("Panel does not exist");
		}
	}

	while (entityRemoveList.size() > 0)
	{
		auto it = find(entities.begin(), entities.end(), entityRemoveList.at(0));
		if (it != entities.end())
		{
			entities.erase(it);
			delete entityRemoveList.at(0);
			entityRemoveList.erase(entityRemoveList.begin());
		}
		else
		{
			throw runtime_error("Entity does not exist");
		}
	}
}

// Replaces current player with another player, useful for 
// Switching between players mid game
// Initial player is initialized in constructor
void GameScene::setPlayer(Player* newPlayer)
{
	player = newPlayer;
}

void GameScene::makeTeam(int size, vector<Player*> members)
{
	for (int i = 0; i < size; i++)
	{
		team.push_back(members.at(i));
	}
	teamSize = size;
}

// The following methods are pretty straightforward
// They just add some element to the vector
void GameScene::addButton(ButtonObject* button)
{
	buttons.push_back(button);
}

void GameScene::addPanel(MessagePanel* panel)
{
	panels.insert(panels.begin(), panel);
}

void GameScene::addPlayerProjectile(Projectile* projectile)
{
	playerProjectiles.push_back(projectile);
}

void GameScene::addObstacle(Obstacle* obstacle)
{
	// Adds an obstacle directly into obstacles list
	obstacles.push_back(obstacle);
}

void GameScene::pushObstacle(Obstacle* obstacle)
{
	obstacles.insert(obstacles.begin(), obstacle);
}

void GameScene::addObstacle(Obstacle* obstacle, float time)
{
	// Creates a deploy pair of already created obstacle and
	// time to deploy into obstaclesToDeploy
	Deploy newObstacle;
	newObstacle.obstacle = obstacle;
	newObstacle.time = time;
	obstaclesToDeploy.push_back(newObstacle);
}

void GameScene::addObstacle(ObstacleStats stat)
{
	// Determines what object to create based on type
	// Checks each stat and makes respective object
	// Size stat is done in the constructor
	Types::Obstacles type = stat.type;

	stat.speedMul *= 1.f +  1.f*level / 40;
	stat.damageMul *= 1.f + 1.f*level / 100;
	stat.healthMul *= 1.f + 1.f*level / 35;
	stat.time /= 1.f + 1.f*level / 40;

	Obstacle* obstacle;
	
	if (type == Types::Obstacles::Bird)
	{
		float randSprite = Math::random();
		if (randSprite < 0.33f)
		{
			obstacle = new Bird(Types::Obstacles::Bird, textureManager->getTexture(Textures::Bird1), this);
		}
		else if (randSprite < 0.67f)
		{
			obstacle = new Bird(Types::Obstacles::Bird, textureManager->getTexture(Textures::Bird2), this);
		}
		else
		{
			obstacle = new Bird(Types::Obstacles::Bird, textureManager->getTexture(Textures::Bird3), this);
		}
	}
	else if (type == Types::Obstacles::Eagle)
	{
		obstacle = new Bird(Types::Obstacles::Eagle, textureManager->getTexture(Textures::Eagle), this);
	}
	else if (type == Types::Obstacles::Flappy)
	{
		float randSprite = Math::random();
		if (randSprite < 0.33f)
		{
			obstacle = new Flappy(Types::Obstacles::Flappy, textureManager->getTexture(Textures::Flappy1), this);
		}
		else if (randSprite < 0.67f)
		{
			obstacle = new Flappy(Types::Obstacles::Flappy, textureManager->getTexture(Textures::Flappy2), this);
		}
		else
		{
			obstacle = new Flappy(Types::Obstacles::Flappy, textureManager->getTexture(Textures::Flappy3), this);
		}
	}
	else if (type == Types::Obstacles::Fireball)
	{
		if (Math::random() < 0.5f)
		{
			obstacle = new Fireball(Types::Obstacles::Fireball, 
				textureManager->getTexture(Textures::Fireball1), 
				textureManager->getTexture(Textures::Fireball1),
				this);
		}
		else
		{
			obstacle = new Fireball(Types::Obstacles::Fireball, 
				textureManager->getTexture(Textures::Fireball2), 
				textureManager->getTexture(Textures::Fireball2),
				this);
		}
	}
	else if (type == Types::Obstacles::Plane)
	{
		obstacle = new Plane(Types::Obstacles::Plane, textureManager->getTexture(Textures::Plane), this);
	}
	else if (type == Types::Obstacles::Cloud)
	{
		float randSprite = Math::random();
		if (randSprite < 0.33f)
		{
			obstacle = new Cloud(Types::Obstacles::Cloud, textureManager->getTexture(Textures::Cloud1), this);
		}
		else if (randSprite < 0.67f)
		{
			obstacle = new Cloud(Types::Obstacles::Cloud, textureManager->getTexture(Textures::Cloud2), this);
		}
		else
		{
			obstacle = new Cloud(Types::Obstacles::Cloud, textureManager->getTexture(Textures::Cloud3), this);
		}
	}
	else if (type == Types::Obstacles::UFO)
	{
		obstacle = new UFO(Types::Obstacles::UFO, textureManager->getTexture(Textures::UFO), this);
	}
	else if (type == Types::Obstacles::SupplyCrate)
	{
		obstacle = new SupplyCrate(Types::Obstacles::SupplyCrate, textureManager->getTexture(Textures::SupplyCrate),
			this, textureManager->getTexture(Textures::Healthpack));
	}
	else if (type == Types::Obstacles::FireballBoss)
	{
		if (Math::random() < 0.5f)
		{
			obstacle = new Fireball(Types::Obstacles::FireballBoss, 
				textureManager->getTexture(Textures::FireballBoss1), 
				textureManager->getTexture(Textures::Fireball1),
				this);
		}
		else
		{
			obstacle = new Fireball(Types::Obstacles::FireballBoss,
				textureManager->getTexture(Textures::FireballBoss2),
				textureManager->getTexture(Textures::Fireball2),
				this);
		}
	}
	else if (type == Types::Obstacles::MrGoose)
	{
		obstacle = new MrGoose(Types::Obstacles::MrGoose, 
			textureManager->getTexture(Textures::Goose),
			textureManager->getTexture(Textures::GooseAttack),
			textureManager->getTexture(Textures::Fireball1), 
			textureManager->getTexture(Textures::Fireball2), this);
	}
	else
	{
		throw runtime_error("Enemy not in list");
	}
	obstacle->setStats(stat.speedMul, stat.healthMul, stat.damageMul, stat.fade, stat.regen);

	if (type == Types::Obstacles::SupplyCrate)
	{
		obstacle->setPosition(Vector2f(300 + 900*Math::random(), -40));
	}
	else
	{
		if (stat.startYPos == -1)
		{
			obstacle->setPosition(Vector2f(1500, 30 + 660 * Math::random()));
		}
		else
		{
			obstacle->setPosition(Vector2f(1500, 20 + 20 * stat.startYPos));
		}
	}
	
	addObstacle(obstacle, stat.time);
}

void GameScene::addEntity(Entity* entity)
{
	entities.push_back(entity);
}

void GameScene::addObstacleToRemove(Obstacle* obstacle)
{
	obstacleRemoveList.push_back(obstacle);
}

void GameScene::addProjectileToRemove(Projectile* projectile)
{
	projectileRemoveList.push_back(projectile);
}

void GameScene::addPanelToRemove(MessagePanel* panel)
{
	panelRemoveList.push_back(panel);
}

void GameScene::addEntityToRemove(Entity* entity)
{
	entityRemoveList.push_back(entity);
}

void GameScene::addObstacleToChoose(ObstacleStats stat)
{
	obstacleChooseList.push_back(stat);
	chooseListLength++;
	autoGenWeight += chooseListLength;
}

void GameScene::autoGenerateObstacle()
{
	// For rounds where obstacles are generated as the game progresses
	// Note: obstacle stats change based on number of obstacles generated
	// Note: The probability of the kth obstacle in choose list being deployed is 2k / n(n+1), where
	// n is the size of the list
	if (obstaclesToDeploy.size() < 5)
	{
		short chooseNum = rand() % autoGenWeight;
		short chooseWeight = chooseListLength;
		short chooseLocation = 0;

		while (chooseNum >= chooseWeight)
		{
			chooseLocation++;
			chooseNum -= chooseWeight;
			chooseWeight--;
		}

		ObstacleStats stat = obstacleChooseList.at(chooseLocation);

		ObstacleStats newStat;

		float x = 0.5f;
		float boost = 0.f;
		if (autoDifficulty < 50)
		{
			boost += 3.f * x * autoDifficulty;
		}
		else if (autoDifficulty < 100 )
		{
			boost += 2.f * x * (autoDifficulty - 50) + 75.f;
		}
		else
		{
			boost += x * (autoDifficulty - 100) + 50.f + 75.f;
		}
		
		newStat.damageMul = stat.damageMul*(1 + boost / 100.f);
		newStat.healthMul = stat.healthMul*(1 + boost / 100.f);
		newStat.speedMul = stat.speedMul*(1 + boost / 150.f);
		newStat.time = stat.time*(max(0.15f, 1.f - boost / 200.f));
		newStat.startYPos = stat.startYPos;
		newStat.fade = stat.fade;
		newStat.regen = stat.regen;
		newStat.type = stat.type;

		if (gameObjective == Types::Objectives::None)
		{
			if (Math::random() < 0.2)
			{
				newStat.fade = true;
			}
			if (Math::random() < 0.2)
			{
				newStat.regen = true;
			}
		}

		addObstacle(newStat);

		spawnToNextDifficulty -= 1;
		if (spawnToNextDifficulty == 0)
		{
			autoDifficulty++;
			spawnToNextDifficulty = 5 + autoDifficulty;
		}
	}
}

void GameScene::addScore(float scoreChange)
{
	if (level == game->freeplayTimeLoopLevel || level == game->freeplayTimeNoLoopLevel)
	{
		score = timeElapsed;
		return;
	}
	score += scoreChange;
	player->gainXp(scoreChange);
}

void GameScene::addMoney(int money, Vector2f coinLoc)
{
	// Enemy and supply crate drops, add money to total money earned in level
	moneyGain += money;

	Coin* coin = new Coin(Types::Entities::Coin, textureManager->getTexture(Textures::Coin), this);
	coin->setPosition(coinLoc);
	addEntity(coin);
}

bool GameScene::isLooping()
{
	return loop;
}

void GameScene::playerSwap()
{
	// For the case that the current player dies
	bool found = false;
	player->doSwitchDamage();
	for (int i = 0; i < team.size(); i++)
	{
		Player* newPlayer = team.at(i);
		if (newPlayer->isAlive())
		{
			player = newPlayer;
			found = true;
			player->resetMovement();
			player->doSwitchDamage();
			break;
		}
	}
	if (!found)
	{
		gameOver = true;
		teamWipe = true;
		gameOverTimer = 3;
	}
}

void GameScene::setBossKilled()
{
	bossKilled = true;
	gameOver = true;
	gameOverTimer = 3.f;
}

void GameScene::endGame()
{
	bool win = false;
	
	MessagePanel* endPanel = new MessagePanel(MessagePanel::OK, this, textureManager, Vector2f(300, 100));

	
	string difficultyName = " ";
	if (difficulty == 1)
	{
		difficultyName = "easy";
	}
	else if (difficulty == 2)
	{
		difficultyName = "medium";
	}
	else if (difficulty == 3)
	{
		difficultyName = "hard";
	}
	else if (difficulty == 4)
	{
		difficultyName = "impossible";
	}

	// Checks win condition
	if (level == game->mrGooseBossLevel)
	{
		if (!bossKilled)
		{
			endPanel->setMessage("Game over. You did not defeat the boss in time.");
		}
		else
		{
			win = true;
		}
	}
	else if (!gameOver)
	{
		endPanel->setMessage("Game Over. You left your turnips behind.");
	}
	else if (gameObjective == Types::Objectives::Survive)
	{
		if (!teamWipe)
		{
			win = true;
		}
		else
		{
			endPanel->setMessage("Game over. All your turnips died.");
		}
	}
	else if (gameObjective == Types::Objectives::SurviveGetScore)
	{
		if (!teamWipe && score >= goalScore)
		{
			win = true;
		}
		else if (teamWipe)
		{
			endPanel->setMessage("Game over. All your turnips died.");
		}
		else
		{
			endPanel->setMessage("Game over. You did not obtain the required score for this level.");
		}
	}
	else if (gameObjective == Types::Objectives::GetScoreEndless)
	{
		if (!bossKilled && level == 29)
		{
			win = false;
			endPanel->setMessage("Game over. You did not defeat the boss.");
		}
		else if (score >= goalScore)
		{
			win = true;
		}
		else
		{
			endPanel->setMessage("Game over. You did obtain the required score for this level.");
		}
	}
	else if (gameObjective == Types::Objectives::None)
	{
		if (level == game->freeplayTimeLoopLevel || level == game->freeplayTimeNoLoopLevel)
		{
			endPanel->setMessage("Game over. Your final score was " + to_string((int)timeElapsed) + ".");
		}
		else
		{
			endPanel->setMessage("Game over. Your final score was " + to_string((int)score) + ".");
		}
	}

	if (win)
	{
		endPanel->setMessage("Congratulations! You beat level " + to_string(level + 1) +
			" on " + difficultyName + " difficulty!");
	}

	if (win || gameObjective == Types::Objectives::None)
	{
		// check high score
		if ((score > highScore) ||
			((level == game->freeplayTimeLoopLevel || level == game->freeplayTimeNoLoopLevel) &&
				timeElapsed > highScore))
		{
			endPanel->skipLine(10);
			endPanel->setMessage("You got a new high score!");
			game->updateHighScore(level, score, timeElapsed, difficulty);
		}
	}

	// Check if this is first time beating on this difficulty
	if ((win || gameObjective == Types::Objectives::None) && levelStatus == difficulty)
	{
		game->updateLevelStatus(level);
	}
	
	// Bonus money for beating a round for the first time
	if (win && levelStatus == difficulty)
	{
		int bonusMoney = 50 + 50 * ((level + 10) / 10) * difficulty;
		game->addMoney(bonusMoney);
		endPanel->setMessage("You earned $" + to_string(bonusMoney) + " for beating the level");
	}

	// Update xp
	float xpFactor = 1;
	if (gameObjective == Types::Objectives::None)
	{
		xpFactor *= 0.75;
	}
	else
	{
		if (difficulty != levelStatus)
		{
			xpFactor *= 0.5;
		}
		if (!win)
		{
			xpFactor *= 0.25;
		}
	}
	game->updateXp(team, xpFactor);
	game->addMoney(moneyGain);

	addPanel(endPanel);

	close = true;

	game->saveProgress();
}

bool GameScene::isGameOver()
{
	return gameOver;
}