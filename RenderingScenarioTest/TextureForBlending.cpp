#include "TextureForBlending.h"
#include "Texture.h"

TextureForBlending::TextureForBlending(int width, int height) :
	_width(width),
	_height(height),
	_target(0),
	_source(1)
{
	_texture[_target] = new Texture(width, height);
	_texture[_source] = new Texture(width, height);
}

TextureForBlending::~TextureForBlending()
{
	delete _texture[0];
	delete _texture[1];
}

void TextureForBlending::SwapTextures()
{
	_target = (_target + 1) % 2;
	_source = (_source + 1) % 2;
}