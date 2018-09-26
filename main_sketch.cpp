// Maksim Grebenev
#include <Servo.h>

const int INITIAL_SERVO_DELAY = 50;
const int UNDEFINED_PORT = -1;

/*
* Обертка для работы с серво машинкой
* Mg995 - сервомашинки для манипулятора ROT3U-6DOF
* https://www.electronicoscaldas.com/datasheet/MG995_Tower-Pro.pdf
*/
class SGServo {
  private:
  int servoPosition = 0;
  Servo servo;
  int portNumber = UNDEFINED_PORT;
  // задержка при повороте на 1 градус
  // каждый сервопривод может иметь свою скорость вращения
  int degDelay = INITIAL_SERVO_DELAY;
  int getDegDelay() {
    return degDelay;
  }
  int getPortNumber() {
    return portNumber;
  }

  public:

  SGServo () {
    servoPosition = 0;
  }

  int getPosition() {
    return servoPosition;
  }

  void setDegDelay(int value) {
    degDelay = value;
  }

  bool isInitialized() {
    return portNumber != UNDEFINED_PORT;
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
      servo.write(servoPosition);
      servoPosition += direction;
      ::delay(delay);
    }
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

const int DPORT1 = 1;

SGServo servo;

// Set initial position
void reset() {
  // Initialize servo positions
  servo.perform(90); // set position to middle for each servo
}

void setup() {
  servo.initialize(DPORT1);
  delay(500);
  reset();
  // подождем 1с пока все приводы закончат позиционирование
  delay(1000);
}

void loop() {
  servo.perform(0);
  delay(500);
  servo.perform(180);
  delay(500);
}
