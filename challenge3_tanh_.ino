#include <Servo.h>
#include <math.h>

#define PIN_TRIG 12
#define PIN_ECHO 13
#define PIN_SERVO 4

Servo myServo;

unsigned long MOVING_TIME = 3000;
unsigned long moveStartTime;
int startAngle = 0;
int stopAngle  = 90;
int currentAngle = startAngle;
bool isCarDetected = false;
#define DIST_THRESHOLD 20.0  

// 함수 선언
float getDistance();
float smoothTanh(float x);

void setup() {
  myServo.attach(PIN_SERVO);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  myServo.write(startAngle);
  delay(500);
  moveStartTime = millis();
}

void loop() {
  float distance = getDistance();

  // 차량 접근 시
  if (distance < DIST_THRESHOLD && !isCarDetected) {
    moveStartTime = millis();
    while (millis() - moveStartTime <= MOVING_TIME) {
      float progress = (float)(millis() - moveStartTime) / MOVING_TIME;
      float rate = smoothTanh((progress - 0.5) * 5.0);
      currentAngle = startAngle + rate * (stopAngle - startAngle);
      myServo.write(currentAngle);
      delay(20);
    }
    isCarDetected = true;
  }

  // 차량 통과 후 멀어질 때
  if (distance > DIST_THRESHOLD + 10 && isCarDetected) {
    moveStartTime = millis();
    while (millis() - moveStartTime <= MOVING_TIME) {
      float progress = (float)(millis() - moveStartTime) / MOVING_TIME;
      float rate = smoothTanh((progress - 0.5) * 5.0);
      currentAngle = stopAngle - rate * (stopAngle - startAngle);
      myServo.write(currentAngle);
      delay(20);
    }
    isCarDetected = false;
  }
}

// tanh 기반 제어 함수
float smoothTanh(float x) {
  // tanh의 범위는 -1~+1 → 0~1로 스케일 변환
  return 0.5 * (tanh(x) + 1.0);
}

// 초음파 거리 측정 함수
float getDistance() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duration = pulseIn(PIN_ECHO, HIGH);
  float distance = duration * 0.0343 / 2.0; // cm 단위 변환
  return distance;
}


