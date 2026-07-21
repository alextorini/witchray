#include <stdint.h>

#include "smetanka_ecs.h"
#include "witch_animation.h"
#include "witch_components.h"
#include "witch_fire.h"
#include "witch_game.h"
#include "witch_player.h"
#include "witch_resources.h"

#define PLAYER_MAX_VELOCITY 200.0
#define PLAYER_ACCELERATION 1000.0
#define PLAYER_BRAKING  500.0

#define PLAYER_DEATH_COOLDOWN 1.0f

static AnimationClip idleAnimation;
static AnimationClip deathAnimation;

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

    deathAnimation.startFrame = 4;
    deathAnimation.endFrame = 5;
    deathAnimation.frameTime = 0.25f;
    deathAnimation.loop = 1;

    animationSet.clips[ANIMATION_IDLE] = idleAnimation;
    animationSet.clips[ANIMATION_DYING] = deathAnimation;

    Animation animation;
    animation.set = &animationSet;
    switchAnimation(&animation, ANIMATION_IDLE);
    ecsAddComponent(handle, game->components[CMP_ANIMATION], &animation);

    initCollisionMap(PLAYER_IMAGE_PATH, render.frame, 6, &game->player.collisions);
    initFireballs(game);

    game->player.deathCooldown = PLAYER_DEATH_COOLDOWN;

    return handle;
}

