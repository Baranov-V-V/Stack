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

    if(*(canary_t*)((char*)stack->data_ - sizeof(canary_t)) != CANARY_LEFT) {
        return LEFT_CANARY_ERROR;
    }
    if(*(canary_t*)((char*)stack->data_ + (stack->capacity_ + 1) * sizeof(Type_t)) != CANARY_RIGHT) {
        return RIGHT_CANARY_ERROR;
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

    #ifdef BARANOV_V_V_DEBUG
    fprintf(fp, "Stack name: <%s>\n\n", stack->stack_name_);
    fprintf(fp, "Stack made\n");
    fprintf(fp, "   ON line %d\n", stack->current_line);
    fprintf(fp, "   IN file %s\n\n", stack->current_file);
    #endif // DEBUG

    fprintf(fp, "Error code : %d (%s)\n\n", err_no, ErrorNames[err_no]);
    fprintf(fp, "adress of Stack: [%p]\n", stack);
    fprintf(fp, "{\n    size = %lld\n    capacity = %lld\n", stack->size_, stack->capacity_);
    fprintf(fp, "    data adress: [%p]\n\n", stack->data_);

    fprintf(fp, "   Initial left canary value: %llX\n", CANARY_LEFT);
    fprintf(fp, "   Current left canary value: %llX\n", *(canary_t*)((char*)stack->data_ - sizeof(canary_t)));
    fprintf(fp, "   Initial right canary value: %llX\n", CANARY_RIGHT);
    fprintf(fp, "   Current right canary value: %llX\n\n", *(canary_t*)((char*)stack->data_ + (stack->capacity_ + 1) * sizeof(Type_t)));

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
    ASSERT_OK(stack);

    *size = stack->size_;

    ASSERT_OK(stack);
    return SUCCESS;
}

Error_t Capacity(StackArray* stack, int_t* capacity) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    *capacity = stack->capacity_;

    ASSERT_OK(stack);
    return SUCCESS;
}

Error_t StackIncrease(struct StackArray* stack) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    int_t old_capacity = stack->capacity_;

    if (stack->capacity_ == 0) {
        stack->capacity_ = 1;
    }
    else if (stack->size_ == stack->capacity_) {
        stack->capacity_ = (int_t) stack->capacity_ * REALLOC_VALUE;
    }
    else {
        return NO_INCREASE;
    }

    void* data_tmp = realloc((char*)stack->data_ - sizeof(canary_t), sizeof(Type_t) * stack->capacity_ + 2 * sizeof(canary_t));
    assert(data_tmp != NULL);

    printf("increased pointer : %p\n", data_tmp);

    stack->data_ = (Type_t*)((char*)data_tmp + sizeof(canary_t));

    *(canary_t*)((char*)stack->data_ - sizeof(canary_t)) = CANARY_LEFT;
    *(canary_t*)((char*)stack->data_ + (stack->capacity_ + 1) * sizeof(Type_t)) = CANARY_RIGHT;


    for(int i = old_capacity; i < stack->capacity_; ++i) {
        stack->data_[i] = NAN;
    }

    ASSERT_OK(stack);
    return SUCCESS;
}

Error_t StackDecrease(struct StackArray* stack) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    if (stack->size_ > 0 && stack->capacity_ / stack->size_ >= DECREASE_LEVEL && stack->capacity_ >= DECREASE_LEVEL) {

        stack->capacity_ = (int_t) stack->capacity_ / REALLOC_VALUE;

        void* data_tmp = realloc((char*)stack->data_ - sizeof(canary_t), sizeof(Type_t) * stack->capacity_ + 2 * sizeof(canary_t));
        assert(data_tmp != NULL);

        stack->data_ = (Type_t*)((char*)data_tmp + sizeof(canary_t));

        *(canary_t*)((char*)stack->data_ - sizeof(canary_t)) = CANARY_LEFT;
        *(canary_t*)((char*)stack->data_ + (stack->capacity_ + 1) * sizeof(Type_t)) = CANARY_RIGHT;

    }
    else {
        return NO_DECREASE;
    }

    ASSERT_OK(stack);
    return SUCCESS;
}

struct StackArray Construct(int start_size) {
    assert(start_size >= 0);

    struct StackArray new_stack;
    new_stack.size_ = 0;
    new_stack.capacity_ = start_size;

    void* data_tmp = calloc(1, sizeof(Type_t) * start_size + 2 * sizeof(canary_t));
    assert(data_tmp != NULL);

    new_stack.data_ = (Type_t*)((char*)data_tmp + sizeof(canary_t));

    *(canary_t*)((char*)new_stack.data_ - sizeof(canary_t)) = CANARY_LEFT;
    *(canary_t*)((char*)new_stack.data_ + (start_size + 1) * sizeof(Type_t)) = CANARY_RIGHT;

    for(int_t i = 0; i < start_size; ++i) {
        new_stack.data_[i] = NAN;
    }

    ASSERT_OK(&new_stack);
    return new_stack;
}

Error_t Push(struct StackArray* stack,Type_t value) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    StackIncrease(stack);

    (stack->data_)[stack->size_++] = value;

    ASSERT_OK(stack);
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
    return SUCCESS;
}

Error_t Destroy(struct StackArray* stack) {
    assert(stack != NULL);

    stack->size_ = -1;
    stack->capacity_ = -1;

    free((char*)stack->data_ - sizeof(canary_t));
    stack->data_ = NULL;

    return SUCCESS;
}

