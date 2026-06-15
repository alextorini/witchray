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

typedef struct {
    EcsEntityId id;
    uint32_t version;
} EcsEntity;

typedef struct {
    uint32_t *sparse_ids;
    EcsEntityId *dense_ids;
    int32_t count;
    uint32_t sparse_cap;
    uint32_t cap;
    size_t data_size;
    void *data;
    char name[MAX_COMPONENT_NAME_LENGTH];
} EcsComponent;

typedef struct {
    EcsSystemCallback callback;
    EcsComponentId *required_components;
    int required_count;
    char name[MAX_SYSTEM_NAME_LENGTH];
} EcsSystem;

typedef struct {
    EcsEntityId current_entity_id;
    uint32_t current_entities_capacity;
    EcsComponentId current_component_id;
    uint32_t current_components_capacity;
    EcsSystemId current_system_id;
    uint32_t current_systems_capacity;
    EcsEntity *entities;
    EcsComponent *components;
    EcsSystem *systems;
} EcsSpace;

static EcsSpace *space;

int ecs_create_space() {
    space = ECS_MALLOC_TYPE(EcsSpace);
    if (!space) {
        return 0;
    }

    space->current_entities_capacity = INITIAL_ENTITIES_CAPACITY;
    space->current_components_capacity = INITIAL_COMPONENTS_CAPACITY;
    space->current_systems_capacity = INITIAL_SYSTEMS_CAPACITY;
    space->current_entity_id = 1;
    space->current_component_id = 0;
    space->current_system_id = 0;

    space->entities = ECS_MALLOC_ARR(EcsEntity, space->current_entities_capacity);
    if (!space->entities) {
        ECS_FREE(space);

        return 0;
    }

    space->components = ECS_MALLOC_ARR(EcsComponent, space->current_components_capacity);
    if (!space->components) {
        ECS_FREE(space->entities);
        ECS_FREE(space);

        return 0;
    }

    space->systems = ECS_MALLOC_ARR(EcsSystem, space->current_components_capacity);
    if (!space->systems) {
        ECS_FREE(space->components);
        ECS_FREE(space->entities);
        ECS_FREE(space);

        return 0;
    }

    return 1;
}

EcsComponentId ecs_register_component(const char *name, size_t data_size) {
    EcsComponentId cmp_id = space->current_component_id;
    EcsComponent *cmp = &space->components[cmp_id];

    cmp->data_size = data_size;

    strncpy(cmp->name, name, MAX_COMPONENT_NAME_LENGTH - 1);
    cmp->name[MAX_COMPONENT_NAME_LENGTH - 1] = '\0';

    cmp->sparse_cap = INITIAL_ENTITIES_CAPACITY;
    cmp->cap = INITIAL_ENTITIES_CAPACITY;

    cmp->count = 1;

    cmp->sparse_ids = ECS_CALLOC_TYPE(uint32_t, cmp->sparse_cap);
    if (!cmp->sparse_ids) {
        return 0;
    }

    cmp->dense_ids = ECS_MALLOC_ARR(uint32_t, cmp->cap);
    if (!cmp->dense_ids) {
        ECS_FREE(cmp->sparse_ids);

        return 0;
    }

    cmp->data = ECS_MALLOC(data_size * cmp->cap);
    if (!cmp->data) {
        ECS_FREE(cmp->sparse_ids);
        ECS_FREE(cmp->dense_ids);

        return 0;
    }

    memset(cmp->data, 0, data_size);

    space->current_component_id++;

    return cmp_id;
}

EcsEntityId ecs_create_entity() {
    EcsEntity *entity = &space->entities[space->current_entity_id];
    entity->id = space->current_entity_id++;
    entity->version = 0;

    return entity->id;
}

int ecs_add_component(EcsEntityId ent_id, EcsComponentId cmp_id, void *cmp_data) {
    EcsComponent *cmp = &space->components[cmp_id];

    cmp->sparse_ids[ent_id] = cmp->count;
    cmp->dense_ids[cmp->count] = ent_id;

    memcpy((char *)cmp->data + (cmp->count * cmp->data_size), cmp_data,
           cmp->data_size);

    cmp->count++;

    return 0;
}

void *ecs_get_entity_component(EcsComponentId cmp_id, EcsEntityId ent_id) {
    EcsComponent *cmp = &space->components[cmp_id];

    if (ent_id >= cmp->sparse_cap) {
        return NULL;
    }

    uint32_t dense_id = cmp->sparse_ids[ent_id];
    if (!dense_id) {
        return NULL;
    }

    return (char *)cmp->data + cmp->data_size * dense_id;
}

int32_t ecs_get_component_count(EcsComponentId cmp_id) {
    if (cmp_id >= space->current_component_id) {
        return -1;
    }

    EcsComponent *cmp = &space->components[cmp_id];

    if (!cmp) {
        return -1;
    }

    return cmp->count;
}

EcsEntityId ecs_get_component_dense(EcsComponentId cmp_id, uint32_t dense_id) {
    if (cmp_id >= space->current_component_id) {
        return 0;
    }

    EcsComponent *cmp = &space->components[cmp_id];

    if (!cmp) {
        return 0;
    }

    if (dense_id > cmp->cap) {
        return 0;
    }

    return cmp->dense_ids[dense_id];
}

EcsSystemId ecs_register_system(
    const char *name,
    EcsSystemCallback callback,
    uint32_t *req_cmpnts,
    uint32_t cmpnts_count
) {
    EcsSystemId sys_id = space->current_system_id;
    EcsSystem *sys = &space->systems[sys_id];

    strncpy(sys->name, name, MAX_SYSTEM_NAME_LENGTH - 1);
    sys->name[MAX_SYSTEM_NAME_LENGTH - 1] = '\0';

    sys->callback = callback;
    sys->required_components = req_cmpnts;
    sys->required_count = cmpnts_count;

    space->current_system_id++;

    return sys_id;
}


int ecs_destroy_space() {
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
