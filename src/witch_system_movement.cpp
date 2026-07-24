#include "smetanka_ecs.h"
#include "witch_components.h"

void systemMoveOrbit(float dt) {
    EcsComponentId componentIdList[] = {COMPONENT_ID(OrbitMovement)};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 1);
    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        OrbitMovement *orbit = getOrbitMovement(entityHandle);

        orbit->angle = fmodf(orbit->angle - orbit->angularSpeed * dt, 2.0f * PI);

        Position *centerPosition = getPosition(orbit->center);
        Rectangle centerFrame = getRender(orbit->center)->frame;

        Rectangle frame = getRender(entityHandle)->frame;
        Position *position = getPosition(entityHandle);
        position->x =
            centerPosition->x + centerFrame.width / 2.0f + cosf(orbit->angle) * orbit->radius - frame.width/2.0f;
        position->y =
            centerPosition->y + centerFrame.height / 2.0f + sinf(orbit->angle) * orbit->radius - frame.height/2.0f;
    }
}

