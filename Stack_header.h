#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <io.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define DUMP(stack)  \
    char* file_name = "Stack_(" #stack ")_Dump.txt";  \
    FILE* fp = fopen(file_name, "w");           \
    fprintf(fp, "Stack <%s> is not OK\n", #stack);\
    StackDump(&stack, file_name, fp);

#define ASSERT_OK(stack) {         \
    if (!StackOk(&stack)) {                  \
        DUMP(stack);                          \
        assert(0);                            \
    }                                                         \
}

const int INCREASE_VALUE = 2;
const int DECREASE_VALUE = 3;

/*!
Error codes
!*/
enum Error_t {
    SUCCESS = 0,
    LENGTH_ERROR = 1,
    PUSH_ERROR = 2,
    TOP_ERROR = 3,
    INCREASE_ERROR = 4,
    DECREASE_ERROR = 5,
};


/*!
Type_t witch will be used in stack
!*/
typedef double Type_t;

struct StackArray {
    int64_t size_;
    int64_t capacity_;
    Type_t* data_;
};

/*!
returns size of stack
@param[in] stack stack with size to return
@param[in] variable to put size

@return size size of stack
!*/
Error_t Size(StackArray* stack, int64_t* size);

/*!
returns capacity of stack
@param[in] stack stack with capacity to return
@param[in] capacity variable to put capacity
@return error code (0 if poped succesfully)
!*/
Error_t Capacity(StackArray* stack, int64_t* capacity);

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

int StackOk(struct StackArray* stack);

void StackDump(struct StackArray* stack, const char* file_name, FILE* fp);
