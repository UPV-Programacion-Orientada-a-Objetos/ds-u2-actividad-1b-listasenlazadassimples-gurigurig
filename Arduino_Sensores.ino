/**
 * Código Arduino para enviar datos de sensores
 * Formato: TIPO:ID:VALOR
 * T = Temperatura (float)
 * P = Presión (int)
 */

// Pines simulados (ajustar según tus sensores reales)
#define PIN_TEMP_1 A0
#define PIN_TEMP_2 A1
#define PIN_PRESION_1 A2
#define PIN_PRESION_2 A3

void setup() {
  Serial.begin(9600);
  pinMode(PIN_TEMP_1, INPUT);
  pinMode(PIN_TEMP_2, INPUT);
  pinMode(PIN_PRESION_1, INPUT);
  pinMode(PIN_PRESION_2, INPUT);
  
  delay(2000); // Esperar conexión serial
  Serial.println("Arduino listo. Enviando datos...");
}

void loop() {
  // Leer sensor de temperatura 1
  int lecturaTempRaw1 = analogRead(PIN_TEMP_1);
  float temperatura1 = (lecturaTempRaw1 * 5.0 / 1023.0) * 20.0 + 15.0; // Conversión a °C
  Serial.print("T:T-001:");
  Serial.println(temperatura1, 1);
  delay(500);
  
  // Leer sensor de temperatura 2
  int lecturaTempRaw2 = analogRead(PIN_TEMP_2);
  float temperatura2 = (lecturaTempRaw2 * 5.0 / 1023.0) * 25.0 + 10.0;
  Serial.print("T:T-002:");
  Serial.println(temperatura2, 1);
  delay(500);
  
  // Leer sensor de presión 1
  int lecturaPresionRaw1 = analogRead(PIN_PRESION_1);
  int presion1 = map(lecturaPresionRaw1, 0, 1023, 900, 1100); // Conversión a hPa
  Serial.print("P:P-101:");
  Serial.println(presion1);
  delay(500);
  
  // Leer sensor de presión 2
  int lecturaPresionRaw2 = analogRead(PIN_PRESION_2);
  int presion2 = map(lecturaPresionRaw2, 0, 1023, 950, 1050);
  Serial.print("P:P-102:");
  Serial.println(presion2);
  delay(500);
  
  // Pausa entre ciclos completos
  delay(2000);
}
