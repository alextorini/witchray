#include "ext/raylib.h"
#include "witch_components.h"
#include "witch_enemies.h"
#include "witch_gameplay.h"
#include "witch_game.h"
#include "witch_spells.h"
#include "witch_gameplay_input_process.h"
#include "witch_background.h"
#include "witch_pickupable.h"
#include "witch_player.h"
#include "witch_time.h"
#include "witch_systems.h"
#include "witch_system_movement.h"
#include "witch_win.h"

#define INTRO_TEXT_COLOR WHITE
#define INTRO_TEXT_FONT game->resources.fonts[FONT_MAIN]
#define INTRO_TEXT_FONT_SIZE 18
#define INTRO_TEXT_FONT_SPACING 1
#define INTRO_TEXT "SURVIVE for 3 min to get closer to THE TRUTH!"

void gameplayInit(Game *game) {
    initGameBackground(game);

    game->player.handle = initPlayer(game);

    initEnemyFactory(game);

    timeInit(game);

    game->enemiesKilled = 0;
    game->enemySpawnCooldown = SPAWN_COOLDOWN;
    game->skyColor = SKY_COLOR_1;
    game->win = 0;
    game->spawnEnemies = 0;
    game->castSpells = 0;
    game->pause = 0;

    game->pickups.speed = PICKAPABLE_SPEED;

    EcsEntityHandle introLabelHandle = ecsCreateEntity();

    Position introLabelPosition = {.x = 30, .y = 320};
    addComponent(introLabelHandle, &introLabelPosition);

    TextRender introLabelTextRender = {
        .offset = {0, 0},
        .color = WHITE,
        .font = INTRO_TEXT_FONT,
        .fontSize = INTRO_TEXT_FONT_SIZE,
        .spacing = INTRO_TEXT_FONT_SPACING,
        .text = INTRO_TEXT,
    };

    addComponent(introLabelHandle, &introLabelTextRender);

    EntityState entityState = {.id = ENTITY_STATE_DYING, .cooldown = 5.0f};
    addComponent(introLabelHandle, &entityState);

    pickupableInit(game);
}

void gameplayUpdate(InputState *input, Game *game, float dt) {
    gameplayInputProcess(input, game, dt);

    if (game->pause) {
        return;
    }

    enemiesSpawn(game, dt);

    systemEnemiesFire(game, dt);

    systemEnemiesClean();

    castPlayerSpells(game, dt);

    systemMoveEntities(dt);

    systemHomingVelocityUpdate();

    systemMoveOrbit(dt);

    systemAnimateEntities(dt);

    systemMoveParallax();

    systemEnemiesCollide(game);

    systemCollideSpells(game);

    systemPickupablePickup(game);

    systemSpellsClean();

    systemProcessEntityStates(game);

    timeUpdate(game, dt);

    game->score = game->enemiesKilled * 10;

    if (game->score > game->highscore) game->highscore = game->score;
}

void gameplayDestroy(Game *game) {
    ecsClearSpace();
}
