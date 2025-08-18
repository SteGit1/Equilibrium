#pragma once
#include "TextureComponent.h"
#include "TransformComponent.h"
#include "PolygonEntity.h"

class GameObject {
private:
    TransformComponent _transform;
    TextureComponent _texture;
    PolygonEntity** _polygons;

public:
    void update(float deltaTime);

    void render();

    TextureComponent& getTextureComponent();
    TransformComponent& getTransformComponent();
};