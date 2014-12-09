#include "camera.h"

CAMERA::CAMERA()
{
	Init(NULL);
}

void CAMERA::Init(IDirect3DDevice9* Dev)
{
	m_pDevice = Dev;
	m_fov = D3DX_PI / 4.0f;
	m_eye = D3DXVECTOR3(50.0f, 50.0f, 50.0f);
	m_focus = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

D3DXMATRIX CAMERA::GetViewMatrix()
{
	D3DXMATRIX  matView;
	D3DXMatrixLookAtLH(&matView, &m_eye, &m_focus, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	return  matView;
}

D3DXMATRIX CAMERA::GetProjectionMatrix()
{
	D3DXMATRIX  matProj;
	D3DVIEWPORT9 v;
	m_pDevice->GetViewport(&v);

	float aspect = v.Width / (float)v.Height;
	D3DXMatrixPerspectiveFovLH(&matProj, m_fov, aspect, 0.1f, 1000.0f );
	return matProj;
}