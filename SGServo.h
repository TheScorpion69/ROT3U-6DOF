#ifndef SGSERVO_H
#define SGSERVO_H

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
  int servoPosition;
  Servo servo;
  int portNumber;
  // задержка при повороте на 1 градус
  // каждый сервопривод может иметь свою скорость вращения
  int degDelay;
  int getDegDelay();
  int getPortNumber();
  int minMicroseconds; // Время ШИМ для положения 0 град
  int maxMicroseconds; // Время ШИМ для положения 180 град

  public:
  SGServo ();
  //Устаноква времени ШИМ
  //Pulse Width Modulation (PWM)
  //Для использования точного позиционирования servoPerformImmediatelyByPWD
  void setTimes(int min, int max);
  //Сброс времени ШИМ
  void resetTimes();
  bool PWDInitialized();
  int getPosition();
  void setDegDelay(int value);
  bool isInitialized();
  void initialize(int port);
  void perform (int to, int delay);
  void performImmediately (int to);
  void servoPerformImmediatelyByPWD(int angle);
  // исполнение сервопривода
  void perform (int to);
};

#endif
