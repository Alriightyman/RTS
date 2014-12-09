//////////////////////////////////////////////////////////////
// Application Framework	   								//
// Written by: C. Granberg, 2005							//
// Direct3D 11 Version:  R. Turner 2014						//
//////////////////////////////////////////////////////////////
#include <d3d11.h>
#include <SpriteFont.h>
#include "debug.h"
#include "intpoint.h"

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

class APPLICATION
{
	public:		
		APPLICATION();
		HRESULT Init(HINSTANCE hInstance, int width, int height, bool windowed);
		HRESULT Update(float deltaTime);
		HRESULT Render();
		HRESULT Cleanup();
		HRESULT Quit();

	private:
		HWND m_mainWindow;
		// direct3d 11 specific objects
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pContext;
		ID3D11RenderTargetView* m_pRenderTargetView;
		IDXGISwapChain* m_pSwapChain;
		// DirectX Tool Kit Specific objects
		std::unique_ptr<DirectX::SpriteFont> m_pFont;
		std::unique_ptr<DirectX::SpriteBatch> m_pSprite;

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    APPLICATION app;

	//Create new window and Initiate Direct3D
	if(FAILED(app.Init(hInstance, 1280, 720, true)))
		return 0;

	MSG msg;
	memset(&msg, 0, sizeof(MSG));
	int startTime = timeGetTime(); 

	//Start the message loop
	while(msg.message != WM_QUIT)
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
        {	
			int t = timeGetTime();
			float deltaTime = (t - startTime)*0.001f;

			//For each frame update and render our app
			app.Update(deltaTime);
			app.Render();

			startTime = t;
        }
    }

	//Cleanup before exit
	app.Cleanup();

    return (int)msg.wParam;
}

APPLICATION::APPLICATION()
{
	m_pDevice = 0; 
	m_pContext = 0;
	m_mainWindow = 0;
	m_pFont = 0;
	m_pRenderTargetView = 0;
	m_pSwapChain = 0;
}

HRESULT APPLICATION::Init(HINSTANCE hInstance, int width, int height, bool windowed)
{
	debug.Print("Application initiated");

	//Create Window Class
	WNDCLASS wc;
	memset(&wc, 0, sizeof(WNDCLASS));
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)::DefWindowProc; 
	wc.hInstance     = hInstance;
	wc.lpszClassName = L"D3DWND";

	//Register Class and Create new Window
	RegisterClass(&wc);
	m_mainWindow = CreateWindow(L"D3DWND", L"Example 3.1: Application Framework", WS_EX_TOPMOST, 0, 0, width, height, 0, 0, hInstance, 0); 
	SetCursor(NULL);
	ShowWindow(m_mainWindow, SW_SHOW);
	UpdateWindow(m_mainWindow);

	// Create Direct3D 11 Interface
	HRESULT hr;
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = width;
	bufferDesc.Height = height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	
	//Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc; 
		
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = m_mainWindow; 
	swapChainDesc.Windowed = TRUE; 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	hr = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0,
		D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, 0, &m_pContext);

	ID3D11Texture2D* backbuffer;
	hr |= m_pSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(void**)&backbuffer);

	//Create our Render Target
	hr |= m_pDevice->CreateRenderTargetView( backbuffer, NULL, &m_pRenderTargetView );
	backbuffer->Release();

	//Set our Render Target
	m_pContext->OMSetRenderTargets( 1, &m_pRenderTargetView, NULL );

	// Setup the viewport for rendering.
	D3D11_VIEWPORT viewport;

	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_pContext->RSSetViewports(1, &viewport);

	// create the spritebatch and load the font
	m_pSprite.reset(new DirectX::SpriteBatch(m_pContext));
	m_pFont.reset(new DirectX::SpriteFont(m_pDevice,L"Font/Font.spritefont"));

	return S_OK;
}

HRESULT APPLICATION::Update(float deltaTime)
{
	if(KEYDOWN(VK_ESCAPE))
		Quit();

	return S_OK;
}	

HRESULT APPLICATION::Render()
{
	using namespace DirectX::SimpleMath;
    // Clear the viewport
    //m_pDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L);
	m_pContext->ClearRenderTargetView(m_pRenderTargetView,DirectX::Colors::Black);

    // Begin the scene 
   	//Render scene here...
	RECT r = {0, 0, 640, 480};
	
	m_pSprite->Begin();
	std::wstring message = L"Hello world!";
	Vector2 pos = m_pFont->MeasureString(message.c_str());
	m_pFont->DrawString(
		m_pSprite.get(),
		message.c_str(),
		Vector2( 1280.0f/2 - pos.x/2.0f,720.0f/2.0f),
		DirectX::Colors::White);
	m_pSprite->End();
	
	m_pSwapChain->Present(0,0);
	return S_OK;
}

HRESULT APPLICATION::Cleanup()
{
	try
	{
		//Release all resources here..
		m_pDevice->Release();
		m_pContext->Release();
		m_pRenderTargetView->Release();
		m_pSwapChain->Release();
		debug.Print("Application terminated");
	}
	catch(...){}

	return S_OK;
}

HRESULT APPLICATION::Quit()
{
	::DestroyWindow(m_mainWindow);
	::PostQuitMessage(0);
	return S_OK;
}