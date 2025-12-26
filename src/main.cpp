#include <Arduino.h>

// Прототип функции
void testPSRAM();

void setup() {
  Serial.begin(115200);
  
  // Ждем подключения USB CDC (важно для ESP32-S3)
  while(!Serial) {
    delay(10);
  }
  delay(1000);
  
  Serial.println("\n=================================");
  Serial.println("   ESP32-S3-N16R8 Memory Test");
  Serial.println("=================================");
  
  // Базовая информация о чипе
  Serial.printf("Chip Model: %s\n", ESP.getChipModel());
  Serial.printf("Chip Revision: %d\n", ESP.getChipRevision());
  Serial.printf("CPU Cores: %d\n", ESP.getChipCores());
  Serial.printf("CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("SDK Version: %s\n", ESP.getSdkVersion());
  
  // Flash память
  Serial.println("\n--- Flash Memory ---");
  Serial.printf("Flash Size: %.2f MB\n", ESP.getFlashChipSize() / (1024.0 * 1024.0));
  Serial.printf("Flash Speed: %d MHz\n", ESP.getFlashChipSpeed() / 1000000);
  Serial.printf("Flash Mode: %d\n", ESP.getFlashChipMode());
  
  // Внутренняя SRAM
  Serial.println("\n--- Internal SRAM ---");
  Serial.printf("Total SRAM: %d KB\n", ESP.getHeapSize() / 1024);
  Serial.printf("Free Heap: %d KB\n", ESP.getFreeHeap() / 1024);
  Serial.printf("Min Free Heap: %d KB\n", ESP.getMinFreeHeap() / 1024);
  Serial.printf("Max Alloc Heap: %d bytes\n", ESP.getMaxAllocHeap());
  
  // PSRAM проверка
  Serial.println("\n--- PSRAM (External RAM) ---");
  if (psramFound()) {
    Serial.println("✅ PSRAM DETECTED!");
    Serial.printf("PSRAM Size: %.2f MB\n", ESP.getPsramSize() / (1024.0 * 1024.0));
    Serial.printf("Free PSRAM: %.2f MB\n", ESP.getFreePsram() / (1024.0 * 1024.0));
    
    // Проверим работу PSRAM
    testPSRAM();
  } else {
    Serial.println("❌ PSRAM NOT FOUND!");
    Serial.println("   Возможно, нужно включить PSRAM в настройках");
  }
  
  // Дополнительная информация
  Serial.println("\n--- Additional Info ---");
  Serial.printf("Cycle Count: %llu\n", ESP.getCycleCount());
  Serial.printf("Sketch Size: %d bytes\n", ESP.getSketchSize());
  Serial.printf("Free Sketch Space: %d bytes\n", ESP.getFreeSketchSpace());
  
  Serial.println("\n=================================");
}

void testPSRAM() {
  Serial.println("\n--- PSRAM Performance Test ---");
  
  size_t test_size = 1024 * 1024; // 1MB
  
  // Выделяем память в PSRAM
  uint32_t start = micros();
  uint8_t* psram_buffer = (uint8_t*)ps_malloc(test_size);
  uint32_t alloc_time = micros() - start;
  
  if (psram_buffer) {
    Serial.printf("Allocated 1MB in PSRAM in %d µs\n", alloc_time);
    
    // Тест записи
    start = micros();
    for (size_t i = 0; i < test_size; i++) {
      psram_buffer[i] = i & 0xFF;
    }
    uint32_t write_time = micros() - start;
    Serial.printf("Write test: %d µs (%.2f MB/s)\n", 
                  write_time, 
                  (test_size / (1024.0 * 1024.0)) / (write_time / 1000000.0));
    
    // Тест чтения
    volatile uint8_t dummy;
    start = micros();
    for (size_t i = 0; i < test_size; i += 256) {
      dummy = psram_buffer[i];
    }
    uint32_t read_time = micros() - start;
    Serial.printf("Read test: %d µs\n", read_time);
    
    // Освобождаем
    free(psram_buffer);
    Serial.println("PSRAM test completed successfully!");
  } else {
    Serial.println("Failed to allocate PSRAM!");
  }
}

void loop() {
  static int counter = 0;
  Serial.printf("\n[%d] Free Heap: %d KB | Free PSRAM: %d KB\n", 
                counter++, 
                ESP.getFreeHeap() / 1024,
                ESP.getFreePsram() / 1024);
  delay(10000);
}