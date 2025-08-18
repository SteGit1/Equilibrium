#include <TextureComponent.h>
#include <TextureManager.h>

void TextureComponent::updateMatrix() {
	if (_isDirty) {
		_uvMatrix = glm::mat4(1.0f);
		_uvMatrix = glm::translate(_uvMatrix, glm::vec3(_offset, 0.0f));
		_uvMatrix = glm::rotate(_uvMatrix, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		_uvMatrix = glm::rotate(_uvMatrix, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		_uvMatrix = glm::rotate(_uvMatrix, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		_uvMatrix = glm::scale(_uvMatrix, _scale);
		_isDirty = false;
	}
}

// Getters with lazy update matrix
const glm::mat4& TextureComponent::getUVMatrix() {
	TextureComponent::updateMatrix();
    return _uvMatrix;
}

// Setters with make dirty flag
void TextureComponent::setOffset(const glm::vec2& newOffset) {
    _offset = newOffset;
    _isDirty = true;
}
void TextureComponent::setRotation(const glm::vec3& newRotation) {
    _rotation = newRotation;
    _isDirty = true;
}
void TextureComponent::setScale(const glm::vec3& newScale) {
    _scale = newScale;
    _isDirty = true;
}
void TextureComponent::setTexture(const std::string& path) {
	_texturePath = path;
	_textureID = TextureManager::getTexture(path);
	_isDirty = true;
}
void TextureComponent::setOffset(const glm::vec2& offset) {
	_offset = offset;
	_isDirty = true;
}