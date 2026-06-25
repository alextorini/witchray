#include <stdint.h>

#include "ext/raylib.h"
#include "ext/raymath.h"

#include "smetanka_ecs.h"
#include "witch_core.h"
#include "witch_systems.h"

void system_move_entities(EcsComponentId position_id, EcsComponentId velocity_id, float delta_time) {
    EcsEntityIterator iterator =
        ecs_get_entity_iterator(CLITERAL(EcsComponentId []){position_id, velocity_id}, 2);

    EcsEntityHandle entity_handle;
while ((entity_handle = ecs_get_next_entity(&iterator)) != INVALID_HANDLE) {
        Position *position = (Position *)ecs_get_entity_component(position_id, entity_handle);
        Velocity *velocity = (Velocity *)ecs_get_entity_component(velocity_id, entity_handle);

        *position = Vector2Add(*position, Vector2Scale(*velocity, delta_time));
    }
}

void system_move_parallax(EcsComponentId parallax_id, EcsComponentId position_id, EcsComponentId render_id) {
    EcsEntityIterator iterator =
        ecs_get_entity_iterator(
            CLITERAL(EcsComponentId []){
                position_id,
                parallax_id,
                render_id
            },
            3
        );

    EcsEntityHandle entity_handle;
    while ((entity_handle = ecs_get_next_entity(&iterator)) != INVALID_HANDLE) {
        Position *position = (Position *)ecs_get_entity_component(position_id, entity_handle);
        Render *render = (Render *)ecs_get_entity_component(render_id, entity_handle);

        if (position->x + render->frame.width < 0) {
            position->x += render->frame.width * 2.0f;
        }
    }
}

void system_animate_entities(EcsComponentId animation_id, EcsComponentId render_id, float delta_time) {
    EcsEntityIterator iterator =
        ecs_get_entity_iterator(CLITERAL(EcsComponentId []){animation_id, render_id}, 2);

    EcsEntityHandle entity_handle;
    while ((entity_handle = ecs_get_next_entity(&iterator)) != INVALID_HANDLE) {
        Animation *animation = (Animation*)ecs_get_entity_component(animation_id, entity_handle);
        Render *render = (Render *)ecs_get_entity_component(render_id, entity_handle);

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

void system_render_entities(EcsComponentId position_id, EcsComponentId render_id) {
    EcsEntityIterator iterator =
        ecs_get_entity_iterator(CLITERAL(EcsComponentId []){position_id, render_id}, 2);

    EcsEntityHandle entity_handle;
    while ((entity_handle = ecs_get_next_entity(&iterator)) != INVALID_HANDLE) {
        Position *position = (Position*)ecs_get_entity_component(position_id, entity_handle);
        Render *render = (Render *)ecs_get_entity_component(render_id, entity_handle);

        DrawTextureRec(*render->spritesheet, render->frame, *position, WHITE);
    }
}
