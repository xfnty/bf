#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <string.h>


#define ARRAY_LENGTH(array) (sizeof(array)/sizeof(array[0]))
#define CONSTRAIN(v, a, b) ((v < a) ? a : ((v > b) ? b : v))
#define WRAP(v, a, b) ((v < a) ? b : ((v > b) ? a : v))
#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)

typedef enum {
    ERROR_SUCCESS = 0,
    ERROR_UNDEFINED,
} error_t;

static const char* ERROR_MESSAGES[] = {
    [ERROR_SUCCESS]         = "OK",
    [ERROR_UNDEFINED]       = "Undefined error",
};

#define ASSERT_RETURN_VALUE(cond, value)    do { if (!(cond)) return value; } while(0);
#define ASSERT_PROPAGATE(expr) do { error_t err = (expr); if (err != ERROR_SUCCESS) return err; } while(0);

// Reference: https://en.wikipedia.org/wiki/ANSI_escape_code#CSI_(Control_Sequence_Introducer)_sequences
#define ANSI_RESET          "\033[0;97m"
#define ANSI_WHITE          "\033[0;97m"
#define ANSI_GRAY           "\033[0;37m"
#define ANSI_GREEN          "\033[0;92m"
#define ANSI_YELLOW         "\033[0;93m"
#define ANSI_RED            "\033[0;91m"
#define ANSI_CYAN           "\033[0;96m"
#define ANSI_WHITE_BOLD     "\033[1;97m"

#define __FILENAME__ ((strrchr(__FILE__, '/') != NULL)                      \
     ? strrchr(__FILE__, '/') + 1                                           \
     : (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))

#define LOG(msg)        printf(ANSI_RESET msg ANSI_RESET "\n")
// #define LOG_TRACE(msg)  printf(ANSI_RESET msg ANSI_WHITE " At %s:%d:%s()" ANSI_RESET "\n", __FILENAME__, __LINE__, __FUNCTION__)
#define LOGF(fmt, ...)  printf(ANSI_RESET fmt ANSI_RESET "\n", __VA_ARGS__)
// #define LOGF_TRACE(fmt, ...)  printf(ANSI_RESET fmt ANSI_WHITE " At %s:%d:%s()" ANSI_RESET "\n", __VA_ARGS__, __FILENAME__, __LINE__, __FUNCTION__)
#define LOG_ERROR(msg) LOG(ANSI_RED "error" ANSI_RESET ": " msg)
#define LOGF_ERROR(fmt, ...) LOGF(ANSI_RED "error" ANSI_RESET ": " fmt, __VA_ARGS__)

#endif  // COMMON_H_
