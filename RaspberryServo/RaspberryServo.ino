/*
 * Control de Servo en Arduino desde Raspberry Pi
 * Ver: https://github.com/mcortex/RaspiTests/blob/master/ArduinoTests/ArduinoServo.py
 */

#include <Servo.h>

Servo servo; // Definimos un objeto servo a controlar desde la Raspi

// Variables del servo
const int servoPin = 9; // El servo conectará en el pin 9 (PWM)
const int cerrado = 90; // Cerrado: Servo a 90 grados (Posicion inicial)
const int abierto = 0; // Abierto: Servo a 0 grados

// Historico de posiciones
int posInicial = cerrado;
int posAnterior;
int posicion;

// Timestamps
const long intervalo = 15;
unsigned long int millisAnteriores = 0;

void accionoServo(int signal) {
  unsigned long millisActuales = millis(); // Momento actual
  /* Veo si el tiempo transcurrido aplica segun el intervalo tomado 
   * para que el Servo cambie de estado 
   */
  if (millisActuales - millisAnteriores >= intervalo) {
      millisAnteriores = millisActuales; // Guardo el estado actual como anterior para la proxima medicion
      servo.write(signal);
      posicion = signal;
  }
}

void setup() {

  servo.attach(servoPin); // Inicializo el servo 
  Serial.begin(9600); // Inicializo el puerto serial a 9600 baudios para comunicar con la raspi

  if (servo.read() != posInicial) {
      servo.write(posInicial); // Si la puerta estaba abierta la cierro
      posicion = posInicial;
      delay(15); // Solo usamos delay aca ya que es solo una accion bloqueante de inicializacion
  }
}

void loop() {
  // Si el puerto serie esta disponible me comunico con la raspi
  if (Serial.available()) {

    char sigRecibida = Serial.read();
    
    if (sigRecibida == 'A' && posicion == cerrado) {
      accionoServo(abierto);
    }
    else if (sigRecibida == 'C' && posicion == abierto) {
      accionoServo(cerrado);
    }
  }

}
