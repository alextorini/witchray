#include <stdint.h>
#include "smetanka_engine.h"
#include "witch_game.h"

typedef struct {
    uint64_t gameVersion;
    uint64_t highscore;
} Save;

void saveGame(Game *game) {
    Save save = {
        .gameVersion = GAME_VERSION,
        .highscore = game->highscore
    };

    smeSaveFileData("save.dat", (unsigned char *)&save, sizeof(save));
}

void loadGame(Game *game) {
    int bytesRead;
    unsigned char *data = smeLoadFileData("save.dat", &bytesRead);

    if (bytesRead != sizeof(Save)) {
        return;
    }

    Save save = *(Save *)data;

    game->highscore = save.highscore;

    smeUnloadFileData(data);
}
