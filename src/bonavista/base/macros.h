#ifndef BONAVISTA_BASE_MACROS_H_
#define BONAVISTA_BASE_MACROS_H_

#define DISALLOW_COPY_AND_ASSIGN(ClassName) \
    ClassName(const ClassName&); \
    void operator=(const ClassName&);

#define ARRAY_SIZE(arr) \
    (sizeof(arr) / sizeof(arr[0]))

#endif
