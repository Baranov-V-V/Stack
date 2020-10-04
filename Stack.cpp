#include "Stack_header.h"


int StackOk(struct StackArray* stack) {
    assert(stack != NULL);
    static int count = 0;
    count++;

    int failure_count = 0;

    if (stack->size_ < 0 || stack->capacity_ < 0 || stack->size_ > stack->capacity_) {
        failure_count++;
    }

    for (int i = stack->size_; i < stack->capacity_; i++) {
        if (stack->data_[i] != NAN) {
            failure_count++;
        }
    }

    return failure_count;
}

void StackDump(struct StackArray* stack, const char* file_name, FILE* fp) {
    assert(stack != NULL);
    assert(fp != NULL);
    assert(file_name != NULL);

    fprintf(fp, "adress of Stack: [%p]\n", stack);
    fprintf(fp, "{\n    size = %lld\n    capacity = %lld\n", stack->size_, stack->capacity_);
    fprintf(fp, "    data adress: [%p]\n", stack->data_);

    fprintf(fp, "   {\n");
    for (int64_t i = 0; i < stack->capacity_; i++) {
        if (i < stack->size_) {
            fprintf(fp, "       *[%lld] = %g\n",i ,stack->data_[i]);
        }
        else {
            fprintf(fp, "        [%lld] = %g (Poison!)\n", i, stack->data_[i]);
        }
    }
    fprintf(fp, "   }\n}\n");

}

Error_t Size(StackArray* stack, int64_t* size) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    if (stack->size_ >= 0) {
        *size = stack->size_;
        return Error_t::LENGTH_ERROR;
    }

    ASSERT_OK(stack);
    return Error_t::LENGTH_ERROR;
}

Error_t Capacity(StackArray* stack, int64_t* capacity) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    if (stack->capacity_ >= 0) {
        *capacity = stack->capacity_;
        return Error_t::SUCCESS;
    }

    ASSERT_OK(stack);
    return Error_t::LENGTH_ERROR;
}

Error_t StackIncrease(struct StackArray* stack) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    int64_t old_capacity = stack->capacity_;

    if (stack->capacity_ == 0) {
        stack->capacity_ = 1;
    }
    else if (stack->size_ == stack->capacity_) {
        stack->capacity_ = (int64_t) stack->capacity_ * INCREASE_VALUE;
    }
    else {
        return Error_t::INCREASE_ERROR;
    }

    realloc(stack->data_, sizeof(Type_t) * stack->capacity_);
    assert(stack->data_ != NULL);

    for(int i = old_capacity; i < stack->capacity_; ++i) {
        stack->data_[i] = NAN;
    }

    ASSERT_OK(stack);
    return Error_t::SUCCESS;
}

Error_t StackDecrease(struct StackArray* stack) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    if (stack->capacity_ / stack->size_ >= DECREASE_VALUE && stack->capacity_ >= DECREASE_VALUE) {
        stack->capacity_ = (int) stack->capacity_ / DECREASE_VALUE;

        realloc(stack->data_, sizeof(Type_t) * stack->capacity_);
        assert(stack->data_ != NULL);
    }

    ASSERT_OK(stack);
    return Error_t::SUCCESS;
}

struct StackArray Construct(int start_size) {
    assert(start_size >= 0);

    struct StackArray new_stack;
    new_stack.size_ = 0;
    new_stack.capacity_ = start_size;
    new_stack.data_ = (Type_t*) calloc(start_size, sizeof(Type_t));

    for(int64_t i = 0; i < start_size; ++i) {
        new_stack.data_[i] = NAN;
    }

    ASSERT_OK(&new_stack);
    return new_stack;
}

Error_t Push(struct StackArray* stack,Type_t value) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    if (stack->size_ == stack->capacity_) {
        StackIncrease(stack);
    }
    else if (stack->size_ > stack->capacity_){
        return Error_t::PUSH_ERROR;
    }
    (stack->data_)[stack->size_++] = value;

    ASSERT_OK(stack);
    return Error_t::SUCCESS;
}

Error_t Top(struct StackArray* stack, Type_t* value) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    if (stack->size_ > 0) {
        *value = stack->data_[stack->size_ - 1];
        return Error_t::SUCCESS;
    }

    ASSERT_OK(stack);
    return Error_t::SUCCESS;
}

Error_t Pop(struct StackArray* stack) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    if (stack->size_ > 0) {
        StackDecrease(stack);
        stack->data_[stack->size_--] = NAN;
    }
    else {
        exit(Error_t::LENGTH_ERROR);
    }

    ASSERT_OK(stack);
    Error_t::SUCCESS;
}

Error_t Destroy(struct StackArray* stack) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    stack->size_ = -1;
    stack->capacity_ = -1;

    free(stack->data_);
    stack->data_ = NULL;

    ASSERT_OK(stack);
    return Error_t::SUCCESS;
}

