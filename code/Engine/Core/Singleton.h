#pragma once

#define SINGLETON(T) \
static T *T::Instance() { \
    static T instance; \
    return &instance; \
}