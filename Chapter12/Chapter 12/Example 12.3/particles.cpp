#include "particles.h"

DWORD FtoDword(float f){return *((DWORD*)&f);}

//Global Particle Textures
IDirect3DTexture9* starTexture = NULL;

struct PARTICLE_VERTEX
{
	D3DXVECTOR3 position;
	D3DCOLOR color;
	static const DWORD FVF;
};

const DWORD PARTICLE_VERTEX::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

int numParticles = 2048;
IDirect3DVertexBuffer9* particleBuffer = NULL;
DWORD bufferOffset = 0;

void LoadParticleResources(IDirect3DDevice9 *Dev)
{
	Dev->CreateVertexBuffer(numParticles * sizeof(PARTICLE_VERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
							PARTICLE_VERTEX::FVF, D3DPOOL_DEFAULT, &particleBuffer, NULL);

	//Load textures
	D3DXCreateTextureFromFile(Dev, "textures/star.dds", &starTexture);
}

void UnloadParticleResources()
{
	if(particleBuffer)
		particleBuffer->Release();
	particleBuffer = NULL;

	//Release textures
	if(starTexture)starTexture->Release();

	starTexture = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//								PARTICLE_SYSTEM												//
//////////////////////////////////////////////////////////////////////////////////////////////

PARTICLE_SYSTEM::PARTICLE_SYSTEM(IDirect3DDevice9 *Dev) : EFFECT(Dev)
{
	m_pTexture = NULL;
	m_blendMode = D3DBLEND_ONE;
	m_particleSize = 3.0f;
}

PARTICLE_SYSTEM::~PARTICLE_SYSTEM()
{
	//Delete all particles
	for(int i=0;i<(int)m_particles.size();i++)
		delete m_particles[i];
	m_particles.clear();
}

void PARTICLE_SYSTEM::Update(float timeDelta)
{
	
}

void PARTICLE_SYSTEM::Render()
{
	if(m_particles.empty() || particleBuffer == NULL)return;

	PreRender();

	m_pDevice->SetTexture(0, m_pTexture);
	m_pDevice->SetFVF(PARTICLE_VERTEX::FVF);
	m_pDevice->SetStreamSource(0, particleBuffer, 0, sizeof(PARTICLE_VERTEX));

	int batchSize = 512;
	for(int i=0;i<(int)m_particles.size();i+=batchSize)
		RenderBatch(i, batchSize);

	PostRender();
}

void PARTICLE_SYSTEM::RenderBatch(int start, int batchSize)
{
	//If we will reach the end of the vertex buffer, start over
	if((int)bufferOffset + batchSize >= numParticles)bufferOffset = 0;

	//Lock the vertex buffer
	PARTICLE_VERTEX *p = NULL;
	particleBuffer->Lock(bufferOffset * sizeof(PARTICLE_VERTEX), 
						 batchSize, (void**)&p, 
						 bufferOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

	int particlesRendered = 0;
	for(int i=start;i<(int)m_particles.size() && i < start + batchSize;i++)
		if(!m_particles[i]->dead)
		{
			p->position = m_particles[i]->position;
			p->color = m_particles[i]->color;
			p++;
			particlesRendered++;
		}

	particleBuffer->Unlock();

	//Render batch 
	if(particlesRendered > 0)
		m_pDevice->DrawPrimitive(D3DPT_POINTLIST, bufferOffset, particlesRendered);

	//Increase offset
	bufferOffset += batchSize;
}

bool PARTICLE_SYSTEM::isDead()
{
	return true;
}

void PARTICLE_SYSTEM::PreRender()
{
	m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pDevice->SetRenderState(D3DRS_POINTSIZE, FtoDword(m_particleSize));
	m_pDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDword(0.0f));
	m_pDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDword(0.0f));
	m_pDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDword(1.0f));
	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);

	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, m_blendMode);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void PARTICLE_SYSTEM::PostRender()
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);

    m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
    m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//								MAGIC SHOWER												//
//////////////////////////////////////////////////////////////////////////////////////////////

MAGIC_SHOWER::MAGIC_SHOWER(IDirect3DDevice9 *Dev, int noParticles, D3DXVECTOR3 _origin) : PARTICLE_SYSTEM(Dev)
{
	m_origin = _origin;

	//Add initial m_particles
	for(int i=0;i<noParticles;i++)
	{
		PARTICLE *p = new PARTICLE();
		memset(p, 0, sizeof(PARTICLE));
		p->time_to_live = rand()%5000 / 1000.0f;
		p->dead = true;
		p->acceleration = D3DXVECTOR3(0.0f, -0.75f, 0.0f);
		m_particles.push_back(p);
	}

	//Set texture
	m_pTexture = starTexture;
}

void MAGIC_SHOWER::Update(float timeDelta)
{
	for(int i=0;i<(int)m_particles.size();i++)
	{
		//Update live particles
		m_particles[i]->time_to_live -= timeDelta;
		m_particles[i]->velocity += m_particles[i]->acceleration * timeDelta;
		m_particles[i]->position += m_particles[i]->velocity * timeDelta;
		m_particles[i]->color.a = m_particles[i]->time_to_live / 5.0f;

		//Re-spawn dead particles
		if(m_particles[i]->time_to_live <= 0.0f)
		{
			m_particles[i]->position = m_origin;
			
			//Random direction
			m_particles[i]->velocity = D3DXVECTOR3((rand()%2000 / 1000.0f) - 1.0f,
												 (rand()%2000 / 1000.0f) - 1.0f,
												 (rand()%2000 / 1000.0f) - 1.0f);

			D3DXVec3Normalize(&m_particles[i]->velocity, &m_particles[i]->velocity);
			m_particles[i]->velocity *= 2.0f;

			//Random color
			m_particles[i]->color = D3DXCOLOR(rand()%1000 / 1000.0f, rand()%1000 / 1000.0f, rand()%1000 / 1000.0f, 1.0f);

			//Random life span
			m_particles[i]->time_to_live = rand()%4000 / 1000.0f + 1.0f;
			m_particles[i]->dead = false;
		}
	}
}

bool MAGIC_SHOWER::isDead()
{
	return false;
}