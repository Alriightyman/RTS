#include "heightMap3.h"
#include "debug.h"

const DWORD PARTICLE::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
DWORD FtoDword(float f){return *((DWORD*)&f);}

HEIGHTMAP3::HEIGHTMAP3(IDirect3DDevice9* Dev)
{
	try
	{
		Device = Dev;

		//Reset the heightMap to 0
		maxHeight = 20.0f;
		memset(&heightMap, 0, sizeof(float)*SIZE_X*SIZE_Y);

		//Set particle vertex buffer to NULL
		vb = NULL;

		selRect.top = selRect.left = SIZE_X / 2 - 5;
		selRect.bottom = selRect.right = SIZE_X / 2 + 5;
	}
	catch(...)
	{
		debug.Print("Error in HEIGHTMAP3::HEIGHTMAP()");
	}
}

HEIGHTMAP3::~HEIGHTMAP3()
{
	if(vb != NULL)vb->Release();
}

void HEIGHTMAP3::RaiseTerrain(RECT r, float f)
{
	for(int y=r.top;y<=r.bottom;y++)
		for(int x=r.left;x<=r.right;x++)
		{
			heightMap[x][y] += f;
			if(heightMap[x][y] < -maxHeight)heightMap[x][y] = -maxHeight;
			if(heightMap[x][y] > maxHeight)heightMap[x][y] = maxHeight;
		}

	CreateParticles();
}

void HEIGHTMAP3::SmoothTerrain()
{
	for(int y=0;y<SIZE_Y;y++)
		for(int x=0;x<SIZE_X;x++)
		{
			float totalHeight = 0.0f;
			int noNodes = 0;

			for(int y1=y-1;y1<=y+1;y1++)
				for(int x1=x-1;x1<=x+1;x1++)
					if(x1 >= 0 && x1 < SIZE_X && y1 >= 0 && y1 < SIZE_Y)
					{
						totalHeight += heightMap[x1][y1];
						noNodes++;
					}

			heightMap[x][y] = totalHeight / (float)noNodes;
		}

	CreateParticles();

	Sleep(500);
}

void HEIGHTMAP3::MoveRect(int dir)
{
	if(dir == LEFT)	{selRect.left--;selRect.right--;}
	if(dir == RIGHT){selRect.left++;selRect.right++;}
	if(dir == UP)	{selRect.top--;selRect.bottom--;}
	if(dir == DOWN)	{selRect.top++;selRect.bottom++;}

	Sleep(100);
	CreateParticles();
}

HRESULT HEIGHTMAP3::CreateParticles()
{
	try
	{
		if(vb != NULL)
		{
			vb->Release();
			vb = NULL;
		}

		if(FAILED(Device->CreateVertexBuffer(SIZE_X * SIZE_Y * sizeof(PARTICLE), D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, PARTICLE::FVF, D3DPOOL_DEFAULT, &vb, 0)))
			debug.Print("Failed to create particle vertex buffer");

		PARTICLE *v = NULL;
		vb->Lock(0, 0, (void**)&v, D3DLOCK_DISCARD);

		for(int y=0;y<SIZE_Y;y++)
			for(int x=0;x<SIZE_X;x++)
			{
				float prc = heightMap[x][y] / maxHeight;
				if(prc < 0.0f)prc = -prc;
				int red =  255 * prc;
				int green = 255 * (1.0f - prc);

				if(x >= selRect.left && x <= selRect.right && y >= selRect.top && y <= selRect.bottom)
					v->color = D3DCOLOR_ARGB(255, 0, 0, 255);
				else v->color = D3DCOLOR_ARGB(255, red, green, 0);

				v->position = D3DXVECTOR3(x, heightMap[x][y], -y);
				v++;
			}
		
		vb->Unlock();

	}
	catch(...)
	{
		debug.Print("Error in HEIGHTMAP3::CreateParticles()");
		return E_FAIL;
	}

	return S_OK;
}

void HEIGHTMAP3::Render()
{
	try
	{
		if(vb != NULL)
		{
			Device->SetRenderState(D3DRS_LIGHTING, false);
			Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
			Device->SetRenderState(D3DRS_POINTSCALEENABLE, true);

			Device->SetRenderState(D3DRS_POINTSIZE, FtoDword(0.7f));
			Device->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDword(0.0f));
			Device->SetRenderState(D3DRS_POINTSCALE_A, 0);
			Device->SetRenderState(D3DRS_POINTSCALE_B, FtoDword(0.0f));
			Device->SetRenderState(D3DRS_POINTSCALE_C, FtoDword(1.0f));
			Device->SetRenderState(D3DRS_ZWRITEENABLE, false);

			Device->SetTexture(0, NULL);
			Device->SetFVF(PARTICLE::FVF);
			Device->SetStreamSource(0, vb, 0, sizeof(PARTICLE));
			Device->DrawPrimitive(D3DPT_POINTLIST, 0, SIZE_X * SIZE_Y);
		}
	}
	catch(...)
	{
		debug.Print("Error in HEIGHTMAP3::Render()");
	}
}