////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "Utility.h"
#include "particleclass.h"
#include "particleshaderclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

using namespace std;
using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{

public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(float, float);

	LightClass* GetLight();
	CameraClass* GetCamera();
private:
	bool Render(float = 0, float = 0, float = 0);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ParticleSystemClass* mParticle;
	ID3D11ShaderResourceView* mNoise;
	ID3D11ShaderResourceView* mAlpha;
	ID3D11ShaderResourceView* mFire;

	ParticleShaderClass* mParticleShader;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	LightClass* mPointLight1;
	LightClass* mPointLight2;
	LightClass* mPointLight3;
	LightClass* mPointLight4;
	LightClass* mPointLight5;
	LightClass* mPointLight6;
	LightClass* mPointLight7;
	LightClass* mPointLight8;
	LightClass* mPointLight9;
	LightClass* mPointLight10;

	vector<DataPath> mPaths;

	float frameTime1;
	float particleTime;
	int frameDirection;
};

#endif