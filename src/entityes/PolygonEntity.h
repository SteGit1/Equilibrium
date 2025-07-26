#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#ifndef POLIGONENTITY_H
#define POLIGONENTITY_H

class PolygonEntity {
private:
	// ������� � ��������� ������������
	std::vector < glm::vec3> _vertices; 

	// ���� ������� RGBA
	glm::vec4 _color;

	// �������������
	glm::vec3 _position = glm::vec3(0.0f);
	glm::vec3 _rotation = glm::vec3(0.0f); // ������� (���� ������) 
	glm::vec3 _scale = glm::vec3(1.0f);

	mutable glm::mat4 _modelMatrix;
	// ����, ����������� �� ������������� ��������� �������
	mutable bool _isDirty = false;


public:
	PolygonEntity(const std::vector<glm::vec3>& vertices, const glm::vec4& color) : _vertices(vertices), _color(color) {}

	const glm::mat4& getModelMatrix() const;

	// ������ �������������
	void translate(const glm::vec3& delta);
	void rotate(float angle, const glm::vec3& axis);

	// �������
	const std::vector<glm::vec3>& PolygonEntity::getVertices() const;
	const glm::vec3& PolygonEntity::getPosition() const;
	const glm::vec3& PolygonEntity::getRotation() const;
	const glm::vec3& PolygonEntity::getScale() const;
	const glm::vec4& PolygonEntity::getColor() const;

	// �������
	void setColor(const glm::vec4& newColor);
	void setPosition(const glm::vec3& newPosition);
	void setRotation(const glm::vec3& eulerAngles);
	void setScale(const glm::vec3& newScale);
};

#endif // !POLIGONENTITY_H