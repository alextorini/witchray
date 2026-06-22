#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "smetanka_ecs.h"
#include "smetanka_misc.h"

#define ECS_MALLOC malloc
#define ECS_MALLOC_TYPE(type) ((type *)malloc(sizeof(type)))
#define ECS_MALLOC_ARR(type, count) ((type *)malloc(sizeof(type) * (count)))
#define ECS_CALLOC calloc
#define ECS_CALLOC_TYPE(type, count) ((type *)calloc((count), sizeof(type)))
#define ECS_REALLOC realloc
#define ECS_REALLOC_ARR(ptr, type, count) ((type *)realloc((ptr), (sizeof(type) * (count))))
#define ECS_FREE free

#define ABORT() printf("info: %s:%d: ", __FILE__, __LINE__); abort();

typedef uint32_t EcsEntityId;
typedef uint32_t EcsEntityGen;

typedef struct {
    EcsEntityHandle handle;
    uint8_t active;
} EcsEntity;

typedef struct {
    uint32_t *sparse_id_list;
    EcsEntityId *dense_id_list;
    uint32_t count;
    uint32_t sparse_capacity;
    uint32_t capacity;
    size_t data_size;
    void *data;
    char name[MAX_COMPONENT_NAME_LENGTH];
} EcsComponent;

typedef struct {
    EcsEntityId *id_list;
    uint32_t count;
    uint32_t capacity;
} EntityFreeSlotPool;

typedef struct {
    EcsEntityId next_entity_id;
    uint32_t current_entities_capacity;
    EcsComponentId current_component_id;
    uint32_t current_components_capacity;
    EcsEntity *entity_list;
    EcsComponent *component_list;
    EntityFreeSlotPool entity_free_slots_pool;
} EcsSpace;

static EcsSpace *space;

void ecs_create_space() {
    space = ECS_MALLOC_TYPE(EcsSpace);
    if (!space) {
        ABORT();
    }

    space->current_entities_capacity = INITIAL_ENTITIES_CAPACITY;
    space->current_components_capacity = INITIAL_COMPONENTS_CAPACITY;
    space->next_entity_id = 0;
    space->current_component_id = 0;

    space->entity_list = ECS_MALLOC_ARR(EcsEntity, space->current_entities_capacity);
    if (!space->entity_list) {
        ECS_FREE(space);

        ABORT();
    }

    space->component_list = ECS_MALLOC_ARR(EcsComponent, space->current_components_capacity);
    if (!space->component_list) {
        ECS_FREE(space->entity_list);
        ECS_FREE(space);

        ABORT();
    }


    space->entity_free_slots_pool.capacity = INITIAL_ENTITIES_CAPACITY;
    space->entity_free_slots_pool.count = 0;
    space->entity_free_slots_pool.id_list = ECS_MALLOC_ARR(uint32_t, space->entity_free_slots_pool.capacity);
    if (!space->entity_free_slots_pool.id_list) {
        ECS_FREE(space->component_list);
        ECS_FREE(space->entity_list);
        ECS_FREE(space);

        ABORT();
    }


    for (int i = 0; i < space->entity_free_slots_pool.capacity; i++) {
        space->entity_free_slots_pool.id_list[i] = INVALID_ID;
    }
}

EcsComponentId ecs_register_component(const char *name, size_t data_size) {
    if (space->current_component_id >= space->current_components_capacity) {
        space->current_components_capacity *= 2;
        EcsComponent *component_list =
            ECS_REALLOC_ARR(space->component_list, EcsComponent, space->current_components_capacity);
        if (!component_list) {
            ABORT();
        }

        space->component_list = component_list;
    }

    EcsComponentId component_id = space->current_component_id;
    EcsComponent *component_pointer = &space->component_list[component_id];

    component_pointer->data_size = data_size;

    strncpy(component_pointer->name, name, MAX_COMPONENT_NAME_LENGTH - 1);
    component_pointer->name[MAX_COMPONENT_NAME_LENGTH - 1] = '\0';

    component_pointer->sparse_capacity = INITIAL_ENTITIES_CAPACITY;
    component_pointer->capacity = INITIAL_ENTITIES_CAPACITY;

    component_pointer->count = 0;

    component_pointer->sparse_id_list = ECS_MALLOC_ARR(uint32_t, component_pointer->sparse_capacity);
    if (!component_pointer->sparse_id_list) {
        ABORT();
    }

    for (uint32_t i = 0; i < component_pointer->sparse_capacity; i++) {
        component_pointer->sparse_id_list[i] = INVALID_ID;
    }

    component_pointer->dense_id_list = ECS_MALLOC_ARR(EcsEntityId, component_pointer->capacity);
    if (!component_pointer->dense_id_list) {
        ECS_FREE(component_pointer->sparse_id_list);

        ABORT();
    }

    component_pointer->data = ECS_MALLOC(data_size * component_pointer->capacity);
    if (!component_pointer->data) {
        ECS_FREE(component_pointer->sparse_id_list);
        ECS_FREE(component_pointer->dense_id_list);

        ABORT();
    }

    space->current_component_id++;

    return component_id;
}

EcsEntityHandle ecs_create_entity() {
    if (space->next_entity_id > MAX_ENTITIES_CAPACITY) {
        ABORT();
    }

    EcsEntityId entity_id;
    if (space->entity_free_slots_pool.count > 0) {
        entity_id = space->entity_free_slots_pool.id_list[--space->entity_free_slots_pool.count];
        if (entity_id == INVALID_ID) {
            ABORT();
        }
    } else {
        entity_id = space->next_entity_id++;
        while (entity_id >= space->current_entities_capacity) {
            uint32_t new_capacity = space->current_entities_capacity * 2;
            if (new_capacity > MAX_ENTITIES_CAPACITY) {
                new_capacity = MAX_ENTITIES_CAPACITY;
            }

            if (new_capacity == space->current_entities_capacity) {
                continue;
            }

            space->current_entities_capacity = new_capacity;

            EcsEntity *entity_list = ECS_REALLOC_ARR(space->entity_list, EcsEntity, space->current_entities_capacity);
            if (!entity_list) {
                ABORT();
            }

            space->entity_list = entity_list;
        }

        space->entity_list[entity_id].handle = pack_handle(entity_id, 0);
    }

    EcsEntity *entity_pointer;
    entity_pointer = &space->entity_list[entity_id];
    entity_pointer->active = 1;

    return entity_pointer->handle;
}

void ecs_add_component(EcsEntityHandle entity_handle, EcsComponentId component_id, void *component_data) {
    EcsEntityId entity_id = get_handle_id(entity_handle);
    if (entity_id >= space->next_entity_id) {
        ABORT();
    }

    if (get_handle_gen(space->entity_list[entity_id].handle) != get_handle_gen(entity_handle)) {
        ABORT();
    }

    if (!space->entity_list[entity_id].active) {
        ABORT();
    }

    if (component_id >= space->current_component_id) {
        ABORT();
    }

    EcsComponent *cmp_ptr = &space->component_list[component_id];

    if (entity_id < cmp_ptr->sparse_capacity && cmp_ptr->sparse_id_list[entity_id] != INVALID_ID) {
        ABORT();
    }

    while (entity_id >= cmp_ptr->sparse_capacity) {
        uint32_t old_cap = cmp_ptr->sparse_capacity;
        cmp_ptr->sparse_capacity *= 2;
        uint32_t *sparse_ids = ECS_REALLOC_ARR(cmp_ptr->sparse_id_list, uint32_t, cmp_ptr->sparse_capacity);
        if (!sparse_ids) {
            ABORT();
        }

        for (uint32_t i = old_cap; i < cmp_ptr->sparse_capacity; i++) {
            sparse_ids[i] = INVALID_ID;
        }

        cmp_ptr->sparse_id_list = sparse_ids;
    }

    while (cmp_ptr->count >= cmp_ptr->capacity) {
        cmp_ptr->capacity *= 2;
        EcsEntityId *dense_ids = ECS_REALLOC_ARR(cmp_ptr->dense_id_list, EcsEntityId, cmp_ptr->capacity);
        if (!dense_ids) {
            ABORT();
        }

        cmp_ptr->dense_id_list = dense_ids;

        void *data = ECS_REALLOC(cmp_ptr->data, cmp_ptr->data_size * cmp_ptr->capacity);
        if (!data) {
            ABORT();
        }

        cmp_ptr->data = data;
    }


    cmp_ptr->sparse_id_list[entity_id] = cmp_ptr->count;
    cmp_ptr->dense_id_list[cmp_ptr->count] = entity_id;

    memcpy((char *)cmp_ptr->data + (cmp_ptr->count * cmp_ptr->data_size), component_data, cmp_ptr->data_size);

    cmp_ptr->count++;
}

void *ecs_get_entity_component(EcsComponentId component_id, EcsEntityHandle entity_handle) {
    EcsComponent *component = &space->component_list[component_id];
    EcsEntityId entity_id = get_handle_id(entity_handle);

    if (component_id >= space->current_component_id) {
        return NULL;
    }

    if (entity_id >= component->sparse_capacity) {
        return NULL;
    }

    if (!space->entity_list[entity_id].active) {
        ABORT();
    }

    if (get_handle_gen(space->entity_list[entity_id].handle) != get_handle_gen(entity_handle)) {
        ABORT();
    }

    uint32_t dense_id = component->sparse_id_list[entity_id];
    if (dense_id == INVALID_ID) {
        return NULL;
    }

    if (dense_id >= component->count) {
        ABORT();
    }

    if (component->dense_id_list[dense_id] != entity_id) {
        ABORT();
    }

    return (char *)component->data + component->data_size * dense_id;
}

uint32_t ecs_get_component_count(EcsComponentId component_id) {
    if (component_id >= space->current_component_id) {
        return 0;
    }

    EcsComponent *cmp = &space->component_list[component_id];

    return cmp->count;
}

EcsEntityHandle ecs_get_component_dense(EcsComponentId component_id, uint32_t dense_id) {
    if (component_id >= space->current_component_id) {
        return INVALID_ID;
    }

    EcsComponent *cmp = &space->component_list[component_id];

    if (!cmp) {
        return INVALID_ID;
    }

    if (dense_id >= cmp->count) {
        return INVALID_ID;
    }

    EcsEntityId entity_id = cmp->dense_id_list[dense_id];

    return space->entity_list[entity_id].handle;
}

static void ecs_remove_from_component(EcsComponent *component, EcsEntityId entity_id) {
    if (entity_id >= component->sparse_capacity) {
        return;
    }

    uint32_t dense_id = component->sparse_id_list[entity_id];
    if (dense_id == INVALID_ID) {
        return;
    }

    if (component->count == 0) {
        ABORT();
    }

    uint32_t last_index = component->count - 1;
    EcsEntityId last_ent = component->dense_id_list[last_index];

    component->dense_id_list[dense_id] = last_ent;
    memcpy(
        (char*)component->data + dense_id * component->data_size,
        (char*)component->data + last_index * component->data_size,
        component->data_size
    );

    component->sparse_id_list[last_ent] = dense_id;

    component->count--;

    component->sparse_id_list[entity_id] = INVALID_ID;
}

void ecs_destroy_entity(EcsEntityHandle entity_handle) {
    EcsEntityId entity_id = get_handle_id(entity_handle);
    if (entity_id >= space->next_entity_id) {
        return;
    }

    EcsEntity *entity = &space->entity_list[entity_id];
    if (!entity->active) {
        return;
    }

    if (get_handle_gen(entity->handle) != get_handle_gen(entity_handle)) {
        ABORT();
    }

    if (entity_id == INVALID_ID) {
        return;
    }

    for (uint32_t i = 0; i < space->current_component_id; i++)
    {
        ecs_remove_from_component(&space->component_list[i], entity_id);
    }

    entity->active = 0;
    entity->handle = increase_handle_gen(entity->handle);

    while (space->entity_free_slots_pool.count >= space->entity_free_slots_pool.capacity) {
        space->entity_free_slots_pool.capacity *= 2;

        EcsEntityId *ids = ECS_REALLOC_ARR(space->entity_free_slots_pool.id_list, EcsEntityId, space->entity_free_slots_pool.capacity);
        if (!ids) {
            ABORT();
        }

        space->entity_free_slots_pool.id_list = ids;
    }

    space->entity_free_slots_pool.id_list[space->entity_free_slots_pool.count++] = entity_id;
}


void ecs_destroy_space() {
    if (!space) {
        return;
    }

    if (space->entity_list) {
        ECS_FREE(space->entity_list);
    }

    for (uint32_t i = 0; i < space->current_component_id; i++) {
        ECS_FREE(space->component_list[i].sparse_id_list);
        ECS_FREE(space->component_list[i].dense_id_list);
        ECS_FREE(space->component_list[i].data);
    }

    ECS_FREE(space->component_list);
    ECS_FREE(space->entity_free_slots_pool.id_list);
    ECS_FREE(space);

    space = NULL;

    return;
}
