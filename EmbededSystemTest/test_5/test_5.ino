//5번
const int BIT_US = 104; // 9600bps
#define TX_HIGH() (PORTB |=  (1 << PB0))
#define TX_LOW()  (PORTB &= ~(1 << PB0))

char sendChars[] = {'a', 'j', 's', 'R'}; // 반복할 문자들
int indexChar = 0;

void setup() {
  DDRB |= (1 << PB0);   // D8을 출력으로 설정
  TX_HIGH();            // idle 상태 HIGH
  Serial.begin(9600);   // 시리얼 모니터용 (D0, D1 하드웨어 UART)
}

void loop() {
  char c = sendChars[indexChar];

  // D8에서 Bit-bang 송신
  sendChar(c);

  // 하드웨어 시리얼 송신 (D0,D1)
  Serial.write(c);

  // 다음 문자로 이동
  indexChar++;
  if (indexChar >= sizeof(sendChars)) indexChar = 0; // 배열 반복

  delay(500);

  // UART 수신 확인
  if (Serial.available() > 0) {
    char r = Serial.read();
    Serial.print("Received: ");
    Serial.println(r);
  }
}

void sendChar(char c) {
  // 스타트 비트 (Low)
  TX_LOW();
  delayMicroseconds(BIT_US);

  // 데이터 비트 (LSB first)
  for (int i = 0; i < 8; i++) {
    if (c & 0x01) TX_HIGH();
    else TX_LOW();
    c >>= 1;
    delayMicroseconds(BIT_US);
  }

  // 스톱 비트 (High)
  TX_HIGH();
  delayMicroseconds(BIT_US);
}
