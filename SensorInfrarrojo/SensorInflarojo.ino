const int LED = 13;
const int SENSOR = 2;
int valor;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(SENSOR, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  valor =  digitalRead(SENSOR); // ME DA LOW (0) CUANDO DETECTE OBSTACULO,HIGHT CUANDO NO
  digitalWrite(LED, !valor); //ENVIO AL LED LO CONTRARIO

    delay(500);
}
