#include <d3dx9.h>

#define SIZE_X 100
#define SIZE_Y 100

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

struct PARTICLE{
	D3DXVECTOR3 position;
	D3DCOLOR color;
	static const DWORD FVF;
};

struct HEIGHTMAP3
{
	//Functions
	HEIGHTMAP3(IDirect3DDevice9* Dev);
	~HEIGHTMAP3();

	void RaiseTerrain(RECT r, float f);
	void SmoothTerrain();
	HRESULT CreateParticles();
	void Render();
	D3DXVECTOR2 GetCentre(){return D3DXVECTOR2(SIZE_X / 2.0f, SIZE_Y / 2.0f);}
	void MoveRect(int dir);
	

	//The actual Heightmap
	float maxHeight;
	float heightMap[SIZE_X][SIZE_Y];

	RECT selRect;

	//Vertex buffer for points
	IDirect3DVertexBuffer9 *vb;

	//Our device
	IDirect3DDevice9* Device; 
};