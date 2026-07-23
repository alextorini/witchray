#include <stdlib.h>

#include "witch_background.h"
#include "witch_components.h"

static EcsEntityHandle createLayerInstance(Texture *texture, Position position, Velocity velocity, Game *game) {
    EcsEntityHandle layerId = ecsCreateEntity();

    ecsAddComponent(layerId, game->components[CMP_POSITION], &position);
    ecsAddComponent(layerId, game->components[CMP_VELOCITY], &velocity);

    Render render = {texture, {0.0f, 0.0f, 1.0f * texture->width, 1.0f * texture->height}};
    ecsAddComponent(layerId, game->components[CMP_RENDER], &render);

    IsParallax isParallax = true;
    ecsAddComponent(layerId, game->components[CMP_PARALLAX], &isParallax);

    return layerId;
}

static EcsEntityHandle *addParallaxBackgroundLayer(Texture *texture, float speed, Game *game) {
    Position position = {0, 0};
    Velocity velocity = {-speed, 0};

    EcsEntityHandle *layers = (EcsEntityHandle *)malloc(sizeof(EcsEntityHandle) * 2);

    layers[0] = createLayerInstance(texture, position, velocity, game);

    position.x = texture->width;
    layers[1] = createLayerInstance(texture, position, velocity, game);

    return layers;
}

void initGameBackground(Game *game) {
    EcsEntityHandle *layerCopies;
    layerCopies = addParallaxBackgroundLayer(&game->resources.sprites[BACKGROUND_CLOUDS], BG_LAYER_1_SPEED, game);
    game->backgrounds[0][0] = layerCopies[0];
    game->backgrounds[0][1] = layerCopies[1];
    layerCopies = addParallaxBackgroundLayer(&game->resources.sprites[BACKGROUND_URBAN], BG_LAYER_2_SPEED, game);
    game->backgrounds[1][0] = layerCopies[0];
    game->backgrounds[1][1] = layerCopies[1];
}
