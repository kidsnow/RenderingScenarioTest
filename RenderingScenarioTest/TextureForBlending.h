#pragma once

class Texture;

class TextureForBlending
{
public:
	TextureForBlending(int width, int height);
	~TextureForBlending();

public:
	void SwapTextures();
	Texture* GetTargetTexture() { return _texture[_target]; }
	Texture* GetSourceTexture() { return _texture[_source]; }

private:
	int _width, _height;
	int _target, _source;
	Texture* _texture[2];
};