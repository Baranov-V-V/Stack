#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>


#ifndef NDEBUG
#define SET_DEBUG(stack)      \
    stack.stack_name_ = #stack
#endif // NDEBUG

#define DUMP(stack, StackErrors)                                                \
    const char* file_name = "Stack_(" #stack ")_Dump.txt";                      \
    FILE* fp = fopen(file_name, "w");                                           \
    StackDump(stack, file_name, fp, StackErrors);

#define ASSERT_OK(stack)                                      \
    if (StackOk(stack) != StackErrors::OK) {                  \
        DUMP(stack, StackOk(stack));                          \
        assert(!"OK");                                        \
    }                                                         \


const int INCREASE_VALUE = 2;
const int DECREASE_VALUE = 3;

/*!
Error codes
!*/
enum Error_t {
    SUCCESS        = 0,
    LENGTH_ERROR   = 1,
    PUSH_ERROR     = 2,
    TOP_ERROR      = 3,
    INCREASE_ERROR = 4,
    DECREASE_ERROR = 5,
    POP_ERROR      = 6
};

/*!
Error codes for stack
!*/
enum StackErrors {
    OK             = 0,
    SIZE_ERROR     = 1,
    CAPACITY_ERROR = 2,
    POISON_ERROR   = 3,
    DATA_NULL      = 4,
    OVERFLOW       = 5
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
    #ifndef NDEBUG
    char* stack_name_;
    #endif // NDEBUG
    int_t size_;
    int_t capacity_;
    Type_t* data_;
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
struct StackArray Construct(int start_size);

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

