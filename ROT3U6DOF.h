#ifndef ROT3U6DOF_H
#define ROT3U6DOF_H

#import "SGServo.h"

#define ROT3U6DOF_SERVO_COUNT 6
#define ROT3U6DOF_SERVO_START_PORT 1 // т.е. сервоприводы подключены к портам (1 .. ROT3U6DOF_SERVO_COUNT)

const SGServo servos[ROT3U6DOF_SERVO_COUNT]; // 6 серво для ROT3U-6DOF

void setupROT3U6DOF (int min, int max) {
  for (int i=0; i<ROT3U6DOF_SERVO_COUNT; i++) {
    SGServo currentServo = servos[i];
    int portNumber = i + ROT3U6DOF_SERVO_START_PORT;
    currentServo.initialize(portNumber);
    currentServo.setTimes(min, max); // врямена ШИМ для сервопривода MG995
  }
}

#define PROCESS_STEPS_COUNT 100 // количество шагов для смены положения сервопривода
#define DELAY_FOR_SERVO_STEP 100
void performAllImmediately(int newPositions[ROT3U6DOF_SERVO_COUNT]) {
  for (int i=0; i<ROT3U6DOF_SERVO_COUNT; i++) {
    SGServo currentServo = servos[i];
    currentServo.performImmediately(newPositions[i]);
  }
}
/*
* Установка всех сервоприводов за один раз
* newPositions - массив новых позиций (если значение UNDEFINED, обработка привода пропускается)
*/
void performAllServos(int newPositions[ROT3U6DOF_SERVO_COUNT], int delay) {
  // сохраняем начальные значения приводов
  int initialRanges[ROT3U6DOF_SERVO_COUNT];
  for (int i=0; i<ROT3U6DOF_SERVO_COUNT; i++) {
    SGServo currentServo = servos[i];
    initialRanges[i] = currentServo.getPosition();
  }

  // выполняем смещение приводов 
  for (int step = 0; step < PROCESS_STEPS_COUNT; step++) {
    // 
    for (int i=0; i<ROT3U6DOF_SERVO_COUNT; i++) {
      if (newPositions[i] == UNDEFINED) { // Менять значение не нужно
        continue;
      }
      SGServo currentServo = servos[i];
      if (newPositions[i] == currentServo.getPosition()) { // Сервопривод уже установлен в текущее положение
        continue;
      }

      int shiftRange = newPositions[i] - initialRanges[i];
      int nextPos = (int) (initialRanges[i] + (((float)(shiftRange*step))/PROCESS_STEPS_COUNT));

      // Перемещаем привод в посчитанное положение.
      currentServo.performImmediately(nextPos);
    }
    ::delay(delay);
  }
}

void performAllServos(int newPositions[ROT3U6DOF_SERVO_COUNT]) {
  performAllServos(newPositions, DELAY_FOR_SERVO_STEP);
}

#define STARTPOS_SERVO1_DEG 45
#define STARTPOS_SERVO2_DEG 0
#define STARTPOS_SERVO3_DEG 48
#define STARTPOS_SERVO4_DEG 160
#define STARTPOS_SERVO5_DEG 180
#define STARTPOS_SERVO6_DEG 20 // 10-60 ; 10-20 - opened
#define INITIAL_DELAY 30
/*
* Установка начального положения манипулятора
*/
void resetROT3U6DOF() {
  int positions[] = {
    STARTPOS_SERVO1_DEG,
    STARTPOS_SERVO2_DEG,
    STARTPOS_SERVO3_DEG,
    STARTPOS_SERVO4_DEG,
    STARTPOS_SERVO5_DEG,
    STARTPOS_SERVO6_DEG
  };
  performAllImmediately(positions);
}

#endif
