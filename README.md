# 🦖 PHYSICAL DINO BOT

![PlatformIO](https://img.shields.io/badge/PlatformIO-Core-orange?logo=platformio)
![Arduino](https://img.shields.io/badge/Arduino-Uno-00979D?logo=arduino)
![Language](https://img.shields.io/badge/Language-C%2B%2B-blue?logo=c%2B%2B)

**Chrome DINO Game**을 물리적으로 플레이하는 오토봇입니다. 조도 센서가 모니터의 장애물을 감지하면 서보 모터가 **Spacebar**를 눌러서 점프합니다.

## 🎥 Demo
![Demo Preview](https://github.com/user-attachments/assets/1b14d24f-287e-4c30-9b63-d175223deedf)

## ⚡ Key Features
* **Register Level Control:** 라이브러리 없이 `Timer1 Fast PWM(Mode 14)`와 `ADC 레지스터`를 직접 제어하여 최적화
* **Auto-Calibration:** 부팅 시 초기 배경 밝기 값을 자동으로 학습
* **Adaptive Logic:** 조명 변화에 따라 기준값을 자동 보정하며, 다크 모드 전환 감지 지원

## 🛠 Hardware Pinout

| Component | Pin (Arduino) | Description |
|:---:|:---:|:---|
| **Servo Motor** | **D9 (PB1)** | Timer1 PWM (OC1A) |
| **Light Sensor** | **A0 (PC0)** | ADC Input |

## ⚙️ Configuration (`src/main.cpp`)
환경에 맞춰 서보 각도와 센서 감도를 조정하세요.

```cpp
const unsigned int SERVO_MIN = 2000; // 스페이스바 뗌 (0도)
const unsigned int SERVO_MAX = 2222; // 스페이스바 누름 (약 20도)
const int SENSITIVITY = 35;          // 장애물 감지 민감도 (낮을수록 예민)
