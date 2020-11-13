#pragma once

#include "Object.h"


typedef enum XRCameraProjectionMode {
	Perspective,
	Orthogonal
} CPM;

class Camera : public Object
{
public:
	Camera() {}
	virtual ~Camera() {}

public:
	CPM GetProjectionMode() { return _projection_mode; }
	void SetProjectionMode(const CPM& projection_mode) { _projection_mode = projection_mode; }

public:
	glm::mat4 GetProjectionTransform() const {
		if (_projection_mode == CPM::Perspective)
		{
			return glm::frustum(-_half_size.x, _half_size.x,
								-_half_size.y, _half_size.y,
								_near_positive, _far_positive);
		}
		else if (_projection_mode == CPM::Orthogonal)
		{
			return glm::ortho(-_half_size.x, _half_size.x,
							  -_half_size.y, _half_size.y,
							  _near_positive, _far_positive);
		}
	}

	void SetFrustum(const glm::vec2& size, float near_positive, float far_positive) {
		_half_size.x = size.x / 2.f;
		_half_size.y = size.y / 2.f;

		_near_positive = near_positive;
		_far_positive = far_positive;
	}

private:
	glm::vec2 _half_size{};
	float _near_positive{};
	float _far_positive{};
	CPM _projection_mode{ CPM::Perspective };
};