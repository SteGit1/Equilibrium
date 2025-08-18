#pragma once 

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <TransformComponent.h>

class PolygonEntity {
private:
	// Вершины в локальном пространстве
	std::vector <glm::vec3> _vertices; 

	mutable glm::mat4 _modelMatrix;
	// Флаг, указывающий на необходимость пересчёта матрицы
	mutable bool _isDirty = false;

	TransformComponent _transform = { glm::vec3(0.0f), glm::vec3(1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f) };

public:
	PolygonEntity(const std::vector<glm::vec3>& vertices) : _vertices(vertices) {}

	const glm::mat4& getModelMatrix() const;

	const std::vector<glm::vec3>& getVertices() const;

	void translate(const glm::vec3& delta);
	void rotate(float angle, const glm::vec3& axis);

	const std::vector<glm::vec3>& getVertices() const;
	const glm::vec3& getPosition() const;
	const glm::quat& getRotation() const;
	const glm::vec3& getScale() const;

	void setPosition(const glm::vec3& newPosition);
	void setRotationQuat(const glm::quat& rotationQuat);
	void setScale(const glm::vec3& newScale);

	void render();
};
