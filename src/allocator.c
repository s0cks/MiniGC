#include <stdio.h>
#include <stdlib.h>
#include "allocator.h"

int
mgc_integer_value(alloc_obj_t* aot){
    if(mgc_is_integer(aot)){
        return mgc_to_integer(aot)->value;
    } else{
        printf("Not an integer\n");
        exit(1);
    }
}

double
mgc_double_value(alloc_obj_t* aot){
    if(mgc_is_double(aot)){
        return mgc_to_double(aot)->value;
    } else{
        printf("Not a double\n");
        exit(1);
    }
}

allocator_t*
allocator_create(size_t max_objects){
    allocator_t* alloc = malloc(sizeof(allocator_t));
    alloc->max_objects = max_objects;
    alloc->num_objects = 0;
    alloc->head = NULL;
    return alloc;
}

alloc_obj_t*
allocator_new_integer(allocator_t* alloc, int value){
    if(alloc->num_objects == alloc->max_objects){
        allocator_collect(alloc);
    }

    mgc_integer_t* integer;
    if((integer = malloc(sizeof(mgc_integer_t))) == NULL){
        return NULL;
    }

    integer->value = value;
    integer->obj.type = MGC_INTEGER;
    integer->obj.marked = 0;
    integer->obj.next = alloc->head;
    alloc->head = &integer->obj;
    alloc->num_objects++;
    return &integer->obj;
}

alloc_obj_t*
allocator_new_double(allocator_t* alloc, double value){
    if(alloc->num_objects == alloc->max_objects){
        allocator_collect(alloc);
    }

    mgc_double_t* doubl;
    if((doubl = malloc(sizeof(mgc_double_t))) == NULL){
        return NULL;
    }

    doubl->obj.marked = 0;
    doubl->obj.type = MGC_DOUBLE;
    doubl->value = value;
    doubl->obj.next = alloc->head;
    alloc->head = &doubl->obj;
    alloc->num_objects++;
    return &doubl->obj;
}

void
allocator_mark(alloc_obj_t* aot){
    if(aot->marked){
        return;
    }

    aot->marked = 1;
}

void
allocator_sweep(allocator_t* alloc){
    alloc_obj_t** aot = &alloc->head;
    while(*aot){
        if(!(*aot)->marked){
            alloc_obj_t* unreached = *aot;
            *aot = unreached->next;
            free(unreached);
            alloc->num_objects--;
        } else{
            (*aot)->marked = 0;
            aot = &(*aot)->next;
        }
    }
}

void
allocator_collect(allocator_t* alloc){
    size_t num_objects = alloc->num_objects;

    allocator_sweep(alloc);

    #ifdef MGC_DEBUG
    printf("Collected %lu objects, %lu remaining.\n", num_objects - alloc->num_objects, alloc->num_objects);
    #endif
}

void
allocator_free(allocator_t* alloc){
    allocator_collect(alloc);
    free(alloc);
}