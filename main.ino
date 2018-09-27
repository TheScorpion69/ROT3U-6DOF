// Maksim Grebenev
#import "SGServo.h"
#import "ROT3U6DOF.h"

//Arduino UNO DIGITAL PORTS 0 .. 13
#define DP1 1

SGServo servo;

void setup() {
  // setupROT3U6DOF(0, 0); // Initialize servos ROT3U6DOF_SERVO_COUNT/ROT3U6DOF_SERVO_START_PORT
  // delay(2000); // подождем пока все приводы проинициализируются
  // resetROT3U6DOF(); // установим кутяплю в исходное положение

  //****************************************
  servo.initialize(DP1);
  // https://arduinomaster.ru/motor-dvigatel-privod/servoprivody-arduino-sg90-mg995-shema-podklyuchenie-upravlenie/
  // servo.setTimes(544, 2400)
  delay(500);
  servo.perform(90);
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
  //****************************************
  // тест для колиброви сервопривода
  servo.perform(0);
  delay(5000);
  servo.perform(180);
  delay(5000);
  //****************************************
}
