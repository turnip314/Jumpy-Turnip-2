#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <fstream>
#include <iostream>

#include "Game.h"
#include "TextureManager.h"
#include "TextBox.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int cmdShow)
//int main()
{
	TextureManager manager;

	// Birds and obstacles
	manager.load(Textures::Bird1, "Images/birdblue.png");
	manager.load(Textures::Bird2, "Images/birdorange.png");
	manager.load(Textures::Bird3, "Images/birdred.png");
	manager.load(Textures::Eagle, "Images/eagle.png");
	manager.load(Textures::Fireball1, "Images/fireball.png");
	manager.load(Textures::Fireball2, "Images/iceball.png");
	manager.load(Textures::FireballBoss1, "Images/fireballBig.png");
	manager.load(Textures::FireballBoss2, "Images/iceballBig.png");
	manager.load(Textures::Goose, "Images/mrgooseclose.png");
	manager.load(Textures::GooseAttack, "Images/mrgooseopen.png");
	manager.load(Textures::Cloud1, "Images/cloud1.png");
	manager.load(Textures::Cloud2, "Images/cloud2.png");
	manager.load(Textures::Cloud3, "Images/cloud3.png");
	manager.load(Textures::Plane, "Images/plane.png");
	manager.load(Textures::UFO, "Images/ufo.png");
	manager.load(Textures::SupplyCrate, "Images/supplycrate.png");
	manager.load(Textures::Parachute, "Images/parachute.png");
	manager.load(Textures::Flappy1, "Images/flappyyellow.png");
	manager.load(Textures::Flappy2, "Images/flappyred.png");
	manager.load(Textures::Flappy3, "Images/flappyblue.png");
	manager.load(Textures::Goose, "Images/mrgooseclose.png");
	manager.load(Textures::GooseAttack, "Images/mrgooseopen.png");

	manager.load(Textures::Coin, "Images/coin.png");
	manager.load(Textures::Healthpack, "Images/healthpack.png");

	manager.load(Textures::Player, "Images/turnip.png");
	manager.load(Textures::Background1, "Images/bg1plains.png");
	manager.load(Textures::Background2, "Images/bg2city.png");
	manager.load(Textures::Background3, "Images/bg3volcano.png");
	manager.load(Textures::Background4, "Images/bg4winter.png");

	manager.load(Textures::NormieSide, "Images/turnipsidenormie.png");
	manager.load(Textures::MathSide, "Images/turnipsidemath.png");
	manager.load(Textures::PhysSide, "Images/turnipsidephys.png");
	manager.load(Textures::ChemSide, "Images/turnipsidechem.png");
	manager.load(Textures::BioSide, "Images/turnipsidebio.png");
	manager.load(Textures::GeoSide, "Images/turnipsidegeo.png");
	manager.load(Textures::EngSide, "Images/turnipsideeng.png");
	manager.load(Textures::NatureSide, "Images/turnipsidenature.png");
	manager.load(Textures::PhysSideJetPack, "Images/turnipsidephysjetpack.png");

	manager.load(Textures::NormieSmall, "Images/turnipnormie.png");
	manager.load(Textures::MathSmall, "Images/turnipmath.png");
	manager.load(Textures::PhysSmall, "Images/turnipphys.png");
	manager.load(Textures::ChemSmall, "Images/turnipchem.png");
	manager.load(Textures::BioSmall, "Images/turnipbio.png");
	manager.load(Textures::GeoSmall, "Images/turnipgeo.png");
	manager.load(Textures::EngSmall, "Images/turnipeng.png");
	manager.load(Textures::NatureSmall, "Images/turnipnature.png");

	manager.load(Textures::NormieHuge, "Images/hugeturnipnormie.png");
	manager.load(Textures::MathHuge, "Images/hugeturnipmath.png");
	manager.load(Textures::PhysHuge, "Images/hugeturnipphys.png");
	manager.load(Textures::ChemHuge, "Images/hugeturnipchem.png");
	manager.load(Textures::BioHuge, "Images/hugeturnipbio.png");
	manager.load(Textures::GeoHuge, "Images/hugeturnipgeo.png");
	manager.load(Textures::EngHuge, "Images/hugeturnipeng.png");
	manager.load(Textures::NatureHuge, "Images/hugeturnipnature.png");

	manager.load(Textures::NormieBig, "Images/bigturnipnormie.png");
	manager.load(Textures::MathBig, "Images/bigturnipmath.png");
	manager.load(Textures::PhysBig, "Images/bigturnipphys.png");
	manager.load(Textures::ChemBig, "Images/bigturnipchem.png");
	manager.load(Textures::BioBig, "Images/bigturnipbio.png");
	manager.load(Textures::GeoBig, "Images/bigturnipgeo.png");
	manager.load(Textures::EngBig, "Images/bigturnipeng.png");
	manager.load(Textures::NatureBig, "Images/bigturnipnature.png");

	manager.load(Textures::NormieBigS, "Images/Sbigturnipnormie.png");
	manager.load(Textures::MathBigS, "Images/Sbigturnipmath.png");
	manager.load(Textures::PhysBigS, "Images/Sbigturnipphys.png");
	manager.load(Textures::ChemBigS, "Images/Sbigturnipchem.png");
	manager.load(Textures::BioBigS, "Images/Sbigturnipbio.png");
	manager.load(Textures::GeoBigS, "Images/Sbigturnipgeo.png");
	manager.load(Textures::EngBigS, "Images/Sbigturnipeng.png");
	manager.load(Textures::NatureBigS, "Images/Sbigturnipnature.png");

	manager.load(Textures::NormieMed, "Images/medturnipnormie.png");
	manager.load(Textures::MathMed, "Images/medturnipmath.png");
	manager.load(Textures::PhysMed, "Images/medturnipphys.png");
	manager.load(Textures::ChemMed, "Images/medturnipchem.png");
	manager.load(Textures::BioMed, "Images/medturnipbio.png");
	manager.load(Textures::GeoMed, "Images/medturnipgeo.png");
	manager.load(Textures::EngMed, "Images/medturnipeng.png");
	manager.load(Textures::NatureMed, "Images/medturnipnature.png");

	manager.load(Textures::NormieMedS, "Images/Smedturnipnormie.png");
	manager.load(Textures::MathMedS, "Images/Smedturnipmath.png");
	manager.load(Textures::PhysMedS, "Images/Smedturnipphys.png");
	manager.load(Textures::ChemMedS, "Images/Smedturnipchem.png");
	manager.load(Textures::BioMedS, "Images/Smedturnipbio.png");
	manager.load(Textures::GeoMedS, "Images/Smedturnipgeo.png");
	manager.load(Textures::EngMedS, "Images/Smedturnipeng.png");
	manager.load(Textures::NatureMedS, "Images/Smedturnipnature.png");

	manager.load(Textures::UnlockPlayerButton, "Images/unlockPlayerButton.png");
	manager.load(Textures::UnlockSlotButton, "Images/unlockSlotButton.png");
	manager.load(Textures::LockPlayerButton, "Images/lockPlayerButton.png");
	manager.load(Textures::LockSlotButton, "Images/lockSlotButton.png");
	manager.load(Textures::UnlockPlayerButtonS, "Images/unlockPlayerButtonS.png");
	manager.load(Textures::UnlockSlotButtonS, "Images/unlockSlotButtonS.png");
	manager.load(Textures::LockPlayerButtonS, "Images/lockPlayerButtonS.png");
	manager.load(Textures::LockSlotButtonS, "Images/lockSlotButtonS.png");

	manager.load(Textures::SlotLock, "Images/slotLock.png");
	manager.load(Textures::SlotLockS, "Images/slotLockS.png");

	manager.load(Textures::menuButton, "Images/mainMenuButton.png");
	manager.load(Textures::menuButtonS, "Images/mainMenuButtonS.png");

	manager.load(Textures::UpgradeBoughtButton, "Images/upgradeBoughtButton.png");
	manager.load(Textures::UpgradeUnlockedButton, "Images/upgradeAvailableButton.png");
	manager.load(Textures::UpgradeLockedButton, "Images/upgradeLockedButton.png");
	manager.load(Textures::UpgradeBoughtButtonS, "Images/upgradeBoughtButtonS.png");
	manager.load(Textures::UpgradeUnlockedButtonS, "Images/upgradeAvailableButtonS.png");
	manager.load(Textures::UpgradeLockedButtonS, "Images/upgradeLockedButtonS.png");

	manager.load(Textures::BackButton, "Images/backButton.png");
	manager.load(Textures::BackButtonS, "Images/backButtonS.png");

	manager.load(Textures::levelAvailableButton, "Images/levelAvailable.png");
	manager.load(Textures::levelAvailableButtonS, "Images/levelAvailableS.png");
	manager.load(Textures::levelBeatenButton, "Images/levelBeaten.png");
	manager.load(Textures::levelBeatenButtonS, "Images/levelBeatenS.png");
	manager.load(Textures::levelLockedButton, "Images/levelLocked.png");
	manager.load(Textures::levelLockedButtonS, "Images/levelLockedS.png");

	manager.load(Textures::freeplayAvailableButton, "Images/freeplayAvailable.png");
	manager.load(Textures::freeplayLockedButton, "Images/freeplayLocked.png");
	manager.load(Textures::freeplayAvailableButtonS, "Images/freeplayAvailableS.png");
	manager.load(Textures::freeplayLockedButtonS, "Images/freeplayLockedS.png");

	manager.load(Textures::confirmButton1, "Images/confirmButton1.png");
	manager.load(Textures::confirmButton2, "Images/confirmButton2.png");
	manager.load(Textures::confirmButton1S, "Images/confirmButton1S.png");
	manager.load(Textures::confirmButton2S, "Images/confirmButton2S.png");

	manager.load(Textures::Cursor, "Images/turnipcursor.png");
	manager.load(Textures::WreckingBall, "Images/wreckingball.png");
	manager.load(Textures::Raindrop, "Images/raindrop.png");

	manager.load(Textures::CreditPanel, "Images/creditpanel.png");

	manager.load(Fonts::Calibri, "Fonts/Calibri.ttf");
	manager.load(Fonts::Atarian, "Fonts/Atarian/SF Atarian System.ttf");

	manager.load(Textures::MessagePanel, "Images/messagePanel.png");

	Game mainProcess(manager);
	mainProcess.run();
	return 0;
}