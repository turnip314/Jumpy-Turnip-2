#pragma once

#include <stack>

#include "Scene.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Player.h"
#include "Obstacle.h"
#include "Projectile.h"
#include "MenuButton.h"
#include "Stats.h"
#include "MessagePanel.h"
#include "Entity.h"

class Game;

struct Deploy
{
	Obstacle* obstacle;
	float time;
};

class GameScene : public Scene
{
public:
	GameScene(Game* thisGame, TextureManager* manager);
	void initializeGame(int level, int levelStatus, Types::Objectives objective, 
		bool doLoop, int scoreNeeded, int difficulty, int highScore);

	void update(Time dt);
	void render(RenderWindow* handle, Vector2f scale);
	void processInput(Keyboard::Key key, bool isPressed);
	void processInput(Vector2i pos, bool isPressed);
	void deleteObjects();

	void setPlayer(Player*);
	void makeTeam(int size, vector<Player*>);
	void addPlayerProjectile(Projectile*);
	void addObstacle(Obstacle*);
	void addEntity(Entity*);
	void addButton(ButtonObject*);
	void addPanel(MessagePanel*);

	// Adds to deploy list
	void addObstacle(Obstacle* object, float time);

	~GameScene();

	// Make this private later
	Player* player;
	int teamSize;
	vector<Player*> team;
	vector<Projectile*> playerProjectiles;
	vector<Obstacle*> obstacles;
	vector<ButtonObject*> buttons;
	vector<MessagePanel*> panels;
	vector<Entity*> entities;

	void addObstacleToRemove(Obstacle* obstacle);
	void addProjectileToRemove(Projectile* projectile);
	void addPanelToRemove(MessagePanel* panel);
	void addEntityToRemove(Entity* entity);

	void addObstacleToChoose(ObstacleStats stat);
	void addObstacle(ObstacleStats stat);

	void autoGenerateObstacle();

	void addScore(float score);
	void addMoney(int money, Vector2f coinLoc);
	void pushObstacle(Obstacle* obstacle);

	void playerSwap();
	void endGame();
	void setBossKilled();

	bool isLooping();
	
	bool isGameOver();
private:
	vector<Deploy> obstaclesToDeploy;
	vector<GameObject*> objectsToDelete;
	TextureManager* textureManager;
	Texture* backgroundTexture;
	Sprite background;

	float scroll = 0;
	float score;
	float timeElapsed;
	float teamWipe;

	Game* game;
	int level;
	int levelStatus;
	int difficulty;
	Types::Objectives gameObjective;
	bool loop;
	int goalScore;
	bool bossKilled;

	bool autoGenerate;
	short autoGenWeight;
	int autoDifficulty = 0;
	int spawnToNextDifficulty = 5;

	int highScore;
	int moneyGain;

	bool close;
	bool drain;

	// For auto generate rounds
	vector<ObstacleStats> obstacleChooseList;
	int chooseListLength;

	vector<Obstacle*> obstacleRemoveList;
	vector<Projectile*> projectileRemoveList;
	vector<MessagePanel*> panelRemoveList;
	vector<Entity*> entityRemoveList;

	bool enemiesLeft; //non-cloud enemies
	bool gameOver;
	float gameOverTimer;
	// If enemy list empty or player out of health, check game objective

	Text scoreDisplay;
	Text moneyDisplay;
};

