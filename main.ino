/*
 * IEEE quarterly project - Spring 2024
 *
 *   `pwm.setOscillatorFrequency`
 *
 * In theory the internal oscillator (clock) is 25MHz but it really isn't
 * that precise. You can 'calibrate' this by tweaking this number until
 * you get the PWM update frequency you're expecting!
 * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
 * is used for calculating things like writeMicroseconds()
 * Analog servos run at ~50 Hz updates, It is importaint to use an
 * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
 * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
 *    the I2C PCA9685 chip you are setting the value for.
 * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
 *    expected value (50Hz for most ESCs)
 * Setting the value here is specific to each individual I2C PCA9685 chip and
 * affects the calculations for the PWM update frequency. 
 * Failure to correctly set the int.osc value will cause unexpected PWM results
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

const int buttonPin = 2;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int lerp(int min, int max, float t) {
  return int(float(max - min) * t + float(min)); 
}

void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");

  pinMode(buttonPin, INPUT);
  
  pwm.begin();
  pwm.setOscillatorFrequency(23000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  pinMode(LED_BUILTIN, OUTPUT);
  delay(10);
}

void moveServo(int pin, int pin1){
  pwm.setPWM(pin , 0, SERVOMAX);
  pwm.setPWM(pin1, 0, SERVOMIN);
  delay(100);
  
  for (float t = 0.0f; t <= 1.0f; t += 0.01) {
    pwm.setPWM(pin , 0, lerp(SERVOMIN, SERVOMAX, t       ));
    pwm.setPWM(pin1, 0, lerp(SERVOMIN, SERVOMAX, 1.0f - t));
    delay(50);
  }
}

void loop() {
  if (digitalRead(buttonPin) == HIGH) {
    moveServo(0, 1); // right
    moveServo(2, 3); // left
    moveServo(4, 5); // center
  }
  else delay(1);
}
