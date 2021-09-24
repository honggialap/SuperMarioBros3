#pragma once
#include <d3d10.h>
#include <d3dx10.h>

/// <summary>
/// 
/// </summary>
struct CGraphics
{
	int backBufferWidth;
	int backBufferHeight;

	ID3D10Device* _device;
	IDXGISwapChain* _swapChain;
	ID3D10RenderTargetView* _renderTargetView;
	ID3D10BlendState* _blendStateAlpha;
	ID3DX10Sprite* _spriteHandler;

	bool Initialize();
	void Shutdown();
	void Draw();
};
typedef CGraphics* pGraphics;