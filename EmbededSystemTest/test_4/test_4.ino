// =====================================
// 소프트 UART 루프백 검증 (단일 문자)
// TX : D8 (PB0)
// RX : D0 (Hardware UART)
// Baudrate : 9600
// =====================================

// 9600bps 기준 1비트 시간 (약 104us)
const int BIT_US = 104;

// PB0(D8)를 직접 제어하는 매크로
#define TX_HIGH() (PORTB |=  (1 << PB0))   // HIGH 출력
#define TX_LOW()  (PORTB &= ~(1 << PB0))   // LOW 출력

void setup() {
  // D8(PB0) 출력 설정
  DDRB |= (1 << PB0);

  // UART idle 상태 = HIGH
  TX_HIGH();

  // 하드웨어 UART 초기화
  Serial.begin(9600);
}

void loop() {

  // 1️⃣ 소프트 UART로 문자 'a' 전송
  sendChar('a');

  // 송신 간격 (관찰용)
  delay(500);

  // 2️⃣ RX로 실제 수신된 데이터만 출력
  if (Serial.available() > 0) {
    char r = Serial.read();
    Serial.print("Received: ");
    Serial.println(r);
  }
}

// ===================================
// 소프트웨어 UART 송신 함수 (8N1)
// ===================================
void sendChar(char c) {

  // 🔹 Start Bit (LOW)
  TX_LOW();
  delayMicroseconds(BIT_US);

  // 🔹 Data Bits (LSB First, 8bit)
  for (int i = 0; i < 8; i++) {
    if (c & 0x01) {
      TX_HIGH();    // 비트가 1
    } else {
      TX_LOW();     // 비트가 0
    }

    c >>= 1;        // 다음 비트
    delayMicroseconds(BIT_US);
  }

  // 🔹 Stop Bit (HIGH)
  TX_HIGH();
  delayMicroseconds(BIT_US);
}
