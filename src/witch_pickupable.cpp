#include "smetanka_ecs.h"
#include "witch_animation.h"
#include "witch_collisions.h"
#include "witch_components.h"
#include "witch_event.h"
#include "witch_game.h"
#include "witch_resources.h"

static AnimationClip expAnimation;
static AnimationSet expAnimSet;

static AnimationClip hpAnimation;
static AnimationSet hpAnimSet;

#define PICKUPABLE_EXP 1
#define PICKAPABLE_HEALTH 2

void pickupableInit(Game *game) {
    expAnimation.startFrame = 0;
    expAnimation.endFrame = 3;
    expAnimation.frameTime = 0.25;
    expAnimation.loop = 1;

    expAnimSet.clips[ANIMATION_IDLE] = expAnimation;

    initCollisionMap(EXP_CRYSTAL_PATH, {0.0f, 0.0f, 13.0f, 13.0f} , 4, &game->pickups.collisions);
}

void pickupableSpawn(Position position, Game *game) {
    EcsEntityHandle handle = ecsCreateEntity();

    addComponent(handle, &position);


    Velocity velocity = {-game->pickups.speed, 0};
    addComponent(handle, &velocity);

    EntityState entityState = {.id = ENTITY_STATE_IDLE, .cooldown = 0.0f};
    addComponent(handle, &entityState);

    Pickapable pickapable = {.componentId = INVALID_ID};

    Render render;
    if ((rand() % 100) > 90) {
        pickapable.type = PICKAPABLE_HEALTH;
        pickapable.value = 5;
        render =
            {.spritesheet = &game->resources.sprites[SPRITESHEET_HEALTH_CRYSTAL], .frame = {0.0f, 0.0f, 13.0f, 13.0f}};
    } else {
        pickapable.type = PICKUPABLE_EXP;
        pickapable.value = 200;
        render =
            {.spritesheet = &game->resources.sprites[SPRITESHEET_EXP_CRYSTAL], .frame = {0.0f, 0.0f, 13.0f, 13.0f}};
    }

    addComponent(handle, &pickapable);
    addComponent(handle, &render);

    Animation animation;
    animation.set = &expAnimSet;
    switchAnimation(&animation, ANIMATION_IDLE);
    addComponent(handle, &animation);
}

void pickupablePickup(EcsEntityHandle handle, Game *game) {
    Pickapable *pickupable = getPickapable(handle);
    if (pickupable->type == PICKUPABLE_EXP) {
        game->player.exp += pickupable->value;

        eventCreate(game->player.handle, EVENT_PICKUP_EXP, game);

        if (game->player.exp >= game->player.max_exp) {
            eventCreate(game->player.handle, EVENT_LEVEL_UP, game);
        }
    } else if (pickupable->type == PICKAPABLE_HEALTH) {
        Health *playerHealth = getHealth(game->player.handle);
        playerHealth->current += pickupable->value;
        if (playerHealth->current > playerHealth->max) {
            playerHealth->current = playerHealth->max;
        }

        eventCreate(game->player.handle, EVENT_PICKUP_HP, game);
    }

    EntityState *pickState = getEntityState(handle);
    pickState->id = ENTITY_STATE_DIE;

    eventCreate(game->player.handle, EVENT_PICKUP_EXP, game);
}

void systemPickupablePickup(Game *game) {
    EcsEntityHandle playerHandle = game->player.handle;

    EcsComponentId componentIdList[] = {COMPONENT_ID(Pickapable), COMPONENT_ID(Position), COMPONENT_ID(Render)};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 3);

    Position *playerPosition = getPosition(playerHandle);
    Render *playerRender = getRender(playerHandle);
    Animation *playerAnim = getAnimation(playerHandle);

    uint32_t playerFrameIndex = playerAnim ? playerAnim->currentFrame : 0;

    EntityState *playerState = getEntityState(playerHandle);
    EcsEntityHandle pickHandle;
    while ((pickHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        EntityState *pickState = getEntityState(pickHandle);
        if (pickState->id != ENTITY_STATE_IDLE || playerState->id != ENTITY_STATE_IDLE) {
            continue;
        }

        Position *pickPosition = getPosition(pickHandle);
        Render *pickRender = getRender(pickHandle);
        Animation *pickAnim = getAnimation(pickHandle);

        uint32_t pickFrameIndex = pickAnim ? pickAnim->currentFrame : 0;

        if (checkCollision(
            &game->player.collisions, playerFrameIndex, playerPosition->x, playerPosition->y,
            &game->pickups.collisions, pickFrameIndex, pickPosition->x, pickPosition->y
        )) {
            pickupablePickup(pickHandle, game);
        }
    }
}

