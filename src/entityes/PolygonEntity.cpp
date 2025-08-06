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
void PolygonEntity::setRotation(const glm::vec3& eulerAngles) {
    glm::vec3 clampedAngles = eulerAngles;

    // Функция для нормализации угла в диапазон [-180, 180]
    auto normalizeAngle = [](float angle) -> float {
        angle = std::fmodf(angle, 360.0f);
        if (angle > 180.0f) angle -= 360.0f;
        if (angle < -180.0f) angle += 360.0f;
        return angle;
        };

    // Ограничение pitch (x) с помощью std::clamp или ручной реализации
#ifdef _MSC_VER
    // Для Visual Studio
    auto clamp = [](float value, float min, float max) {
        return (value < min) ? min : (value > max) ? max : value;
        };
    clampedAngles.x = clamp(clampedAngles.x, -89.0f, 89.0f);
#else
    // Для других компиляторов
    clampedAngles.x = std::clamp(clampedAngles.x, -89.0f, 89.0f);
#endif

    // Нормализация yaw (y) и roll (z)
    clampedAngles.y = normalizeAngle(clampedAngles.y);
    clampedAngles.z = normalizeAngle(clampedAngles.z);

    _rotation = clampedAngles;
    _isDirty = true;
}
void PolygonEntity::setScale(const glm::vec3& newScale) {
    // Убеждаемся, что все компоненты положительные
    glm::vec3 clampedScale = glm::abs(newScale);

    // Устанавливаем минимальный порог, чтобы избежать деления на ноль
    // и визуального "исчезновения" объекта
    const float minScale = 0.001f;
    clampedScale = glm::max(clampedScale, glm::vec3(minScale));

    _scale = clampedScale;
    _isDirty = true;
}