////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_rotation.x = 0.0f;
	m_rotation.y = 0.0f;
	m_rotation.z = 0.0f;

	moveSpeed = 0.03f;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}


XMFLOAT3 CameraClass::GetPosition()
{
	return m_position;
}


XMFLOAT3 CameraClass::GetRotation()
{
	return m_rotation;
}

void CameraClass::MoveX(float dir)
{
	XMVECTOR moveVector = XMVectorSet(dir, 0, 0, 0);
	XMFLOAT3 movePos;
	XMFLOAT3 temp = m_position;
	XMMATRIX rotationMatrix;
	float yaw, pitch, roll;

	pitch = m_rotation.x * 0.0174532925f;
	yaw = m_rotation.y * 0.0174532925f;
	roll = m_rotation.z * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	moveVector = XMVector3TransformCoord(moveVector, rotationMatrix);
	XMVector3Normalize(moveVector);

	XMStoreFloat3(&movePos, moveVector);

	temp.x += movePos.x * moveSpeed;
	temp.y += 0;
	temp.z += movePos.z * moveSpeed;
	SetPosition(temp.x, temp.y, temp.z);

	return;
}

void CameraClass::MoveZ(float dir)
{

	XMVECTOR moveVector = XMVectorSet(0, 0, dir, 0);
	XMFLOAT3 movePos;
	XMFLOAT3 temp = m_position;
	XMMATRIX rotationMatrix;
	float yaw, pitch, roll;

	pitch = m_rotation.x * 0.0174532925f;
	yaw = m_rotation.y * 0.0174532925f;
	roll = m_rotation.z * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	moveVector = XMVector3TransformCoord(moveVector, rotationMatrix);
	XMVector3Normalize(moveVector);

	XMStoreFloat3(&movePos, moveVector);

	temp.x += movePos.x * moveSpeed;
	temp.y += 0;
	temp.z += movePos.z * moveSpeed;
	SetPosition(temp.x, temp.y, temp.z);

	return;
}

void CameraClass::RotateVertical(float dir)
{

}

void CameraClass::RotateHorizontal(float dir)
{

}

// This uses the position and rotation of the camera to build and to update the view matrix.
void CameraClass::Render(float rotateYaw, float rotatePitch)
{
	XMVECTOR up, position;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;
	XMVECTOR lookAt;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.

	m_rotation.y += rotateYaw;
	m_rotation.x += rotatePitch;

	pitch = m_rotation.x * 0.0174532925f;
	yaw   = m_rotation.y * 0.0174532925f;
	roll  = m_rotation.z * 0.0174532925f;

	// Setup the vector that points upwards.
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// Setup the position of the camera in the world.
	position = XMLoadFloat3(&m_position);

	// Setup where the camera is looking by default.
	lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);

	return;
}


void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}
