#include "MessagePanel.h"
#include <iostream>
#include "Scene.h"



MessagePanel::MessagePanel(int thisType, Scene* thisScene, TextureManager* manager, Vector2f topLeftCoord)
{
	type = thisType;
	scene = thisScene;
	textureManager = manager;
	topLeft = topLeftCoord;

	adjustPosition();
	initializeButtons();

	Texture* backgroundTexture = manager->getTexture(Textures::MessagePanel);
	backgroundImage.setTexture(*backgroundTexture, true);
	backgroundImage.setPosition(topLeftCoord);
}

MessagePanel::MessagePanel(int thisType, ButtonObject* thisButton, Scene* thisScene, TextureManager* manager, Vector2f topLeftCoord)
{
	type = thisType;
	button = thisButton;
	scene = thisScene;
	textureManager = manager;
	topLeft = topLeftCoord;

	Texture* backgroundTexture = manager->getTexture(Textures::MessagePanel);
	backgroundImage.setTexture(*backgroundTexture, true);
	backgroundImage.setPosition(topLeftCoord);

	adjustPosition();
	initializeButtons();
}

MessagePanel::~MessagePanel()
{
}

void MessagePanel::update(Time dt)
{
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->update(dt);
	}
}

void MessagePanel::render(RenderWindow* handle, Vector2f scale)
{
	//backgroundImage.setTexture(*textureManager->getTexture(Textures::MessagePanel), true);
	backgroundImage.setPosition(topLeft);
	handle->draw(backgroundImage);
	// Render itself and draw message
	for (int i = 0; i < textLines.size(); i++)
	{
		handle->draw(textLines.at(i));
	}
	// Render buttons
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->render(handle, scale);
	}

}

void MessagePanel::processInput(Keyboard::Key key, bool isPressed)
{
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->processInput(key, isPressed);
	}
}

void MessagePanel::processInput(Vector2i pos, bool isPressed)
{
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->processInput(pos, isPressed);
	}
}

void MessagePanel::doAction(Types::Confirm confirm)
{
	if (button != nullptr)
		button->confirmButtonAction(confirm);
	
	// For button actions,
	// Call doConfirmedAction on button (when it exists)
	addToRemoveList();
}

void MessagePanel::addToRemoveList()
{
	scene->addPanelToRemove(this);
}

void MessagePanel::adjustPosition()
{
	// Adjust coordinates of everything relative to window based on location of panel
	float x = topLeft.x;
	float y = topLeft.y;

	OKPos.x += x;
	OKPos.y += y;
	YesPos.x += x;
	YesPos.y += y;
	NoPos.x += x;
	NoPos.y += y;
	EasyPos.x += x;
	EasyPos.y += y;
	MedPos.x += x;
	MedPos.y += y;
	HardPos.x += x;
	HardPos.y += y;
	ImpossiblePos.x += x;
	ImpossiblePos.y += y;
	CancelPos.x += x;
	CancelPos.y += y;
	HighScoresPos.x += x;
	HighScoresPos.y += y;
}

void MessagePanel::initializeButtons()
{
	if (type == MessagePanel::OK)
	{
		ConfirmButton* OKButton = new ConfirmButton(this, Types::Confirm::OK, textureManager, OKPos,
			Vector2f(OKPos.x + buttonSize.x, OKPos.y + buttonSize.y));
		OKButton->setText(Fonts::Calibri, 25, "OK");
		OKButton->centerText();
		buttons.push_back(OKButton);
	}
	else if (type == MessagePanel::CANCEL)
	{
		ConfirmButton* CANCELButton = new ConfirmButton(this, Types::Confirm::CANCEL, textureManager, OKPos,
			Vector2f(OKPos.x + buttonSize.x, OKPos.y + buttonSize.y));
		CANCELButton->setText(Fonts::Calibri, 25, "Cancel");
		CANCELButton->centerText();
		buttons.push_back(CANCELButton);
	}
	else if (type == MessagePanel::CONFIRMBUTTON)
	{
		ConfirmButton* yesButton = new ConfirmButton(this, Types::Confirm::Yes, textureManager, YesPos,
			Vector2f(YesPos.x + buttonSize.x, YesPos.y + buttonSize.y));
		ConfirmButton* noButton = new ConfirmButton(this, Types::Confirm::No, textureManager, NoPos,
			Vector2f(NoPos.x + buttonSize.x, NoPos.y + buttonSize.y));
		yesButton->setText(Fonts::Calibri, 28, "Yes");
		yesButton->centerText();
		noButton->setText(Fonts::Calibri, 28, "No");
		noButton->centerText();
		buttons.push_back(yesButton);
		buttons.push_back(noButton);
	}
	else if (type == MessagePanel::EASYMEDHARD)
	{
		ConfirmButton* easyButton = new ConfirmButton(this, Types::Confirm::Easy, textureManager, EasyPos,
			Vector2f(EasyPos.x + buttonSize2.x, EasyPos.y + buttonSize2.y));
		easyButton->setText(Fonts::Calibri, 28, "Easy");
		easyButton->centerText();
		buttons.push_back(easyButton);

		ConfirmButton* medButton = new ConfirmButton(this, Types::Confirm::Normal, textureManager, MedPos,
			Vector2f(MedPos.x + buttonSize2.x, MedPos.y + buttonSize2.y));
		medButton->setText(Fonts::Calibri, 28, "Medium");
		medButton->centerText();
		buttons.push_back(medButton);

		ConfirmButton* hardButton = new ConfirmButton(this, Types::Confirm::Hard, textureManager, HardPos,
			Vector2f(HardPos.x + buttonSize2.x, HardPos.y + buttonSize2.y));
		hardButton->setText(Fonts::Calibri, 28, "Hard");
		hardButton->centerText();
		buttons.push_back(hardButton);

		ConfirmButton* impossibleButton = new ConfirmButton(this, Types::Confirm::Impossible, textureManager, ImpossiblePos,
			Vector2f(ImpossiblePos.x + buttonSize2.x, ImpossiblePos.y + buttonSize2.y));
		impossibleButton->setText(Fonts::Calibri, 28, "Impossible");
		impossibleButton->centerText();
		buttons.push_back(impossibleButton);

		ConfirmButton* cancelButton = new ConfirmButton(this, Types::Confirm::CANCEL, textureManager, CancelPos,
			Vector2f(CancelPos.x + buttonSize2.x, CancelPos.y + buttonSize2.y));
		cancelButton->setText(Fonts::Calibri, 28, "Cancel");
		cancelButton->centerText();
		buttons.push_back(cancelButton);

		ConfirmButton* highScoresButton = new ConfirmButton(this, Types::Confirm::HighScores, textureManager, HighScoresPos,
			Vector2f(HighScoresPos.x + buttonSize2.x, HighScoresPos.y + buttonSize2.y));
		highScoresButton->setText(Fonts::Calibri, 28, "High Score");
		highScoresButton->centerText();
		buttons.push_back(highScoresButton);
	}
	else if (type == MessagePanel::PAUSE)
	{
		ConfirmButton* resumeButton = new ConfirmButton(this, Types::Confirm::Resume, textureManager, YesPos,
			Vector2f(YesPos.x + buttonSize.x, YesPos.y + buttonSize.y));
		ConfirmButton* leaveButton = new ConfirmButton(this, Types::Confirm::Leave, textureManager, NoPos,
			Vector2f(NoPos.x + buttonSize.x, NoPos.y + buttonSize.y));
		resumeButton->setText(Fonts::Calibri, 28, "Resume");
		leaveButton->setText(Fonts::Calibri, 28, "Leave");
		buttons.push_back(resumeButton);
		buttons.push_back(leaveButton);
	}
	else
	{
		cout << "Confirm Button not yet implemeted";
	}
}

void MessagePanel::skipLine(int lineSize)
{
	Text newLine;
	newLine.setCharacterSize(lineSize);
	textLines.push_back(newLine);
}

void MessagePanel::setMessage(string text)
{
	vector<string> lines = TextBox::getTextLines(size, textureManager->getFont(fontID), fontSize, text);

	for (int i = 0; i < lines.size(); i++)
	{
		Text newLine;
		newLine.setFont(*textureManager->getFont(fontID));
		newLine.setCharacterSize(fontSize);
		newLine.setPosition(Vector2f((int) (80 + topLeft.x), (int) (70 + topLeft.y + fontSize * 1.1 * textLines.size())));
		newLine.setString(lines[i]);
		newLine.setFillColor(Color::Black);
		textLines.push_back(newLine);
	}
}