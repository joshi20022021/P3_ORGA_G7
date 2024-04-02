#include <Servo.h>

const int s0 = 2;
const int s1 = 3;
const int s2 = 4;
const int s3 = 5;
const int out = 6;

const int servopin1 = 7;
const int servopin2 = 8; 

const int trigPin = 9;
const int echoPin = 10;

const int clockPulsePin = 11;
const int leftPulsePin = 12;

Servo servo1;
Servo servo2;
int angle1 = 90;
int angle2 = 105;

// Pseudo multitasking in arduino
// 0.1 sec frequency
unsigned long intervalUS=5000;    // the time we need to wait
unsigned long previousMillisUS=0; // millis() returns an unsigned long.

// 0.01 frequency
unsigned long intervalColor = 10;
unsigned long previousMillisColor = 0;
unsigned long previousMillisCooldownColor = 0;
unsigned long colorCooldown = 2000;

// 1 sec. frequency
unsigned long intervalCP=1000;    // the time we need to wait
unsigned long previousMillisCP=0; // millis() returns an unsigned long.

const int RED = 0;
const int GREEN = 1;
const int BLUE = 3;

int lastColor = -1;



long duration, distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(clockPulsePin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(leftPulsePin, OUTPUT);
  Serial.begin(9600);

  servo1.attach(servopin1);
  servo2.attach(servopin2);

  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);

  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);
}

void loop() {
  // Clock pulse
  if ((unsigned long)(millis() - previousMillisCP) >= intervalCP){
    sendClockPulse(clockPulsePin);
    Serial.println("SEND CLOCK PULSE");
  }

  if (((unsigned long)(millis() - previousMillisColor) >= intervalColor) && ((unsigned long)(millis() - previousMillisCooldownColor)) >= colorCooldown){
    int color = processColor();

    // move discardter servo
    if ((color == RED || color == GREEN) && angle1 > 0){
      for(angle1 = 90; angle1 > 0; angle1--) {
        servo1.write(angle1);
        delay(5);
      }
      angle1 = 0;
      previousMillisCooldownColor = millis();
    } else if (angle1 == 0){
      for(angle1 = 0; angle1 < 90; angle1++){
        servo1.write(angle1);
        delay(5);
      }
      angle1 = 90;
  
    }

    if (color == RED && color != lastColor && angle2 > 0){
      lastColor = RED;
      for (angle2 = 105; angle2 > 70; angle2--) {
        servo2.write(angle2);
        delay(5);           
      }
      Serial.println("YO SOY ROJO A 0");
      Serial.println(angle2);
      angle2 = 70;
    } else if (color == GREEN && color != lastColor && angle2 < 90){
      lastColor = GREEN;
      Serial.println("HOLA SOY VERDE");
      for (angle2 = 70; angle2 < 105; angle2++){
        servo2.write(angle2);
        delay(5);
      }
      Serial.println("YO SOY VERDE A 90");
      Serial.println(angle2);
      angle2 = 105;
    }
  }

  // ULTRASONIC SENSOR
  if (((unsigned long)(millis() - previousMillisUS) >= intervalUS)) {
    processUltraSonic();
  }
}

int processColor(){
  previousMillisColor = millis();


  int R = getRed();
  delay(100);
  int G = getGreen();
  delay(100);
  int B = getBlue();
  delay(100);

  Serial.print("Int R "+ String(R));
  Serial.print("  --  Int G "+ String(G));
  Serial.print("  --  Int B "+ String(B));

  if ( R < G && R < B && G > 25 && R < 15 ) {
    Serial.println("  EL COLOR ES ROJO");
    return RED;
  }
  else if (R > G && B > G && G <= 20 && B < 23 && G > 15) {
    Serial.println("  EL COLOR ES VERDE");
    return GREEN;
  }
  else if (R > G  && G > B && R >= 31 && B <= 22) {
    Serial.println("  EL COLOR ES AZUL");
    return BLUE;
  }
  else
  {
    Serial.println("  EL COLOR NO HA SIDO REGISTRADO");
    return -1;
  }
}

void processUltraSonic(){
  previousMillisUS = millis();
  // clean low pulse to the sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // high pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
    
  if (distance < 10){
    previousMillisCP = millis();
    digitalWrite(leftPulsePin, HIGH);
    delay(10);
    digitalWrite(leftPulsePin, LOW);
    
  }
  
  Serial.print("Distance: ");
  Serial.print(distance);  
  Serial.println(" ");
}

void sendClockPulse(int clockPulse){
  previousMillisCP = millis();
  digitalWrite(clockPulse, HIGH);
  delay(10);
  digitalWrite(clockPulse, LOW);
}


int getRed(){
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  return pulseIn(out, LOW);;
}

int getBlue(){
  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  return pulseIn(out, LOW);;
}

int getGreen(){
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  return pulseIn(out, LOW);
}
  
