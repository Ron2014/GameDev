#pragma once

/**
 * use SINGLETON inside class
*/

#define SINGLETON(T) \
static T *Instance() { \
    static T instance; \
    return &instance; \
}