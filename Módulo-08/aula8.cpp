#include <Arduino.h>

static TimerHandle_t one_shot_timer = NULL;
static TimerHandle_t auto_reload_timer = NULL;

static int led_pin = 2;
int count = 0;

void myTimerCallback(TimerHandle_t xTimer){
  
  if((uint32_t)pvTimerGetTimerID(xTimer) == 0){
    Serial.print("Timer expired");
    
  } else if((uint32_t)pvTimerGetTimerID(xTimer) == 1) {
    digitalWrite(led_pin, count % 2);
    count++;
  }
}

void setup(){
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);

  one_shot_timer = xTimerCreate("one-shot timer", 2000 / portTICK_PERIOD_MS, pdFALSE, (void *)0, myTimerCallback);
  auto_reload_timer = xTimerCreate("auto-reload timer", 1000 / portTICK_PERIOD_MS, pdTRUE, (void *)1, myTimerCallback);

  if(one_shot_timer == NULL || auto_reload_timer == NULL){
    Serial.println("Could not create one of the timers");
  } else {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.println("Starting timers...");

    xTimerStart(one_shot_timer, portMAX_DELAY);
    xTimerStart(auto_reload_timer, portMAX_DELAY);
  }

  vTaskDelete(NULL);
}

void loop(){
}