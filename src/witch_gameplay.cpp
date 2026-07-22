#include "witch_components.h"
#include "witch_enemies.h"
#include "witch_gameplay.h"
#include "witch_fire.h"
#include "witch_gameplay_input_process.h"
#include "witch_background.h"
#include "witch_player.h"
#include "witch_systems.h"


void gameplayInit(Game *game) {
    initGameBackground(game);
    game->player.handle = initPlayer(game);
    game->player_handle = game->player.handle;
    initEnemyFactory(game);
    game->timer = 0.0f;
    game->enemiesKilled = 0;
    game->enemySpawnCooldown = 1.5f;
    game->skyColor = SKY_COLOR_1;
}

void gameplayUpdate(InputState *input, Game *game, float dt) {
    gameplayInputProcess(input, game, dt);

    if (game->pause) {
        return;
    }

    systemSpawnEnemies(game, dt);
    systemEnemiesFire(game, dt);
    systemCleanEnemies(game);
    castFireballs(game, dt);

    systemMoveEntities(game->components[CMP_POSITION], game->components[CMP_VELOCITY], dt);

    systemAnimateEntities(
        game->components[CMP_ANIMATION],
        game->components[CMP_RENDER],
        game->components[CMP_RENDER],
        dt
    );

    systemMoveParallax(game->components[CMP_PARALLAX], game->components[CMP_POSITION], game->components[CMP_RENDER]);

    systemCollideEnemies(game);

    systemFireballsCollide(game);
    systemCleanFireballs(
        game->components[CMP_FIREBALL],
        game->components[CMP_POSITION],
        game->components[CMP_ENTITY_STATE]
    );

    systemProcessEntityStates(game->components[CMP_ENTITY_STATE], game);
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
