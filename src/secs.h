#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdint.h>

#define INITIAL_ENTITIES_CAPACITY 100
#define INITIAL_COMPONENTS_CAPACITY 10
#define MAX_COMPONENT_NAME_LENGTH 32

typedef struct {
    uint64_t mask_bit;
    size_t data_struct_size;
    void *data;
    char name[MAX_COMPONENT_NAME_LENGTH];
} EcsComponent;

typedef struct {
    int id;
    int version;
    uint64_t comp_mask;
} EcsEntity;

typedef struct {
    int current_entity_id;
    int current_entities_capacity;
    int current_components_capacity;
    int current_component_id;
    EcsEntity *entities;
    EcsComponent *components;
} EcsSpace;

EcsSpace *ecs_create_space();

EcsComponent *ecs_register_component(EcsSpace *space, const char *name, size_t data_struct_size);

EcsEntity *ecs_create_entity(EcsSpace *space);

int ecs_add_component(EcsEntity *entity, EcsComponent *component, void *component_data);

void *ecs_get_entity_component(EcsComponent *component, EcsEntity *entity);

int ecs_destroy_space(EcsSpace *space);
