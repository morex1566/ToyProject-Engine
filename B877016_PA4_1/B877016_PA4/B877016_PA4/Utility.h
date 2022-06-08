#pragma once

#include <directxmath.h>
#include "textureclass.h"

using namespace DirectX;

const int NUM_LIGHTS = 10;


enum LightType
{
	AMBIENT,
	DIFFUSE,
	SPECULAR,
	NONE
};

enum class BlendMode
{
	ADDITIVE,
	COMBINE
};

struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
	XMFLOAT4 color;
};

struct FaceType
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
};

struct ModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};

struct InstanceType
{
	XMFLOAT3 Position;
};

struct MatrixBufferType
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

struct CameraBufferType
{
	XMFLOAT3 cameraPosition;
	float padding;
};

struct LightBufferType
{
	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
	XMFLOAT3 lightDirection;
	float specularPower;
	XMFLOAT4 specularColor;
	float ambientOnOff;
	float diffuseOnOff;
	float specularOnOff;
	float padding;
};

struct ParticleType
{
	XMFLOAT3 position;
	XMFLOAT3 color;
	float velocity;
	bool isActive;
};

struct PointLightColorBufferType
{
	XMFLOAT4 DiffuseColor[NUM_LIGHTS];
};

struct PointLightPositionBufferType
{
	XMFLOAT4 LightPosition[NUM_LIGHTS];
};

struct NoiseBufferType
{
	float frameTime;
	XMFLOAT3 scrollSpeeds;
	XMFLOAT3 scales;
	float padding;
};

struct DistortionBufferType
{
	XMFLOAT2 distortion1;
	XMFLOAT2 distortion2;
	XMFLOAT2 distortion3;
	float distortionScale;
	float distortionBias;
};

struct Model
{
	const wchar_t* name;
	ModelType* Coordinate;
	TextureClass* Texture;
	int InstanceCount;
	int IndexCount;
	int VertexCount;
	XMMATRIX WorldMatrix;
	float RotateSpeed;
	XMFLOAT3 Position;
};

struct DataPath
{
	const wchar_t* ObjectPaths;
	const wchar_t* TexturePaths;
};

struct ToggleBufferType
{
	int fireOn;
	XMFLOAT3 padding;
};

