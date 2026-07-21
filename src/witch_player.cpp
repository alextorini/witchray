#include <stdint.h>

#include "smetanka_ecs.h"
#include "witch_components.h"
#include "witch_fire.h"
#include "witch_game.h"
#include "witch_player.h"
#include "witch_resources.h"

#define PLAYER_MAX_VELOCITY 200.0
#define PLAYER_ACCELERATION 1000.0
#define PLAYER_BRAKING  500.0

static AnimationClip idleAnimation;
static AnimationSet animationSet;

EcsEntityHandle initPlayer(Game *game) {
    EcsEntityHandle handle = ecsCreateEntity();

    Position position = PLAYER_START_POS;
    ecsAddComponent(handle, game->components[CMP_POSITION], &position);

    Velocity velocity = {0.0, 0.0};
    ecsAddComponent(handle, game->components[CMP_VELOCITY], &velocity);

    Health health = {.current = PLAYER_MAX_HEALTH, .max = PLAYER_MAX_HEALTH};
    ecsAddComponent(handle, game->components[CMP_HEALTH], &health);

    EntityState state = {.id = ENTITY_STATE_IDLE, .cooldown = 0};
    ecsAddComponent(handle, game->components[CMP_ENTITY_STATE], &state);

    Render render = {&game->resources.sprites[SPRITESHEET_PLAYER], PLAYER_DEFAULT_FRAME};
    ecsAddComponent(handle, game->components[CMP_RENDER], &render);

    idleAnimation.startFrame = 0;
    idleAnimation.endFrame = 1;
    idleAnimation.frameTime = PLAYER_IDLE_ANIM_SPEED;
    idleAnimation.loop = 1;

    animationSet.clips = WR_MALLOC_TYPE(AnimationClip);
    animationSet.count = 1;
    animationSet.clips[0] = idleAnimation;

    Animation animation;
    animation.set = &animationSet;
    animation.currentClip = 0;
    animation.currentFrame = 0;
    animation.timer = 0.0;
    ecsAddComponent(handle, game->components[CMP_ANIMATION], &animation);

    initCollisionMap(PLAYER_IMAGE_PATH, render.frame, 6, &game->player.collisions);
    initFireballs(game);

    return handle;
}

