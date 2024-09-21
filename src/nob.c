#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#define NOBUILD_IMPLEMENTATION
#include "./tools/nobuild/nobuild.h"
#define LOADER_IMPLEMENTATION
#include "./tools/term/loader.h"

#define TERMINAL_CONTROLL_IMPLEMENTATION
#include "./tools/term/terminal_controll.h"

// macros
#include "./tools/term/macronomicon.h"

#define MAX_ARRAY 10
// build flags
enum {
  BUILD_ALL,
  BUILD_DESK,
  BUILD_STM32,
  FLASH_STM32,
  EXIT
};

void drawLogo() {
  // terminal params
  int rows = 0;
  int cols = 0;

  struct winsize win = terminalSize(&rows, &cols);
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
    // file 
  char file[] = "flight_controller_logo.txt";
  FILE *fd = fopen("./tools/assets/flight_controller_logo.txt", "r");
  char logo[512];
  
  // draw loop
  clearScreen();
  setCursor(0, 0);
 
  while(!feof(fd) && fgets(logo, 512, fd)) {
    fprintf(stdout, ESC_CL, HEX_2_RGB(CL_GREEN));
    printf("\r");
    for(int i = 0; i < rows; ++i) { 
      (i == (cols - strlen(logo)) / 2) ? fprintf(stdout, "%s", logo) : fprintf(stdout, " ");
      fflush(stdout);
    } 
  }

  fprintf(stdout, CODE_END);
  // memory free
  fclose(fd);
}


void build_desktop() {
  Cmd cmd = {0};
  Cstr_Array command = cstr_array_make(
    COMPILER,
    CFLAGS,
    DESK_PROG".c",
    "-o",
    DESK_PROG,
    OPENGL,
    '\0'
  );
  cmd.line = command;
  cmd_run_sync(cmd);
  CODE_OK("complete desktop!");
}

void build_stm32(void) {
  Cmd cmd = {0};
  Cstr_Array command = cstr_array_make(
    COMPILER_STM32, 
    STM32_FLAGS, 
    "-I.", "-I.", "-I"INCLUDES, 
    "-I.", "-I.", "-I"INCLUDES, 
    "-DSTM32F1", "-DSTM32F103C8", "-I"INCLUDES, "-o", 
    STM32_PATH"main.o", "-c", 
    STM32_PATH"main.c", '\0');
  // object file
  cmd.line = command;
  cmd_run_sync(cmd);
  CODE_OK("complete stm32 compile!");
}

void generate_link(void) {
  // genlink 
  Cmd cmd = {0};
  Cstr_Array command = cstr_array_make(
    COMPILER_STM32, 
    "-E", COMMON, 
    "-DSTM32F1", "-DSTM32F103C8", 
    "-D_ROM=64K", "-D_RAM=20K", 
    "-D_ROM_OFF=0x08000000", "-D_RAM_OFF=0x20000000", 
    "-P", "-E", INC_PATH"ld/linker.ld.S", 
    "-o", "generated.stm32f103c8.ld",
    '\0');
  cmd.line = command;
  cmd_run_sync(cmd);
  CODE_OK("Link generated successfuly!");
}

void linking_process(void) {
  // linking process
  Cmd cmd = {0};
  Cstr_Array command = cstr_array_make(
    COMPILER_STM32, 
    "-Tgenerated.stm32f103c8.ld", 
    "-L"INC_PATH"lib", 
    "-nostartfiles", COMMON, 
    "-specs=nano.specs", 
    "-Wl,--gc-sections", 
    "-L"INC_PATH"lib", 
    STM32_PATH"main.o", 
    "-lopencm3_stm32f1", 
    "-Wl,--start-group", 
    "-lc", "-lgcc", "-lnosys", 
    "-Wl,--end-group", "-o", STM32_PATH"main.elf", 
    '\0');
  cmd.line = command;
  cmd_run_sync(cmd);
  CODE_OK("Linking success!");
}

void generate_binary(void) {
  Cmd cmd = {0};
  Cstr_Array command = cstr_array_make(
    "arm-none-eabi-objcopy", 
    "-O", "binary", 
    STM32_PATH"main.elf", 
    STM32_PATH"main.bin",
    '\0');
  cmd.line = command;
  cmd_run_sync(cmd);
  CODE_OK("Generating binary successfull!");
}

void flash_stm32(void) {
  INFO("flashing stm32");
  CMD("st-flash", "--reset", "write", STM32_PATH"main.bin", "0x8000000");
}

void testing_paths(void) {
  fprintf(stdout, "# -----------------------------TESTING----------------------------- #\n");
  PATH_TEST(NOB_PATH);
  PATH_TEST(STM32_PATH);
  PATH_TEST(INCLUDES"/");
  PATH_TEST("./tools/server/");
  PATH_TEST("./tools/client/");
  PATH_TEST("./tools/database/");
  PATH_TEST("./tools/assets/");
  PATH_TEST("./tools/others/");
  PATH_TEST("./tools/nonexist/");
  fprintf(stdout, "# ----------------------------------------------------------------- #\n");
  fflush(stdout);
}

int main(int argc, char **argv) {
  GO_REBUILD_URSELF(argc, argv);
  // projct's paths 
  int check = 1;
  int next = 0;
  
  drawLogo();

  fprintf(stdout, "Check your compilation: \n"
                  "[%1d] build all \n"
                  "[%1d] build desktop \n"
                  "[%1d] build STM \n"
                  "[%1d] flash STM \n"
                  "[%1d] exit \n ",
  BUILD_ALL, BUILD_DESK, BUILD_STM32, FLASH_STM32, EXIT);

  printf("%14s", "Your choice: ");
  PANIC_EXIT(scanf("%1i", &check) != 1, "NaN input!");
  // begin building the project
      char *log[MAX_ARRAY]; 
      void *func_array[MAX_ARRAY];
  switch(check) {
  // build project
    case BUILD_ALL:
      log[next] = "Testing";
      func_array[next] = testing_paths;

      log[++next] = "Desktop build";
      func_array[next] = build_desktop;

      log[++next] = "Building STM32 code";
      func_array[next] = build_stm32;

      log[++next] = "Generating link for STM32";
      func_array[next] = generate_link;

      log[++next] = "Link process for STM32";
      func_array[next] = linking_process;

      log[++next] = "Generate binary WSAD";
      func_array[next] = generate_binary;

      log[++next] = NULL;
      func_array[next] = NULL;
      break;

  // build desktop (only)
    case BUILD_DESK:
      log[next] = "Testing";
      func_array[next] = testing_paths;

      log[++next] = "Desktop build";
      func_array[next] = build_desktop;

      log[++next] = NULL;
      func_array[next] = NULL; 
      break;
  
  // build stm32 (only)
    case BUILD_STM32:
      log[++next] = "Building STM32 code";
      func_array[next] = build_stm32;

      log[++next] = "Generating link for STM32";
      func_array[next] = generate_link;

      log[++next] = "Link process for STM32";
      func_array[next] = linking_process;

      log[++next] = "Generate binary WSAD";
      func_array[next] = generate_binary;

      log[++next] = NULL;
      func_array[next] = NULL;
      break;
  
  // flash bluepill 
    case FLASH_STM32:
      flash_stm32();
      break;

    case EXIT:
      fprintf(stdout, "%s", "Exitning");
      for(int i = 0; i < 3; ++i) {
        fprintf(stdout, ".");
        fflush(stdout);
        usleep(500000);
      }
      fprintf(stdout, "\n");
      clearScreen();
      setCursor(0,0);
      return EXIT_SUCCESS;

    default:
      PANIC_EXIT(-1, "Option not available!");
  }
  
  Func_array arr = createArray(log, (void*)func_array);
  prog(&arr);
  // end building the project
  fflush(stdout);
  CODE_OK("COMPLETE!");

  return 0;
}
