/*
   Verificacion de presion en un servo
 */
#include <Servo.h>
// SERVO:
Servo servo; // Definimos un objeto servo a controlar desde la Raspi

// Variables del servo
const int servoPin = 9; // El servo conectará en el pin 9 (PWM)
const int cerrado = 90; // Cerrado: Servo a 90 grados (Posicion inicial)
const int abierto = 180; // Abierto: Servo a 0 grados

// Historico de posiciones
int posInicial = cerrado;
int posAnterior;
int posicion;

// Historico de forzados
unsigned int forzado = 0;
unsigned int estacionario = 1;
unsigned int estadoAnterior = estacionario;

// VOLTIMETRO:
float vArduino = 3.3; // Cuando esta alimentado por USB la salida de 5V solo llega a 3.3V
float vFuente = 3.3;

// Tensiones del servo:
// Pata PWM:
//float vEst = 3.1;
float vEst;
float vForce = 2.9999; // 2.5975

// Timestamps
const long intervaloServo = 20;
const long intervaloVolt = 200;
unsigned long int millisAnteriores = 0;
unsigned long int millisAnterioresVolt = 0;

void accionarServo(char signal) {
  unsigned long millisActuales = millis(); // Momento actual
  /* Veo si el tiempo transcurrido aplica segun el intervalo tomado
   para que el Servo cambie de estado
   */
  if (millisActuales - millisAnteriores >= intervaloServo) {
    millisAnteriores = millisActuales; // Guardo el estado actual como anterior para la proxima medicion

    if (posicion == cerrado && signal == 'A') {
      servo.write(abierto);
      posicion = abierto;
      forzado = 0;
      Serial.println("ABIERTO");
    }
    else if (posicion == abierto && signal == 'A') {
      servo.write(cerrado);
      posicion = cerrado;
      forzado = 0;
      Serial.println("CERRADO");
    }
  }
}

void verificarForzado() {


  unsigned long millisActuales = millis(); // Momento actual
  /* Veo si el tiempo transcurrido aplica segun el intervalo tomado
   para que el Servo cambie de estado
   */
  if (millisActuales - millisAnterioresVolt >= intervaloVolt) {
    millisAnterioresVolt = millisActuales;

    int valorMedido = analogRead(A0);

    // Convierto la lectura (de 0 a 1023) a voltaje (0 a 5V):
    float voltajeMedido = valorMedido * (vEst / 1023);

    // Imprimo el valor medido:
    // Serial.print("Voltaje medido: ");
    // Serial.println(voltajeMedido,4);

    //if (voltajeMedido < vEst &&  voltajeMedido <= vForce && forzado == 0 && posicion != abierto ) {
    if (voltajeMedido < vEst &&  voltajeMedido <= vForce ) {
      forzado = 1;
      estadoAnterior = estacionario;
      Serial.println("FORZADO");
    }
    if (voltajeMedido > vForce) {
      forzado = 0;
      estadoAnterior = forzado;
      Serial.println("ESTACIONARIO");
    }
  }
}



void setup() {

  servo.attach(servoPin); // Inicializo el servo

  Serial.begin(9600);

  if (servo.read() != posInicial) {
    servo.write(posInicial); // Si la puerta estaba abierta la cierro
    posicion = posInicial;

    delay(intervaloServo); // Solo usamos delay aca ya que es solo una accion bloqueante de inicializacion
  }
  Serial.print("Inicializando servo, posicion incial CERRADO, angulo: ");
  Serial.println(posicion);

  // Medicion inicial de la tension estacionaria
  vEst = analogRead(A0) * (vFuente / 1023);
  //Serial.println(posicion);

}

void loop() {

  if (Serial.available()) {

    char sigRecibida = Serial.read();
    accionarServo(sigRecibida);
  }

  verificarForzado();

}


