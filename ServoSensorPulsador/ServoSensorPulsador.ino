/*
   Verificacion de presion en un servo
*/
#include <Servo.h>
// SERVO:
Servo servo; // Definimos un objeto servo a controlar desde la Raspi

// Variables del servo
const int servoPin = 9; // El servo conectará en el pin 9 (PWM)
const int cerrado = 90; // Cerrado: Servo a 90 grados (Posicion inicial)
const int abierto = 0; // Abierto: Servo a 0 grados

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
float r1 = 100000; // valor en ohms de la primer resistencia
float r2 = 10000; // valor en ohms de la segunda resistencia

// Tensiones del servo:
// Pata PWM:
float vEst = 1.6613;
float vForce = 1.7;

// Timestamps
const long intervaloServo = 20;
const long intervaloVolt = 20;
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
      Serial.println("ABIERTO");
    }
    else if (posicion == abierto && signal == 'A') {
      servo.write(cerrado);
      posicion = cerrado;
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
   float voltajeMedido = valorMedido * (vArduino / 1023);
//    Serial.print("Voltaje medido: ");
//    // Imprimo el valor medido:
//    Serial.println(voltajeMedido,4);

    if (voltajeMedido > vEst &&  voltajeMedido <= vForce && forzado == 0 ) {
//    if (voltajeMedido > vEst &&  voltajeMedido <= vForce) {
      forzado = 1;
      estadoAnterior = estacionario;
      Serial.println("FORZADO");
    }
    else 
      if (voltajeMedido <= vEst &&  voltajeMedido < vForce && forzado == 1) {
      forzado = 0;
      estadoAnterior = forzado;
      Serial.println("ESTACIONARIO");
    }

    //    if (vInput == vEst && vForce != 0) {
    //      vForce = 0;
    //      Serial.println("ESTACIONARIO (cerrado)");
    //    }
    //    if (vInput > vEst && vForce == 0) {
    //      vForce = 1;
    //      Serial.println("FORZADO");
    //    }
    //    if (vInput > vEst && vForce == 0) {
    //      vForce = 1;
    //      Serial.println("ABIERTO");
    //    }
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

}

void loop() {

  if (Serial.available()) {

    char sigRecibida = Serial.read();
    accionarServo(sigRecibida);
  }

  //accionoServo(posInicial);
  verificarForzado();

}
