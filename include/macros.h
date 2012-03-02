#ifndef MACROS_H
#define MACROS_H

#define CONCATENATE_DIRECT(a, b) a ## b
#define CONCATENATE(a, b) CONCATENATE_DIRECT(a, b)
#define ANONYMOUS_VAR(begin) CONCATENATE(begin, __LINE__)

#endif
