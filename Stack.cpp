#include "Stack_header.h"

StackErrors StackOk(struct StackArray* stack) {
    assert(stack != NULL);

    if (stack->size_ < 0) {
        return SIZE_ERROR;
    }
    if (stack->capacity_ < 0) {
        return CAPACITY_ERROR;
    }
    if (stack->size_ > stack->capacity_) {
        return OVERFLOW;
    }
    if (stack->data_ == NULL) {
        return DATA_NULL;
    }

    for (int i = stack->size_; i < stack->capacity_; i++) {
        if ( isfinite(stack->data_[i]) != 0 ) {
            return POISON_ERROR;
        }
    }

    return OK;
}

void StackDump(struct StackArray* stack, const char* file_name, FILE* fp, StackErrors err_no) {
    assert(stack != NULL);
    assert(fp != NULL);
    assert(file_name != NULL);

    #ifndef NDEBUG
    fprintf(fp, "Stack name: <%s>\n\n", stack->stack_name_);
    #endif // NDEBUG

    fprintf(fp, "Error code : %d\n\n", err_no);
    fprintf(fp, "adress of Stack: [%p]\n", stack);
    fprintf(fp, "{\n    size = %lld\n    capacity = %lld\n", stack->size_, stack->capacity_);
    fprintf(fp, "    data adress: [%p]\n", stack->data_);

    fprintf(fp, "   {\n");
    for (int_t i = 0; i < stack->capacity_; i++) {
        if (i < stack->size_) {
            fprintf(fp, "       *[%lld] = %g\n",i ,stack->data_[i]);
        }
        else {
            fprintf(fp, "        [%lld] = %g (Poison!)\n", i, stack->data_[i]);
        }
    }
    fprintf(fp, "   }\n}\n");

}

Error_t Size(StackArray* stack, int_t* size) {
    assert(stack != NULL);
    StackOk(stack);

    *size = stack->size_;

    return SUCCESS;
}

Error_t Capacity(StackArray* stack, int_t* capacity) {
    assert(stack != NULL);
    StackOk(stack);

    *capacity = stack->capacity_;

    ASSERT_OK(stack);
    return SUCCESS;
}

Error_t StackIncrease(struct StackArray* stack) {
    assert(stack != NULL);
    StackOk(stack);

    int_t old_capacity = stack->capacity_;

    if (stack->capacity_ == 0) {
        stack->capacity_ = 1;
    }
    else if (stack->size_ == stack->capacity_) {
        stack->capacity_ = (int_t) stack->capacity_ * INCREASE_VALUE;
    }
    else {
        return INCREASE_ERROR;
    }

    stack->data_ = (Type_t*) realloc(stack->data_, sizeof(Type_t) * stack->capacity_);
    assert(stack->data_ != NULL);

    for(int i = old_capacity; i < stack->capacity_; ++i) {
        stack->data_[i] = NAN;
    }

    StackOk(stack);
    return SUCCESS;
}

Error_t StackDecrease(struct StackArray* stack) {
    assert(stack != NULL);
    StackOk(stack);

    if (stack->size_ > 0 && stack->capacity_ / stack->size_ >= DECREASE_VALUE && stack->capacity_ >= DECREASE_VALUE) {
        stack->capacity_ = (int) stack->capacity_ / DECREASE_VALUE;

        stack->data_ = (Type_t*) realloc(stack->data_, sizeof(Type_t) * stack->capacity_);
        assert(stack->data_ != NULL);
    }

    StackOk(stack);
    return SUCCESS;
}

struct StackArray Construct(int start_size) {
    assert(start_size >= 0);

    struct StackArray new_stack;
    new_stack.size_ = 0;
    new_stack.capacity_ = start_size;
    new_stack.data_ = (Type_t*) calloc(start_size, sizeof(Type_t));

    for(int_t i = 0; i < start_size; ++i) {
        new_stack.data_[i] = NAN;
    }

    StackOk(&new_stack);
    return new_stack;
}

Error_t Push(struct StackArray* stack,Type_t value) {
    assert(stack != NULL);
    StackOk(stack);

    StackIncrease(stack);

    (stack->data_)[stack->size_++] = value;

    StackOk(stack);
    return SUCCESS;
}

Error_t Top(struct StackArray* stack, Type_t* value) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    if (stack->size_ > 0) {
        *value = stack->data_[stack->size_ - 1];
        return SUCCESS;
    }

    ASSERT_OK(stack);
    return TOP_ERROR;
}

Error_t Pop(struct StackArray* stack) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    StackDecrease(stack);
    if (stack->size_ > 0) {
        stack->data_[--stack->size_] = NAN;
    }
    else {
        return POP_ERROR;
    }
    ASSERT_OK(stack);
    SUCCESS;
}

Error_t Destroy(struct StackArray* stack) {
    assert(stack != NULL);

    stack->size_ = -1;
    stack->capacity_ = -1;

    free(stack->data_);
    stack->data_ = NULL;

    return SUCCESS;
}

