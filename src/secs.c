#include "secs.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

EcsSpace *ecs_create_space() {
    EcsSpace *space = (EcsSpace *)malloc(sizeof(EcsSpace));
    if (!space) {
        return NULL;
    }

    space->current_entities_capacity = INITIAL_ENTITIES_CAPACITY;
    space->current_components_capacity = INITIAL_COMPONENTS_CAPACITY;
    space->current_entity_id = 0;
    space->current_component_id = 0;

    space->entities = (EcsEntity *)malloc(sizeof(EcsEntity) * INITIAL_ENTITIES_CAPACITY);
    if (!space->entities) {
        free(space);
        return NULL;
    }

    space->components = (EcsComponent *)malloc(sizeof(EcsComponent) * INITIAL_COMPONENTS_CAPACITY);
    if (!space->components) {
        free(space->entities);
        free(space);
        return NULL;
    }

    return space;
}

EcsComponent *ecs_register_component(EcsSpace *space, const char *name, size_t data_struct_size) {
    EcsComponent *component = &space->components[space->current_component_id];

    component->data_struct_size = data_struct_size;

    strncpy(component->name, name, MAX_COMPONENT_NAME_LENGTH - 1);
    component->name[MAX_COMPONENT_NAME_LENGTH - 1] = '\0';

    component->mask_bit = (1ULL << space->current_component_id);

    component->data = calloc(space->current_entities_capacity, data_struct_size);
    if (!component->data) {
        return NULL;
    }

    space->current_component_id++;

    return component;
}

EcsEntity *ecs_create_entity(EcsSpace *space) {
    EcsEntity *entity = &space->entities[space->current_entity_id];
    entity->id = space->current_entity_id++;
    entity->version = 0;

    return entity;
}

int ecs_add_component(EcsEntity *entity, EcsComponent *component, void *component_data) {
    memcpy((char *)component->data + (entity->id * component->data_struct_size), component_data,
           component->data_struct_size);

    entity->comp_mask = entity->comp_mask | component->mask_bit;

    return 0;
}

void *ecs_get_entity_component(EcsComponent *component, EcsEntity *entity) {
    return (char *)component->data + component->data_struct_size * entity->id;
}

int ecs_destroy_space(EcsSpace *space) {
    if (!space) {
        return -1;
    }

    if (space->entities) {
        free(space->entities);
    }

    for (int i = 0; i < space->current_component_id; i++) {
        free(space->components[i].data);
    }

    free(space->components);
    free(space);

    return 0;
}
