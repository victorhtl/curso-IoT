#include <Arduino.h>

/*
  4 timers
  Prescaler de 16 bits
  Contador de 64 bits
*/
static const uint16_t timer_divider = 80; // Divide o clock por 80
static const uint64_t timer_max_count = 1000000;

static const int led_pin = 2;

static hw_timer_t *timer = NULL;

/*
  IRAM_ATTR -> a função fica carregada na memória
*/
void IRAM_ATTR onTimer(){
  int pin_state = digitalRead(led_pin);
  digitalWrite(led_pin, !pin_state);
}

void setup(){
  pinMode(led_pin, OUTPUT);

  // Cria o hardware timer
  timer = timerBegin(0, timer_divider, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, timer_max_count, true);
  timerAlarmEnable(timer);
}

void loop(){}