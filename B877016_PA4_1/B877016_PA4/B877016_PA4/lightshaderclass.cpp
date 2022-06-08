////////////////////////////////////////////////////////////////////////////////
// Filename: lightshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightshaderclass.h"


LightShaderClass::LightShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_sampleState1 = 0;
	m_matrixBuffer = 0;
	m_lightBuffer = 0;
	mPointLightColorBuffer = 0;
	mPointLightPositionBuffer = 0;
	mNoiseBuffer = 0;
	mDistortionBuffer = 0;
	mToggleBuffer = 0;
}


LightShaderClass::LightShaderClass(const LightShaderClass& other)
{
}


LightShaderClass::~LightShaderClass()
{
}

// The new light.vs and light.ps HLSL shader files are used as input to initialize the light shader.
bool LightShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"./data/light.vs", L"./data/light.ps");
	if(!result)
	{
		return false;
	}

	return true;
}


void LightShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}

bool LightShaderClass::Render(ID3D11DeviceContext* deviceContext, vector<Model*> models,
	XMFLOAT4 pointLightPosition[], XMFLOAT4 pointLightDiffuse[],
	XMMATRIX viewMatrix, XMMATRIX projectionMatrix, 
	XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor,
	XMFLOAT3 cameraPosition, XMFLOAT4 specularColor,
	ID3D11ShaderResourceView* fireTexture, ID3D11ShaderResourceView* alphatTexture, ID3D11ShaderResourceView* noiseTexture, 
	XMFLOAT3 scrollspeed, XMFLOAT3 scales, XMFLOAT2 distortion1, XMFLOAT2 distortion2, XMFLOAT2 distortion3,
	float frameTime, float distortionScale, float distortionBias,
	float specularPower, float ambientOnOff, float diffuseOnOff, float specularOnOff)
{
	bool result;
	int fireToggle;

	// Set the shader parameters that it will use for rendering.
	for (int i = 0; i < models.size(); i++)
	{
		int startVertexCount = 0;

		models[i]->name == L"./data/Fire.obj" ? fireToggle = 1 : fireToggle = 0;

		result = SetShaderParameters(deviceContext, models[i]->WorldMatrix, viewMatrix, projectionMatrix,
			pointLightPosition, pointLightDiffuse,
			models[i]->Texture->GetTexture(),
			lightDirection, ambientColor, diffuseColor, cameraPosition, specularColor,
			fireTexture, alphatTexture, noiseTexture, 
			scrollspeed, scales, distortion1, distortion2, distortion3,
			frameTime, distortionScale, distortionBias,
			specularPower, ambientOnOff, diffuseOnOff, specularOnOff, fireToggle);
		if (!result)
		{
			return false;
		}

		for (int temp = 0; temp < i; temp++)
		{
			startVertexCount += models[temp]->VertexCount;
		}

		// Now render the prepared buffers with the shader.
		RenderShader(deviceContext, models[i]->VertexCount, models[i]->IndexCount, startVertexCount, models[i]->InstanceCount, fireToggle);
	}


	return true;
}


bool LightShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
	unsigned int numElements;
    D3D11_SAMPLER_DESC samplerDesc;
	D3D11_SAMPLER_DESC samplerDesc2;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC pointLightPositionBufferDesc;
	D3D11_BUFFER_DESC pointLightColorBufferDesc;
	D3D11_BUFFER_DESC distortionBufferDesc;
	D3D11_BUFFER_DESC noiseBufferDesc;
	D3D11_BUFFER_DESC ToggleBufferDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

    // Compile the vertex shader code.
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

    // Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

    // Create the vertex shader from the buffer.
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
	{
		return false;
	}

    // Create the pixel shader from the buffer.
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TEXCOORD";
	polygonLayout[3].SemanticIndex = 1;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 1;
	polygonLayout[3].AlignedByteOffset = 0;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[3].InstanceDataStepRate = 1;

	// Get a count of the elements in the layout.
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), 
		                               &m_layout);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 16;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	samplerDesc2.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc2.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc2.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc2.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc2.MipLODBias = 0.0f;
	samplerDesc2.MaxAnisotropy = 16;
	samplerDesc2.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc2.BorderColor[0] = 0;
	samplerDesc2.BorderColor[1] = 0;
	samplerDesc2.BorderColor[2] = 0;
	samplerDesc2.BorderColor[3] = 0;
	samplerDesc2.MinLOD = 0;
	samplerDesc2.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
    result = device->CreateSamplerState(&samplerDesc, &m_sampleState1);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
	if (FAILED(result))
	{
		return false;
	}

	pointLightColorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pointLightColorBufferDesc.ByteWidth = sizeof(PointLightColorBufferType);
	pointLightColorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pointLightColorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pointLightColorBufferDesc.MiscFlags = 0;
	pointLightColorBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&pointLightColorBufferDesc, NULL, &mPointLightColorBuffer);
	if (FAILED(result))
	{
		return false;
	}

	pointLightPositionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pointLightPositionBufferDesc.ByteWidth = sizeof(PointLightPositionBufferType);
	pointLightPositionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pointLightPositionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pointLightPositionBufferDesc.MiscFlags = 0;
	pointLightPositionBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&pointLightPositionBufferDesc, NULL, &mPointLightPositionBuffer);
	if (FAILED(result))
	{
		return false;
	}

	noiseBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	noiseBufferDesc.ByteWidth = sizeof(NoiseBufferType);
	noiseBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	noiseBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	noiseBufferDesc.MiscFlags = 0;
	noiseBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&noiseBufferDesc, NULL, &mNoiseBuffer);
	if (FAILED(result))
	{
		return false;
	}

	distortionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	distortionBufferDesc.ByteWidth = sizeof(DistortionBufferType);
	distortionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	distortionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	distortionBufferDesc.MiscFlags = 0;
	distortionBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&distortionBufferDesc, NULL, &mDistortionBuffer);
	if (FAILED(result))
	{
		return false;
	}

	ToggleBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	ToggleBufferDesc.ByteWidth = sizeof(ToggleBufferType);
	ToggleBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ToggleBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ToggleBufferDesc.MiscFlags = 0;
	ToggleBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&ToggleBufferDesc, NULL, &mToggleBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void LightShaderClass::ShutdownShader()
{
	// Release the light constant buffer.
	if(m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	// Release the matrix constant buffer.
	if(m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	if (mPointLightColorBuffer)
	{
		mPointLightColorBuffer->Release();
		mPointLightColorBuffer = 0;
	}

	if (mPointLightPositionBuffer)
	{
		mPointLightPositionBuffer->Release();
		mPointLightPositionBuffer = 0;
	}

	// Release the sampler state.
	if(m_sampleState1)
	{
		m_sampleState1->Release();
		m_sampleState1 = 0;
	}

	// Release the layout.
	if(m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if(m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if(m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	// Release the camera constant buffer.
	if (m_cameraBuffer)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = 0;
	}

	if (mDistortionBuffer)
	{
		mDistortionBuffer->Release();
		mDistortionBuffer = 0;
	}

	if (mNoiseBuffer)
	{
		mNoiseBuffer->Release();
		mNoiseBuffer = 0;
	}

	if (mToggleBuffer)
	{
		mToggleBuffer->Release();
		mToggleBuffer = 0;
	}

	// 두번째 샘플러 스테이트를 해제합니다.
	if (mSampleState2)
	{
		mSampleState2->Release();
		mSampleState2 = 0;
	}

	return;
}


void LightShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

// The SetShaderParameters function now takes in lightDirection and diffuseColor as inputs.
bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, 
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, 
	XMFLOAT4 pointLightPosition[], XMFLOAT4 pointLightDiffuse[],
	ID3D11ShaderResourceView* texture, 
	XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 cameraPosition, XMFLOAT4 specularColor,
	ID3D11ShaderResourceView* fireTexture, ID3D11ShaderResourceView* alphatTexture, ID3D11ShaderResourceView* noiseTexture,
	XMFLOAT3 scrollspeed, XMFLOAT3 scales, XMFLOAT2 distortion1, XMFLOAT2 distortion2, XMFLOAT2 distortion3,
	float frameTime, float distortionScale, float distortionBias,
	float specularPower, float ambientOnOff, float diffuseOnOff, float specularOnOff, int fireToggle)
{
	HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	CameraBufferType* dataPtr3;
	PointLightPositionBufferType* dataPtr4;
	PointLightColorBufferType* dataPtr5;
	NoiseBufferType* dataPtr6;
	DistortionBufferType* dataPtr7;
	ToggleBufferType* dataPtr8;

	// Transpose the matrices to prepare them for the shader.
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
    deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetShaderResources(1, 1, &fireTexture);
	deviceContext->PSSetShaderResources(2, 1, &noiseTexture);
	deviceContext->PSSetShaderResources(3, 1, &alphatTexture);

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	dataPtr2->ambientColor = ambientColor;
	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightDirection = lightDirection;
	dataPtr2->specularColor = specularColor;
	dataPtr2->specularPower = specularPower;
	dataPtr2->ambientOnOff = ambientOnOff;
	dataPtr2->diffuseOnOff = diffuseOnOff;
	dataPtr2->specularOnOff = specularOnOff;
	dataPtr2->padding = 0.0f;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	// Lock the camera constant buffer so it can be written to.
	result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr3 = (CameraBufferType*)mappedResource.pData;

	// Copy the camera position into the constant buffer.
	dataPtr3->cameraPosition = cameraPosition;
	dataPtr3->padding = 0.0f;

	// Unlock the camera constant buffer.
	deviceContext->Unmap(m_cameraBuffer, 0);

	// Set the position of the camera constant buffer in the vertex shader.
	bufferNumber = 1;

	// Now set the camera constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);

	// Lock the light position constant buffer so it can be written to.
	result = deviceContext->Map(mPointLightPositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr4 = (PointLightPositionBufferType*)mappedResource.pData;

	dataPtr4->LightPosition[0] = pointLightPosition[0];
	dataPtr4->LightPosition[1] = pointLightPosition[1];
	dataPtr4->LightPosition[2] = pointLightPosition[2];
	dataPtr4->LightPosition[3] = pointLightPosition[3];
	dataPtr4->LightPosition[4] = pointLightPosition[4];
	dataPtr4->LightPosition[5] = pointLightPosition[5];
	dataPtr4->LightPosition[6] = pointLightPosition[6];
	dataPtr4->LightPosition[7] = pointLightPosition[7];
	dataPtr4->LightPosition[8] = pointLightPosition[8];
	dataPtr4->LightPosition[9] = pointLightPosition[9];

	// Unlock the constant buffer.
	deviceContext->Unmap(mPointLightPositionBuffer, 0);

	bufferNumber = 2;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mPointLightPositionBuffer);

	result = deviceContext->Map(mPointLightColorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr5 = (PointLightColorBufferType*)mappedResource.pData;

	// Copy the light color variables into the constant buffer.
	dataPtr5->DiffuseColor[0] = pointLightDiffuse[0];
	dataPtr5->DiffuseColor[1] = pointLightDiffuse[1];
	dataPtr5->DiffuseColor[2] = pointLightDiffuse[2];
	dataPtr5->DiffuseColor[3] = pointLightDiffuse[3];
	dataPtr5->DiffuseColor[4] = pointLightDiffuse[4];
	dataPtr5->DiffuseColor[5] = pointLightDiffuse[5];
	dataPtr5->DiffuseColor[6] = pointLightDiffuse[6];
	dataPtr5->DiffuseColor[7] = pointLightDiffuse[7];
	dataPtr5->DiffuseColor[8] = pointLightDiffuse[8];
	dataPtr5->DiffuseColor[9] = pointLightDiffuse[9];

	// Unlock the constant buffer.
	deviceContext->Unmap(mPointLightColorBuffer, 0);

	// Set the position of the constant buffer in the pixel shader.
	bufferNumber = 1;

	// Finally set the constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &mPointLightColorBuffer);

	result = deviceContext->Map(mNoiseBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr6 = (NoiseBufferType*)mappedResource.pData;

	dataPtr6->frameTime = frameTime;
	dataPtr6->scales = scales;
	dataPtr6->scrollSpeeds = scrollspeed;
	dataPtr6->padding = 0.0f;

	deviceContext->Unmap(mNoiseBuffer, 0);

	bufferNumber = 4;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &mNoiseBuffer);


	result = deviceContext->Map(mNoiseBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr6 = (NoiseBufferType*)mappedResource.pData;

	dataPtr6->frameTime = frameTime;
	dataPtr6->scales = scales;
	dataPtr6->scrollSpeeds = scrollspeed;
	dataPtr6->padding = 0.0f;

	deviceContext->Unmap(mNoiseBuffer, 0);

	bufferNumber = 3;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mNoiseBuffer);


	result = deviceContext->Map(mDistortionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr7 = (DistortionBufferType*)mappedResource.pData;

	dataPtr7->distortion1 = distortion1;
	dataPtr7->distortion2 = distortion2;
	dataPtr7->distortion3 = distortion3;
	dataPtr7->distortionScale = distortionScale;
	dataPtr7->distortionBias = distortionBias;

	deviceContext->Unmap(mDistortionBuffer, 0);

	bufferNumber = 2;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &mDistortionBuffer);

	result = deviceContext->Map(mToggleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr8 = (ToggleBufferType*)mappedResource.pData;
	dataPtr8->fireOn = fireToggle;
	dataPtr8->padding = XMFLOAT3(0.0f, 0.0f, 0.0f);

	deviceContext->Unmap(mToggleBuffer, 0);

	bufferNumber = 3;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &mToggleBuffer);

	result = deviceContext->Map(mToggleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr8 = (ToggleBufferType*)mappedResource.pData;
	dataPtr8->fireOn = fireToggle;
	dataPtr8->padding = XMFLOAT3(0.0f, 0.0f, 0.0f);

	deviceContext->Unmap(mToggleBuffer, 0);

	bufferNumber = 4;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mToggleBuffer);




	return true;
}


void LightShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int vertexCount, int indexCount, int startVertexCount, int instanceCount, int fireToggle)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState1);

	if (!fireToggle)
	{
		// Render the triangle.
		deviceContext->DrawInstanced(vertexCount, instanceCount, startVertexCount, 0);
	}
	else
	{
		deviceContext->PSSetSamplers(1, 1, &mSampleState2);
		deviceContext->DrawInstanced(vertexCount, instanceCount, startVertexCount, 0);
	}


	return;
}