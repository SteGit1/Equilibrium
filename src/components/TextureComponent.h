#pragma once

#include <glm/glm.hpp>
#include <string>
#include <glad.h>
#include <matrix_transform.hpp>
#include <algorithm>

class TextureComponent {
private:
	GLuint _textureID = 0;

	bool _isOnlyColorTexturing = false; // Флаг, указывающий на то, что текстурирование осуществляется только цветом
	glm::vec4 _color = { 0.0f, 0.0f, 0.0f, 0.0f };

	std::string _texturePath;

	glm::vec2 _offset = { 0.0f, 0.0f };
	glm::vec3 _scale = { 1.0f, 1.0f, 1.0f };
	glm::vec3 _rotation = { 0.0f, 0.0f, 0.0f };
	glm::mat4 _uvMatrix = glm::mat4(1.0f);
	bool _isDirty = true;

	void updateMatrix();

public:
	void setTexture(const std::string& texturePath);
	void setRotation(const glm::vec3& eulerAngles);
	void setScale(const glm::vec3& newScale);
	void setOffset(const glm::vec2& offset);

	GLuint getTextureID() const { return _textureID; }
	const glm::mat4& getUVMatrix();
};