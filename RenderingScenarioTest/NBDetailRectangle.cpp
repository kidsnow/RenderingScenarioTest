#include "NBDetailRectangle.h"

glm::mat3 NBDetailRectangle::GetTransform()
{
	if (_type == DETAIL_CENTER)
		return glm::mat3(1.0);

	glm::mat3 scale(1.0);
	scale[1][1] = 10.0;

	glm::mat3 rotation(1.0);
	glm::mat3 translation(1.0);
	float angle = 0.0;
	switch (_type)
	{
	case DETAIL_TOP:
		break;
	case DETAIL_BOTTOM:
		angle = 180.0;
		break;
	case DETAIL_LEFT:
		break;
	case DETAIL_RIGHT:
		break;
	}

	float rad = angle * 3.141592653589793 / 180.0;

	rotation[0][0] = cos(rad);
	rotation[1][0] = -sin(rad);
	rotation[0][1] = sin(rad);
	rotation[1][1] = cos(rad);

	return translation * rotation * scale;
}