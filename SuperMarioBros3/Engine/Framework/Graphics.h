#pragma once
#include <d3d10.h>
#include <d3dx10.h>
#include <unordered_map>


struct CTexture
{
	ID3D10Texture2D* _texture;
	ID3D10ShaderResourceView* _rsview;
	int _width;
	int _height;

	CTexture(ID3D10Texture2D* texture, ID3D10ShaderResourceView* rsview);
	~CTexture();
};
typedef CTexture* pTexture;


struct CGraphics
{
	/* Direct 3D */
	unsigned int _backBufferWidth = 0;
	unsigned int _backBufferHeight = 0;
	ID3D10Device* _device = NULL;
	IDXGISwapChain* _swapChain = NULL;
	ID3D10RenderTargetView* _renderTargetView = NULL;
	ID3D10BlendState* _blendStateAlpha = NULL;
	ID3D10SamplerState* _pointSamplerState = NULL;
	ID3DX10Sprite* _spriteHandler = NULL;

	void Initialize(HWND hWnd);
	void Shutdown();


	/* Texture database */
	std::unordered_map<unsigned int, pTexture> _textures;

	void LoadTexture(unsigned int id, std::wstring texturePath);
	pTexture GetTexture(unsigned int id);
};
typedef CGraphics* pGraphics;