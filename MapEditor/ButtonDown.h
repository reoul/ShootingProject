#pragma once

class ButtonDown
{
public:
	ButtonDown();
	~ButtonDown();
	void KeyDown();
	void KeyCheckUp();
	void KeyInitDown();
	bool KeyCheckClick() const;
private:
	int count;
	bool m_bIsDown;
};
