#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

class TransformComponent {

private:
	glm::vec3 _position = glm::vec3(0.0f);
	glm::vec3 _scale = glm::vec3(1.0f);
	glm::quat _rotationQuat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

public:
	TransformComponent(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotationQuat)
		: _position(position), _scale(scale), _rotationQuat(rotationQuat) {}

	// Методы трансформации
	void translate(const glm::vec3& delta);
	void rotate(float angle, const glm::vec3& axis);

	// Геттеры
	const glm::vec3& getPosition() const;
	const glm::quat& getRotationQuat() const;
	const glm::vec3& getScale() const;

	// Сеттеры
	void setPosition(const glm::vec3& newPosition);
	void setRotationQuat(const glm::quat& rotationQuat);
	void setScale(const glm::vec3& newScale);

};