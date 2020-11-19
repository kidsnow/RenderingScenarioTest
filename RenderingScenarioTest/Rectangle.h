#pragma once

#include "Renderable.h"
#include "NBDetailRectangle.h"
#include "glm/vec2.hpp"
#include <vector>

class TextureForBlending;
class Renderable;

typedef enum NormalBlendMode
{
	LINEAR,
	OVERLAY,
	PARTIAL_DERIVATIVE,
	WHITEOUT,
	UDN,
	REORIENTED,
	UNITY,
	MODE_NUM
} NBM;

class Rectangle : public Renderable
{
public:
	Rectangle(glm::vec2 size);
	virtual ~Rectangle();

	virtual void Render();
	void AddDetail(NBDetailType type, Rectangle* detail);
	void BlendNormalMap(bool dumpFlag = false);
	void SetNormalBlendMode(NBM mode);
	void ToggleNormalBlendMode();
	void LogCurrentMode();

private:
	void createVertexArray();

	glm::vec2 _size;
	TextureForBlending* _textureForBlending;
	NBM _normalBlendMode;
	std::vector<NBDetailRectangle*> _detailList;
};