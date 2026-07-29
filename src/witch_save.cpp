#include <cstdint>
#include <stdint.h>
#include "smetanka_engine.h"
#include "witch_game.h"

typedef struct {
    uint64_t gameVersion;
    uint64_t highscore;
} Save;

void saveHighscore(uint64_t highscore) {
    Save save = {
        .gameVersion = GAME_VERSION,
        .highscore = highscore
    };

    smeSaveFileData("save.dat", (unsigned char *)&save, sizeof(save));
}

uint64_t loadHighscore() {
    int bytesRead;
    unsigned char *data = smeLoadFileData("save.dat", &bytesRead);

    if (bytesRead != sizeof(Save)) {
        return 0;
    }

    Save save = *(Save *)data;

    smeUnloadFileData(data);

    return save.highscore;
}
