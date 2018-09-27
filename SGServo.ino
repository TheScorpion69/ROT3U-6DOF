// Maksim Gebenev
// Объект сервопривода для управления манипулятором
#include <Servo.h>
#include <math.h>
#include <Arduino.h>

#define INITIAL_SERVO_DELAY 50
#define UNDEFINED -1

/*
* Обертка для работы с серво машинкой
* Mg995 - сервомашинки для манипулятора ROT3U-6DOF
* https://www.electronicoscaldas.com/datasheet/MG995_Tower-Pro.pdf
*/
class SGServo {
  private:
  int servoPosition = 0;
  Servo servo;
  int portNumber = UNDEFINED;
  // задержка при повороте на 1 градус
  // каждый сервопривод может иметь свою скорость вращения
  int degDelay = INITIAL_SERVO_DELAY;
  int getDegDelay() {
    return degDelay;
  }
  int getPortNumber() {
    return portNumber;
  }
  int minMicroseconds = 0; // Время ШИМ для положения 0 град
  int maxMicroseconds = 0; // Время ШИМ для положения 180 град

  public:

  SGServo () {
    servoPosition = 0;
  }

  //Устаноква времени ШИМ
  //Pulse Width Modulation (PWM)
  //Для использования точного позиционирования servoPerformImmediatelyByPWD
  void setTimes(int min, int max) {
    minMicroseconds = min;
    maxMicroseconds = max;
  }

  //Сброс времени ШИМ
  void resetTimes() {
    minMicroseconds = 0;
    maxMicroseconds = 0;
  }
    
  bool PWDInitialized() {
    return (minMicroseconds > 0 && maxMicroseconds > 0);
  }
  
  int getPosition() {
    return servoPosition;
  }

  void setDegDelay(int value) {
    degDelay = value;
  }

  bool isInitialized() {
    return portNumber != UNDEFINED;
  }
  
  // Инициализация серво привода
  void initialize(int port) {
    if (isInitialized()) {
      return; // сервопривод уже инициализирован
    }
    portNumber = port; // сохраняем назначенный порт для проверки 
    servo.attach(portNumber); // поключить сервопривод к порту
  }

  // исполнение сервопривода
  void perform (int to, int delay) {
    int direction = (to - getPosition()) > 0 ? 1 : -1;
    while (to != servoPosition) {
      performImmediately(servoPosition);
      servoPosition += direction;
      ::delay(delay);
    }
  }
  
  void performImmediately (int to) {
    if (PWDInitialized()) {
      servoPerformImmediatelyByPWD(to);
    } else {
      servo.write(to);
    }
    servoPosition = to;
  }

  //Прямое управление ШИМ
  void servoPerformImmediatelyByPWD(int angle) {
    int mappedAngleToTime = map(angle, 0, 180, minMicroseconds, maxMicroseconds);
    servo.writeMicroseconds(mappedAngleToTime);
  }

  // исполнение сервопривода с установленной задержкой
  // Предпочтительно настроить все сервоприводы при их инициализации
  // присоединяем к порту
  // servo.initialize(DIGITAL_PORT_NUMBER);
  // настраиваем скорость вращения путем инициализации времени задержки (в мс)
  // после поворота сервомашинки на 1 градус
  // servo.setDegDelay(DELAY_TIME_PER_1GRAD_ms)
  void perform (int to) {
    perform(to, degDelay);
  }

  //__declspec(property(get = getDegDelay, put = setDegDelay)) int DelayPerGrad; 
  // __declspec(property(get = getPortNumber, put = Initialize)) int Port; 
};
