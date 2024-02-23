/*
    xTaskCreate -> Cria uma nova taks
    vTaskDelete( NULL ) -> Deleta uma Task
    vTaskDelay -> delay especial que não trava o programa

    Mutex -> Guarda um recurso crítico para evitar concorrência
    
    taskENTER_CRITICAL(); -> desativa as interrupções
*/

// O cabecalho Arduino.h já inclui o freeRTOS
#include <Arduino.h>

#define CORE_0 0 
#define CORE_1 1
// tskNO_AFFINITY

void vPrintString( const char *pcString);

void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

const char *pcTaskName1 = "Task 1 is running";
const char *pcTaskName2 = "Task 2 is running";

portMUX_TYPE myMutex = portMUX_INITIALIZER_UNLOCKED;

void setup() {
  Serial.begin( 115200 ); 
   
  xTaskCreatePinnedToCore( vTask1, "Task 1", 768, (void *) pcTaskName1, 1, NULL, CORE_0);   
  xTaskCreatePinnedToCore( vTask2, "Task 2", 768, (void *) pcTaskName2, 1, NULL, CORE_1);
}

void loop() {
  vTaskDelay( 100 / portTICK_PERIOD_MS );
}

void vPrintString( const char *pcString ){
  taskENTER_CRITICAL( &myMutex ); // Liga a interrupção

  Serial.println( (char*)pcString );

  taskEXIT_CRITICAL( &myMutex ); // Desliga a interrupção
}

void vTask1( void *pvParameters ){
  for( ;; ){
    vPrintString((char *) pvParameters);      
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
  }
  vTaskDelete(NULL);
}

void vTask2( void *pvParameters ){
  for( ;; ){
    vPrintString((char *) pvParameters);
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
  }
  vTaskDelete(NULL);
}