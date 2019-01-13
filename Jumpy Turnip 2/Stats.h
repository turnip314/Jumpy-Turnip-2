#pragma once

struct PlayerStats
{
	int health;
	int damage;
	// Regen, defense, other stuff
};

struct BaseObstacleStats
{
	int health;
	int damage;
	int speed;
};

struct ObstacleStats
{
	Types::Obstacles type;
	float time;
	float speedMul;
	float healthMul;
	float damageMul;
	bool fade;
	bool regen;
	int startYPos;
};

struct Levels
{
	Types::Objectives objective;
	vector<ObstacleStats> obstacleList;
	int scoreNeeded;
	bool loops;
};

struct Effects
{
	float time;
	float damage;
};

struct LevelScores
{
	// For each level, 0 means locked, 1 unlocked, 2 beat easy, 3 beat medium,
	// 4 beat hard, 5 beat impossible
	int levelStatus;

	int easyScore;
	int mediumScore;
	int hardScore;
	int impossibleScore;
};

struct PlayerRanks
{
	int rank;
	int xp;
};