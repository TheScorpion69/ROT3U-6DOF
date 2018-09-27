// Maksim Grebenev
#include <Servo.h>
#include <math.h>

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

//Arduino UNO DIGITAL PORTS 0 .. 13
#define DP1 1

SGServo servo;

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
#define DELAY_FOR_SERVO_STEP 10
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
      int nextPos = (int) (initialRanges[i] + ((float)(shiftRange*step)/PROCESS_STEPS_COUNT));

      // Перемещаем привод в посчитанное положение. В зависимости от необходимости калибровки
      currentServo.performImmediately(nextPos);
      //currentServo.servoPerformImmediatelyByPWD(nextPos);
      /*-----------------------*/ 
    }
    ::delay(delay);
  }
}
void performAllServos(int newPositions[ROT3U6DOF_SERVO_COUNT]) {
  performAllServos(newPositions, DELAY_FOR_SERVO_STEP);
}

#define STARTPOS_SERVO1_DEG 90
#define STARTPOS_SERVO2_DEG 90
#define STARTPOS_SERVO3_DEG 90
#define STARTPOS_SERVO4_DEG 90
#define STARTPOS_SERVO5_DEG 90
#define STARTPOS_SERVO6_DEG 90
#define INITIAL_DELAY 40
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
  performAllServos(positions, INITIAL_DELAY);
}

// Set initial position
void reset() {
  // Initialize servo positions
  servo.perform(90); // set position to middle for each servo
}

void setup() {
  // setupROT3U6DOF(0, 0); // Initialize servos ROT3U6DOF_SERVO_COUNT/ROT3U6DOF_SERVO_START_PORT
  // delay(2000); // подождем пока все приводы проинициализируются
  // resetROT3U6DOF(); // установим кутяплю в исходное положение

  //****************************************
  servo.initialize(DP1);
  delay(500);
  reset();
  // подождем 1с пока все приводы закончат позиционирование
  delay(1000);
  //****************************************
}

void loop() {
  /*
  // TODO
  int position_step_01[] = {
    0, // Servo 1 position
    10, // Servo 2 position
    12, // Servo 3 position
    UNDEFINED, // Servo 4 position
    UNDEFINED, // Servo 5 position
    UNDEFINED // Servo 6 position
  };
  performAllServos(position_step_01);
  */
  servo.perform(0);
  delay(500);
  servo.perform(180);
  delay(500);
}
