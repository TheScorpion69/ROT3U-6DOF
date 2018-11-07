// Maksim Grebenev
#import "SGServo.h"
#import "ROT3U6DOF.h"

//Arduino UNO DIGITAL PORTS 0 .. 13
/*
#define DP1 1
#define DP2 2
#define DP3 3
#define DP4 4
#define DP5 5
#define DP6 6

SGServo servo1;
SGServo servo2;
SGServo servo3;
SGServo servo4;
SGServo servo5;
SGServo servo6;
*/
void setup() {
  setupROT3U6DOF(0, 0); // 500-900Initialize servos ROT3U6DOF_SERVO_COUNT/ROT3U6DOF_SERVO_START_PORT
  resetROT3U6DOF(); // установим кутяплю в исходное положение
  delay(2000); // подождем 2 сек
  
  /*
  servo1.initialize(DP1);
  servo2.initialize(DP2);
  servo3.initialize(DP3);
  servo4.initialize(DP4);
  servo5.initialize(DP5);
  servo6.initialize(DP6);

  servo1.performImmediately(45);
  servo2.performImmediately(120);
  servo3.performImmediately(120);
  servo4.performImmediately(90);
  servo5.performImmediately(180);
  servo6.performImmediately(20);
  */

}

void loop() {
  int positions1[] = {UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED, 60};
  performAllServos(positions1);
  delay(3000);
  
  int positions2[] = {UNDEFINED, UNDEFINED, 22, 120, UNDEFINED, UNDEFINED};
  performAllServos(positions2);
  delay(3000);
 
  int positions3[] = {90, UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED};
  performAllServos(positions3);
  delay(3000);
  
  int positions4[] = {UNDEFINED, UNDEFINED, 52, 160, UNDEFINED, UNDEFINED};
  performAllServos(positions4);
  delay(30000);
  
}
