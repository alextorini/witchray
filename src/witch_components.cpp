#include "witch_components.h"

void initComponents(EcsComponentId (*cmpIds)[CMP_COUNT]) {
    (*cmpIds)[CMP_ANIMATION] = ecsRegisterComponent((char *)"Animation", sizeof(Animation));
    (*cmpIds)[CMP_ENEMY] = ecsRegisterComponent((char *)"Enemy", sizeof(IsEnemy));
    (*cmpIds)[CMP_FIREBALL] = ecsRegisterComponent((char *)"Fireball", sizeof(Fireball));
    (*cmpIds)[CMP_PARALLAX] = ecsRegisterComponent((char *)"Parallax", sizeof(IsParallax));
    (*cmpIds)[CMP_POSITION] = ecsRegisterComponent((char *)"Position", sizeof(Position));
    (*cmpIds)[CMP_RENDER] = ecsRegisterComponent((char *)"Render", sizeof(Render));
    (*cmpIds)[CMP_TEXT_RENDER] = ecsRegisterComponent((char *)"Text", sizeof(TextRender));
    (*cmpIds)[CMP_VELOCITY] = ecsRegisterComponent((char *)"Velocity", sizeof(Velocity));
}
