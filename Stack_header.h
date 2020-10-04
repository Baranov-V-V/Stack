#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <time.h>
#include <io.h>
#include <stdlib.h>
#include <math.h>

/*!
Error codes
!*/
enum Error_t {
    SUCCESS = 0,
    SIZE_ERROR = 1,
    PUSH_ERROR = 2,
    TOP_ERROR = 3,
    INCREASE_ERROR = 4,
    DECREASE_ERROR = 5,
};


/*!
Type witch will be used in stack
!*/
typedef double Type;

struct StackArray {
    size_t size_;
    size_t capacity_;
    size_t increase_value_;
    size_t decrease_value_;
    Type* data_;
};

/*!
returns size of stack
@param[in] stack stack with size to return
@return size size of stack
!*/
size_t Size(StackArray* stack);

/*!
returns capacity of stack
@param[in] stack stack with capacity to return
@return capacity capacity of stack
!*/
size_t Capacity(StackArray* stack);

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
@return error code (0 if pushed successfully)
!*/
Error_t Push(struct StackArray* stack,Type value);

/*!
Gets last element of stack if it has
@param[in] stack stack from witch take element
@return elem last element of stack
!*/
Type Top(struct StackArray* stack);

/*!
Pops elem from stack if it has one
@param[in] stack stack to pop
@return error code (0 if poped succesfully)
!*/
Error_t Pop(struct StackArray* stack);

/*!
 Destroys stack so it can not be used again
@param[in] stack stack to destroy
@return
!*/
Error_t Destroy(struct StackArray* stack);

