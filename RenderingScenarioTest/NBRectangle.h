#pragma once

#include "Rectangle.h"
#include "glm/vec2.hpp"

class NBRectangle : public Rectangle
{
public:
	NBRectangle(glm::vec2 size);
	virtual ~NBRectangle();

	virtual void Render();
	void AddDetail(NBDetailType type, Rectangle* detail);
	void BlendNormalMap(bool dumpFlag = false);
	void SetNormalBlendMode(NBM mode);
	void ToggleNormalBlendMode();
	void LogCurrentMode();

private:
	TextureForBlending* _textureForBlending;
	NBM _normalBlendMode;
	std::vector<NBDetailRectangle*> _detailList;
};