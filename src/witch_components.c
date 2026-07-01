#include "witch_components.h"

void init_components(EcsComponentId (*cmp_ids)[CMP_COUNT]) {
    (*cmp_ids)[CMP_POSITION] = ecs_register_component((char *)"Position", sizeof(Position));
    (*cmp_ids)[CMP_VELOCITY] = ecs_register_component((char *)"Velocity", sizeof(Velocity));
    (*cmp_ids)[CMP_RENDER] = ecs_register_component((char *)"Render", sizeof(Render));
    (*cmp_ids)[CMP_ANIMATION] = ecs_register_component((char *)"Animation", sizeof(Animation));
    (*cmp_ids)[CMP_PARALLAX] = ecs_register_component((char *)"Parallax", sizeof(IsParallax));
    (*cmp_ids)[CMP_ENEMY] = ecs_register_component((char *)"Enemy", sizeof(IsEnemy));
    (*cmp_ids)[CMP_TEXT_RENDER] = ecs_register_component((char *)"Text", sizeof(TextRender));
}
