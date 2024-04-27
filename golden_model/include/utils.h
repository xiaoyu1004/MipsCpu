#ifndef UTILS_H
#define UTILS_H

#include <iostream>

#define fatal_fmt(fmt, ...)                                                                        \
    {                                                                                              \
        fprintf(stderr, "fatal: %s:%d " fmt "\n", __FILE__, __LINE__, __VA_ARGS__);                \
        exit(1);                                                                                   \
    }

#define fatal_msg(msg) fatal_fmt("%s", msg)

#endif