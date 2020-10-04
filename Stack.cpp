#include "Stack_header.h"

size_t Size(StackArray* stack) {
    assert(stack != NULL);

    if (stack->size_ >= 0) {
        return stack->size_;
    }

    exit(Error_t::SIZE_ERROR);
}

size_t Capacity(StackArray* stack) {
    assert(stack != NULL);

    if (stack->capacity_ >= 0) {
        return stack->capacity_;
    }

    exit(Error_t::SIZE_ERROR);
}

Error_t StackIncrease(struct StackArray* stack) {
    size_t old_capacity = stack->capacity_;

    if (stack->capacity_ == 0) {
        stack->capacity_ = 1;
    }
    else if (stack->size_ == stack->capacity_) {
        stack->capacity_ = (size_t) stack->capacity_ * stack->increase_value_;
    }
    else {
        exit(Error_t::INCREASE_ERROR);
    }

    realloc(stack->data_, sizeof(Type_t) * stack->capacity_);
    assert(stack->data_ != NULL);

    for(size_t i = old_capacity; i < Capacity(stack); ++i) {
        stack->data_[i] = NAN;
    }

    return Error_t::SUCCESS;
}

Error_t StackDecrease(struct StackArray* stack) {
    assert(stack != NULL);

    if (Capacity(stack) / Size(stack) >= stack->decrease_value_ && Capacity(stack) >= stack->decrease_value_) {
        stack->capacity_ = (size_t) stack->capacity_ / stack->decrease_value_;

        realloc(stack->data_, sizeof(Type_t) * stack->capacity_);
        assert(stack->data_ != NULL);
    }

    return Error_t::SUCCESS;
}

struct StackArray Construct(int start_size) {
    assert(start_size >= 0);

    struct StackArray new_stack;
    new_stack.size_ = 0;
    new_stack.capacity_ = start_size;
    new_stack.increase_value_ = 2;
    new_stack.decrease_value_ = 3;
    new_stack.data_ = (Type_t*) calloc(start_size, sizeof(Type_t));

    for(size_t i = 0; i < start_size; ++i) {
        new_stack.data_[i] = NAN;
    }

    return new_stack;
}

Error_t Push(struct StackArray* stack,Type_t value) {
    assert(stack != NULL);

    if (stack->size_ == stack->capacity_) {
        StackIncrease(stack);
    }
    else if (stack->size_ > stack->capacity_){
        exit(Error_t::PUSH_ERROR);
    }
    printf("%d ", Size(stack));
    (stack->data_)[stack->size_++] = value;

    return Error_t::SUCCESS;
}

Type_t Top(struct StackArray* stack) {
    assert(stack != NULL);

    if (Size(stack) > 0) {
        return stack->data_[stack->size_ - 1];
    }

    exit(Error_t::TOP_ERROR);
}

Error_t Pop(struct StackArray* stack) {
    assert(stack != NULL);

    if (stack->size_ > 0) {
        StackDecrease(stack);
        stack->data_[stack->size_--] = NAN;

        return Error_t::SUCCESS;
    }
    else {
        exit(Error_t::SIZE_ERROR);
    }
}

Error_t Destroy(struct StackArray* stack) {
    assert(stack != NULL);

    stack->size_ = -1;
    stack->capacity_ = -1;
    stack->decrease_value_ = -1;
    stack->increase_value_ = -1;

    free(stack->data_);
    stack->data_ = NULL;

    return Error_t::SUCCESS;
}

