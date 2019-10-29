#pragma once
#include "main.h"
//
const int VISION_PORT = 19;

const int DRIVE_MOTOR_LEFT_1 = 1;  //CHANGE
const int DRIVE_MOTOR_LEFT_2 = -2;

const int DRIVE_MOTOR_LEFT_3 = 3;
const int DRIVE_MOTOR_LEFT_4 = -4;

const int DRIVE_MOTOR_RIGHT_1 = -5;
const int DRIVE_MOTOR_RIGHT_2 = 6;

const int DRIVE_MOTOR_RIGHT_3 = -7;
const int DRIVE_MOTOR_RIGHT_4 = 8;

const int ARM_MOTOR = 10;
inline MotorGroup baseLF({DRIVE_MOTOR_LEFT_1, DRIVE_MOTOR_LEFT_2});
inline MotorGroup baseLB({DRIVE_MOTOR_LEFT_3, DRIVE_MOTOR_LEFT_4});
inline MotorGroup baseRF({DRIVE_MOTOR_RIGHT_1, DRIVE_MOTOR_RIGHT_2});
inline MotorGroup baseRB({DRIVE_MOTOR_RIGHT_3, DRIVE_MOTOR_RIGHT_4});

inline Controller masterController;

inline ControllerButton RightBumperUP(ControllerDigital::R1);
inline ControllerButton LeftBumperUP(ControllerDigital::L1);
inline ControllerButton LeftBumperDOWN(ControllerDigital::L2);
inline ControllerButton RightBumperDOWN(ControllerDigital::R2);
inline ControllerButton ButtonA(ControllerDigital::A);
inline ControllerButton ButtonB(ControllerDigital::B);
inline ControllerButton ButtonX(ControllerDigital::X);
inline ControllerButton ButtonY(ControllerDigital::Y);
inline ControllerButton ButtonUP(ControllerDigital::up);
inline ControllerButton ButtonDOWN(ControllerDigital::down);
inline ControllerButton ButtonLEFT(ControllerDigital::left);
inline ControllerButton ButtonRIGHT(ControllerDigital::right);

void pidTurn(float set, int maxVoltage);
void fwbwVoltage(int voltage);
void turnVoltage(int voltage);
void strafeVoltage(int voltage);
void stopDrive();
int calcVoltage(float volts);

 const auto WHEEL_DIAMETER = 4_in;     //edit
 const auto CHASSIS_WIDTH = 12.75_in;  //edit
 inline auto driveController = ChassisControllerFactory::create(
    {DRIVE_MOTOR_RIGHT_1, DRIVE_MOTOR_RIGHT_2, DRIVE_MOTOR_RIGHT_3, DRIVE_MOTOR_RIGHT_4},
    {DRIVE_MOTOR_LEFT_1, DRIVE_MOTOR_LEFT_2, DRIVE_MOTOR_LEFT_3, DRIVE_MOTOR_LEFT_4},
    AbstractMotor::gearset::green,
    {WHEEL_DIAMETER, CHASSIS_WIDTH}
   );

inline Motor arm_motor(ARM_MOTOR);
inline Vision vision_sensor(VISION_PORT);

class PID
{
private:

 float Kp;
 float Ki;
 float Kd;

 float set_point = 0;
 bool antiwindup = false;
 bool windupmax;

 float iTerm=0;
 float error;
 float dt = 0.02;
 float Dterm;
 float outValue = 0;
 float last_error = 0;
 float last_input = 0;

public:
 PID(float Kp_, float Ki_, float Kd_, float windupmax_=0)
 {
   Kp = Kp_;
   Ki = Ki_;
   Kd = Kd_;
   windupmax = windupmax_;

   if(windupmax_ != 0)
   {antiwindup = true;}
   else
 {antiwindup = false;}
 }

float output(float input)
{
  error = set_point - input;

  iTerm = iTerm + Ki*error*dt;
  iTerm = antiWindup(iTerm);
  Dterm =  (last_input - input)/dt;
  float out = Kp*error + iTerm + Dterm;

  last_error = error;
  last_input = input;
  return out;
}
   // Anti windup system
   float antiWindup(float u)
   {
       if(antiwindup == false) //if(!antiwindup)
       {return u;}

       else if (u >= windupmax)
       {return windupmax;}

       else if (u <= -windupmax)
       {return -windupmax;}

     else
       {return u;}
   }

void set_set_point(float set_point_)
  {set_point = set_point_;}

  // Get the setpoint (print it)
  float get_set_point()
  {return set_point;}

  void set_error(float error_)
    {error = error_;}


  float get_error()
  {return error;}

  float get_dt()
  {return dt;}

  float get_Dterm()
  {return Dterm;}

  void set_Dterm(float Dterm_)
    {Dterm = Dterm_;}
};

inline PID armPID(50, 70, 0, 0);  //object creation



class visionSensor
{
  public:
  visionSensor();

  std::string largestSigStr()
  {
    std::string sigColor;
    pros::vision_object_s_t largestSig = vision_sensor.get_by_size(0);

    if (largestSig.signature == 1)
    {sigColor = "Orange";}
    else if (largestSig.signature == 2)
    {sigColor = "Purple";}
    else if (largestSig.signature == 3)
    {sigColor = "Green";}
    else
    {sigColor = "No sig detected";}

    return sigColor;
  }

  bool colorCheck(std::string Color)
  {
    int n;
    bool colorMatch;
      if (largestSigStr() != Color)
      { colorMatch = false;}
      else
      colorMatch = true;
    return colorMatch;
  }

  void centerCube(std::string Color, int voltage, int threshold)
  {
    if (colorCheck(Color) == true)
      {
        pros::vision_object_s_t cube = vision_sensor.get_by_size(0);
        while((-threshold <= cube.x_middle_coord <= threshold) && (cube.x_middle_coord != 0))
        {
          if (cube.x_middle_coord > 10)
          {
           strafeVoltage(-voltage); //strafe left
          }
          else if(cube.x_middle_coord < -10 )
          {
           strafeVoltage(voltage);  //strafe right
          }
           stopDrive(); //stop motors
        }

      }
    else;
  }
};
visionSensor vision(); //object creation
