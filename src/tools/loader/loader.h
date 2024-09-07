#ifndef _LOADER_H_
#define _LOADER_H_
  #include <stdio.h>
  #include <unistd.h>
  #include <stdbool.h>
  #include <string.h>
  #include <locale.h>

  typedef struct {
    char **log;
    int len;
    void (**func)(void);
  } Func_array;

  void prog(Func_array *arr);
  Func_array createArray(char **log, void (**func)(void));
    
#endif // _LOADER_H_

#ifndef _LOADER_IMPLEMENTATION_
#define _LOADER_IMPLEMENTATION_

Func_array createArray(char **log, void (**func)(void)) {
  int len = 0;
  while(log[len] != NULL && func[len] != NULL) {
    len++;
  } 
  Func_array array = (Func_array){log, len, (void*)func};
  return array;
}

void prog(Func_array *arr){
  char *old_local = setlocale(LC_ALL, "");
  setlocale(LC_ALL, "en_US.UTF-8");

  char squere[] = "\u25A0";
  char squere_e[] = "\u25A1";
  printf("[ ");
  for(int i = 0; i < 100; ++i) printf("%s", squere_e);
  printf(" ] 0\%");
  fflush(stdout);

  int i = 0;
  int count = 0;
  int iter = 0;
  for(i = 0; i <= 100; ++i) {
    iter = (i%arr->len + 1) * 100/arr->len;
    printf("\r");

    // printing logs & call functions
    printf("%-110s\n", arr->log[i%arr->len]); // pass log and write it 
    arr->func[i%arr->len](); // function call from struct list

    // printing progress bar 
    printf("[ ");
    for(count = 0; count <= iter; ++count) printf("%s", squere);
    for(int j = count; j < 100; ++j) printf("%s", squere_e);
    printf(" ]%3i%%", iter);
    fflush(stdout);
    usleep(300000); // temporary
    if(iter >= 100) break;
    else printf("\r");
    
    fflush(stdout);
  }
  printf("\n");

  setlocale(LC_ALL, old_local);
}

#endif // _LOADER_IMPLEMENTATION_
