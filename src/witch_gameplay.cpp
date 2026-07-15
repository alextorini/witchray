#include "witch_enemies.h"
#include "witch_gameplay.h"
#include "witch_fire.h"
#include "witch_gameplay_input_process.h"
#include "witch_background.h"
#include "witch_player.h"
#include "witch_systems.h"


void gameplay_init(Game *game) {
    init_game_background(game);
    game->player.handle = init_player(game);
    game->player_handle = game->player.handle;
    init_enemy_factory(game);
    game->seconds_alive = 0.0f;
    game->enemies_killed = 0;
}

void gameplay_update(InputState *input, Game *game, float dt) {
    gameplay_input_process(input, game, dt);

    system_spawn_enemies(game, dt);
    system_clean_enemies(game);
    cast_fireballs(game, dt);

    system_move_entities(game->components[CMP_POSITION], game->components[CMP_VELOCITY], dt);
    system_animate_entities(game->components[CMP_ANIMATION], game->components[CMP_RENDER], dt);

    system_move_parallax(game->components[CMP_PARALLAX], game->components[CMP_POSITION], game->components[CMP_RENDER]);
    system_collide_enemies(game);
    system_fireballs_collide_enemies(game);
    system_clean_fireballs(game->components[CMP_FIREBALL], game->components[CMP_POSITION]);
    game->seconds_alive += dt;

    game->score = (uint64_t)(game->seconds_alive) + game->enemies_killed * 10;

    if (game->score > game->highscore) game->highscore = game->score;
}

void gameplay_destroy(Game *game) {
}
