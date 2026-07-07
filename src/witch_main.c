#include "witch_core.h"
#include "smetanka_engine.h"

int main() {
    SmetankaInitData init_data = {
        .window_name = "Witch Alice",
        .window_width = WINDOW_WIDTH,
        .window_height = WINDOW_HEIGHT,
        .virtual_width = VIRTUAL_WIDTH,
        .virutal_height = VIRTUAL_HEIGHT,
        .config_flags = FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI,
        .fps = 60,
        .resource_dir = "res",
        .fullscreen = 0
    };
    smetanka_init(&init_data);

    Game *game = WR_MALLOC_TYPE(Game);

    init(game);

    while (!window_should_close() && !should_close) {
        begin_texture_mode();

        update_and_draw(game, get_dt());

        end_texture_mode();

        smetanka_render();
    }

    unload(game);

    return 0;
}
