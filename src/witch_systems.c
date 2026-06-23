#include <stdlib.h>
#include <stdint.h>

#include "ext/raylib.h"
#include "ext/raymath.h"

#include "smetanka_ecs.h"
#include "witch_core.h"
#include "witch_systems.h"

// void system_process_input() {
//     EcsEntityHandle plr_id = ecs_hndls.entts.plr;
//     Position *plr_pos = (Position *)ecs_get_entity_component(ecs_hndls.cmpnts.pos, plr_id);
//     Render *plr_rndr = (Render *)ecs_get_entity_component(ecs_hndls.cmpnts.rndr, plr_id);
//
//     float dx = 0.0;
//     float dy = 0.0;
//     if (IsKeyDown(KEY_LEFT)) dx--;
//     if (IsKeyDown(KEY_RIGHT)) dx++;
//     if (IsKeyDown(KEY_UP)) dy--;
//     if (IsKeyDown(KEY_DOWN)) dy++;
//
//     if (IsGamepadAvailable(0)) {
//         dx += GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
//         dy += GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
//
//         if (dx <= GAMEPAD_DEADZONE && dx > -GAMEPAD_DEADZONE) dx = 0;
//         if (dy <= GAMEPAD_DEADZONE && dy > -GAMEPAD_DEADZONE) dy = 0;
//     }
//
//     if (dx > 1.0) dx = 1.0;
//     if (dx < -1.0) dx = -1.0;
//     if (dy > 1.0) dy = 1.0;
//     if (dy < -1.0) dy = -1.0;
//
//     if (dx != 0 && dy != 0) {
//         float len = sqrt(dx * dx + dy * dy);
//         dx = dx / len;
//         dy = dy / len;
//     }
//
//     plr_pos->x += dx * PLAYER_SPEED;
//     plr_pos->y += dy * PLAYER_SPEED;
//
//     if (plr_pos->x < 1) plr_pos->x = 1;
//     if (plr_pos->x > VIRTUAL_WIDTH - plr_rndr->frame.width - 1) plr_pos->x = VIRTUAL_WIDTH - plr_rndr->frame.width - 1;
//     if (plr_pos->y < 1) plr_pos->y = 1;
//     if (plr_pos->y > VIRTUAL_HEIGHT - plr_rndr->frame.height - 1) plr_pos->y = VIRTUAL_HEIGHT - plr_rndr->frame.height - 1;
// }

void system_move_entities(float delta_time) {
    EcsComponentId pos_cmp_id = ecs_hndls.cmpnts.pos;
    EcsComponentId vel_cmp_id = ecs_hndls.cmpnts.vel;

    uint32_t vel_cmp_count = ecs_get_component_count(vel_cmp_id);

    for (uint32_t i = 0; i < vel_cmp_count; i++) {
        EcsEntityHandle ent_id = ecs_get_component_dense(vel_cmp_id, i);
        if (ent_id == INVALID_ID) {
            abort();
        }

        Position *ent_pos = (Position *)ecs_get_entity_component(pos_cmp_id, ent_id);
        if (!ent_pos) {
            continue;
        }

        Velocity *ent_vel = (Velocity *)ecs_get_entity_component(vel_cmp_id, ent_id);
        if (!ent_vel) {
            continue;
        }

        *ent_pos = Vector2Add(*ent_pos, Vector2Scale(*ent_vel, delta_time));
    }
}

void system_move_parallax() {
    EcsComponentId pos_cmp = ecs_hndls.cmpnts.pos;
    EcsComponentId rndr_cmp_id = ecs_hndls.cmpnts.rndr;
    EcsComponentId prlx_cmp_id = ecs_hndls.cmpnts.prlx;

    uint32_t prlx_cmp_count = ecs_get_component_count(prlx_cmp_id);

    for (uint32_t i = 0; i < prlx_cmp_count; i++) {
        EcsEntityHandle ent_id = ecs_get_component_dense(prlx_cmp_id, i);
        if (ent_id == INVALID_ID) {
            abort();
        }

        Position *ent_pos = (Position *)ecs_get_entity_component(pos_cmp, ent_id);
        if (!ent_pos) {
            continue;
        }

        Render *ent_rndr = (Render *)ecs_get_entity_component(rndr_cmp_id,  ent_id);
        if (!ent_rndr) {
            continue;
        }

        IsParallax *ent_prlx = (IsParallax *)ecs_get_entity_component(prlx_cmp_id, ent_id);
        if (!ent_prlx) {
            continue;
        }

        if (ent_pos->x + ent_rndr->frame.width < 0) {
            ent_pos->x += ent_rndr->frame.width * 2.0f;
        }
    }
}

void system_animate_entities(float delta_time) {
    EcsComponentId anim_cmp_id = ecs_hndls.cmpnts.anim;
    EcsComponentId rndr_cmp_id = ecs_hndls.cmpnts.rndr;

    uint32_t anim_cmp_count = ecs_get_component_count(anim_cmp_id);

    for (int i = 0; i < anim_cmp_count; i++) {
        EcsEntityHandle ent_id = ecs_get_component_dense(anim_cmp_id, i);
        if (ent_id == INVALID_ID) {
            abort();
        }

        Animation *ent_anim = (Animation *)ecs_get_entity_component(anim_cmp_id, ent_id);
        if (!ent_anim) {
            continue;
        }

        Render *ent_rndr = (Render *)ecs_get_entity_component(rndr_cmp_id,  ent_id);
        if (!ent_rndr) {
            continue;
        }

        AnimationClip *cur_clp = &ent_anim->set->clips[ent_anim->current_clip];
        ent_anim->timer += delta_time;

        if (cur_clp->frame_time <= 0.00001f) return;

        while (ent_anim->timer >= cur_clp->frame_time) {
            ent_anim->current_frame++;
            if (ent_anim->current_frame > cur_clp->end_frame) {
                ent_anim->current_frame = cur_clp->start_frame;
            }

            ent_anim->timer -= cur_clp->frame_time;
        }

        ent_rndr->frame.x = ent_rndr->frame.width * ent_anim->current_frame;
    }
}

void system_render_entities() {
    EcsComponentId pos_cmp_id = ecs_hndls.cmpnts.pos;
    EcsComponentId rndr_cmp_id = ecs_hndls.cmpnts.rndr;

    uint32_t rndr_cmp_count = ecs_get_component_count(rndr_cmp_id);

    Position draw_pos;
    for (int i = 0; i < rndr_cmp_count; i++) {
        EcsEntityHandle ent_id = ecs_get_component_dense(rndr_cmp_id, i);
        if (ent_id == INVALID_ID) {
            abort();
        }

        Position *ent_pos = (Position *)ecs_get_entity_component(pos_cmp_id, ent_id);
        if (!ent_pos) {
            continue;
        }

        Render *ent_rndr = (Render *)ecs_get_entity_component(rndr_cmp_id,  ent_id);
        if (!ent_rndr) {
            continue;
        }

        draw_pos.x = /* (float)(int)floorf */(ent_pos->x);
        draw_pos.y = /* (float)(int)floorf */(ent_pos->y);
        DrawTextureRec(*ent_rndr->spritesheet, ent_rndr->frame, draw_pos, WHITE);
    }
}
