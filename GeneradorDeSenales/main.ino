const int channel1Pin = A0;  // Pin analógico para el canal 1
const int channel2Pin = A1;  // Pin analógico para el canal 2
const float frequency1 = 1000.0;  // Frecuencia en Hz para la onda senoidal (canal 1)
const float frequency2 = 500.0;   // Frecuencia en Hz para la onda cuadrada (canal 2)
const int amplitude1 = 2047;      // Amplitud para la onda senoidal (canal 1)
const int amplitude2 = 1023;      // Amplitud para la onda cuadrada (canal 2)

void setup() {
  // No es necesario realizar ninguna configuración adicional en el setup
}

void loop() {
  float time = millis() / 1000.0;  // Tiempo en segundos
  
  // Generar onda senoidal (canal 1)
  int sensorValue1 = amplitude1 * sin(2 * PI * frequency1 * time) + amplitude1;
  analogWrite(channel1Pin, sensorValue1 / 4);  // Escalar la señal a un rango de 0-255

  // Generar onda cuadrada (canal 2)
  int sensorValue2 = (time - floor(time)) < 0.5 ? amplitude2 : 0;
  analogWrite(channel2Pin, sensorValue2 / 4);  // Escalar la señal a un rango de 0-255
  
  delay(1);  // Pequeña pausa para estabilizar las señales
}

