////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightclass.h"


LightClass::LightClass()
{
	mAmbientOnOff = 1.0f;
	mDiffuseOnOff = 1.0f;
	mSpecularOnOff = 1.0f;
}


LightClass::LightClass(const LightClass& other)
{
	
}


LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
	return;
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetSpecularPower(float power)
{
	m_specularPower = power;
	return;
}

void LightClass::SetPosition(float x, float y, float z)
{
	mPosition.x = x;
	mPosition.y = y;
	mPosition.z = z;
	mPosition.w = 1.0f;

	return;
}

XMFLOAT4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}

XMFLOAT3 LightClass::GetDirection()
{
	return m_direction;
}

XMFLOAT4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}

XMFLOAT4 LightClass::GetPosition()
{
	return mPosition;
}

float LightClass::GetSpecularPower()
{
	return m_specularPower;
}

void LightClass::SetAmbientOnOff()
{
	if (mAmbientOnOff == 1.0f)
	{
		mAmbientOnOff = 0.0f;
	}
	else
	{
		mAmbientOnOff = 1.0f;
	}
}

void LightClass::SetDiffuseOnOff()
{
	if (mDiffuseOnOff == 1.0f)
	{
		mDiffuseOnOff = 0.0f;
	}
	else
	{
		mDiffuseOnOff = 1.0f;
	}
}

void LightClass::SetSpecularOnOff()
{
	if (mSpecularOnOff == 1.0f)
	{
		mSpecularOnOff = 0.0f;
	}
	else
	{
		mSpecularOnOff = 1.0f;
	}
}

float LightClass::GetAmbientOnOff()
{
	return mAmbientOnOff;
}

float LightClass::GetDiffuseOnOff()
{
	return mDiffuseOnOff;
}

float LightClass::GetSpecularOnOff()
{
	return mSpecularOnOff;
}