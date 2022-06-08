////////////////////////////////////////////////////////////////////////////////
// Filename: light.vs
////////////////////////////////////////////////////////////////////////////////

#define NUM_LIGHTS 10

/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
	float padding;
};

cbuffer PointLightPositionBuffer
{
    float4 PointlightPosition[NUM_LIGHTS];
};

cbuffer NoiseBuffer
{
    float frameTime;
    float3 scrollSpeeds;
    float3 scales;
    float padding1;
};

cbuffer ToggleBuffer
{
    int fireOn;
    float3 togglePadding;
};


//////////////
// TYPEDEFS //
//////////////

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float3 instancePosition : TEXCOORD1;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
	float3 pointLightPos1 : TEXCOORD2;
	float3 pointLightPos2 : TEXCOORD3;
	float3 pointLightPos3 : TEXCOORD4;
	float3 pointLightPos4 : TEXCOORD5;
	float3 pointLightPos5 : TEXCOORD6;
	float3 pointLightPos6 : TEXCOORD7;
	float3 pointLightPos7 : TEXCOORD8;
	float3 pointLightPos8 : TEXCOORD9;
	float3 pointLightPos9 : TEXCOORD10;
	float3 pointLightPos10 : TEXCOORD11;
	float3 pointIntensity1 : TEXCOORD12;
    float3 pointIntensity2 : TEXCOORD13;
    float3 pointIntensity3 : TEXCOORD14;
    float3 pointIntensity4 : TEXCOORD15;
    float3 pointIntensity5 : TEXCOORD16;
	float3 pointIntensity6 : TEXCOORD17;
    float3 pointIntensity7 : TEXCOORD18;
    float3 pointIntensity8 : TEXCOORD19;
    float3 pointIntensity9 : TEXCOORD20;
    float3 pointIntensity10 : TEXCOORD21;
    float2 texCoords1 : TEXCOORD22;
    float2 texCoords2 : TEXCOORD23;
    float2 texCoords3 : TEXCOORD24;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
	float4 worldPosition;

    // initialize pointIntensity
    output.pointIntensity1.x = 0;
    output.pointIntensity2.x = 0;
    output.pointIntensity3.x = 0;
    output.pointIntensity4.x = 0;
    output.pointIntensity5.x = 0;
    output.pointIntensity6.x = 0;
    output.pointIntensity7.x = 0;
    output.pointIntensity8.x = 0;
    output.pointIntensity9.x = 0;
    output.pointIntensity10.x = 0;


	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    input.position.x += input.instancePosition.x;
    input.position.y += input.instancePosition.y;
    input.position.z += input.instancePosition.z;
    
	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
    // Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);

    // check distance vertex between point light
    if(distance(input.position, PointlightPosition[0]) < 5.0f)
    {
        output.pointIntensity1.x = saturate(1.0f / pow(distance(input.position, PointlightPosition[0]), 2));
    }

    if(distance(input.position, PointlightPosition[1]) < 5.0f)
    {
        output.pointIntensity2.x = saturate(1.0f / pow(distance(input.position, PointlightPosition[1]), 2));
    }

    if(distance(input.position, PointlightPosition[2]) < 1.0f)
    {
        output.pointIntensity3.x = saturate(1.0f / pow(distance(input.position, PointlightPosition[2]), 2));
    }

    if(distance(input.position, PointlightPosition[3]) < 2.0f)
    {
        output.pointIntensity4.x = saturate(1.0f / pow(distance(input.position, PointlightPosition[3]), 2));
    }

    if(distance(input.position, PointlightPosition[4]) < 2.0f)
    {
        output.pointIntensity5.x = saturate(1.0f / pow(distance(input.position, PointlightPosition[4]), 2));
    }

    if(distance(input.position, PointlightPosition[5]) < 2.0f)
    {
        output.pointIntensity6.x = saturate(1.0f / pow(distance(input.position, PointlightPosition[5]), 2));
    }

    if(distance(input.position, PointlightPosition[6]) < 2.0f)
    {
        output.pointIntensity7.x = saturate(1.0f / pow(distance(input.position, PointlightPosition[6]), 2));
    }

    if(distance(input.position, PointlightPosition[7]) < 5.0f)
    {
        output.pointIntensity8.x = saturate(1.0f / pow(distance(input.position, PointlightPosition[7]), 2));
    }

    if(distance(input.position, PointlightPosition[8]) < 1.0f)
    {
        output.pointIntensity9.x = saturate(1.0f / pow(distance(input.position, PointlightPosition[8]), 2));
    }

    if(distance(input.position, PointlightPosition[9]) < 1.0f)
    {
        output.pointIntensity10.x = saturate(1.0f / pow(distance(input.position, PointlightPosition[9]), 2));
    }

    // set pointlight
    output.pointLightPos1 = PointlightPosition[0].xyz - worldPosition.xyz;
    output.pointLightPos2 = PointlightPosition[1].xyz - worldPosition.xyz;
    output.pointLightPos3 = PointlightPosition[2].xyz - worldPosition.xyz;
    output.pointLightPos4 = PointlightPosition[3].xyz - worldPosition.xyz;
    output.pointLightPos5 = PointlightPosition[4].xyz - worldPosition.xyz;
    output.pointLightPos6 = PointlightPosition[5].xyz - worldPosition.xyz;
    output.pointLightPos7 = PointlightPosition[6].xyz - worldPosition.xyz;
    output.pointLightPos8 = PointlightPosition[7].xyz - worldPosition.xyz;
    output.pointLightPos9 = PointlightPosition[8].xyz - worldPosition.xyz;
    output.pointLightPos10 = PointlightPosition[9].xyz - worldPosition.xyz;

    output.pointLightPos1 = normalize(output.pointLightPos1);
    output.pointLightPos2 = normalize(output.pointLightPos2);
    output.pointLightPos3 = normalize(output.pointLightPos3);
    output.pointLightPos4 = normalize(output.pointLightPos4);
    output.pointLightPos5 = normalize(output.pointLightPos5);
    output.pointLightPos6 = normalize(output.pointLightPos6);
    output.pointLightPos7 = normalize(output.pointLightPos7);
    output.pointLightPos8 = normalize(output.pointLightPos8);
    output.pointLightPos9 = normalize(output.pointLightPos9);
    output.pointLightPos10 = normalize(output.pointLightPos10);


    // set fire texture pos
    if(fireOn)
    {
        output.texCoords1 = (input.tex * scales.x);
        output.texCoords2 = (input.tex * scales.y);
        output.texCoords3 = (input.tex * scales.z);

        output.texCoords1.y = output.texCoords1.y + (frameTime * scrollSpeeds.x);
        output.texCoords2.y = output.texCoords2.y + (frameTime * scrollSpeeds.y);
        output.texCoords3.y = output.texCoords3.y + (frameTime * scrollSpeeds.z);
    }

    return output;
}