#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "smetanka_ecs.h"

#define ECS_MALLOC malloc
#define ECS_MALLOC_TYPE(type) ((type *)malloc(sizeof(type)))
#define ECS_MALLOC_ARR(type, count) ((type *)malloc(sizeof(type) * (count)))
#define ECS_CALLOC calloc
#define ECS_CALLOC_TYPE(type, count) ((type *)calloc((count), sizeof(type)))
#define ECS_REALLOC realloc
#define ECS_REALLOC_ARR(ptr, type, count) ((type *)realloc((ptr), (sizeof(type) * (count))))
#define ECS_FREE free

typedef struct {
    EcsEntityId id;
    uint32_t version;
} EcsEntity;

typedef struct {
    uint32_t *sparse_ids;
    EcsEntityId *dense_ids;
    uint32_t count;
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

void ecs_create_space() {
    space = ECS_MALLOC_TYPE(EcsSpace);
    if (!space) {
        abort();
    }

    space->current_entities_capacity = INITIAL_ENTITIES_CAPACITY;
    space->current_components_capacity = INITIAL_COMPONENTS_CAPACITY;
    space->current_systems_capacity = INITIAL_SYSTEMS_CAPACITY;
    space->current_entity_id = 0;
    space->current_component_id = 0;
    space->current_system_id = 0;

    space->entities = ECS_MALLOC_ARR(EcsEntity, space->current_entities_capacity);
    if (!space->entities) {
        ECS_FREE(space);

        abort();
    }

    space->components = ECS_MALLOC_ARR(EcsComponent, space->current_components_capacity);
    if (!space->components) {
        ECS_FREE(space->entities);
        ECS_FREE(space);

        abort();
    }

    space->systems = ECS_MALLOC_ARR(EcsSystem, space->current_components_capacity);
    if (!space->systems) {
        ECS_FREE(space->components);
        ECS_FREE(space->entities);
        ECS_FREE(space);

        abort();
    }
}

EcsComponentId ecs_register_component(const char *name, size_t data_size) {
    if (space->current_component_id >= space->current_components_capacity) {
        space->current_components_capacity *= 2;
        EcsComponent *cmpnts = ECS_REALLOC_ARR(space->components, EcsComponent, space->current_components_capacity);
        if (!cmpnts) {
            abort();
        }

        space->components = cmpnts;
    }

    EcsComponentId cmp_id = space->current_component_id;
    EcsComponent *cmp = &space->components[cmp_id];

    cmp->data_size = data_size;

    strncpy(cmp->name, name, MAX_COMPONENT_NAME_LENGTH - 1);
    cmp->name[MAX_COMPONENT_NAME_LENGTH - 1] = '\0';

    cmp->sparse_cap = INITIAL_ENTITIES_CAPACITY;
    cmp->cap = INITIAL_ENTITIES_CAPACITY;

    cmp->count = 0;

    cmp->sparse_ids = ECS_MALLOC_ARR(uint32_t, cmp->sparse_cap);
    if (!cmp->sparse_ids) {
        abort();
    }

    for (uint32_t i = 0; i < cmp->sparse_cap; i++) {
        cmp->sparse_ids[i] = INVALID_ID;
    }

    cmp->dense_ids = ECS_MALLOC_ARR(uint32_t, cmp->cap);
    if (!cmp->dense_ids) {
        ECS_FREE(cmp->sparse_ids);

        abort();
    }

    cmp->data = ECS_MALLOC(data_size * cmp->cap);
    if (!cmp->data) {
        ECS_FREE(cmp->sparse_ids);
        ECS_FREE(cmp->dense_ids);

        abort();
    }

    memset(cmp->data, 0, data_size);

    space->current_component_id++;

    return cmp_id;
}

EcsEntityId ecs_create_entity() {
    while (space->current_entity_id >= space->current_entities_capacity) {
        space->current_entities_capacity *= 2;
        EcsEntity *entts = ECS_REALLOC_ARR(space->entities, EcsEntity, space->current_entities_capacity);
        if (!entts) {
            abort();
        }

        space->entities = entts;
    }

    EcsEntity *entity = &space->entities[space->current_entity_id];
    entity->id = space->current_entity_id++;
    entity->version = 0;

    return entity->id;
}

void ecs_add_component(EcsEntityId ent_id, EcsComponentId cmp_id, void *cmp_data) {
    EcsComponent *cmp = &space->components[cmp_id];

    while (ent_id >= cmp->sparse_cap) {
        uint32_t old_cap = cmp->sparse_cap;
        cmp->sparse_cap *= 2;
        uint32_t *sparse_ids = ECS_REALLOC_ARR(cmp->sparse_ids, uint32_t, cmp->sparse_cap);
        if (!sparse_ids) {
            abort();
        }

        for (uint32_t i = old_cap; i < cmp->sparse_cap; i++) {
            sparse_ids[i] = INVALID_ID;
        }

        cmp->sparse_ids = sparse_ids;
    }

    while (cmp->count >= cmp->cap) {
        cmp->cap *= 2;
        EcsEntityId *dense_ids = ECS_REALLOC_ARR(cmp->dense_ids, uint32_t, cmp->cap);
        if (!dense_ids) {
            abort();
        }

        cmp->dense_ids = dense_ids;

        void *data = ECS_REALLOC(cmp->data, cmp->data_size * cmp->cap);

        if (!data) {
            abort();
        }

        cmp->data = data;
    }


    cmp->sparse_ids[ent_id] = cmp->count;
    cmp->dense_ids[cmp->count] = ent_id;

    memcpy((char *)cmp->data + (cmp->count * cmp->data_size), cmp_data, cmp->data_size);

    cmp->count++;
}

void *ecs_get_entity_component(EcsComponentId cmp_id, EcsEntityId ent_id) {
    EcsComponent *cmp = &space->components[cmp_id];

    if (ent_id >= cmp->sparse_cap) {
        return NULL;
    }

    uint32_t dense_id = cmp->sparse_ids[ent_id];
    if (dense_id == INVALID_ID) {
        return NULL;
    }

    if (dense_id >= cmp->count)
        abort();

    if (cmp->dense_ids[dense_id] != ent_id)
        abort();

    return (char *)cmp->data + cmp->data_size * dense_id;
}

uint32_t ecs_get_component_count(EcsComponentId cmp_id) {
    if (cmp_id >= space->current_component_id) {
        return INVALID_ID;
    }

    EcsComponent *cmp = &space->components[cmp_id];

    if (!cmp) {
        return INVALID_ID;
    }

    return cmp->count;
}

EcsEntityId ecs_get_component_dense(EcsComponentId cmp_id, uint32_t dense_id) {
    if (cmp_id >= space->current_component_id) {
        return INVALID_ID;
    }

    EcsComponent *cmp = &space->components[cmp_id];

    if (!cmp) {
        return INVALID_ID;
    }

    if (dense_id >= cmp->cap) {
        return INVALID_ID;
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

void ecs_destroy_space() {
    if (!space) {
        return;
    }

    if (space->entities) {
        ECS_FREE(space->entities);
    }

    for (int i = 0; i < space->current_component_id; i++) {
        ECS_FREE(space->components[i].data);
    }

    ECS_FREE(space->components);
    ECS_FREE(space);

    return;
}
