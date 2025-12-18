// ===============================
// 비트뱅 UART 송신 + RX 검증
// 송신 : D8 (PB0, Bit-bang)
// 수신 : D0 (Hardware UART)
// Baudrate : 9600
// ===============================

const int BIT_US = 104;   // 9600bps 기준 1비트 시간

#define TX_HIGH() (PORTB |=  (1 << PB0))   // D8 HIGH
#define TX_LOW()  (PORTB &= ~(1 << PB0))   // D8 LOW

// 테스트 문자 배열
char sendChars[] = { 'a', 'j', 's', 'R' };
int indexChar = 0;

void setup() {
  // D8 출력 설정
  DDRB |= (1 << PB0);

  // UART idle = HIGH
  TX_HIGH();

  // RX 확인용 하드웨어 UART
  Serial.begin(9600);
}

void loop() {
  char c = sendChars[indexChar];

  // 🔹 1) 비트뱅으로만 송신 (D8)
  sendChar(c);

  // 다음 문자로 이동
  indexChar++;
  if (indexChar >= sizeof(sendChars)) {
    indexChar = 0;
  }

  delay(500);   // 송신 간격 (관찰용)

  // 🔹 2) RX로 실제 수신된 데이터만 출력
  if (Serial.available() > 0) {
    char r = Serial.read();
    Serial.print("Received: ");
    Serial.println(r);
  }
}

// ===================================
// 비트뱅 UART 송신 함수 (8N1)
// ===================================
void sendChar(char c) {

  // Start Bit (LOW)
  TX_LOW();
  delayMicroseconds(BIT_US);

  // Data Bits (LSB First)
  for (int i = 0; i < 8; i++) {
    if (c & 0x01) {
      TX_HIGH();
    } else {
      TX_LOW();
    }

    c >>= 1;
    delayMicroseconds(BIT_US);
  }

  // Stop Bit (HIGH)
  TX_HIGH();
  delayMicroseconds(BIT_US);
}
