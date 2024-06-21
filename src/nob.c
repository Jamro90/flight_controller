#define NOBUILD_IMPLEMENTATION
#include "../tools/nobuild/nobuild.h"

#define DESKTOP 
#define STM32CODE

// paths
#define PATH_TEST(path) (IS_DIR(path)) ? INFO("%s\t\t\tOK", path) : ERRO("%s\t\t\tNOT OK", path);
#define NOB_PATH "../tools/nobuild/"
#define STM32_PATH "./controller_stm32/"

// build options desktop
#define COMPILER "clang"
#define CFLAGS "-Wall", "-Wextra", "-O2"
#define OPENGL "-lGL", "-lGLEW", "-lglfw"
#define DESK_PROG "main"

// build options stm32
#define COMPILER_STM32  "arm-none-eabi-gcc"
#define COMMON "-mcpu=cortex-m3", "-mthumb", "-msoft-float"
#define STM32_FLAGS "-Os", "-std=c99", "-ggdb3", COMMON, "-fno-common", "-ffunction-sections", "-fdata-sections", "-Wextra", "-Wshadow", "-Wno-unsude-variable", "-Wimplicit-function-declaration", "-Wredundant-decls", "-Wstrict-prototypes", "-Wmissing-prototypes", "-MD", "-Wall", "-Wundef"
#define INCLUDES "../tools/libopencm3/include"
#define DSTM "-DSTM32F1", "-DSTM32F103C8"
// build flags
enum {
  BUILD_ALL,
  BUILD_DESK,
  BUILD_STM32,
  FLASH_STM32
};

void build_desktop(void) {
  INFO("building desktop");
  CMD(COMPILER, CFLAGS, DESK_PROG".c", "-o", DESK_PROG, OPENGL);
  INFO("complete desktop!");
}

void build_stm32(void) {
  INFO("building stm32");
  // object file
  CMD(COMPILER_STM32, STM32_FLAGS, "-I.", "-I.", "-I"INCLUDES, "-I.", "-I.", "-I"INCLUDES, DSTM, "-I"INCLUDES, "-o", STM32_PATH"main.o", "-c", STM32_PATH"main.c");
  // genlink 
  CMD(COMPILER_STM32, "-E", COMMON, DSTM, "-D_ROM=64K", "-D_RAM=20K", "-D_ROM_OFF=0x08000000", "-D_RAM_OFF=0x20000000", "-P", "-E", "../tools/libopencm3/ld/linker.ld.S", "-o", "generated.stm32f103c8.ld");
  // linking process
  CMD(COMPILER_STM32, "-Tgenerated.stm32f103c8.ld", "-L../tools/libopencm3/lib", "-nostartfiles", COMMON, "-specs=nano.specs", "-Wl,--gc-sections", "-L../tools/libopencm3/lib", STM32_PATH"main.o", "-lopencm3_stm32f1", "-Wl,--start-group", "-lc", "-lgcc", "-lnosys", "-Wl,--end-group", "-o", STM32_PATH"main.elf");
  // generate bin
  CMD("arm-none-eabi-objcopy", "-O", "binary", STM32_PATH"main.elf", STM32_PATH"main.bin");
}

void flash_stm32(void) {
  INFO("flashing stm32");
  CMD("st-flash", "--reset", "write", STM32_PATH"main.bin", "0x8000000");
}

int main(int argc, char **argv) {
  GO_REBUILD_URSELF(argc, argv);
  
  // projct's paths 
  INFO("# -----------------TESTING----------------- #");
  PATH_TEST(STM32_PATH)
  PATH_TEST(NOB_PATH)
  PATH_TEST(INCLUDES)
  INFO("# ----------------------------------------- #");

  // begin building the project
  INFO("Let there be light!");
  switch(0) {
  // build project
    case BUILD_ALL:
      build_desktop();
      build_stm32();
      break;

  // build desktop (only)
    case BUILD_DESK:
      build_desktop();
      break;
  
  // build stm32 (only)
    case BUILD_STM32:
      build_stm32();
      break;
  
  // flash bluepill 
    case FLASH_STM32:
      flash_stm32();
      break;

    default:
      PANIC("Something went wrong! Abort!");
      break;
  }
  
  // end building the project
  INFO("COMPLETE!");

  return 0;
}
