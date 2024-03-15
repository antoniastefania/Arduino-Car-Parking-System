#include <Wire.h>
#include <Servo.h>

#define IR_SENSOR_1 3
#define IR_SENSOR_2 5
#define SERVO_PIN 4

Servo myservo;

int availableSlots = 4;  // Numărul total de locuri disponibile

int flag1 = 0; // Variabila pentru starea senzorului IR1
int flag2 = 0; // Variabila pentru starea senzorului IR2

void setup() {
  Serial.begin(9600); // Inițializează comunicarea serială la 9600 bps
  Serial.println(); // Imprimă o linie goală în consolă
  Serial.println("BINE ATI VENIT!"); // Imprimă un mesaj de bun venit în consolă
  Serial.print("LOCURI DISPONIBILE IN PARCARE :  "); // Imprimă mesajul "LOCURI DISPONIBILE IN PARCARE :  " în consolă
  Serial.println(availableSlots); // Imprimă numărul de locuri disponibile în consolă
  
  pinMode(IR_SENSOR_1, INPUT); // Configurează pinul IR_SENSOR_1 ca intrare
  pinMode(IR_SENSOR_2, INPUT); // Configurează pinul IR_SENSOR_2 ca intrare
  
  myservo.attach(SERVO_PIN); // Atașează obiectul Servo la pinul SERVO_PIN
  myservo.write(0);  // Setează inițial microservo la 0 grade
}

void loop() { 

  // Verifică starea senzorului IR1
  if(digitalRead(IR_SENSOR_1) == LOW && flag1 == 0) { // Verifică dacă senzorul IR1 este activat și dacă flag1 este 0
    if(availableSlots > 0) { // Verifică dacă mai sunt locuri disponibile în parcarea
      flag1 = 1; // Setează flag1 la 1 pentru a semnala că senzorul IR1 a fost activat
      if(flag2 == 0) {
        myservo.write(110);// Deschide bariera
        availableSlots = availableSlots - 1; // Scade numărul de locuri disponibile
        Serial.print("LOCURI DISPONIBILE IN PARCARE :  ");
        Serial.println(availableSlots);

      }
    } else {
      Serial.println("    IMI PARE RAU :(   PARCAREA ESTE PLINA ");  // Afiseaza un mesaj daca parcarea este plina
      delay(3000); // Așteaptă 3000 de milisecunde (3 secunde)
    }
  }

  // Verifică starea senzorului IR2
  if(digitalRead(IR_SENSOR_2) == LOW && flag2 == 0) { // Verifică dacă senzorul IR2 este activat și dacă flag2 este 0
    flag2 = 1; // Setează flag2 la 1 pentru a semnala că senzorul IR2 a fost activat
    if(flag1 == 0) {
      myservo.write(110); // Deschide bariera
      availableSlots = availableSlots + 1; // Crește numărul de locuri disponibile
      Serial.print("LOCURI DISPONIBILE IN PARCARE :  ");
      Serial.println(availableSlots);
    }
  }

  // Dacă ambii senzori au fost activați, așteaptă o secundă și închide bariera
  if(flag1 == 1 && flag2 == 1) {
    delay(1000); // Așteaptă 1000 de milisecunde (1 secundă)
    // Microservo la poziția inițială
    myservo.write(0); // Închide bariera
    flag1 = 0, flag2 = 0; // Resetează flagurile la 0 pentru a pregăti următoarea iterație
  }
}
