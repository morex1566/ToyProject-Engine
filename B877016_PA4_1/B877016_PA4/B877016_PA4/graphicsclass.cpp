////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include "DDSTextureLoader.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_Light = 0;
	mParticle = 0;
	mParticleShader = 0;
	mPointLight1 = 0;
	mPointLight2 = 0;
	mPointLight3 = 0;
	mPointLight4 = 0;
	mPointLight5 = 0;
	mPointLight6 = 0;
	mPointLight7 = 0;
	mPointLight8 = 0;
	mPointLight9 = 0;
	mPointLight10 = 0;

	mNoise = 0;
	mAlpha = 0;
	mFire = 0;
	frameTime1 = 0.0f;
	frameDirection = 1;
	particleTime = 0.0f;
	
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	HRESULT hresult;

	// Read data.

	// house
	mPaths.push_back({ L"./data/Wall.obj" , L"./data/wall_d.dds" });
	mPaths.push_back({ L"./data/Floor.obj" , L"./data/roof_d.dds" });
	mPaths.push_back({ L"./data/Roof.obj" , L"./data/floor_d.dds" });

	// door
	mPaths.push_back({ L"./data/DoorFrame.obj" , L"./data/doorframe_d.dds" });
	mPaths.push_back({ L"./data/Door.obj" , L"./data/door_d.dds" });

	// bed
	mPaths.push_back({ L"./data/Bed.obj" , L"./data/bed_d.dds" });
	mPaths.push_back({ L"./data/Pillow1.obj" , L"./data/blanket_d.dds" });
	mPaths.push_back({ L"./data/Pillow2.obj" , L"./data/blanket_d.dds" });
	mPaths.push_back({ L"./data/Blanket.obj" , L"./data/blanket_d.dds" });

	// fireplace
	mPaths.push_back({ L"./data/Fireplace.obj" , L"./data/fireplace_d.dds" });

	// lamp
	mPaths.push_back({ L"./data/Lamp.obj" , L"./data/lamp_d.dds" });

	// bookshelf
	mPaths.push_back({ L"./data/Bookshelf.obj" , L"./data/bookshelf_d.dds" });
	mPaths.push_back({ L"./data/Booktable.obj" , L"./data/booktable_d.dds" });

	// table
	mPaths.push_back({ L"./data/Table.obj" , L"./data/table_d.dds" });

	// candels
	mPaths.push_back({ L"./data/Candels.obj" , L"./data/candels_d.dds" });
	mPaths.push_back({ L"./data/StickCandels.obj" , L"./data/stickcandels_d.dds" });

	// stool
	mPaths.push_back({ L"./data/Stool.obj" , L"./data/stool_d.dds" });

	// window
	mPaths.push_back({ L"./data/Window.obj" , L"./data/window_d.dds" });

	// books
	mPaths.push_back({ L"./data/Books.obj" , L"./data/books_d.dds" });
	mPaths.push_back({ L"./data/Summonbook.obj" , L"./data/Summonbook_d.dds" });

	// scroll
	mPaths.push_back({ L"./data/Scroll.obj" , L"./data/scroll_d.dds" });

	// tube
	mPaths.push_back({ L"./data/Irontube.obj" , L"./data/irontube_d.dds" });
	mPaths.push_back({ L"./data/Woodtube.obj" , L"./data/woodtube_d.dds" });

	// staff
	mPaths.push_back({ L"./data/Staff.obj" , L"./data/staff_d.dds" });

	// eye
	mPaths.push_back({ L"./data/Eye.obj" , L"./data/eye_d.dds" });

	// fire
	mPaths.push_back({ L"./data/Fire.obj" , L"./data/fire_d.dds" });


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(4.0f, 2.1f, 2.0f);	// for cube
	
		
	// Create the model object.
	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}

	// Initialize the model object.

	result = m_Model->Initialize(m_D3D->GetDevice(), mPaths);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// direction light
	m_Light->SetAmbientColor(0.01f, 0.01f, 0.01f, 1.0f);
	m_Light->SetDiffuseColor(0.005f, 0.005f, 0.005f, 1.0f);
	m_Light->SetDirection(0.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(360.0f);
	
	// multipoint light
	mPointLight1 = new LightClass;
	mPointLight1->SetDiffuseColor(1.0f, 0.615f, 0.215f, 1.0f);
	mPointLight1->SetPosition(-0.24f, 1.5f, 3.0f);

	mPointLight2 = new LightClass;
	mPointLight2->SetDiffuseColor(0.878f, 0.615f, 0.215f, 1.0f);
	mPointLight2->SetPosition(2.48f, 1.93f, 1.59f);

	mPointLight3 = new LightClass;
	mPointLight3->SetDiffuseColor(0.878f, 0.615f, 0.215f, 1.0f);
	mPointLight3->SetPosition(-0.19f, 2.18f, 3.65f);

	mPointLight4 = new LightClass;
	mPointLight4->SetDiffuseColor(0.878f, 0.615f, 0.215f, 1.0f);
	mPointLight4->SetPosition(1.32f, 1.4f, 4.13f);

	mPointLight5 = new LightClass;
	mPointLight5->SetDiffuseColor(0.878f, 0.615f, 0.215f, 1.0f);
	mPointLight5->SetPosition(0.38f, 1.27f, 2.22f);

	mPointLight6 = new LightClass;
	mPointLight6->SetDiffuseColor(0.878f, 0.615f, 0.215f, 1.0f);
	mPointLight6->SetPosition(4.98f, 1.39f, 2.25f);

	mPointLight7 = new LightClass;
	mPointLight7->SetDiffuseColor(0.878f, 0.615f, 0.215f, 1.0f);
	mPointLight7->SetPosition(5.00f, 1.84f, 4.70f);

	mPointLight8 = new LightClass;
	mPointLight8->SetDiffuseColor(0.878f, 0.615f, 0.215f, 1.0f);
	mPointLight8->SetPosition(2.97f, 2.00f, 1.47f);

	mPointLight9 = new LightClass;
	mPointLight9->SetDiffuseColor(0.878f, 0.615f, 0.215f, 1.0f);
	mPointLight9->SetPosition(-0.22f, 2.23f, 2.39f);

	mPointLight10 = new LightClass;
	mPointLight10->SetDiffuseColor(0.878f, 0.615f, 0.215f, 1.0f);
	mPointLight10->SetPosition(0.81f, 2.21f, 3.96f);

	

	// fire
	hresult = CreateDDSTextureFromFile(m_D3D->GetDevice(), L"./data/fire_d.dds", nullptr, &mFire);
	if (FAILED(hresult))
	{
		MessageBox(hwnd, L"Could not initialize the fire texture object.", L"Error", MB_OK);
		return false;
	}

	hresult = CreateDDSTextureFromFile(m_D3D->GetDevice(), L"./data/noise_d.dds", nullptr, &mNoise);
	if (FAILED(hresult))
	{
		MessageBox(hwnd, L"Could not initialize the fire texture object.", L"Error", MB_OK);
		return false;
	}

	hresult = CreateDDSTextureFromFile(m_D3D->GetDevice(), L"./data/alpha_d.dds", nullptr, &mAlpha);
	if (FAILED(hresult))
	{
		MessageBox(hwnd, L"Could not initialize the fire texture object.", L"Error", MB_OK);
		return false;
	}

	// particle
	mParticleShader = new ParticleShaderClass;
	if (!mParticleShader)
	{
		return false;
	}

	// 파티클 셰이더 객체를 초기화합니다.
	result = mParticleShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}

	mParticle = new ParticleSystemClass;
	if (!mParticle)
	{
		return false;
	}

	// 파티클 시스템 객체를 초기화합니다.
	result = mParticle->Initialize(m_D3D->GetDevice(), L"./data/star_d.dds");
	if (!result)
	{
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if (mPointLight1)
	{
		delete mPointLight1;
		mPointLight1 = 0;
	}

	if (mPointLight2)
	{
		delete mPointLight2;
		mPointLight2 = 0;
	}

	if (mPointLight3)
	{
		delete mPointLight3;
		mPointLight3 = 0;
	}

	if (mPointLight4)
	{
		delete mPointLight4;
		mPointLight4 = 0;
	}

	if (mPointLight5)
	{
		delete mPointLight5;
		mPointLight5 = 0;
	}

	if (mPointLight6)
	{
		delete mPointLight6;
		mPointLight6 = 0;
	}

	if (mPointLight7)
	{
		delete mPointLight7;
		mPointLight7 = 0;
	}

	if (mPointLight8)
	{
		delete mPointLight8;
		mPointLight8 = 0;
	}

	if (mPointLight9)
	{
		delete mPointLight9;
		mPointLight9 = 0;
	}

	if (mPointLight10)
	{
		delete mPointLight10;
		mPointLight10 = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (mParticle)
	{
		mParticle->Shutdown();
		delete mParticle;
		mParticle = 0;
	}

	if (mParticleShader)
	{
		mParticleShader->Shutdown();
		delete mParticleShader;
		mParticleShader = 0;
	}
	
	return;
}

bool GraphicsClass::Frame(float rotateYaw, float rotatePitch)
{
	bool result;
	
	m_Model->Update();
	mParticle->Frame(frameTime1, m_D3D->GetDeviceContext());

	// Render the graphics scene.
	result = Render(0, rotateYaw, rotatePitch);
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation, float rotateYaw, float rotatePitch)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMFLOAT4 lightPosition[NUM_LIGHTS];
	XMFLOAT4 diffuseColor[NUM_LIGHTS];
	XMFLOAT3 scrollSpeeds, scales;
	XMFLOAT2 distortion1, distortion2, distortion3;
	float distortionScale, distortionBias;
	bool result;
	
	diffuseColor[0] = mPointLight1->GetDiffuseColor();
	diffuseColor[1] = mPointLight2->GetDiffuseColor();
	diffuseColor[2] = mPointLight3->GetDiffuseColor();
	diffuseColor[3] = mPointLight4->GetDiffuseColor();
	diffuseColor[4] = mPointLight5->GetDiffuseColor();
	diffuseColor[5] = mPointLight6->GetDiffuseColor();
	diffuseColor[6] = mPointLight7->GetDiffuseColor();
	diffuseColor[7] = mPointLight8->GetDiffuseColor();
	diffuseColor[8] = mPointLight9->GetDiffuseColor();
	diffuseColor[9] = mPointLight10->GetDiffuseColor();

	lightPosition[0] = mPointLight1->GetPosition();
	lightPosition[1] = mPointLight2->GetPosition();
	lightPosition[2] = mPointLight3->GetPosition();
	lightPosition[3] = mPointLight4->GetPosition();
	lightPosition[4] = mPointLight5->GetPosition();
	lightPosition[5] = mPointLight6->GetPosition();
	lightPosition[6] = mPointLight7->GetPosition();
	lightPosition[7] = mPointLight8->GetPosition();
	lightPosition[8] = mPointLight9->GetPosition();
	lightPosition[9] = mPointLight10->GetPosition();

	frameTime1 = frameTime1 + (0.01f * frameDirection);
	particleTime = 0.001f;

	if (frameTime1 > 2.0f)
	{
		frameTime1 = 0.0f;
	}

	if (frameTime1 < 0.0f)
	{
		frameDirection = 1;
	}

	scrollSpeeds = XMFLOAT3(0.13f, 2.1f, 0.23f);
	scales = XMFLOAT3(1.0f, 2.0f, 3.0f);
	distortion1 = XMFLOAT2(0.05f, 0.4f);
	distortion2 = XMFLOAT2(0.05f, 0.6f);
	distortion3 = XMFLOAT2(0.05f, 0.2f);
	distortionScale = 0.8f;
	distortionBias = 0.5f;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render(rotateYaw, rotatePitch);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_D3D->TurnOnAlphaBlending();

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetModels(),
		lightPosition, diffuseColor,
		viewMatrix, projectionMatrix,
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), 
		mFire, mAlpha, mNoise, 
		scrollSpeeds, scales, distortion1, distortion2, distortion3,
		frameTime1, distortionScale, distortionBias,
		m_Light->GetSpecularPower(), m_Light->GetAmbientOnOff(), m_Light->GetDiffuseOnOff(), m_Light->GetSpecularOnOff());
	if(!result)
	{
		return false;
	}

	m_D3D->TurnOffAlphaBlending();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	// particle render
	m_D3D->TurnOnAlphaBlending();
	m_D3D->SwitchAlphaBlendingMode(BlendMode::ADDITIVE);

	result = mParticleShader->Render(m_D3D->GetDeviceContext(), mParticle->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		mParticle->GetTexture());
	if (!result)
	{
		return false;
	}

	m_D3D->TurnOffAlphaBlending();
	m_D3D->SwitchAlphaBlendingMode(BlendMode::COMBINE);

	return true;
}

LightClass* GraphicsClass::GetLight()
{
	return m_Light;
}

CameraClass* GraphicsClass::GetCamera()
{
	return m_Camera;
}