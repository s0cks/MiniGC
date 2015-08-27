#include "allocator.h"

int main(int argc, char** argv){
    allocator_t* alloc = allocator_create(2);
    alloc_obj_t* alloc_int_1 = allocator_new_integer(alloc, 1);
    alloc_obj_t* alloc_int_2 = allocator_new_integer(alloc, 100);
    alloc_obj_t* alloc_double_1 = allocator_new_double(alloc, 102.0);
    alloc_obj_t* alloc_double_2 = allocator_new_double(alloc, 12973.44);
    allocator_free(alloc);
    return 0;
}