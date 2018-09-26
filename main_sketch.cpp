// Maksim Grebenev
#include <Servo.h>

const int INITIAL_SERVO_DELAY = 50;
const int UNDEFINED_PORT = -1;

const int DPORT1 = 1;
/*
* Обертка для работы с серво машинкой
* Mg995 - сервомашинки для манипулятора ROT3U-6DOF
* https://www.electronicoscaldas.com/datasheet/MG995_Tower-Pro.pdf
*/
class rot3u_6dof_servo {
  private int servoPosition = 0;
  private Servo servo;
  private int portNumber = UNDEFINED_PORT;
  // задержка при повороте на 1 градус
  // каждый сервопривод может иметь свою скорость вращения
  private int degDelay = INITIAL_SERVO_DELAY;
  private int getDegDelay() {
    return degDelay;
  }
  private void setDegDelay(int value) {
    degDelay = value;
  }
  __declspec(property(get = getDegDelay, put = setDegDelay)) int DelayPerGrad; 
  
  public int getPosition() {
    return servoPosition;
  }

  public bool isInitialized() {
    return portNumber != UNDEFINED_PORT;
  }
  
  // Инициализация серво привода
  public void Initialize(int port) {
    if (isInitialized()) {
      return; // сервопривод уже инициализирован
    }
    portNumber = port; // сохраняем назначенный порт для проверки 
    servo.attach(portNumber); // поключить сервопривод к порту
  }
  
  private int getPortNumber() {
    return portNumber;
  }
  // обертка над инициализацией
  __declspec(property(get = getPortNumber, put = Initialize)) int Port; 
  
  // исполнение сервопривода
  void perform (int to, int delay) {
    int direction = (to - getPosition()) > 0 ? 1 : -1;
    while (to != servoPosition) {
      servo.write(servoPosition);
      servoPosition += direction;
      delay(delay);
  }

  void perform (int to) {
    perform(to, DelayPerGrad)
  }
}

rot3u_6dof_servo servo;

// Set initial position
void reset() {
  // Initialize servo positions
  servo.perform(90); // set position to middle for each servo
}

void setup() {
  rot3u_6dof_servo.Initialize(DPORT1);
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
