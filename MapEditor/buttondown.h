#ifndef __buttondown_h_
#define __buttondown_h_

class CButtonDown
{
private:
	int count;
	bool m_bIsDown;
public:
	CButtonDown();
	~CButtonDown();
	void KeyDown();
	void KeyCheckUp();
	void KeyInitDown();
	bool KeyCheckClick();
};
#endif