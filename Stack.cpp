#include "Stack_header.h"

hash_t RotateLeft(hash_t value, int shift) {
    assert(shift >= 0);
    const int BITS = 8;

    return (value << shift) | (value >> (sizeof(hash_t) * BITS - shift));
}

hash_t MakeHash(void* data, int_t size) {
    assert(data != NULL);
    assert(size >= 0);

    if (size == 0) {
        return 0;
    }

    hash_t hash_value = *((char*)data + 0);

    for(int_t i = 1; i < size; i++) {
        hash_value += RotateLeft(hash_value, i) ^ *((char*)data + i);
    }

    return hash_value;
}

void HashStack(struct StackArray* stack) {
    assert(stack != NULL);

    stack->data_hash = 0;
    stack->stack_hash = 0;

    stack->stack_hash = MakeHash(stack, sizeof(struct StackArray));
    stack->data_hash = MakeHash(stack->data_, stack->capacity_ * sizeof(Type_t));

}


StackErrors StackOk(struct StackArray* stack) {
    assert(stack != NULL);

    hash_t data_hash_tmp = stack->data_hash;
    hash_t stack_hash_tmp = stack->stack_hash;
    HashStack(stack);

    if (stack->data_hash != data_hash_tmp) {
        return DATA_HASH_ERROR;
    }
    if (stack->stack_hash != stack_hash_tmp) {
        return STACK_HASH_ERROR;
    }

    if (stack->size_ < 0) {
        return INVALID_SIZE;
    }
    if (stack->capacity_ < 0) {
        return INVALID_CAPACITY;
    }
    if (stack->size_ > stack->capacity_) {
        return OVERFLOW;
    }
    if (stack->data_ == NULL) {
        return DATA_NULL;
    }

    if(*(canary_t*)((char*)stack->data_ - sizeof(canary_t)) != CANARY) {
        return BEGIN_DATA_CANARY_ERROR;
    }
    if(*(canary_t*)((char*)stack->data_ + (stack->capacity_ + 1) * sizeof(Type_t)) != CANARY) {
        return END_DATA_CANARY_ERROR;
    }
    if(stack->canary_begin != CANARY) {
        return BEGIN_STACK_CANARY_ERROR;
    }
    if(stack->canary_end != CANARY) {
        return END_STACK_CANARY_ERROR;
    }

    for (int i = stack->size_; i < stack->capacity_; i++) {
        if ( isfinite(stack->data_[i]) != 0 ) {
            return POISON_ERROR;
        }
    }

    //HashStack(stack);
    return OK;
}

void StackRealloc(struct StackArray* stack, int_t new_capacity) {
    assert(stack != NULL);
    assert(new_capacity >= 0);

    void* data_tmp = realloc((char*)stack->data_ - sizeof(canary_t), sizeof(Type_t) * stack->capacity_ + 2 * sizeof(canary_t));
    assert(data_tmp != NULL);

    stack->data_ = (Type_t*)((char*)data_tmp + sizeof(canary_t));

    *(canary_t*)((char*)stack->data_ - sizeof(canary_t)) = CANARY;
    *(canary_t*)((char*)stack->data_ + (stack->capacity_ + 1) * sizeof(Type_t)) = CANARY;

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

    #ifdef BARANOV_V_V_DEBUG
    fprintf(fp, "    Data hash value: %lld\n", stack->data_hash);
    fprintf(fp, "    Stack hash value: %lld\n\n", stack->stack_hash);

    fprintf(fp,"    Canary type |  Initial value   |  Current value\n");
    fprintf(fp,"    Stack begin | %llX | %llX\n", CANARY, stack->canary_begin);
    fprintf(fp,"    Stack end   | %llX | %llX\n", CANARY, stack->canary_begin);
    fprintf(fp,"    Data begin  | %llX | %llX\n", *(canary_t*)((char*)stack->data_ - sizeof(canary_t)));
    fprintf(fp,"    Data end    | %llX | %llX\n", *(canary_t*)((char*)stack->data_ + (stack->capacity_ + 1) * sizeof(Type_t)));
    #endif // BARANOV_V_V_DEBUG

    fprintf(fp, "\n    {\n");
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

    HashStack(stack);
    ASSERT_OK(stack);
    return SUCCESS;
}

Error_t Capacity(StackArray* stack, int_t* capacity) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    if (stack->capacity_ >= 0) {
        *capacity = stack->capacity_;
    } else {
        return CAPACITY_ERROR;
    }


    HashStack(stack);
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

    StackRealloc(stack, stack->capacity_);

    for(int i = old_capacity; i < stack->capacity_; ++i) {
        stack->data_[i] = NAN;
    }

    HashStack(stack);
    ASSERT_OK(stack);
    return SUCCESS;
}

Error_t StackDecrease(struct StackArray* stack) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    if (stack->size_ > 0 && stack->capacity_ / stack->size_ >= DECREASE_LEVEL && stack->capacity_ >= DECREASE_LEVEL) {

        stack->capacity_ = (int_t) stack->capacity_ / REALLOC_VALUE;
        StackRealloc(stack, stack->capacity_);

    }
    else {
        return NO_DECREASE;
    }

    HashStack(stack);
    ASSERT_OK(stack);
    return SUCCESS;
}

Error_t Construct(int start_size, struct StackArray* new_stack) {
    assert(start_size >= 0);

    new_stack->canary_begin = CANARY;
    new_stack->canary_end = CANARY;
    new_stack->size_ = 0;
    new_stack->capacity_ = start_size;

    void* data_tmp = calloc(1, sizeof(Type_t) * start_size + 2 * sizeof(canary_t));
    assert(data_tmp != NULL);

    new_stack->data_ = (Type_t*)((char*)data_tmp + sizeof(canary_t));

    *(canary_t*)((char*)new_stack->data_ - sizeof(canary_t)) = CANARY;
    *(canary_t*)((char*)new_stack->data_ + (start_size + 1) * sizeof(Type_t)) = CANARY;

    for(int_t i = 0; i < start_size; ++i) {
        new_stack->data_[i] = NAN;
    }

    HashStack(new_stack);
    ASSERT_OK(new_stack);
    return SUCCESS;
}

Error_t ShrinkToFit(struct StackArray* stack) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    stack->capacity_ = stack->size_;
    StackRealloc(stack, stack->capacity_);

    HashStack(stack);
    ASSERT_OK(stack);

    return SUCCESS;
}

Error_t Push(struct StackArray* stack, Type_t value) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    StackIncrease(stack);

    (stack->data_)[stack->size_++] = value;

    HashStack(stack);
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

    HashStack(stack);
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

    HashStack(stack);
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

