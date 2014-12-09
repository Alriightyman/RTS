#include "HeightMap.h"
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>


HEIGHTMAP::HEIGHTMAP(ID3D11Device* Dev,ID3D11DeviceContext* Con, DirectX::SpriteBatch* sprite, INTPOINT size)
{
	try
	{
		m_pDevice = Dev;
		m_pContext - Con;
		m_pSprite = sprite;
		m_size = size;

		// reset the heightmap to zero
		m_maxHeight = 15.0f;

		// create a new 2D array with size.x * size.y height values
		m_pHeightMap = new float[m_size.x * m_size.y];

		// set all heights to 0.0f
		memset(m_pHeightMap,0,sizeof(float) * m_size.x*m_size.y);

		// set buffer and texture to null
		m_pVb = nullptr;
		m_pHeightMapTexture = nullptr;
	}
	catch(...)
	{
		debug.Print("Error in HEIGHTMAP::HEIGHTMAP()");
	}
}


HEIGHTMAP::~HEIGHTMAP(void)
{
	Release();
}

void HEIGHTMAP::Release()
{
	if(m_pHeightMap != NULL)delete [] m_pHeightMap;
	if(m_pVb != NULL)m_pVb->Release();
	if(m_pSprite != NULL) m_pSprite = 0;
	if(m_pHeightMapTexture != NULL)m_pHeightMapTexture->Release();
}
HRESULT HEIGHTMAP::LoadFromFile(std::wstring filename)
{
	try
	{
		// reset the heightmap to 0.0f
		memset(m_pHeightMap,0,sizeof(float) * m_size.x * m_size.y);
		// initialize texture variables
		if (m_pHeightMapTexture != nullptr) m_pHeightMapTexture->Release();
		m_pHeightMapTexture = nullptr;

		// load the texture (and scale it to out heightmap size)

		// check rastertek terrain tut 02

	}
	catch(...)
	{

	}
}
HRESULT HEIGHTMAP::CreateParticles()
{
}
void HEIGHTMAP::Render()
{
}