#include "witch_enemies.h"
#include "witch_gameplay.h"
#include "witch_spells.h"
#include "witch_gameplay_input_process.h"
#include "witch_background.h"
#include "witch_pickupable.h"
#include "witch_player.h"
#include "witch_systems.h"
#include "witch_system_movement.h"

void gameplayInit(Game *game) {
    initGameBackground(game);
    game->player.handle = initPlayer(game);
    game->player_handle = game->player.handle;
    initEnemyFactory(game);
    game->timer = 0.0f;
    game->enemiesKilled = 0;
    game->enemySpawnCooldown = SPAWN_COOLDOWN;
    game->skyColor = SKY_COLOR_1;

    game->pickups.speed = PICKAPABLE_SPEED;

    pickupableInit(game);
}

void gameplayUpdate(InputState *input, Game *game, float dt) {
    gameplayInputProcess(input, game, dt);

    if (game->pause) {
        return;
    }

    systemSpawnEnemies(game, dt);

    systemEnemiesFire(game, dt);

    systemCleanEnemies();

    castPlayerSpells(game, dt);

    systemMoveEntities(dt);

    systemHomingVelocityUpdate();

    systemMoveOrbit(dt);

    systemAnimateEntities(dt);

    systemMoveParallax();

    systemCollideEnemies(game);

    systemCollideSpells(game);

    systemPickupablePickup(game);

    systemCleanFireballs();

    systemProcessEntityStates(game);

    if (game->timer > 30.0f) {
        game->skyColor = SKY_COLOR_2;
    }

    game->timer += dt;

    game->score = (uint64_t)(game->timer) + game->enemiesKilled * 10;

    if (game->score > game->highscore) game->highscore = game->score;
}

void gameplayDestroy(Game *game) {
    ecsClearSpace();
}
