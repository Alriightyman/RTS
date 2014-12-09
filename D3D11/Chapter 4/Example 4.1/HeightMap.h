#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include "intpoint.h"

struct PARTICLE
{
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Color color;
};


struct HEIGHTMAP
{
	HEIGHTMAP(ID3D11Device* Dev,ID3D11DeviceContext* Con, DirectX::SpriteBatch* sprite, INTPOINT size);
	~HEIGHTMAP(void);
	void Release();
	HRESULT LoadFromFile(std::wstring filename);
	HRESULT CreateParticles();
	void Render();
	DirectX::SimpleMath::Vector2 GetCentre() { return DirectX::SimpleMath::Vector2(m_size.x / 2.0f,m_size.y / 2.0f); }

	INTPOINT m_size;
	float m_maxHeight;
	float* m_pHeightMap;

	// vertexBuffer
	ID3D11Buffer* m_pVb;

	// our device and context
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

	// sprite
	DirectX::SpriteBatch* m_pSprite;
	ID3D11ShaderResourceView* m_pHeightMapTexture;
};

