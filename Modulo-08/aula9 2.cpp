#include <Arduino.h>

/*
  4 timers
  Prescaler de 16 bits
  Contador de 64 bits
*/
static const uint16_t timer_divider = 8; // Divide o clock por 80
static const uint64_t timer_max_count = 1000000;
static const TickType_t task_delay = 2000 / portTICK_PERIOD_MS;

static hw_timer_t *timer = NULL;
static volatile int isr_counter;
static portMUX_TYPE myMutex = portMUX_INITIALIZER_UNLOCKED;

/*
  IRAM_ATTR -> a função fica carregada na memória
*/
void IRAM_ATTR onTimer(){
  portENTER_CRITICAL_ISR(&myMutex);
  isr_counter++;
  portEXIT_CRITICAL_ISR(&myMutex);
}

void printValues(void *params){
  while(1){
    while(isr_counter > 0){
      Serial.println(isr_counter);
      portENTER_CRITICAL(&myMutex);
      isr_counter--;
      portEXIT_CRITICAL(&myMutex);
    }

    vTaskDelay(task_delay);
  }
}

void setup(){
  Serial.begin(115200);

  xTaskCreatePinnedToCore(printValues, "Print values", 1024, NULL, 1, NULL, APP_CPU_NUM);

  // Cria o hardware timer
  timer = timerBegin(0, timer_divider, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, timer_max_count, true);
  timerAlarmEnable(timer);

  vTaskDelete(NULL);
}

void loop(){}