#include <stdlib.h>

#include "witch_render.h"
#include "witch_core.h"

Texture load_pixel_texture(const char *path) {
    Texture texture = LoadTexture(path);
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);

    return texture;
}

static EcsEntity *create_layer_instance(EcsSpace *space, Texture *texture, Position position, Velocity velocity) {
    EcsEntity *layer = ecs_create_entity(space);

    ecs_add_component(layer, ecs_ptrs.cmpnts.pos, &position);
    ecs_add_component(layer, ecs_ptrs.cmpnts.vel, &velocity);

    Render render = {texture, {0.0f, 0.0f, 1.0f * texture->width, 1.0f * texture->height}};
    ecs_add_component(layer, ecs_ptrs.cmpnts.rndr, &render);

    IsParallax is_parallax = true;
    ecs_add_component(layer, ecs_ptrs.cmpnts.prlx, &is_parallax);

    return layer;
}

EcsEntity **add_parallax_background_layer(EcsSpace *space, Texture *texture, float speed) {
    Position position = {0, 0};
    Velocity velocity = {-speed, 0};

    EcsEntity **layers = (EcsEntity **)malloc(sizeof(EcsEntity) * 2);

    layers[0] = create_layer_instance(space, texture, position, velocity);

    position.x = texture->width;
    layers[1] = create_layer_instance(space, texture, position, velocity);

    return layers;
}
