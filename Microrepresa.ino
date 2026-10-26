#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// --- Definición de Pines ---
// Ultrasonico 1
const int trig1 = 2;
const int echo1 = 3;
// Ultrasonico 2
const int trig2 = 6;
const int echo2 = 7;

// Servomotores de las compuertas
Servo servo1;
Servo servo2;
const int pinServo1 = 9;
const int pinServo2 = 8;

// Sensor de Voltaje (Módulo FZ0430)
const int pinVoltaje = A0;

// LEDs
const int ledRojo = 12;
const int ledBlanco = 13;

// Bluetooth HC-05 usando SoftwareSerial
// RX del Arduino en pin 10 (conectar al TX del HC-05)
// TX del Arduino en pin 11 (conectar al RX del HC-05)
SoftwareSerial BT(10, 11); 

// LCD I2C (La dirección suele ser 0x27 o 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Variables Globales ---
String estadoCompuertas = "CERRADAS";
unsigned long tiempoAnterior = 0;
const long intervaloEnvio = 1000; // Enviar datos por Bluetooth cada 1 segundo (1000ms)

// Variables para calibrar el % de los contenedores de basura (en cm)
// Ajusta estos valores según la profundidad real de tus contenedores
const int distanciaVacio = 30; // Distancia en cm cuando está 0%
const int distanciaLleno = 5;  // Distancia en cm cuando está 100%

void setup() {
  Serial.begin(9600);
  BT.begin(9600); // Velocidad por defecto del HC-05

  // Configurar sensores ultrasónicos
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);

  // Configurar LEDs
  pinMode(ledRojo, OUTPUT);
  pinMode(ledBlanco, OUTPUT);

  // El LED blanco siempre debe estar encendido
  digitalWrite(ledBlanco, HIGH);

  // Inicializar Servos (Posición inicial cerrada = 0 grados)
  servo1.attach(pinServo1);
  servo2.attach(pinServo2);
  cerrarCompuertas();

  // Inicializar Pantalla LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");
  delay(1500);
  lcd.clear();
}

void loop() {
  // 1. Leer comandos desde MIT App Inventor
  if (BT.available()) {
    char comando = BT.read();
    if (comando == '1') {
      abrirCompuertas();
    } else if (comando == '0') {
      cerrarCompuertas();
    }
  }

  // 2. Medir distancias de la basura y calcular porcentajes
  int dist1 = medirDistancia(trig1, echo1);
  int dist2 = medirDistancia(trig2, echo2);

  // Convertir la distancia a un porcentaje (0% a 100%)
  int porc1 = map(dist1, distanciaVacio, distanciaLleno, 0, 100);
  int porc2 = map(dist2, distanciaVacio, distanciaLleno, 0, 100);

  // Asegurar que el porcentaje no baje de 0 ni pase de 100
  porc1 = constrain(porc1, 0, 100);
  porc2 = constrain(porc2, 0, 100);

  // 3. Control del LED Rojo (Alerta si cualquier contenedor >= 80%)
  if (porc1 >= 80 || porc2 >= 80) {
    digitalWrite(ledRojo, HIGH);
  } else {
    digitalWrite(ledRojo, LOW);
  }

  // 4. Leer y calcular el sensor de voltaje
  // El FZ0430 divide el voltaje entre 5. La lectura máxima del Arduino es 5V.
  int lecturaAnalogica = analogRead(pinVoltaje);
  float voltajePin = (lecturaAnalogica * 5.0) / 1023.0;
  float voltajeReal = voltajePin * 5.0; // Multiplicamos por 5 para el voltaje real generado

  // 5. Actualizar la LCD y enviar datos a la App cada segundo
  unsigned long tiempoActual = millis();
  if (tiempoActual - tiempoAnterior >= intervaloEnvio) {
    tiempoAnterior = tiempoActual;

    // --- Mostrar en la LCD ---
    lcd.setCursor(0, 0);
    lcd.print("C1:"); lcd.print(porc1); lcd.print("% C2:"); lcd.print(porc2); lcd.print("%   "); // Espacios extra para limpiar
    lcd.setCursor(0, 1);
    lcd.print(voltajeReal, 1); lcd.print("V "); 
    lcd.print(estadoCompuertas == "ABIERTAS" ? "ABIERTA " : "CERRADA ");

    // --- Enviar datos al Bluetooth (Formato esperado por tu App) ---
    // Estructura: "Porcentaje1% Porcentaje2% VoltajeV 0cm Estado"
    String datos = String(porc1) + " " + 
                   String(porc2) + " " + 
                   String(voltajeReal, 1) + "V " + 
                   "0cm " +  // Dato fijo para la etiqueta del Nivel del Río
                   (estadoCompuertas == "ABIERTAS" ? "ABIERTAS" : "CERRADAS");
                   
    BT.print(datos); // Se envía la cadena separada por espacios
  }
}

// --- Funciones Auxiliares ---

// Función para obtener la distancia de los ultrasónicos en centímetros
int medirDistancia(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  long duracion = pulseIn(echo, HIGH, 30000); // 30ms de límite para evitar bloqueos
  if(duracion == 0) return 999; // Retorna un número alto si no hay eco
  
  int distancia = duracion * 0.034 / 2;
  return distancia;
}

void abrirCompuertas() {
  servo1.write(90); // Ajusta este ángulo (ej. 90 o 180) según cuánto necesiten abrirse tus compuertas
  servo2.write(90);
  estadoCompuertas = "ABIERTAS";
}

void cerrarCompuertas() {
  servo1.write(0);  // Ángulo de fábrica para compuertas cerradas
  servo2.write(0);
  estadoCompuertas = "CERRADAS";
}