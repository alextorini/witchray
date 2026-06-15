#pragma once

#include <stdint.h>
#include <stddef.h>

#define INITIAL_ENTITIES_CAPACITY 100
#define INITIAL_COMPONENTS_CAPACITY 10
#define INITIAL_SYSTEMS_CAPACITY 10
#define MAX_COMPONENT_NAME_LENGTH 32
#define MAX_SYSTEM_NAME_LENGTH 32

typedef uint32_t EcsEntityId;
typedef uint32_t EcsComponentId;
typedef uint32_t EcsSystemId;

typedef void (*EcsSystemCallback)(EcsComponentId *components, uint32_t components_count, float delta_time);

int ecs_create_space();

EcsComponentId ecs_register_component(const char *name, size_t data_size);

EcsEntityId ecs_create_entity();

int ecs_add_component(EcsEntityId entity_id, EcsComponentId component_id, void *component_data);

void *ecs_get_entity_component(EcsComponentId component_id, EcsEntityId entity_id);

int32_t ecs_get_component_count(EcsComponentId cmp_id);

EcsEntityId ecs_get_component_dense(EcsComponentId cmp_id, uint32_t dense_id);

EcsSystemId ecs_register_system(
    const char *name,
    EcsSystemCallback callback,
    uint32_t *req_cmpnts,
    uint32_t cmpnts_count
);

int ecs_destroy_space();
