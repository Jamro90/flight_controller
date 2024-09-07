#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#define CODE_RED "\x1b[31m"
#define CODE_YELLOW "\x1b[33m"
#define CODE_GREEN "\x1b[32m"
#define CODE_BLUE "\x1b[36m"
#define CODE_END "\x1b[0m"

#define CODE_OK(FORMAT, ...) INFO(CODE_GREEN FORMAT CODE_END, ##__VA_ARGS__)
#define CODE_NOK(FORMAT, ...) ERRO(CODE_RED FORMAT CODE_END, ##__VA_ARGS__)
#define CODE_WARN(FORMAT, ...) WARN(CODE_YELLOW FORMAT CODE_END, ##__VA_ARGS__)

#define PANIC_EXIT(ERROR, FORMAT, ...) {                                          \
  if(ERROR) {                                                                     \
    fprintf(stderr, CODE_RED "\t+-------------------+\n"                          \
                             "\t [ERROR]    %-20i \n"                             \
                             "\t [FILE]     %-20s \n"                             \
                             "\t [FUNCTION] %-20s \n"                             \
                             "\t [LINE]     %-20i \n"                             \
                             "\t+-------------------+\n" CODE_END FORMAT"\n",     \
            ERROR, __FILE_NAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);         \
  exit(ERROR);                                                                    \
  }                                                                               \
}

#define TEXT(FORMAT, ...) INFO(CODE_BLUE FORMAT CODE_END, ##__VA_ARGS__)

// paths
#define PATH_TEST(path) ((IS_DIR(path)) ? CODE_OK("%-50s%-50s", path, "[OK]") : CODE_WARN("%-50s%-50s", path, "[MISSING]"))
#define NOB_PATH "./tools/nobuild/"
#define STM32_PATH "./controller_stm32/"

// build options desktop
#define COMPILER "clang"
#define CFLAGS ("-Wall", "-Wextra", "-O2")
#define OPENGL ("-lGL", "-lGLEW", "-lglfw")
#define DESK_PROG "main"

// build options stm32
#define COMPILER_STM32  "arm-none-eabi-gcc"
#define INC_PATH "./tools/libopencm3/"
#define COMMON ("-mcpu=cortex-m3", "-mthumb", "-msoft-float")
#define STM32_FLAGS ("-Os", "-std=c99", "-ggdb3", COMMON, "-fno-common", "-ffunction-sections", "-fdata-sections", "-Wextra", "-Wshadow", "-Wno-unsude-variable", "-Wimplicit-function-declaration", "-Wredundant-decls", "-Wstrict-prototypes", "-Wmissing-prototypes", "-MD", "-Wall", "-Wundef")
#define INCLUDES INC_PATH"include"
