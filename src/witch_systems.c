#include <stdlib.h>
#include <stdint.h>
#include "ext/raylib.h"
#include "ext/raymath.h"

#include "smetanka_ecs.h"
#include "witch_core.h"
#include "witch_systems.h"

void system_move_entities() {
    EcsComponentId pos_cmp_id = ecs_hndls.cmpnts.pos;
    EcsComponentId vel_cmp_id = ecs_hndls.cmpnts.vel;

    int32_t vel_cmp_count = ecs_get_component_count(vel_cmp_id);

    for (uint32_t i = 1; i < vel_cmp_count; i++) {
        uint32_t ent_id = ecs_get_component_dense(vel_cmp_id, i);
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

        *ent_pos = Vector2Add(*ent_pos, Vector2Scale(*ent_vel, GetFrameTime()));
    }
}

void system_move_parallax() {
    EcsComponentId pos_cmp = ecs_hndls.cmpnts.pos;
    EcsComponentId rndr_cmp_id = ecs_hndls.cmpnts.rndr;
    EcsComponentId prlx_cmp_id = ecs_hndls.cmpnts.prlx;

    int32_t prlx_cmp_count = ecs_get_component_count(prlx_cmp_id);

    for (uint32_t i = 1; i < prlx_cmp_count; i++) {
        uint32_t ent_id = ecs_get_component_dense(prlx_cmp_id, i);
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

void system_animate_entities() {
    EcsComponentId anim_cmp_id = ecs_hndls.cmpnts.anim;
    EcsComponentId rndr_cmp_id = ecs_hndls.cmpnts.rndr;

    int32_t anim_cmp_count = ecs_get_component_count(anim_cmp_id);

    for (int i = 1; i < anim_cmp_count; i++) {
        uint32_t ent_id = ecs_get_component_dense(anim_cmp_id, i);
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
        ent_anim->timer += GetFrameTime();

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

    int32_t rndr_cmp_count = ecs_get_component_count(rndr_cmp_id);

    Position draw_pos;
    for (int i = 1; i < rndr_cmp_count; i++) {
        uint32_t ent_id = ecs_get_component_dense(rndr_cmp_id, i);
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

        draw_pos.x = (float)(int)floorf(ent_pos->x);
        draw_pos.y = (float)(int)floorf(ent_pos->y);
        DrawTextureRec(*ent_rndr->spritesheet, ent_rndr->frame, draw_pos, WHITE);
    }
}
