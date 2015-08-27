#ifndef MINIGC_ALLOCATOR_H
#define MINIGC_ALLOCATOR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifndef MGC_ASS
#define MGC_ASS 256
#endif

#ifndef __attribute__
#define __attribute__(x)
#endif

#define MGC_DEBUG 1

typedef enum {
    MGC_INTEGER,
    MGC_DOUBLE
} alloc_obj_type;

typedef struct salloc_obj_t{
    unsigned int marked;
    alloc_obj_type type;

    struct salloc_obj_t* next;
} alloc_obj_t;

typedef struct{
    alloc_obj_t obj;
    double value;
} mgc_double_t;

typedef struct{
    alloc_obj_t obj;
    int value;
} mgc_integer_t;

#ifndef container_of
#define container_of(ptr_, type_, member_)({ \
    const typeof(((type_*) 0)->member_)* __mbptr = ((void*) ptr_); \
    (type_*)((char*) __mbptr - offsetof(type_, member_)); \
    })
#endif

#define mgc_typeof(aot_) ((aot_)->type)
#define mgc_is_integer(aot_) ((aot_) && mgc_typeof(aot_) == MGC_INTEGER)
#define mgc_is_double(aot_) ((aot_) && mgc_typeof(aot_) == MGC_DOUBLE)
#define mgc_to_integer(aot_) container_of(aot_, mgc_integer_t, obj)
#define mgc_to_double(aot_) container_of(aot_, mgc_double_t, obj)

int mgc_integer_value(alloc_obj_t* aot);

double mgc_double_value(alloc_obj_t* aot);

typedef struct{
    alloc_obj_t* head;
    size_t num_objects;
    size_t max_objects;
} allocator_t;

allocator_t* allocator_create(size_t max_objects);
alloc_obj_t* allocator_new_integer(allocator_t* alloc, int value);
alloc_obj_t* allocator_new_double(allocator_t* alloc, double value);
void allocator_mark(alloc_obj_t* aot);
void allocator_free(allocator_t* alloc);
void allocator_sweep(allocator_t* alloc);
void allocator_collect(allocator_t* alloc);

#ifdef __cplusplus
};
#endif

#endif