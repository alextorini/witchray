#include <stdint.h>

#include "ext/raylib.h"
#include "ext/raymath.h"
#include "smetanka_ecs.h"
#include "witch_systems.h"
#include "witch_components.h"

void system_move_entities(EcsComponentId position_id, EcsComponentId velocity_id, float delta_time) {
    EcsComponentId component_id_list[] = {position_id, velocity_id};
    EcsEntityIterator iterator = ecs_get_entity_iterator(component_id_list, 2);

    EcsEntityHandle entity_handle;
    while ((entity_handle = ecs_get_next_entity(&iterator)) != INVALID_HANDLE) {
        Position *position = (Position *)ecs_get_entity_component(position_id, entity_handle);
        Velocity *velocity = (Velocity *)ecs_get_entity_component(velocity_id, entity_handle);

        *position = Vector2Add(*position, Vector2Scale(*velocity, delta_time));
    }
}

void system_move_parallax(EcsComponentId parallax_id, EcsComponentId position_id, EcsComponentId render_id) {
    EcsComponentId component_id_list[] = {parallax_id, position_id, render_id};
    EcsEntityIterator iterator = ecs_get_entity_iterator(component_id_list, 3);

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
    EcsComponentId component_id_list[] = {animation_id, render_id};
    EcsEntityIterator iterator = ecs_get_entity_iterator(component_id_list, 2);

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

        render->frame.x = (int)(render->frame.width * animation->current_frame) % render->spritesheet->width;
        render->frame.y = (/*NOLINT*/(int)(render->frame.width * animation->current_frame) /
            render->spritesheet->width) * render->frame.height;
    }
}

void system_render_entities(EcsComponentId position_id, EcsComponentId render_id) {
    EcsComponentId component_id_list[] = {position_id, render_id};
    EcsEntityIterator iterator = ecs_get_entity_iterator(component_id_list, 2);

    EcsEntityHandle entity_handle;
    while ((entity_handle = ecs_get_next_entity(&iterator)) != INVALID_HANDLE) {
        Position *position = (Position*)ecs_get_entity_component(position_id, entity_handle);
        Render *render = (Render *)ecs_get_entity_component(render_id, entity_handle);

        DrawTextureRec(*render->spritesheet, render->frame, *position, WHITE);
    }
}

void system_render_text(EcsComponentId position_id, EcsComponentId text_render_id) {
    EcsComponentId component_id_list[] = {position_id, text_render_id};
    EcsEntityIterator iterator = ecs_get_entity_iterator(component_id_list, 2);

    EcsEntityHandle entity_handle;
    while ((entity_handle = ecs_get_next_entity(&iterator)) != INVALID_HANDLE) {
        Position *position = (Position*)ecs_get_entity_component(position_id, entity_handle);
        TextRender *text_render = (TextRender *)ecs_get_entity_component(text_render_id, entity_handle);

        DrawTextEx(
            text_render->font,
            text_render->text,
            Vector2Add(*position, text_render->offset),
            text_render->font_size,
            text_render->spacing,
            text_render->color
        );
    }
}
