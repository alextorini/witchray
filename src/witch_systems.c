#include "ext/raylib.h"
#include "ext/raymath.h"

#include "smetanka_ecs.h"
#include "witch_core.h"
#include <stdint.h>
#include "witch_systems.h"

void system_move_entities() {
    EcsComponent *pos_cmp = ecs_ptrs.cmpnts.pos;
    EcsComponent *vel_cmp = ecs_ptrs.cmpnts.vel;

    EcsComponent *least_used_cmp = (pos_cmp->count < vel_cmp->count) ? pos_cmp : vel_cmp;

    for (uint32_t i = 1; i < least_used_cmp->count; i++) {
        uint32_t ent_id = least_used_cmp->dense_ids[i];

        Position *ent_pos = (Position *)ecs_get_entity_component(pos_cmp, ent_id);
        if (!ent_pos) {
            continue;
        }

        Velocity *ent_vel = (Velocity *)ecs_get_entity_component(vel_cmp, ent_id);
        if (!ent_vel) {
            continue;
        }

        *ent_pos = Vector2Add(*ent_pos, Vector2Scale(*ent_vel, GetFrameTime()));
    }
}

void system_move_parallax() {
    EcsComponent *pos_cmp = ecs_ptrs.cmpnts.pos;
    EcsComponent *rndr_cmp = ecs_ptrs.cmpnts.rndr;
    EcsComponent *prlx_cmp = ecs_ptrs.cmpnts.prlx;

    EcsComponent *least_used_cmp =
        (prlx_cmp->count < pos_cmp->count)
        ? ((prlx_cmp->count < rndr_cmp->count) ? prlx_cmp : rndr_cmp)
        : ((pos_cmp->count < rndr_cmp->count) ? pos_cmp : rndr_cmp);

    for (uint32_t i = 1; i < least_used_cmp->count; i++) {
        uint32_t ent_id = least_used_cmp->dense_ids[i];

        Position *ent_pos = (Position *)ecs_get_entity_component(pos_cmp, ent_id);
        if (!ent_pos) {
            continue;
        }

        Render *ent_rndr = (Render *)ecs_get_entity_component(rndr_cmp,  ent_id);
        if (!ent_rndr) {
            continue;
        }

        IsParallax *ent_prlx = (IsParallax *)ecs_get_entity_component(prlx_cmp, ent_id);
        if (!ent_prlx) {
            continue;
        }

        if (ent_pos->x + ent_rndr->frame.width < 0) {
            ent_pos->x += ent_rndr->frame.width * 2.0f;
        }
    }
}

void system_animate_entities() {
    EcsComponent * anim_cmp = ecs_ptrs.cmpnts.anim;
    EcsComponent *rndr_cmp = ecs_ptrs.cmpnts.rndr;

    EcsComponent *least_used_cmp = (anim_cmp->count < rndr_cmp->count) ? anim_cmp : rndr_cmp;

    for (int i = 1; i < least_used_cmp->count; i++) {
        uint32_t ent_id = least_used_cmp->dense_ids[i];

        Animation *ent_anim = (Animation *)ecs_get_entity_component(anim_cmp, ent_id);
        if (!ent_anim) {
            continue;
        }

        Render *ent_rndr = (Render *)ecs_get_entity_component(rndr_cmp,  ent_id);
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
    EcsComponent *pos_cmp = ecs_ptrs.cmpnts.pos;
    EcsComponent *rndr_cmp = ecs_ptrs.cmpnts.rndr;

    EcsComponent *least_used_cmp = (pos_cmp->count < rndr_cmp->count) ? pos_cmp : rndr_cmp;

    Position draw_pos;
    for (int i = 1; i < least_used_cmp->count; i++) {
        uint32_t ent_id = least_used_cmp->dense_ids[i];

        Position *ent_pos = (Position *)ecs_get_entity_component(pos_cmp, ent_id);
        if (!ent_pos) {
            continue;
        }

        Render *ent_rndr = (Render *)ecs_get_entity_component(rndr_cmp,  ent_id);
        if (!ent_rndr) {
            continue;
        }

        draw_pos.x = (float)(int)floorf(ent_pos->x);
        draw_pos.y = (float)(int)floorf(ent_pos->y);
        DrawTextureRec(*ent_rndr->spritesheet, ent_rndr->frame, draw_pos, WHITE);
    }
}
