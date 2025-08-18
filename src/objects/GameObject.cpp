#include "GameObject.h"

void GameObject::update(float deltaTime) {
    // Обновление логики
}

void GameObject::render() {
    // Рендеринг с использованием компонентов
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture.getTextureID());

    // Передача матрицы трансформации UV в шейдер
}

TextureComponent& GameObject::getTextureComponent() { return _texture; }
TransformComponent& GameObject::getTransformComponent() { return _transform; }