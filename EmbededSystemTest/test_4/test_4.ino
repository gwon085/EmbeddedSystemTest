const int BIT_US = 104;   // 9600bps에서 1비트 시간(약 104us)

// PB0(D8)를 HIGH/LOW로 직접 제어하는 매크로
#define TX_HIGH() (PORTB |=  (1 << PB0))
#define TX_LOW()  (PORTB &= ~(1 << PB0))

// 반복 송신할 문자 배열
char sendChars[] = {'a', 'j', 's', 'R'};
int indexChar = 0;

void setup() {
  DDRB |= (1 << PB0);   // PB0(D8)를 출력으로 설정
  TX_HIGH();            // UART idle 상태는 HIGH

  Serial.begin(9600);   // 하드웨어 UART (D0, D1) 속도
}

void loop() {
  char c = sendChars[indexChar];

  sendChar(c);   // 소프트웨어 UART로 전송(D8)
  Serial.write(c); // 하드웨어 UART 전송(D0,D1)

  indexChar++;
  if (indexChar >= sizeof(sendChars)) indexChar = 0;  // 배열 반복

  delay(500);

  // UART 수신 확인
  if (Serial.available() > 0) {
    char r = Serial.read();
    Serial.print("Received: ");
    Serial.println(r);
  }
}

void sendChar(char c) {
  // 🔸 1) Start Bit (Low)
  TX_LOW();
  delayMicroseconds(BIT_US);

  // 🔸 2) Data Bits (8비트, LSB First)
  for (int i = 0; i < 8; i++) {
    if (c & 0x01) TX_HIGH();   // 현재 비트가 1이면 HIGH
    else          TX_LOW();    // 0이면 LOW

    c >>= 1;                   // 다음 비트를 위해 이동
    delayMicroseconds(BIT_US);
  }

  // 🔸 3) Stop Bit (High)
  TX_HIGH();
  delayMicroseconds(BIT_US);
}
