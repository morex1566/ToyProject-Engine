////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"


InputClass::InputClass()
{
	mInput = 0;
	mDIMouse = 0;
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}


void InputClass::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	int i;
	
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mInput, NULL);

	mInput->CreateDevice(GUID_SysMouse, &mDIMouse, NULL);
	mDIMouse->SetDataFormat(&c_dfDIMouse);
	mDIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	// Initialize all the keys to being released and not pressed.
	for(i=0; i<256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

void InputClass::Shutdown()
{
	if (mDIMouse)
	{
		mDIMouse->Unacquire();
		mDIMouse->Release();
		mDIMouse = nullptr;
	}

	if (mInput)
	{
		mInput->Release();
		mInput = nullptr;
	}
}


void InputClass::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
	return;
}


void InputClass::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
	return;
}


bool InputClass::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}

bool InputClass::Frame()
{

	mDIMouse->Acquire();
	mDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mCurrMousetState);

	rotateYaw = 0.0f;
	rotatePitch = 0.0f;

	if ((mCurrMousetState.lX != mLastMouseState.lX) || (mCurrMousetState.lY != mLastMouseState.lY))
	{
		rotateYaw = mLastMouseState.lX * 0.1f;

		rotatePitch = mCurrMousetState.lY * 0.1f;

		mLastMouseState = mCurrMousetState;
		return true;
	}
	else
	{
		return false;
	}

}

float InputClass::GetRotateYaw()
{
	return rotateYaw;
}

float InputClass::GetRotatePitch()
{
	return rotatePitch;
}