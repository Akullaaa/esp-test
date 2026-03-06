#include <Arduino.h>
#include "memory_tests.h"

void setup() {
  Serial.begin(115200);
  
  // Ждем подключения USB CDC (важно для ESP32-S3)
  // Добавляем таймаут 3 секунды, чтобы не зависнуть навечно
  unsigned long startWait = millis();
  while(!Serial && (millis() - startWait < 3000)) {
    delay(10);
  }
  delay(2000); // Увеличиваем задержку, чтобы терминал успел "проснуться"
  
  Serial.println("\n=================================");
  Serial.println("   ESP32-S3-N16R8 Memory Test");
  Serial.println("=================================");
  
  printChipInfo();
  printFlashInfo();
  printSramInfo();
  printPsramInfo();
  printAdditionalInfo();
  
  Serial.println("\n=================================");
}

void loop() {
  static int counter = 0;
  Serial.printf("\n[%d] Free Heap: %d KB | Free PSRAM: %d KB\n", 
                counter++, 
                ESP.getFreeHeap() / 1024,
                ESP.getFreePsram() / 1024);
  delay(10000);
}