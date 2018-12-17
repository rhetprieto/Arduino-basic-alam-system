#include <Keypad.h>
#include <Servo.h>
#define PINA A0
#define PINB A1
#define PINC A2
#define PINSENSOR 3
const byte rowsCount = 4;
const byte columsCount = 4;

Servo servoMotor;
int pos = 0;

#define DESARMADO 0
#define ARMADO   1
#define ENALERTA 2

String password="1529"; // Password del sistema
String inputPassword="";	//Variable para almacenar las entradas

byte val = 0;

byte estado = 0; // 0 desarmado, 1 sistema está armado, 2 el sistema ha detectado movimiento.
char keys[rowsCount][columsCount] = {
   { '1','2','3', 'A' },
   { '4','5','6', 'B' },
   { '7','8','9', 'C' },
   { '*','0','#', 'D' }
};

byte rowPins[rowsCount]      = { 11, 10, 9, 8 };
byte columnPins[columsCount] = { 7, 6, 5, 4 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rowsCount, columsCount);

void setup() {
   Serial.begin(9600);
   pinMode(PINA, OUTPUT);
   pinMode(PINB, OUTPUT);
   pinMode(PINC, OUTPUT);
   pinMode(PINSENSOR, INPUT);
   servoMotor.attach(12);
}

void loop() {
   leerTeclado();
   if (estado == ARMADO )    //Si el estado es 0, no detectamos movimiento.
     detectarMovimiento();
    //Serial.println(inputPassword);
}

void leerTeclado(){
  char key = keypad.getKey();
  if (key) {
     inputPassword += key;
  }

  if ( inputPassword.length() == 4) {
     if (password == inputPassword) {
       if (!estado ){
         estado = ARMADO; // El sistema ha sido armado
         Serial.println("El sistema ha sido armado");
       }else if (estado == ENALERTA) {
         estado = DESARMADO; // El sistema estaba en alerta y ahora ha sido desarmado.
         apagarLeds();
         desbloquearPuertas();
         Serial.println("El sistema ha sido desarmado!");
       }
       }
     else {
      Serial.println("Clave Incorrecta!");
    }
    inputPassword = "";
  }

}

void detectarMovimiento(){
  val = digitalRead(PINSENSOR);
  if (val == HIGH) {
    encenderLeds();
    bloquearPuertas();
    estado = ENALERTA;
  }
}

void encenderLeds(){
   digitalWrite(PINA, HIGH);
   digitalWrite(PINB, HIGH);
   digitalWrite(PINC, HIGH);
}

void apagarLeds(){
   digitalWrite(PINA, LOW);
   digitalWrite(PINB, LOW);
   digitalWrite(PINC, LOW);
}

void desbloquearPuertas(){
  for (pos = 0; pos <= 180; pos += 1) {
    servoMotor.write(pos);
    delay(15);
  }
  Serial.println("Las puertas han sido desbloqueadas.");
}

void bloquearPuertas(){
  for (pos = 180; pos >= 0; pos -= 1) {
    servoMotor.write(pos);
    delay(15);
  }
  Serial.println("Las puertas han sido bloqueadas.");
}
