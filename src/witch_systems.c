#include "ext/raylib.h"
#include "ext/raymath.h"

#include "smetanka_ecs.h"
#include "witch_core.h"
#include <stdint.h>
#include "witch_systems.h"

void system_move_entities() {
    Position *ent_pos;
    Velocity *ent_vel;

    EcsComponent *pos_cmp = ecs_ptrs.cmpnts.pos;
    EcsComponent *vel_cmp = ecs_ptrs.cmpnts.vel;

    EcsComponent *least_used_cmp = (pos_cmp->count < vel_cmp->count) ? pos_cmp : vel_cmp;

    for (uint32_t i = 1; i < least_used_cmp->count; i++) {
        uint32_t id = least_used_cmp->dense_ids[i];

        ent_pos = (Position *)pos_cmp->data + pos_cmp->sparse_ids[id];
        if (!ent_pos) {
            continue;
        }

        ent_vel = (Velocity *)vel_cmp->data + vel_cmp->sparse_ids[id];
        if (!ent_vel) {
            continue;
        }

        *ent_pos = Vector2Add(*ent_pos, Vector2Scale(*ent_vel, GetFrameTime()));
    }
}

void system_move_parallax() {
    IsParallax *ent_prlx;
    Position *ent_pos;
    Render *ent_rndr;

    EcsComponent *prlx_cmp = ecs_ptrs.cmpnts.prlx;
    EcsComponent *pos_cmp = ecs_ptrs.cmpnts.pos;
    EcsComponent *rndr_cmp = ecs_ptrs.cmpnts.rndr;

    EcsComponent *least_used_cmp =
        (prlx_cmp->count < pos_cmp->count)
        ? ((prlx_cmp->count < rndr_cmp->count) ? prlx_cmp : rndr_cmp)
        : ((pos_cmp->count < rndr_cmp->count) ? pos_cmp : rndr_cmp);

    for (uint32_t i = 1; i < least_used_cmp->count; i++) {
        uint32_t id = least_used_cmp->dense_ids[i];

        ent_pos = (Position *)pos_cmp->data + pos_cmp->sparse_ids[id];
        if (!ent_pos) {
            continue;
        }

        ent_rndr = (Render *)rndr_cmp->data + rndr_cmp->sparse_ids[id];
        if (!ent_rndr) {
            continue;
        }

        ent_prlx = (IsParallax *)prlx_cmp->data + prlx_cmp->sparse_ids[id];
        if (!ent_prlx) {
            continue;
        }

        if (ent_pos->x + ent_rndr->frame.width < 0) {
            ent_pos->x += ent_rndr->frame.width * 2.0f;
        }
    }
}

void system_render_entities() {
    Render *ent_rndr;
    Position *ent_pos;

    EcsComponent *pos_cmp = ecs_ptrs.cmpnts.pos;
    EcsComponent *rndr_cmp = ecs_ptrs.cmpnts.rndr;

    EcsComponent *least_used_cmp = (pos_cmp->count < rndr_cmp->count) ? pos_cmp : rndr_cmp;

    Position draw_pos;
    for (int i = 1; i < least_used_cmp->count; i++) {
        uint32_t id = least_used_cmp->dense_ids[i];

        ent_pos = (Position *)pos_cmp->data + pos_cmp->sparse_ids[id];
        if (!ent_pos) {
            continue;
        }

        ent_rndr = (Render *)rndr_cmp->data + rndr_cmp->sparse_ids[id];
        if (!ent_rndr) {
            continue;
        }

        draw_pos.x = (float)(int)floorf(ent_pos->x);
        draw_pos.y = (float)(int)floorf(ent_pos->y);
        DrawTextureRec(*ent_rndr->spritesheet, ent_rndr->frame, draw_pos, WHITE);
    }
}
