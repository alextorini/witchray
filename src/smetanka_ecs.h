#pragma once

#include <stdint.h>
#include <stddef.h>

#define INITIAL_ENTITIES_CAPACITY 100
#define INITIAL_COMPONENTS_CAPACITY 10
#define MAX_COMPONENT_NAME_LENGTH 32

typedef struct {
    uint32_t *sparse_ids;
    uint32_t *dense_ids;
    uint32_t count;
    uint32_t sparse_cap;
    uint32_t cap;
    size_t data_size;
    void *data;
    char name[MAX_COMPONENT_NAME_LENGTH];
} EcsComponent;

typedef struct {
    uint32_t id;
    uint32_t version;
} EcsEntity;

typedef struct {
    uint32_t current_entity_id;
    uint32_t current_entities_capacity;
    uint32_t current_components_capacity;
    uint32_t current_component_id;
    EcsEntity *entities;
    EcsComponent *components;
} EcsSpace;

EcsSpace *ecs_create_space();

EcsComponent *ecs_register_component(EcsSpace *space, const char *name, size_t data_size);

EcsEntity *ecs_create_entity(EcsSpace *space);

int ecs_add_component(EcsEntity *entity, EcsComponent *component, void *component_data);

void *ecs_get_entity_component(EcsComponent *component, EcsEntity *entity);

int ecs_destroy_space(EcsSpace *space);
