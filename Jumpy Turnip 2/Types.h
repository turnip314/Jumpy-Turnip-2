#pragma once

namespace Types
{
	// Each GameObject should have type as part of their constructor
	enum Players {
		Normie, Math, Physics, Chemistry, Biology,
		Geology, Engineer, Nature, PlayersEND
	};
	enum Obstacles {
		Bird, Eagle, Flappy, Fireball, Plane, Cloud, UFO,
		SupplyCrate, FireballBoss, MrGoose, EnemiesEND
	};
	enum Projectiles { Seed, ObstaclesEND };
	enum Buttons { BackButton, AdventureButton, UpgradeMenuButton, TeamButton, CreditButton, LvlButton, UpgradeButton, PrevUpgradesButton,
		NextUpgradesButton, PrevLevelsButton, NextLevelsButton, PlayerButton, TeamSlotButton, PauseButton, ButtonsEND };
	enum PlayerEffects { Weakness, Paralysis, PlayerEffectsEND };
	enum GameModes { Adventure, EndlessLoop, EndlessNoLoop, ApocalypseLoop, ApocalypseNoLoop, GameModesEND };
	enum Objectives { Survive, SurviveGetScore, GetScoreEndless, None, ObjectiveEND };

	enum Confirm { Default, Yes, No, OK, CANCEL, Easy, Normal, Hard, Impossible, HighScores, 
		Resume, Leave, Used};
	enum Entities { Lightning, Coin, Healthpack, EntitiesEND };
	enum Tutorials {Intro1, Intro2, Intro3, Intro4, FireballIntro, LoopIntro, EndlessIntro,
		FlappyIntro, RegenIntro, FadeIntro, FlappyWarning, FireballBossWarning, CloudIntro,
		PlaneIntro, UFOIntro, FireballDodgeWarning, MigratingBirdsWarning, FinalLevelWarning,
		FreeplayLevelIntro, UpgradesIntro, TeamIntro, TeamPlayIntro, AbilityIntro, TutorialsEND};
	enum Sizes{Small, Medium, MediumScaled, Big, BigScaled};
}

struct TimeEffect
{
	float time;
	int effectLevel;
};