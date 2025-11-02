#include <Servo.h>

#define PIN_SERVO 5
#define INTERVAL 20   // ms

// 1단계 (빠른 속도)
#define SPEED_FAST 3.0
#define START_FAST 0
#define END_FAST 180

// 2단계 (느린 속도)
#define SPEED_SLOW 0.3
#define START_SLOW 0
#define END_SLOW 90

Servo myservo;

float angle_curr = 0;
unsigned long last_time;
int phase = 1; // 1: 첫 번째 실험, 2: 두 번째 실험

void setup() {
  Serial.begin(57600);
  myservo.attach(PIN_SERVO);

  angle_curr = START_FAST;
  myservo.write(angle_curr);
  last_time = millis();
}

void loop() {
  unsigned long now = millis();
  float dt = (now - last_time) / 1000.0;
  if (dt < INTERVAL / 1000.0) return;
  last_time = now;

  float speed, start, end;

  if (phase == 1) {
    speed = SPEED_FAST;
    start = START_FAST;
    end = END_FAST;
  } else if (phase == 2) {
    speed = SPEED_SLOW;
    start = START_SLOW;
    end = END_SLOW;
  } else {
    // 모든 실험 종료
    return;
  }

  // 등속 이동
  if (angle_curr < end) {
    angle_curr += speed * dt;
    if (angle_curr > end) angle_curr = end;
    myservo.write((int)angle_curr);
    Serial.println(angle_curr);
  } else {
    // 한 번만 이동했으므로 다음 단계로 넘어감
    if (phase == 1) {
      phase = 2;
      angle_curr = START_SLOW;
      myservo.write(angle_curr);
      delay(2000); // 다음 실험 전 대기
    } else {
      while (true); // loop 종료
    }
  }
}
