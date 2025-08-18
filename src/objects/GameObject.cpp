#include "GameObject.h"

void GameObject::update(float deltaTime) {
    // ���������� ������
}

void GameObject::render() {
    // ��������� � �������������� �����������
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture.getTextureID());

    // �������� ������� ������������� UV � ������
}

TextureComponent& GameObject::getTextureComponent() { return _texture; }
TransformComponent& GameObject::getTransformComponent() { return _transform; }