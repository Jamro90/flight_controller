#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main(void) {
  if(!glfwInit()) {
    printf("GLFW failed!\n");
    glfwTerminate();
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "window", NULL, NULL);

  if(window == NULL) {
    printf("Window failed!\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if(glewInit() != GLEW_OK) {
    printf("glew failed!\n");
    glfwTerminate();
    return -1;
  }
  
  glViewport(0, 0, 800, 600);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  do {
      glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glfwSwapBuffers(window);
      glfwPollEvents();
  } while(!glfwWindowShouldClose(window));


  printf("Hello World!\n");
  return 0;
}
