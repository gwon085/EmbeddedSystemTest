// ==========================================
// 소프트 UART 루프백 검증 (정확도 판정)
// 송신 : D8 (PB0, 소프트 UART)
// 수신 : D0 (하드웨어 UART RX)
// Baudrate : 9600
// 검증 : ASCII 값 비교 ('a' == 0x61)
// ==========================================

// 9600bps 기준 1비트 시간 (1 / 9600 ≒ 104us)
const int BIT_US = 104;

// PB0(D8) 직접 제어 (타이밍 정확도 확보)
#define TX_HIGH() (PORTB |=  (1 << PB0))   // HIGH 출력
#define TX_LOW()  (PORTB &= ~(1 << PB0))   // LOW 출력

void setup() {
  // D8(PB0)을 출력으로 설정
  DDRB |= (1 << PB0);

  // UART idle 상태는 HIGH
  TX_HIGH();

  // 하드웨어 UART 초기화 (D0 = RX)
  Serial.begin(9600);

  // 실험 시작 알림
  Serial.println("UART Loopback Test Start");
}

void loop() {

  // 1️⃣ 소프트 UART로 문자 'a' 전송
  sendChar('a');

  // 송신 간격 (관찰용)
  delay(500);

  // 2️⃣ RX 수신 데이터 정확성 판정
  if (Serial.available() > 0) {
    char r = Serial.read();

    // ASCII 값으로 정확히 비교
    if (r == 'a') {
      Serial.println("OK : 0x61");
    } else {
      Serial.print("ERROR : 0x");
      Serial.println((uint8_t)r, HEX);
    }
  }
}

// ===================================
// 소프트웨어 UART 송신 함수 (8N1)
// Start 1bit, Data 8bit, Stop 1bit
// ===================================
void sendChar(char c) {

  // 🔹 1) Start Bit (LOW)
  TX_LOW();
  delayMicroseconds(BIT_US);

  // 🔹 2) Data Bits (LSB First)
  for (int i = 0; i < 8; i++) {

    if (c & 0x01) {
      TX_HIGH();    // 비트 = 1
    } else {
      TX_LOW();     // 비트 = 0
    }

    c >>= 1;        // 다음 비트로 이동
    delayMicroseconds(BIT_US);
  }

  // 🔹 3) Stop Bit (HIGH)
  TX_HIGH();
  delayMicroseconds(BIT_US);
}
