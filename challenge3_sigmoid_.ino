#include <Servo.h>

// 핀 설정
#define PIN_TRIG 12
#define PIN_ECHO 13
#define PIN_SERVO 4

// 객체 생성
Servo myServo;

unsigned long MOVING_TIME = 3000; // 3초 동안 이동
unsigned long moveStartTime;
int startAngle = 180;   // 닫힘
int stopAngle  = 90;  // 열림
int currentAngle = startAngle;

bool isCarDetected = false;
#define DIST_THRESHOLD 20.0  // 차량 감지 거리 (cm)

// 함수 선언
float sigmoid(float x);
float getDistance();

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

  // 차량 감지
  if (distance < DIST_THRESHOLD && !isCarDetected) {
    moveStartTime = millis();
    while (millis() - moveStartTime <= MOVING_TIME) {
      float progress = (float)(millis() - moveStartTime) / MOVING_TIME;
      float rate = sigmoid((progress - 0.5) * 10.0); // sigmoid 곡선 적용
      currentAngle = startAngle + rate * (stopAngle - startAngle);
      myServo.write(currentAngle);
      delay(20);
    }
    isCarDetected = true;
  }

  // 차량이 멀어짐
  if (distance > DIST_THRESHOLD + 10 && isCarDetected) {
    moveStartTime = millis();
    while (millis() - moveStartTime <= MOVING_TIME) {
      float progress = (float)(millis() - moveStartTime) / MOVING_TIME;
      float rate = sigmoid((progress - 0.5) * 10.0);
      currentAngle = stopAngle - rate * (stopAngle - startAngle);
      myServo.write(currentAngle);
      delay(20);
    }
    isCarDetected = false;
  }
}

// Sigmoid 함수 정의
float sigmoid(float x) {
  return 1.0 / (1.0 + exp(-x));
}

// 초음파 거리 측정 함수
float getDistance() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duration = pulseIn(PIN_ECHO, HIGH);
  float distance = duration * 0.0343 / 2.0;  // cm 단위 변환
  return distance;
}

