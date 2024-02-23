/*
    Taskhandle_t -> Um ponteiro para a task

    Funções que utilizam este parâmetro:
    vTaskDelete
    vTaskSuspend
    vTaskResume
    vTaskPrioritySet
    uxTaskPriorityGet
*/
#include <Arduino.h>

#define CORE_0 0 
#define CORE_1 1
// tskNO_AFFINITY

void vPrintString( const char *pcString);
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

portMUX_TYPE myMutex = portMUX_INITIALIZER_UNLOCKED;

// Tasks handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

void setup(){
  Serial.begin(115200); 

  xTaskCreatePinnedToCore( vTask1, "Task 1", 768, NULL, 1, &task_1, tskNO_AFFINITY);   
  xTaskCreatePinnedToCore( vTask2, "Task 2", 768, NULL, 1, &task_2, tskNO_AFFINITY);
}

void loop() {
  vPrintString("Loop principal");
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void vPrintString(const char *pcString){
  taskENTER_CRITICAL( &myMutex ); // Liga a interrupção

  Serial.println((char*)pcString);

  taskEXIT_CRITICAL( &myMutex ); // Desliga a interrupção
}

void vTask1( void *pvParameters ){
  while(1){
    vPrintString("Task 1 is running");
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
  }
} 

void vTask2( void *pvParameters ){
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  if(task_1 != NULL){
    vTaskDelete(task_1);
    vPrintString("Task 1 DELETE");
    task_1 = NULL;
  }
  while(1){
    vPrintString("Task 2 is running");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}