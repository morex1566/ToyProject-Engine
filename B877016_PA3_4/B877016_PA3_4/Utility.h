#pragma once

#include <directxmath.h>
#include "textureclass.h"

using namespace DirectX;

enum LightType
{
	AMBIENT,
	DIFFUSE,
	SPECULAR,
	NONE
};

struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
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

struct Model
{
	const wchar_t* name;
	ModelType* Coordinate;
	TextureClass* Texture;
	int InstanceCount;
	int IndexCount;
	int VertexCount;
};

struct DataPath
{
	const wchar_t* ObjectPaths;
	const wchar_t* TexturePaths;
};