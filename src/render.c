#include "raylib.h"
#include "secs.h"
#include "witch.h"

Texture load_pixel_texture(const char *path) {
    Texture texture = LoadTexture(path);
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);

    return texture;
}

void create_layer_instance(EcsSpace *space, Texture *texture, Position position, Velocity velocity) {
    EcsEntity *primary_layer = ecs_create_entity(space);
    ecs_add_component(primary_layer, ecs_pointers.components.position, &position);
    ecs_add_component(primary_layer, ecs_pointers.components.velocity, &velocity);
    Render temp_render = {texture, {0.0f, 0.0f, 1.0f * texture->width, 1.0f * texture->height}};
    ecs_add_component(primary_layer, ecs_pointers.components.render, &temp_render);
    IsParalax temp_paralax = true;
    ecs_add_component(primary_layer, ecs_pointers.components.paralax, &temp_paralax);
}

void add_parallax_background_layer(EcsSpace *space, Texture *texture, float speed) {
    Position position = {0, 0};
    Velocity velocity = {-speed, 0};

    create_layer_instance(space, texture, position, velocity);

    position.x = texture->width;
    create_layer_instance(space, texture, position, velocity);
}
