#include "smetanka_engine.h"
#include "witch_animation.h"
#include "witch_components.h"
#include "witch_damage.h"
#include "witch_game.h"
#include "witch_save.h"

#define WIN_TEXT_COLOR WHITE
#define WIN_TEXT_FONT game->resources.fonts[FONT_MAIN]
#define WIN_TEXT_FONT_SIZE 18
#define WIN_TEXT_FONT_SPACING 1
#define WIN_TEXT "You have gotten closer to the truth. Perhaps."

void win(Game *game) {
    game->win = 1;
    game->spawnEnemies = 0;
    game->castSpells = 0;

    smeStopMusicStream(game->resources.music[MUSIC_MAIN]);

    EcsComponentId componentIdList[] = {COMPONENT_ID(Enemy)};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 1);
    EcsEntityHandle handle;
    EntityState *entityState;
    while ((handle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        damage(handle, 100, game);
    }

    componentIdList[0] = COMPONENT_ID(Spell);
    iterator = ecsGetEntityIterator(componentIdList, 1);
    while ((handle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        entityState = getEntityState(handle);
        entityState->id = ENTITY_STATE_DYING;
    }

    Animation *anim = getAnimation(game->player.handle);

    switchAnimation(anim, ANIMATION_WIN);

    EcsEntityHandle labelHandle = ecsCreateEntity();

    Position labelPosition = {.x = 30, .y = 320};
    addComponent(labelHandle, &labelPosition);

    TextRender labelTextRender = {
        .offset = {0, 0},
        .color = WHITE,
        .font = WIN_TEXT_FONT,
        .fontSize = WIN_TEXT_FONT_SIZE,
        .spacing = WIN_TEXT_FONT_SPACING,
        .text = WIN_TEXT,
    };

    addComponent(labelHandle, &labelTextRender);

    EntityState labelState = {.id = ENTITY_STATE_DYING, .cooldown = 10.0f};
    addComponent(labelHandle, &labelState);

    saveHighscore(game->highscore);
    //stateRequestChange(game, STATE_START_SCREEN);
}
