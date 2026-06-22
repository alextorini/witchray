#include <stdlib.h>

#include "witch_core.h"

static EcsEntityHandle create_layer_instance(Texture *texture, Position position, Velocity velocity) {
    EcsEntityHandle layer_id = ecs_create_entity();

    ecs_add_component(layer_id, ecs_hndls.cmpnts.pos, &position);
    ecs_add_component(layer_id, ecs_hndls.cmpnts.vel, &velocity);

    Render render = {texture, {0.0f, 0.0f, 1.0f * texture->width, 1.0f * texture->height}};
    ecs_add_component(layer_id, ecs_hndls.cmpnts.rndr, &render);

    IsParallax is_parallax = true;
    ecs_add_component(layer_id, ecs_hndls.cmpnts.prlx, &is_parallax);

    return layer_id;
}

EcsEntityHandle *add_parallax_background_layer(Texture *texture, float speed) {
    Position position = {0, 0};
    Velocity velocity = {-speed, 0};

    EcsEntityHandle *layers = (EcsEntityHandle *)malloc(sizeof(EcsEntityHandle) * 2);

    layers[0] = create_layer_instance(texture, position, velocity);

    position.x = texture->width;
    layers[1] = create_layer_instance(texture, position, velocity);

    return layers;
}
