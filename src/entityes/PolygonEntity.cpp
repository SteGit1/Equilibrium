#include <PolygonEntity.h>

const glm::mat4& PolygonEntity::getModelMatrix() const {
	if (_isDirty) {
		_modelMatrix = glm::mat4(1.0f);

		_modelMatrix = glm::translate(_modelMatrix, _position);
		
		_modelMatrix = glm::rotate(_modelMatrix, _rotation.x, glm::vec3(0.0f, 0.0f, 1.0f));
		_modelMatrix = glm::rotate(_modelMatrix, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		_modelMatrix = glm::rotate(_modelMatrix, _rotation.z, glm::vec3(1.0f, 0.0f, 0.0f));
	
		_modelMatrix = glm::scale(_modelMatrix, _scale);

		_isDirty = false;
	}
	return _modelMatrix;
}

void PolygonEntity::translate(const glm::vec3& delta) { _position += delta; _isDirty = true; }
void PolygonEntity::rotate(float angle, const glm::vec3& axis) { _rotation += angle * axis; _isDirty = true; }

const std::vector<glm::vec3>& PolygonEntity::getVertices() const { return _vertices; }
const glm::vec3& PolygonEntity::getPosition() const { return _position; }
const glm::vec3& PolygonEntity::getRotation() const { return _rotation; }
const glm::vec3& PolygonEntity::getScale() const { return _scale; }
const glm::vec4& PolygonEntity::getColor() const { return _color; }

void PolygonEntity::setColor(const glm::vec4& newColor) { _color = newColor; }
void PolygonEntity::setPosition(const glm::vec3& newPosition) { _position = newPosition; }
void PolygonEntity::setRotation(const glm::vec3& eulerAngles) {_rotation = eulerAngles, _isDirty = true; }
void PolygonEntity::setScale(const glm::vec3& newScale) {_scale = newScale, _isDirty = true; }