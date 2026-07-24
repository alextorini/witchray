#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "smetanka_ecs.h"
#include "smetanka_misc.h"

#define ECS_MALLOC malloc
#define ECS_MALLOC_TYPE(type) ((type *)malloc(sizeof(type)))
#define ECS_MALLOC_ARR(type, count) ((type *)malloc(sizeof(type) * (count)))
#define ECS_CALLOC calloc
#define ECS_CALLOC_TYPE(type, count) ((type *)calloc((count), sizeof(type)))
#define ECS_REALLOC realloc
#define ECS_REALLOC_ARR(ptr, type, count) ((type *)realloc((ptr), (sizeof(type) * (count))))
#define ECS_FREE free

#define INITIAL_DESTROY_QUEUE_CAPACITY 16

typedef uint32_t EcsEntityId;
typedef uint32_t EcsEntityGen;

typedef struct {
    EcsEntityHandle handle;
    uint8_t active;
} EcsEntity;

typedef struct {
    uint32_t *sparseIdList;
    EcsEntityId *denseIdList;
    uint32_t count;
    uint32_t sparseCapacity;
    uint32_t capacity;
    size_t dataSize;
    void *data;
    char name[MAX_COMPONENT_NAME_LENGTH];
} EcsComponent;

typedef struct {
    EcsEntityId *idList;
    uint32_t count;
    uint32_t capacity;
} EntityFreeSlotPool;

typedef struct {
    EcsEntityHandle *handleList;
    uint32_t count;
    uint32_t capacity;
} EntityDestroyQueue;

typedef struct {
    EcsEntityId nextEntityId;
    uint32_t currentEntitiesCapacity;
    EcsComponentId currentComponentId;
    uint32_t currentComponentsCapacity;
    EcsEntity *entityList;
    EcsComponent *componentList;
    EntityFreeSlotPool entityFreeSlotsPool;
    EntityDestroyQueue destroyQueue;
} EcsSpace;

static EcsSpace *space;

void ecsCreateSpace() {
    space = ECS_MALLOC_TYPE(EcsSpace);
    if (!space) {
        ABORT();
    }

    space->currentEntitiesCapacity = INITIAL_ENTITIES_CAPACITY;
    space->currentComponentsCapacity = INITIAL_COMPONENTS_CAPACITY;
    space->nextEntityId = 0;
    space->currentComponentId = 0;

    space->entityList = ECS_MALLOC_ARR(EcsEntity, space->currentEntitiesCapacity);
    if (!space->entityList) {
        ECS_FREE(space);

        ABORT();
    }

    space->componentList = ECS_MALLOC_ARR(EcsComponent, space->currentComponentsCapacity);
    if (!space->componentList) {
        ECS_FREE(space->entityList);
        ECS_FREE(space);

        ABORT();
    }

    space->entityFreeSlotsPool.capacity = INITIAL_ENTITIES_CAPACITY;
    space->entityFreeSlotsPool.count = 0;
    space->entityFreeSlotsPool.idList = ECS_MALLOC_ARR(uint32_t, space->entityFreeSlotsPool.capacity);
    if (!space->entityFreeSlotsPool.idList) {
        ECS_FREE(space->componentList);
        ECS_FREE(space->entityList);
        ECS_FREE(space);

        ABORT();
    }


    for (int i = 0; i < space->entityFreeSlotsPool.capacity; i++) {
        space->entityFreeSlotsPool.idList[i] = INVALID_ID;
    }

    space->destroyQueue.capacity = INITIAL_DESTROY_QUEUE_CAPACITY;
    space->destroyQueue.count = 0;
    space->destroyQueue.handleList = ECS_MALLOC_ARR(EcsEntityHandle, space->destroyQueue.capacity);
    if (!space->destroyQueue.handleList) {
        ECS_FREE(space->componentList);
        ECS_FREE(space->entityList);
        ECS_FREE(space->entityFreeSlotsPool.idList);
        ECS_FREE(space);

        ABORT();
    }
}

EcsComponentId ecsRegisterComponent(const char *name, size_t dataSize) {
    if (space->currentComponentId >= space->currentComponentsCapacity) {
        space->currentComponentsCapacity *= 2;
        EcsComponent *componentList =
            ECS_REALLOC_ARR(space->componentList, EcsComponent, space->currentComponentsCapacity);
        if (!componentList) {
            ABORT();
        }

        space->componentList = componentList;
    }

    EcsComponentId componentId = space->currentComponentId;
    EcsComponent *componentPointer = &space->componentList[componentId];

    componentPointer->dataSize = dataSize;

    strncpy(componentPointer->name, name, MAX_COMPONENT_NAME_LENGTH - 1);
    componentPointer->name[MAX_COMPONENT_NAME_LENGTH - 1] = '\0';

    componentPointer->sparseCapacity = INITIAL_ENTITIES_CAPACITY;
    componentPointer->capacity = INITIAL_ENTITIES_CAPACITY;

    componentPointer->count = 0;

    componentPointer->sparseIdList = ECS_MALLOC_ARR(uint32_t, componentPointer->sparseCapacity);
    if (!componentPointer->sparseIdList) {
        ABORT();
    }

    for (uint32_t i = 0; i < componentPointer->sparseCapacity; i++) {
        componentPointer->sparseIdList[i] = INVALID_ID;
    }

    componentPointer->denseIdList = ECS_MALLOC_ARR(EcsEntityId, componentPointer->capacity);
    if (!componentPointer->denseIdList) {
        ECS_FREE(componentPointer->sparseIdList);

        ABORT();
    }

    componentPointer->data = ECS_MALLOC(dataSize * componentPointer->capacity);
    if (!componentPointer->data) {
        ECS_FREE(componentPointer->sparseIdList);
        ECS_FREE(componentPointer->denseIdList);

        ABORT();
    }

    space->currentComponentId++;

    return componentId;
}

EcsEntityHandle ecsCreateEntity() {
    if (space->nextEntityId > MAX_ENTITIES_CAPACITY) {
        ABORT();
    }

    EcsEntityId entityId;
    if (space->entityFreeSlotsPool.count > 0) {
        entityId = space->entityFreeSlotsPool.idList[--space->entityFreeSlotsPool.count];
        if (entityId == INVALID_ID) {
            ABORT();
        }
    } else {
        entityId = space->nextEntityId++;
        while (entityId >= space->currentEntitiesCapacity) {
            uint32_t newCapacity = space->currentEntitiesCapacity * 2;
            if (newCapacity > MAX_ENTITIES_CAPACITY) {
                newCapacity = MAX_ENTITIES_CAPACITY;
            }

            if (newCapacity == space->currentEntitiesCapacity) {
                continue;
            }

            space->currentEntitiesCapacity = newCapacity;

            EcsEntity *entity_list = ECS_REALLOC_ARR(space->entityList, EcsEntity, space->currentEntitiesCapacity);
            if (!entity_list) {
                ABORT();
            }

            space->entityList = entity_list;
        }

        space->entityList[entityId].handle = packHandle(entityId, 0);
    }

    EcsEntity *entity_pointer;
    entity_pointer = &space->entityList[entityId];
    entity_pointer->active = 1;

    return entity_pointer->handle;
}

void *ecsAddComponent(EcsEntityHandle entityHandle, EcsComponentId componentId, void *componentData) {
    EcsEntityId entityId = getHandleId(entityHandle);
    if (entityId >= space->nextEntityId) {
        ABORT();
    }

    if (getHandleGen(space->entityList[entityId].handle) != getHandleGen(entityHandle)) {
        ABORT();
    }

    if (!space->entityList[entityId].active) {
        ABORT();
    }

    if (componentId >= space->currentComponentId) {
        ABORT();
    }

    EcsComponent *cmpPtr = &space->componentList[componentId];

    if (entityId < cmpPtr->sparseCapacity && cmpPtr->sparseIdList[entityId] != INVALID_ID) {
        ABORT();
    }

    while (entityId >= cmpPtr->sparseCapacity) {
        uint32_t oldCap = cmpPtr->sparseCapacity;
        cmpPtr->sparseCapacity *= 2;
        uint32_t *sparseIds = ECS_REALLOC_ARR(cmpPtr->sparseIdList, uint32_t, cmpPtr->sparseCapacity);
        if (!sparseIds) {
            ABORT();
        }

        for (uint32_t i = oldCap; i < cmpPtr->sparseCapacity; i++) {
            sparseIds[i] = INVALID_ID;
        }

        cmpPtr->sparseIdList = sparseIds;
    }

    while (cmpPtr->count >= cmpPtr->capacity) {
        cmpPtr->capacity *= 2;
        EcsEntityId *denseIds = ECS_REALLOC_ARR(cmpPtr->denseIdList, EcsEntityId, cmpPtr->capacity);
        if (!denseIds) {
            ABORT();
        }

        cmpPtr->denseIdList = denseIds;

        void *data = ECS_REALLOC(cmpPtr->data, cmpPtr->dataSize * cmpPtr->capacity);
        if (!data) {
            ABORT();
        }

        cmpPtr->data = data;
    }


    cmpPtr->sparseIdList[entityId] = cmpPtr->count;
    cmpPtr->denseIdList[cmpPtr->count] = entityId;

    void *newData = (char *)cmpPtr->data + (cmpPtr->count * cmpPtr->dataSize);

    memcpy(newData, componentData, cmpPtr->dataSize);

    cmpPtr->count++;

    return newData;
}

uint8_t ecsDoesEntityExist(EcsEntityHandle entityHandle) {
    EcsEntityId entityId = getHandleId(entityHandle);
    if (!space->entityList[entityId].active) {
        return 0;
    }

    if (getHandleGen(space->entityList[entityId].handle) != getHandleGen(entityHandle)) {
        return 0;
    }

    return 1;
}

void *ecsGetEntityComponent(EcsComponentId componentId, EcsEntityHandle entityHandle) {
    EcsComponent *component = &space->componentList[componentId];
    EcsEntityId entityId = getHandleId(entityHandle);

    if (componentId >= space->currentComponentId) {
        return NULL;
    }

    if (entityId >= component->sparseCapacity) {
        return NULL;
    }

    if (!space->entityList[entityId].active) {
        ABORT();
    }

    if (getHandleGen(space->entityList[entityId].handle) != getHandleGen(entityHandle)) {
        ABORT();
    }

    uint32_t denseId = component->sparseIdList[entityId];
    if (denseId == INVALID_ID) {
        return NULL;
    }

    if (denseId >= component->count) {
        ABORT();
    }

    if (component->denseIdList[denseId] != entityId) {
        ABORT();
    }

    return (char *)component->data + component->dataSize * denseId;
}

uint32_t ecsGetComponentCount(EcsComponentId componentId) {
    if (componentId >= space->currentComponentId) {
        return 0;
    }

    EcsComponent *cmp = &space->componentList[componentId];

    return cmp->count;
}

EcsEntityHandle ecsGetComponentDense(EcsComponentId componentId, uint32_t denseId) {
    if (componentId >= space->currentComponentId) {
        return INVALID_ID;
    }

    EcsComponent *cmp = &space->componentList[componentId];

    if (!cmp) {
        return INVALID_ID;
    }

    if (denseId >= cmp->count) {
        return INVALID_ID;
    }

    EcsEntityId entityId = cmp->denseIdList[denseId];

    return space->entityList[entityId].handle;
}

EcsEntityIterator ecsGetEntityIterator(EcsComponentId *componentIdList, uint16_t count) {
    EcsEntityIterator iterator;

    iterator.componentCount = count;
    iterator.index = 0;
    iterator.baseComponentId = INVALID_ID;

    uint32_t minEntityCount = UINT32_MAX;
    uint32_t secondaryComponentIndex = 0;

    for (uint32_t i = 0; i < count; i++) {
        uint32_t componentEntityCount = space->componentList[componentIdList[i]].count;
        if (componentEntityCount < minEntityCount) {
            minEntityCount = componentEntityCount;

            if (iterator.baseComponentId != INVALID_ID) {
                iterator.secondaryComponentIdList[secondaryComponentIndex++] = iterator.baseComponentId;
            }

            iterator.baseComponentId = componentIdList[i];

            continue;
        }

        iterator.secondaryComponentIdList[secondaryComponentIndex++] = componentIdList[i];
    }

    return iterator;
}

EcsEntityHandle ecsGetNextEntity(EcsEntityIterator *iterator) {
    EcsEntityHandle entityHandle;

    uint32_t baseComponentCount = ecsGetComponentCount(iterator->baseComponentId);
    while (iterator->index < baseComponentCount) {
        entityHandle = ecsGetComponentDense(iterator->baseComponentId, iterator->index);

        if (entityHandle == INVALID_ID) {
            return INVALID_HANDLE;
        }

        if (iterator->componentCount == 1) {
            iterator->index++;

            return entityHandle;
        }

        uint8_t isEntityValid = 1;
        void *componentData;
        for (uint32_t i = 0; i < iterator->componentCount - 1; i++) {
            componentData = ecsGetEntityComponent(iterator->secondaryComponentIdList[i], entityHandle);
            if (!componentData) {
                isEntityValid = 0;
                break;
            }
        }

        iterator->index++;

        if (isEntityValid) {
            return entityHandle;
        }
    }

    return INVALID_HANDLE;
}

static void ecsRemoveFromComponent(EcsComponent *component, EcsEntityId entityId) {
    if (entityId >= component->sparseCapacity) {
        return;
    }

    uint32_t denseId = component->sparseIdList[entityId];
    if (denseId == INVALID_ID) {
        return;
    }

    if (component->count == 0) {
        ABORT();
    }

    uint32_t lastIndex = component->count - 1;
    EcsEntityId lastEntity = component->denseIdList[lastIndex];

    component->denseIdList[denseId] = lastEntity;
    memcpy(
        (char*)component->data + denseId * component->dataSize,
        (char*)component->data + lastIndex * component->dataSize,
        component->dataSize
    );

    component->sparseIdList[lastEntity] = denseId;

    component->count--;

    component->sparseIdList[entityId] = INVALID_ID;
}

void ecsDestroyEntity(EcsEntityHandle entityHandle) {
    EcsEntityId entityId = getHandleId(entityHandle);
    if (entityId >= space->nextEntityId) {
        return;
    }

    EcsEntity *entity = &space->entityList[entityId];
    if (!entity->active) {
        return;
    }

    if (getHandleGen(entity->handle) != getHandleGen(entityHandle)) {
        ABORT();
    }

    if (entityId == INVALID_ID) {
        return;
    }

    for (uint32_t i = 0; i < space->currentComponentId; i++)
    {
        ecsRemoveFromComponent(&space->componentList[i], entityId);
    }

    entity->active = 0;
    entity->handle = increaseHandleGen(entity->handle);

    while (space->entityFreeSlotsPool.count >= space->entityFreeSlotsPool.capacity) {
        space->entityFreeSlotsPool.capacity *= 2;

        EcsEntityId *ids = ECS_REALLOC_ARR(space->entityFreeSlotsPool.idList, EcsEntityId, space->entityFreeSlotsPool.capacity);
        if (!ids) {
            ABORT();
        }

        space->entityFreeSlotsPool.idList = ids;
    }

    space->entityFreeSlotsPool.idList[space->entityFreeSlotsPool.count++] = entityId;
}

void ecsAddToDestroyQueue(EcsEntityHandle entityHandle) {
    EntityDestroyQueue *queue = &space->destroyQueue;

    if (queue->count >= queue->capacity) {
        queue->capacity *= 2;
        EcsEntityHandle *handleList = ECS_REALLOC_ARR(queue->handleList, EcsEntityHandle, queue->capacity);
        if (!handleList) {
            ABORT();
        }
        queue->handleList = handleList;
    }

    queue->handleList[queue->count++] = entityHandle;
}

void ecsFlushDestroyQueue() {
    EntityDestroyQueue *queue = &space->destroyQueue;

    for (uint32_t i = 0; i < queue->count; i++) {
        ecsDestroyEntity(queue->handleList[i]);
    }

    queue->count = 0;
}

void ecsClearSpace() {
    for (uint32_t i = 0; i < space->currentComponentId; i++) {
        EcsComponent *cmp = &space->componentList[i];

        for (uint32_t j = 0; j < cmp->count; j++) {
            cmp->sparseIdList[cmp->denseIdList[j]] = INVALID_ID;
        }

        cmp->count = 0;
    }

    space->entityFreeSlotsPool.count = 0;

    for (EcsEntityId id = 0; id < space->nextEntityId; id++) {
        EcsEntity *entity = &space->entityList[id];

        if (!entity->active) {
            continue;
        }

        entity->active = 0;
        entity->handle = increaseHandleGen(entity->handle);

        space->entityFreeSlotsPool.idList[space->entityFreeSlotsPool.count++] = id;
    }
}

void ecsDestroySpace() {
    if (!space) {
        return;
    }

    if (space->entityList) {
        ECS_FREE(space->entityList);
    }

    for (uint32_t i = 0; i < space->currentComponentId; i++) {
        ECS_FREE(space->componentList[i].sparseIdList);
        ECS_FREE(space->componentList[i].denseIdList);
        ECS_FREE(space->componentList[i].data);
    }

    ECS_FREE(space->componentList);
    ECS_FREE(space->entityFreeSlotsPool.idList);
    ECS_FREE(space);

    space = NULL;

    return;
}
