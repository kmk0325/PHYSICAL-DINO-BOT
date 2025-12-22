#include <Arduino.h>

// Timer1 설정값
// 주파수 50Hz (20ms) -> 39999
const unsigned int PWM_TOP = 39999; 

// 각도 값 (0.5us 단위)
// 0도 (1.0ms) = 2000, 180도 (2.0ms) = 4000
const unsigned int SERVO_MIN = 2000; // 스페이스바 뗌. 0도
const unsigned int SERVO_MAX = 2222; // 스페이스바 누름. 20도(실험필요)

// 조도센서
float backgroundValue = 512; // 중간값으로 시작
const int SENSITIVITY = 35; // 배경과 50 이상 차이나면 장애물로 간주 (실험필요)
bool isCalibrated = false;

// 함수 선언
void jump();
int readADC(int channel);

void setup() {
    Serial.begin(9600);

    // 서보모터 연결 9번 핀(PB1)
    DDRB |= (1 << PB1);
    
    // Timer1 초기화 (TCCR1A, TCCR1B)
    TCCR1A = 0;
    TCCR1B = 0;

    // Fast PWM Mode 14 설정 (WGM 비트)
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12);
    // 비반전 모드 설정 (COM1A 비트)
    TCCR1A |= (1 << COM1A1);
  
    // 주파수(TOP) 설정 (ICR1 레지스터). 50Hz(20ms)
    ICR1 = 39999;
    // 초기 각도 설정 (OCR1A 레지스터)
    OCR1A = SERVO_MIN;

    // ADC 설정
    ADMUX = 0; ADCSRA = 0; // 초기화
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS0); // ADC Enable, 분주비 32

    // 타이머 시작
    // 분주비 8
    TCCR1B |= (1 << CS11);
}

void loop() {
    // 조도센서 현재 값 읽기. PC0(ADC0, A0)
    int currentValue = readADC(PC0);

    // 처음 100개의 데이터의 평균을 배경값으로 설정
    if(!isCalibrated) {
        static int count = 0;
        static long sum = 0;
        sum += currentValue;
        count++;
        if(count >= 100) {
            backgroundValue = sum / 100.0;
            isCalibrated = true;
            Serial.print("Background: ");
            Serial.println(backgroundValue);
        }
        return; // 초기 배경값을 설정하기 전에는 여기까지 실행
    }

    // 현재 값과 배경 값의 차이 
    int diff = abs(currentValue - (int)backgroundValue);

    if(diff > 130){
        // 다크모드일 때
        backgroundValue = currentValue;
        Serial.println("Mode Switched");
    }
    else if(diff > SENSITIVITY) {
        jump();
        // 점프 중에는 배경 업데이트를 멈추기 위해 delay를 주거나 로직을 건너 뛴다
        delay(300);
    }
    else {
        // 배경 값 자동 추적하기
        // 다크모드 반전시에도 작동하도록 하기
        backgroundValue = (backgroundValue * 0.95) + (currentValue * 0.05);
    }
    // 디버깅용
    Serial.print("CV: "); Serial.print(currentValue);
    Serial.print(" | BG: "); Serial.print(backgroundValue);
    Serial.print(" | Diff: "); Serial.println(diff);
    // delay(10); // print 안쓸 때는 delay 활성화하기
}

// 점프 함수
void jump() {
    // 누르기
    OCR1A = SERVO_MAX;
    // 누르고 있는 시간
    delay(100); 
    // 떼기
    OCR1A = SERVO_MIN;
}
// ADC로 아날로그 값 읽기 함수
int readADC(int channel) {
    // 채널선택: 기준전압(REFS0=1, AVCC) + 채널(A0 = 0)
    ADMUX = (1 << REFS0) | (channel & 0x07);

    // 변환 시작
    ADCSRA |= (1 << ADSC);

    // 값을 받을 때까지 기다리기
    while(ADCSRA & (1 << ADSC));

    return ADC;
}