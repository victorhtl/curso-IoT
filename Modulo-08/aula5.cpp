/*
  uxTaskGetStackHighWaterMark
*/

#include <Arduino.h>

#define CORE_1 1

void testTask(void *parameter){
  int a = 1;
  int b[100];

  for(int i=0; i<100; i++){
    b[i] = a + 1;
    Serial.println(i);
  }
  UBaseType_t size = uxTaskGetStackHighWaterMark(NULL);
  Serial.print("Size: ");
  Serial.println(size);
  vTaskDelete(NULL);
}

void setup(){
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(testTask, "Test Task", 1000, NULL, 1, NULL, CORE_1);
}

void loop(){
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  Serial.println("*");
}