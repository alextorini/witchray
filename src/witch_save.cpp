#include <stdint.h>
#include "smetanka_engine.h"
#include "witch_game.h"

typedef struct {
    uint64_t game_version;
    uint64_t highscore;
} Save;

void save_game(Game *game) {
    Save save = {
        .game_version = GAME_VERSION,
        .highscore = game->highscore
    };

    save_file_data("save.dat", (unsigned char *)&save, sizeof(save));
}

void load_game(Game *game) {
    int bytes_read;
    unsigned char *data = load_file_data("save.dat", &bytes_read);

    if (bytes_read != sizeof(Save)) {
        return;
    }

    Save save = *(Save *)data;

    game->highscore = save.highscore;

    unload_file_data(data);
}
