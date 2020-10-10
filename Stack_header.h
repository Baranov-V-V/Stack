#pragma once

#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#ifdef BARANOV_V_V_DEBUG

#define CONSTRUCT(stack, new_size)      \
    Construct(new_size, &stack);        \
    stack.stack_name_ = #stack;         \
    stack.current_line = __LINE__;      \
    stack.current_file = __FILE__;

#define DUMP(stack, StackErrors)                                    \
    const char* file_name = "Stack_(" #stack ")_Dump.txt";          \
    FILE* fp = fopen(file_name, "w");                               \
    StackDump(stack, file_name, fp, StackErrors);

#define ASSERT_OK(stack)                                      \
    if (StackOk(stack) != StackErrors::OK) {                  \
        DUMP(stack, StackOk(stack));                          \
        assert(!"OK");                                        \
    }

#else

#define CONSTRUCT(stack)
#define ASSERT_OK(stack)
#define DUMP(stack, StackErrors)

#endif // MYDEBUG

const int REALLOC_VALUE  = 2;
const int INCREASE_LEVEL = 2;
const int DECREASE_LEVEL = 4;

typedef uint64_t canary_t;
typedef int64_t hash_t;

const canary_t CANARY = 0xBAAADCFEFEBDCBFE;

/*!
Error codes
!*/
enum Error_t {
    SUCCESS        = 0,
    LENGTH_ERROR   = 1,
    PUSH_ERROR     = 2,
    TOP_ERROR      = 3,
    NO_INCREASE    = 4,
    NO_DECREASE    = 5,
    POP_ERROR      = 6,
    SIZE_ERROR     = 7,
    CAPACITY_ERROR = 8
};

/*!
Error codes for stack
!*/
enum StackErrors {
    OK                       = 0,
    INVALID_SIZE             = 1,
    INVALID_CAPACITY         = 2,
    POISON_ERROR             = 3,
    DATA_NULL                = 4,
    OVERFLOW                 = 5,
    BEGIN_DATA_CANARY_ERROR  = 6,
    END_DATA_CANARY_ERROR    = 7,
    BEGIN_STACK_CANARY_ERROR = 8,
    END_STACK_CANARY_ERROR   = 9,
    STACK_HASH_ERROR         = 10,
    DATA_HASH_ERROR          = 11
};

static char* ErrorNames[] = {
    "OK",
    "SIZE_ERROR",
    "CAPACITY_ERROR",
    "POISON_ERROR",
    "DATA IS NULL",
    "STACK OVERFLOW",
    "LEFT DATA CANARY VALUE IS CHANGED",
    "RIGHT DATA CANARY VALUE IS CHANGED",
    "LEFT STACK CANARY VALUE IS CHANGED",
    "RIGHT STACK CANARY VALUE IS CHANGED",
    "STACK HASH VALUES DOES NOT MATCH",
    "DATA HASH VALUES DOES NOT MATCH"
};


/*!
type to store size and capacity
!*/
typedef int64_t int_t;

/*!
Type_t witch will be used in stack as value type
!*/
typedef double Type_t;

struct StackArray {
    canary_t canary_begin;
    #ifdef BARANOV_V_V_DEBUG
    char* stack_name_;
    char* current_file;
    int current_line;
    #endif // DEBUG
    
    hash_t data_hash;
    hash_t stack_hash;
    
    int_t size_;
    int_t capacity_;
    Type_t* data_;
    canary_t canary_end;
};

/*!
returns size of stack
@param[in] stack stack with size to return
@param[in] variable to put size

@return size size of stack
!*/
Error_t Size(StackArray* stack, int_t* size);

/*!
returns capacity of stack
@param[in] stack stack with capacity to return
@param[in] capacity variable to put capacity
@return error code (0 if poped succesfully)
!*/
Error_t Capacity(StackArray* stack, int_t* capacity);

/*!
 Increases stack in capacity by its Increase_value_
@param[in] stack stack with capacity to Increase
@return error_code (0 if increased successfully)
!*/
Error_t StackIncrease(struct StackArray* stack);

/*!
 Decreases stack in capacity by its increase_value_
@param[in] stack stack with capacity to decrease
@return error_code (0 if increased successfully)
!*/
Error_t StackDecrease(struct StackArray* stack);

/*!
creates new stack with given parameters
@param[in] start_size start size of stack to create
@return stack
!*/
Error_t Construct(int start_size, struct StackArray* new_stack);

/*!
Pushes elem in stack with increases in capacity if needed
@param[in] stack stack to push
@param[in] value variable to push
@return error code (0 if pushed successfully)
!*/
Error_t Push(struct StackArray* stack,Type_t value);

/*!
Gets last element of stack if it has
@param[in] stack stack from witch take element
@param[in] value variable to put top element
@return error code (0 if poped succesfully)
!*/
Error_t Top(struct StackArray* stack, Type_t* value);

/*!
Pops elem from stack if it has one
@param[in] stack stack to pop
@return error code (0 if poped succesfully)
!*/
Error_t Pop(struct StackArray* stack);

/*!
 Destroys stack so it can not be used again
@param[in] stack stack to destroy
@return error code (0 if poped succesfully)
!*/
Error_t Destroy(struct StackArray* stack);

StackErrors StackOk(struct StackArray* stack);

void StackDump(struct StackArray* stack,const char* file_name, FILE* fp, StackErrors err_no);

hash_t RotateLeft(hash_t value, int shift);

hash_t MakeHash(void* data, int_t size);

void HashStack(struct StackArray* stack);
