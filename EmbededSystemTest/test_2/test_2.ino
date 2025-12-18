#define BIT_DELAY 104   // 9600 bps → 1/9600 ≈ 104us

#define TX_HIGH() (PORTB |=  (1 << PB0)) // PB0 HIGH
#define TX_LOW()  (PORTB &= ~(1 << PB0)) // PB0 LOW

void setup() {
  // PB0를 출력으로 설정
  DDRB |= (1 << PB0);

  // 하드웨어 시리얼 시작
  Serial.begin(9600);
}

void loop() {

  // PB0 핀으로 Bit-bang 'a' 전송
  sendBitbang('a');

  delay(100); // 0.1초 간격
}

// Bit-bang 전송 함수
void sendBitbang(char c) {
  // 스타트 비트
  TX_LOW();
  delayMicroseconds(BIT_DELAY);

  // 데이터 8비트 (LSB 먼저)
  for (int i = 0; i < 8; i++) {
    if (c & (1 << i)) TX_HIGH();
    else TX_LOW();
    delayMicroseconds(BIT_DELAY);
  }

  // 스톱 비트
  TX_HIGH();
  delayMicroseconds(BIT_DELAY);
}