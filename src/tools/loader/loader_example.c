#include <stdio.h>
#include <stdlib.h>
#include "loader.h"

int print_int(void) {

  printf("%s\n", "Hello int!");
  return EXIT_SUCCESS;
}

bool print_bool(void) {
  printf("%s\n", "Hello bool!");
  return true;
}

float print_float(void) {
  printf("%s\n", "Hello float!");
  return 1.0f;
}
int main(void) {

  // NULL at the end determine save end of chain command
  char *log[] = {"1st command", "2nd command", "3rd command", "4th command", NULL};
  void *func[] = {print_int, print_bool, print_float, print_int, print_bool, print_float, NULL};
  // choose from hardcode length or more flexible semi-auto length
  //Func_array r = {log, 3, (void*)func};       // solution #1
  Func_array r = createArray(log, (void*)func); // solution #2
  prog(&r);
  
  return EXIT_SUCCESS;
}
