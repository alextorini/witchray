#include <stdint.h>

#include "ext/raylib.h"
#include "ext/raymath.h"

#include "smetanka_ecs.h"
#include "witch_core.h"
#include "witch_systems.h"

void system_move_entities(float delta_time) {
    EcsEntityIterator iterator =
        ecs_get_entity_iterator(CLITERAL(EcsComponentId []){ecs_hndls.cmpnts.pos, ecs_hndls.cmpnts.vel}, 2);

    EcsEntityHandle entity_handle;
    while ((entity_handle = ecs_get_next_entity(&iterator)) != INVALID_HANDLE) {
        Position *position = (Position *)ecs_get_entity_component(ecs_hndls.cmpnts.pos, entity_handle);
        Velocity *velocity = (Velocity *)ecs_get_entity_component(ecs_hndls.cmpnts.vel, entity_handle);

        *position = Vector2Add(*position, Vector2Scale(*velocity, delta_time));
    }
}

void system_move_parallax() {
    EcsEntityIterator iterator =
        ecs_get_entity_iterator(
            CLITERAL(EcsComponentId []){
                ecs_hndls.cmpnts.pos,
                ecs_hndls.cmpnts.prlx,
                ecs_hndls.cmpnts.rndr
            },
            3
        );

    EcsEntityHandle entity_handle;
    while ((entity_handle = ecs_get_next_entity(&iterator)) != INVALID_HANDLE) {
        Position *position = (Position *)ecs_get_entity_component(ecs_hndls.cmpnts.pos, entity_handle);
        Render *render = (Render *)ecs_get_entity_component(ecs_hndls.cmpnts.rndr, entity_handle);

        if (position->x + render->frame.width < 0) {
            position->x += render->frame.width * 2.0f;
        }
    }
}

void system_animate_entities(float delta_time) {
    EcsEntityIterator iterator =
        ecs_get_entity_iterator(CLITERAL(EcsComponentId []){ecs_hndls.cmpnts.anim, ecs_hndls.cmpnts.rndr}, 2);

    EcsEntityHandle entity_handle;
    while ((entity_handle = ecs_get_next_entity(&iterator)) != INVALID_HANDLE) {
        Animation *animation = (Animation*)ecs_get_entity_component(ecs_hndls.cmpnts.anim, entity_handle);
        Render *render = (Render *)ecs_get_entity_component(ecs_hndls.cmpnts.rndr, entity_handle);

        AnimationClip *current_clip = &animation->set->clips[animation->current_clip];
        animation->timer += delta_time;

        if (current_clip->frame_time <= 0.00001f) return;

        while (animation->timer >= current_clip->frame_time) {
            animation->current_frame++;
            if (animation->current_frame > current_clip->end_frame) {
                animation->current_frame = current_clip->start_frame;
            }

            animation->timer -= current_clip->frame_time;
        }

        render->frame.x = render->frame.width * animation->current_frame;
    }
}

void system_render_entities() {
    EcsEntityIterator iterator =
        ecs_get_entity_iterator(CLITERAL(EcsComponentId []){ecs_hndls.cmpnts.pos, ecs_hndls.cmpnts.rndr}, 2);

    EcsEntityHandle entity_handle;
    while ((entity_handle = ecs_get_next_entity(&iterator)) != INVALID_HANDLE) {
        Position *position = (Position*)ecs_get_entity_component(ecs_hndls.cmpnts.pos, entity_handle);
        Render *render = (Render *)ecs_get_entity_component(ecs_hndls.cmpnts.rndr, entity_handle);

        DrawTextureRec(*render->spritesheet, render->frame, *position, WHITE);
    }
}
