#include <stdlib.h>

#include "witch_background.h"

static EcsEntityHandle create_layer_instance(Texture *texture, Position position, Velocity velocity, Game *game) {
    EcsEntityHandle layer_id = ecs_create_entity();

    ecs_add_component(layer_id, game->components[CMP_POSITION], &position);
    ecs_add_component(layer_id, game->components[CMP_VELOCITY], &velocity);

    Render render = {texture, {0.0f, 0.0f, 1.0f * texture->width, 1.0f * texture->height}};
    ecs_add_component(layer_id, game->components[CMP_RENDER], &render);

    IsParallax is_parallax = true;
    ecs_add_component(layer_id, game->components[CMP_PARALLAX], &is_parallax);

    return layer_id;
}

static EcsEntityHandle *add_parallax_background_layer(Texture *texture, float speed, Game *game) {
    Position position = {0, 0};
    Velocity velocity = {-speed, 0};

    EcsEntityHandle *layers = (EcsEntityHandle *)malloc(sizeof(EcsEntityHandle) * 2);

    layers[0] = create_layer_instance(texture, position, velocity, game);

    position.x = texture->width;
    layers[1] = create_layer_instance(texture, position, velocity, game);

    return layers;
}

void init_game_background(Game *game) {
    EcsEntityHandle *layer_copies;
    layer_copies = add_parallax_background_layer(&game->resources.sprites[BACKGROUND_CLOUDS], BG_LAYER_1_SPEED, game);
    game->backgrounds[0][0] = layer_copies[0];
    game->backgrounds[0][1] = layer_copies[1];
    layer_copies = add_parallax_background_layer(&game->resources.sprites[BACKGROUND_URBAN], BG_LAYER_2_SPEED, game);
    game->backgrounds[1][0] = layer_copies[0];
    game->backgrounds[1][1] = layer_copies[1];
}
