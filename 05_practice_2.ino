#define LED_PIN 7

void setup() {
  pinMode(LED_PIN, OUTPUT); // 7번 핀을 출력으로 설정
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  // 1. loop() 시작 → 처음 1초 동안 LED 켜기
  digitalWrite(LED_PIN, LOW);
  delay(1000);

  // 2. 다음 1초 동안 LED를 5회 깜빡이기
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);   // 꺼짐 0.1초
    digitalWrite(LED_PIN, LOW);
    delay(100);   // 켜짐 0.1초
  }

  // 3. LED 끄기
  digitalWrite(LED_PIN, HIGH);

  // 4. 무한 루프 상태로 종료
  while (1) {
    // 아무 동작 없음 (프로그램 정지 상태 유지)
  }
}
