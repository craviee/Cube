#include <Servo.h>

Servo claw;
Servo base;

#define turn_left_degrees 58
#define turn_right_degrees 60
#define rotation_degrees 31
#define delay_time 2000
#define turn_cube 76

void setup()
{
  // Define claw on pin 6
  claw.attach(6);
  // Define Base on pin 11
  base.attach(11);
  // Define Arduino's clock
  Serial.begin(9600);
  // Init claw and base
  init();
}

void loop()
{
  if (Serial.available())
  {
    // Receive Command
    char char_comando = Serial.read();
    int comando = Serial.parseInt();
    Serial.println(comando);
    switch(comando)
    {
      case 1: // from up to front // 5s
        turn(turn_right_degrees);
        break;
      case 2: // from front to down // 5s
        turn(turn_right_degrees);
        break;
      case 3: // from down to back // 5s
        turn(turn_right_degrees);
        break;
      case 4: // from back to right // 9s
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn_base_left();
        turn(turn_left_degrees);
        turn_base_right();
        break;
      case 5: // from right to left // 7s
        turn_base_left();
        turn(turn_left_degrees);
        turn(turn_left_degrees);
        turn_base_right();
        break;
      case 6: // retorna do left to up // 12s
        turn_base_left();
        turn(turn_left_degrees);
        turn_base_right();
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        break;
      case 7: //U // 35s
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        rotate_clockwise(rotation_degrees);
        
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn_base_left();
        turn(turn_left_degrees);
        turn_base_right();
        turn(turn_right_degrees);
        
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        break;
      case 8: //U' // 20s
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        rotate_anticlockwise(rotation_degrees);
        
        turn(turn_right_degrees);
        turn_base_left();
        turn(turn_left_degrees);
        turn_base_right();
        
        turn(turn_right_degrees);
        break;
      case 9: //F // 35s
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        rotate_clockwise(rotation_degrees);
        
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn_base_left();
        turn(turn_left_degrees);
        turn_base_right();
        turn(turn_right_degrees);
        
        turn(turn_right_degrees);
        break;
      case 10: //F' // 20s
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        rotate_anticlockwise(rotation_degrees);

        turn(turn_right_degrees);
        turn_base_left();
        turn(turn_left_degrees);
        turn_base_right();

        break;
      case 11: //D // 31s
        rotate_clockwise(rotation_degrees);

        turn(turn_right_degrees);
        turn_base_left();
        turn(turn_left_degrees);
        turn(turn_left_degrees);
        turn(turn_left_degrees);
        turn_base_right();
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        
        break;
      case 12: //D' // 20s
        rotate_anticlockwise(rotation_degrees);
        turn(turn_right_degrees);
        turn_base_left();
        turn(turn_left_degrees);
        turn_base_right();
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        break;
      case 13: //B // 27s
        turn(turn_right_degrees);
        rotate_clockwise(rotation_degrees);

        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn_base_left();
        turn(turn_left_degrees);
        turn_base_right();
        break;
      case 14: //B' // 19s
        turn(turn_right_degrees);
        rotate_anticlockwise(rotation_degrees);

        turn(turn_right_degrees);
        turn_base_left();
        turn(turn_left_degrees);
        turn_base_right();
        
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        break;
      case 15: //R // 39s
        turn_base_left();
        turn(turn_left_degrees);
        turn(turn_left_degrees);
        turn(turn_left_degrees);
        turn_base_right();
        rotate_clockwise(rotation_degrees);
        
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn_base_left();
        turn(turn_left_degrees);
        turn_base_right();
        turn(turn_right_degrees);
        
        turn_base_left();
        turn(turn_left_degrees);
        turn_base_right();
        break;
      case 16: //R' // 29s
        turn_base_left();
        turn(turn_left_degrees);
        turn(turn_left_degrees);
        turn(turn_left_degrees);
        turn_base_right();
        rotate_anticlockwise(rotation_degrees);

        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn_base_left();
        turn(turn_left_degrees);
        turn(turn_left_degrees);
        turn(turn_left_degrees);
        turn_base_right();
        turn(turn_right_degrees);
        break;
      case 17: //L // 39s
        turn_base_left();
        turn(turn_left_degrees);
        turn_base_right();
        rotate_clockwise(rotation_degrees);

        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn(turn_right_degrees);
        turn_base_left();
        turn(turn_left_degrees);
        turn_base_right();
        turn(turn_right_degrees);
        
        turn_base_left();
        turn(turn_left_degrees);
        turn(turn_left_degrees);
        turn(turn_left_degrees);
        turn_base_right();
        break;
      case 18: //L'// 21s
        turn_base_left();
        turn(turn_left_degrees);
        turn_base_right();
        rotate_anticlockwise(rotation_degrees);
        
        turn_base_left();
        turn(turn_left_degrees);
        turn(turn_left_degrees);
        turn(turn_left_degrees);
        turn_base_right();
        turn(turn_right_degrees);
        break;
    }
  }
}

void turn_base_left()
{
  base.write(5);
  delay(delay_time); 
}

void turn_base_right()
{
  base.write(90);
  delay(delay_time);
}


void init()
{
  claw.write(0);
  base.write(90);
  delay(delay_time);
}

void turn(int n)
{
  claw.write(n);
  delay(delay_time);
  claw.write(0);
  delay(delay_time);
}

void rotate_anticlockwise(int n)
{
  base.write(180);
  delay(delay_time);
  claw.write(n);
  delay(delay_time);
  base.write(turn_cube);
  delay(delay_time);
  claw.write(0);
  delay(delay_time);
  base.write(90);
  delay(delay_time);
}

void rotate_clockwise(int n)
{
  base.write(180);
  delay(delay_time);
  claw.write(n);
  delay(delay_time);
  base.write(turn_cube);
  delay(delay_time);
  claw.write(0);
  delay(delay_time);
  base.write(180);
  delay(delay_time);
  claw.write(n);
  delay(delay_time);
  base.write(turn_cube);
  delay(delay_time);
  claw.write(0);
  delay(delay_time);
  base.write(180);
  delay(delay_time);
  claw.write(n);
  delay(delay_time);
  base.write(turn_cube);
  delay(delay_time);
  claw.write(0);
  delay(delay_time);
  base.write(90);
  delay(delay_time);
}
