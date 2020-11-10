#pragma once

#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"


class Object
{
public:
	Object() {}
	virtual ~Object() {}

public:
	void SetPosition(const glm::vec4& position) { _position = position; }
	void SetQuaternion(const glm::quat& quaternion) { _orientation = quaternion; }

	glm::quat GetQuaternion() const { return _orientation; }
	glm::mat4 GetTransform() const
	{
		glm::mat4 transform = glm::mat4_cast(_orientation);
		transform[3] = _position;
		return transform;
	}
	glm::mat4 GetInvTransform() const
	{
		glm::mat4 inv_transform = glm::transpose(glm::mat4_cast(_orientation));
		inv_transform[3] = -(inv_transform * _position);
		inv_transform[3][3] = 1.f;
		return inv_transform;
	}

public:
	void Move(const glm::vec3& distance) { _position += glm::vec4(distance, 0); }
	void Rotate(float angle, const glm::vec3& v) { _orientation = glm::rotate(_orientation, angle, v); }

private:
	glm::vec4 _position{};
	glm::quat _orientation{ glm::identity<glm::quat>() };
};