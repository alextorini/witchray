#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "smetanka_ecs.h"

#define ECS_MALLOC malloc
#define ECS_MALLOC_TYPE(type) ((type *)malloc(sizeof(type)))
#define ECS_MALLOC_ARR(type, count) ((type *)malloc(sizeof(type) * (count)))
#define ECS_CALLOC calloc
#define ECS_CALLOC_TYPE(type, count) ((type *)calloc(sizeof(type), (count)))
#define ECS_FREE free

EcsSpace *ecs_create_space() {
    EcsSpace *space = ECS_MALLOC_TYPE(EcsSpace);
    if (!space) {
        return NULL;
    }

    space->current_entities_capacity = INITIAL_ENTITIES_CAPACITY;
    space->current_components_capacity = INITIAL_COMPONENTS_CAPACITY;
    space->current_entity_id = 1;
    space->current_component_id = 0;

    space->entities = ECS_MALLOC_ARR(EcsEntity, space->current_entities_capacity);
    if (!space->entities) {
        ECS_FREE(space);
        return NULL;
    }

    space->components = ECS_MALLOC_ARR(EcsComponent, space->current_components_capacity);
    if (!space->components) {
        ECS_FREE(space->entities);
        ECS_FREE(space);
        return NULL;
    }

    return space;
}

EcsComponent *ecs_register_component(EcsSpace *space, const char *name, size_t data_size) {
    EcsComponent *cmp = &space->components[space->current_component_id];

    cmp->data_size = data_size;

    strncpy(cmp->name, name, MAX_COMPONENT_NAME_LENGTH - 1);
    cmp->name[MAX_COMPONENT_NAME_LENGTH - 1] = '\0';

    cmp->sparse_cap = INITIAL_ENTITIES_CAPACITY;
    cmp->cap = INITIAL_ENTITIES_CAPACITY;

    cmp->count = 1;

    cmp->sparse_ids = ECS_CALLOC_TYPE(uint32_t, cmp->sparse_cap);
    if (!cmp->sparse_ids) {
        return NULL;
    }

    cmp->dense_ids = ECS_MALLOC_ARR(uint32_t, cmp->cap);
    if (!cmp->dense_ids) {
        ECS_FREE(cmp->sparse_ids);

        return NULL;
    }

    cmp->data = ECS_MALLOC(data_size * cmp->cap);
    if (!cmp->data) {
        ECS_FREE(cmp->sparse_ids);
        ECS_FREE(cmp->dense_ids);

        return NULL;
    }

    memset(cmp->data, 0, data_size);

    space->current_component_id++;

    return cmp;
}

EcsEntity *ecs_create_entity(EcsSpace *space) {
    EcsEntity *entity = &space->entities[space->current_entity_id];
    entity->id = space->current_entity_id++;
    entity->version = 0;

    return entity;
}

int ecs_add_component(EcsEntity *ent, EcsComponent *cmp, void *cmp_data) {
    cmp->sparse_ids[ent->id] = cmp->count;
    cmp->dense_ids[cmp->count] = ent->id;

    memcpy((char *)cmp->data + (cmp->count * cmp->data_size), cmp_data,
           cmp->data_size);

    cmp->count++;

    return 0;
}

void *ecs_get_entity_component(EcsComponent *cmp, uint32_t ent_id) {
    if (ent_id >= cmp->sparse_cap) {
        return NULL;
    }

    uint32_t dense_id = cmp->sparse_ids[ent_id];
    if (!dense_id) {
        return NULL;
    }

    return (char *)cmp->data + cmp->data_size * dense_id;
}

int ecs_destroy_space(EcsSpace *space) {
    if (!space) {
        return -1;
    }

    if (space->entities) {
        ECS_FREE(space->entities);
    }

    for (int i = 0; i < space->current_component_id; i++) {
        ECS_FREE(space->components[i].data);
    }

    ECS_FREE(space->components);
    ECS_FREE(space);

    return 0;
}
