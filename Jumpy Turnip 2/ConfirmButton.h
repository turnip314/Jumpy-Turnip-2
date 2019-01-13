#pragma once
#include "ButtonObject.h"

class MessagePanel;
class ConfirmButton : public ButtonObject
{
public:
	ConfirmButton(MessagePanel* panel, Types::Confirm thisConfirm, TextureManager* manager, Vector2f topLeftCoord, Vector2f bottomRightCoord);
	~ConfirmButton();

	void doButtonAction();

private:
	Types::Confirm confirm;
	MessagePanel* messagePanel;
};
