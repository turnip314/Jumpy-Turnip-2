#pragma once
#include "ButtonObject.h"
#include "Stats.h"

class LevelButton : public ButtonObject
{
public:
	LevelButton(int level, LevelScores thisLevel, Game* thisGame, TextureManager* manager, Vector2f topLeftCoord, Vector2f bottomRightCoord);
	~LevelButton();

	const int easy = 0;

	void doButtonAction();
	void confirmButtonAction(Types::Confirm confirm);
	void render(RenderWindow* handle, Vector2f scale);

private:
	LevelScores levelStat;
	int level;
	int difficulty;
};

