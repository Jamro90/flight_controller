#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

#define WIDTH 800
#define HEIGHT 600

#define PANIC(ERROR, FORMAT, ...)                                                                              \
  if(ERROR) {                                                                                                  \
    fprintf(stderr, "\x1b[31m# ---------------------- ERROR ---------------------- #\n");                      \
    fprintf(stderr, "%-25s %25i\n%-25s %25s\n%-25s %25s\n%-25s %25i\n" FORMAT"\n",                             \
    " ERROR:", ERROR, " FILE:", __FILE_NAME__, " FUNCTION:", __FUNCTION__, " LINE:", __LINE__, ##__VA_ARGS__); \
    fprintf(stderr, "# ---------------------- ERROR ---------------------- #\x1b[0m\n");                       \
  exit(ERROR);                                                                                                 \
  };
typedef struct {
  uint32_t graphicsFamily;
} QueueFamilyIndices;

int main(void) {
  printf("Hello World!\n");
  const char *appCrypto = "client";
  const uint32_t version = VK_MAKE_VERSION(0, 0, 1);
  // init glfw
  PANIC(!glfwInit(), "GLFW initialization failed!");
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  // create window
  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, appCrypto, NULL, NULL);
  PANIC(window == NULL, "Window failed to create!");

  // create instance
  VkInstance instance;
  VkApplicationInfo appInfo;
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = appCrypto;
  appInfo.applicationVersion = version;
  appInfo.pEngineName = "NoEngine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_3;

  // instance info
  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  // extensions
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;

  // extesions layer
  createInfo.enabledLayerCount = 0;

  PANIC(vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS, "Failed to create an instance!");

  uint32_t extesionCount = 0;
  VkExtensionProperties *extensions;
  vkEnumerateInstanceExtensionProperties(NULL, &extesionCount,extensions);

  // enumarate support layers
  const char *validationLayers[] = {"VK_LAYER_KHRONOS_validation"};
  size_t validationCount = sizeof(validationLayers)/sizeof(validationLayers[0]);

  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, NULL);
  VkLayerProperties *availableLayers = (VkLayerProperties*)calloc(layerCount, sizeof(VkLayerProperties));
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

  for(size_t i = 0; i < validationCount; ++i) {
    bool layerFound = false;
    const char *layerName = validationLayers[i];

    for(size_t j = 0; j < layerCount; ++j){
      if(strcmp(layerName, availableLayers[j].layerName) == 0) {
        layerFound = true;
        break;
      }
    }
  }

  // devices
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);
  PANIC(deviceCount == 0, "No GPU available!");

  VkPhysicalDevice *devices = (VkPhysicalDevice*)calloc(deviceCount, sizeof(VkPhysicalDevice));
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices); 
  deviceCount = sizeof(devices)/sizeof(devices[0]);
  for(size_t i = 0; i < deviceCount; ++i) {
    if(devices[i]) {
      physicalDevice = devices[i];
      break;
    }
  }
  PANIC(physicalDevice == VK_NULL_HANDLE, "filed to find suitable GPU!");
;
  // device suitability checks
  VkPhysicalDeviceProperties deviceProperties;
  vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
  VkPhysicalDeviceFeatures deviceFeatures;
  vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

  PANIC(!(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader), "device cant featured");

  // queue families
  QueueFamilyIndices indices;
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);
  VkQueueFamilyProperties *queueFamilies = (VkQueueFamilyProperties*)calloc(queueFamilyCount, sizeof(VkQueueFamilyProperties));
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);

  for(size_t i = 0; i < queueFamilyCount; ++i) {
    if(queueFamilies->queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }
  }

  for(size_t i = 0; i < queueFamilyCount; ++i) {
    if(indices.graphicsFamily == 0){
      break;
    } else{ 
      PANIC(indices.graphicsFamily == 0, "Indices 0!");
    }
  }


  // loop
  do {
    glfwPollEvents();
  } while(!glfwWindowShouldClose(window));

  // cleaning memory
  free(availableLayers);
  free(devices);
  vkDestroyInstance(instance, NULL);
  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}
