#include "ext/raylib.h"
#include "ext/raymath.h"

#include "witch_systems.h"

void system_move_entities() {
    Render *e_rndr;
    Position *e_pos;
    Velocity *e_vel;
    IsParallax *e_is_prlx;
    for (int i = 1; i < spc->current_entity_id; i++) {
        e_pos = (Position *)ecs_ptrs.cmpnts.pos->data + ecs_ptrs.cmpnts.pos->sparse_ids[i];
        if (!e_pos) {
            continue;
        }

        e_vel = (Velocity *)ecs_ptrs.cmpnts.vel->data + ecs_ptrs.cmpnts.vel->sparse_ids[i];
        if (!e_vel) {
            continue;
        }

        *e_pos = Vector2Add(*e_pos, Vector2Scale(*e_vel, GetFrameTime()));
    }
}

void system_move_parallax() {
    Render *e_rndr;
    Position *e_pos;
    Velocity *e_vel;
    IsParallax *e_is_prlx;
    for (int i = 1; i < spc->current_entity_id; i++) {
        e_pos = (Position *)ecs_ptrs.cmpnts.pos->data + ecs_ptrs.cmpnts.pos->sparse_ids[i];
        if (!e_pos) {
            continue;
        }

        e_rndr = (Render *)ecs_ptrs.cmpnts.rndr->data + ecs_ptrs.cmpnts.rndr->sparse_ids[i];
        if (!e_rndr) {
            continue;
        }

        e_is_prlx = (IsParallax *)ecs_ptrs.cmpnts.is_prlx->data + ecs_ptrs.cmpnts.is_prlx->sparse_ids[i];
        if (!e_is_prlx) {
            continue;
        }

        if (e_pos->x + e_rndr->frame.width < 0) {
            e_pos->x += e_rndr->frame.width * 2.0f;
        }
    }
}

void system_render_entities() {
    Render *e_rndr;
    Position *e_pos;

    Position draw_pos;
    for (int i = 1; i < spc->current_entity_id; i++) {
        e_pos = (Position *)ecs_ptrs.cmpnts.pos->data + ecs_ptrs.cmpnts.pos->sparse_ids[i];
        if (!e_pos) {
            continue;
        }

        e_rndr = (Render *)ecs_ptrs.cmpnts.rndr->data + ecs_ptrs.cmpnts.rndr->sparse_ids[i];
        if (!e_rndr) {
            continue;
        }

        draw_pos.x = (float)(int)floorf(e_pos->x);
        draw_pos.y = (float)(int)floorf(e_pos->y);
        DrawTextureRec(*e_rndr->spritesheet, e_rndr->frame, draw_pos, WHITE);
    }
}
