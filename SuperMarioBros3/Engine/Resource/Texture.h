#pragma once
#include <unordered_map>
#include <d3d10.h>

/// <summary>
/// 
/// </summary>
struct CTexture
{
	ID3D10Texture2D* _texture;
	ID3D10ShaderResourceView* _rsview;
	int _width;
	int _height;

	CTexture();
	CTexture(ID3D10Texture2D* texture, ID3D10ShaderResourceView* rsview);
	~CTexture();

};
typedef CTexture* pTexture;


/// <summary>
/// 
/// </summary>
struct CTextures
{
	CTextures();
	~CTextures();

	std::unordered_map<unsigned int, pTexture> _textures;
	bool LoadTexture();
	pTexture GetTexture();
};
typedef CTextures* pTextures;