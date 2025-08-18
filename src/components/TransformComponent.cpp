#pragma once

#include <TransformComponent.h>

void TransformComponent::translate(const glm::vec3& delta) {
    _position += delta;
}
void TransformComponent::rotate(float angle, const glm::vec3& axis) {
    _rotationQuat = glm::rotate(_rotationQuat, glm::radians(angle), axis);
}

const glm::vec3& TransformComponent::getPosition() const {
    return _position;
}
const glm::quat& TransformComponent::getRotationQuat() const {
    return _rotationQuat;
}
const glm::vec3& TransformComponent::getScale() const {
    return _scale;
}

void TransformComponent::setPosition(const glm::vec3& newPosition) {
    _position = newPosition;
}
void TransformComponent::setRotationQuat(const glm::quat& rotationQuat) {
    _rotationQuat = rotationQuat;
}
void TransformComponent::setScale(const glm::vec3& newScale) {
    // Убеждаемся, что все компоненты положительные
    glm::vec3 clampedScale = glm::abs(newScale);

    // Устанавливаем минимальный порог, чтобы избежать деления на ноль
    // и визуального "исчезновения" объекта
    const float minScale = 0.001f;
    clampedScale = glm::max(clampedScale, glm::vec3(minScale));

    _scale = clampedScale;
}