#include <Arduino.h>

void printString(const char *pcString);

void vTask1(void *params);
void vTask2(void *params);

SemaphoreHandle_t xMutex;
const TickType_t xMaxBlockTimeTicks = 1;

void setup(){
  Serial.begin(115200);
  randomSeed(analogRead(0));

  xMutex = xSemaphoreCreateMutex();
  
  if(xMutex != NULL){
    xTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
  }

  vTaskDelete(NULL);
}

void loop() {}

void printString(const char *msg){
  xSemaphoreTake(xMutex, portMAX_DELAY);
  Serial.println((char*)msg);
  xSemaphoreGive(xMutex);
}

void vTask1(void *params){
  const char *msg = "Task 1 is running";
  while(1){
    printString(msg);
    vTaskDelay(random(10, 20) / portTICK_PERIOD_MS);
  }
}

void vTask2(void *params){
  const char *msg = "Task 2 is running";
  while(1){
    printString(msg);
    vTaskDelay(random(10, 20) / portTICK_PERIOD_MS);
  }
}