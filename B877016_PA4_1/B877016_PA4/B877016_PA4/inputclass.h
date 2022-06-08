////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#include <dinput.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize(HINSTANCE, HWND);
	void Shutdown();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);
	bool Frame();
	
	float GetRotateYaw();
	float GetRotatePitch();

private:
	bool m_keys[256];
	IDirectInput8* mInput;
	IDirectInputDevice8* mDIMouse;
	DIMOUSESTATE mLastMouseState;
	DIMOUSESTATE mCurrMousetState;

	float rotateYaw;
	float rotatePitch;
};

#endif