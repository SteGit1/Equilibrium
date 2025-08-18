#include <PolygonEntity.h>
#include <glm/gtc/quaternion.hpp>

const glm::mat4& PolygonEntity::getModelMatrix() const {
    if (_isDirty) {
        _modelMatrix = glm::mat4(1.0f);
        _modelMatrix = glm::translate(_modelMatrix, _transform.getPosition());
        _modelMatrix *= glm::mat4_cast(_transform.getRotationQuat());
        _modelMatrix = glm::scale(_modelMatrix, _transform.getScale());
        _isDirty = false;
    }
    return _modelMatrix;
}

const std::vector<glm::vec3>& PolygonEntity::getVertices() const {
    return _vertices;
}
const glm::vec3& PolygonEntity::getPosition() const {
    return _transform.getPosition(); 
}
const glm::quat& PolygonEntity::getRotation() const {
    return _transform.getRotationQuat(); 
}
const glm::vec3& PolygonEntity::getScale() const {
    return _transform.getScale(); 
}

void PolygonEntity::setPosition(const glm::vec3& newPosition) {
    _transform.setPosition(newPosition);
    _isDirty = true;
}
void PolygonEntity::setRotationQuat(const glm::quat& rotationQuat) {
	_transform.setRotationQuat(rotationQuat);
	_isDirty = true;
}
void PolygonEntity::setScale(const glm::vec3& newScale) {
    _transform.setScale(newScale);
    _isDirty = true;
}

void PolygonEntity::translate(const glm::vec3& delta) {
    _transform.translate(delta);
    _isDirty = true;
}
void PolygonEntity::rotate(float angle, const glm::vec3& axis) {
    _transform.rotate(angle, axis);
    _isDirty = true;
}

void render() {
	// реализация рендеринга полигональной сущности
}