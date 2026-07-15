#pragma once

#include <stdint.h>
#include <stddef.h>

#define INVALID_ID UINT32_MAX
#define INVALID_HANDLE UINT64_MAX

#define INITIAL_ENTITIES_CAPACITY 1
#define MAX_ENTITIES_CAPACITY 1000000
#define INITIAL_COMPONENTS_CAPACITY 1
#define MAX_COMPONENT_NAME_LENGTH 32
#define MAX_ITERATOR_COMPONENT_COUNT 16

typedef uint64_t EcsEntityHandle;
typedef uint32_t EcsComponentId;
typedef struct EcsSpace EcsSpace;

typedef struct {
    EcsComponentId secondaryComponentIdList[MAX_ITERATOR_COMPONENT_COUNT];
    uint16_t componentCount;
    uint32_t index;
    EcsComponentId baseComponentId;
} EcsEntityIterator;

EcsSpace *ecsCreateSpace();

EcsComponentId ecsRegisterComponent(const char *name, size_t dataSize);

EcsEntityHandle ecsCreateEntity();

void *ecsAddComponent(EcsEntityHandle entityHandle, EcsComponentId componentId, void *componentData);

void *ecsGetEntityComponent(EcsComponentId componentId, EcsEntityHandle entityHandle);

uint32_t ecsGetComponentCount(EcsComponentId cmpId);

EcsEntityHandle ecsGetComponentDense(EcsComponentId cmpId, uint32_t denseId);

EcsEntityIterator ecsGetEntityIterator(EcsComponentId *componentIdList, uint16_t count);

EcsEntityHandle ecsGetNextEntity(EcsEntityIterator *iterator);

void ecsDestroyEntity(EcsEntityHandle entityHandle);

void ecsClearSpace();

void ecsDestroySpace();
