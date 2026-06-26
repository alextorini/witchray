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
    EcsComponentId secondary_component_id_list[MAX_ITERATOR_COMPONENT_COUNT];
    uint16_t component_count;
    uint32_t index;
    EcsComponentId base_component_id;
} EcsEntityIterator;

EcsSpace *ecs_create_space();

EcsComponentId ecs_register_component(const char *name, size_t data_size);

EcsEntityHandle ecs_create_entity();

void *ecs_add_component(EcsEntityHandle entity_handle, EcsComponentId component_id, void *component_data);

void *ecs_get_entity_component(EcsComponentId component_id, EcsEntityHandle entity_handle);

uint32_t ecs_get_component_count(EcsComponentId cmp_id);

EcsEntityHandle ecs_get_component_dense(EcsComponentId cmp_id, uint32_t dense_id);

EcsEntityIterator ecs_get_entity_iterator(EcsComponentId *component_id_list, uint16_t count);

EcsEntityHandle ecs_get_next_entity(EcsEntityIterator *iterator);

void ecs_destroy_entity(EcsEntityHandle ent_id);

void ecs_clear_space();

void ecs_destroy_space();
