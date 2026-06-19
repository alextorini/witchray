#pragma once

#include <stdint.h>
#include <stddef.h>

#define INVALID_ID UINT32_MAX

#define INITIAL_ENTITIES_CAPACITY 1
#define MAX_ENTITIES_CAPACITY 1000000
#define INITIAL_COMPONENTS_CAPACITY 1
#define MAX_COMPONENT_NAME_LENGTH 32

typedef uint32_t EcsEntityId;
typedef uint32_t EcsComponentId;

void ecs_create_space();

EcsComponentId ecs_register_component(const char *name, size_t data_size);

EcsEntityId ecs_create_entity();

void ecs_add_component(EcsEntityId entity_id, EcsComponentId component_id, void *component_data);

void *ecs_get_entity_component(EcsComponentId component_id, EcsEntityId entity_id);

uint32_t ecs_get_component_count(EcsComponentId cmp_id);

EcsEntityId ecs_get_component_dense(EcsComponentId cmp_id, uint32_t dense_id);

void ecs_destroy_entity(EcsEntityId ent_id);

void ecs_destroy_space();
