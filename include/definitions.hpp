#pragma once
#include "main.h"
//
const int VISION_PORT = 19;
const int DRIVE_MOTOR_LEFT_1 = 1;
const int DRIVE_MOTOR_LEFT_2 = 2;
const int DRIVE_MOTOR_RIGHT_1 = -3;
const int DRIVE_MOTOR_RIGHT_2 = -4;


const int ARM_MOTOR = 10;
inline MotorGroup baseL({DRIVE_MOTOR_LEFT_1, DRIVE_MOTOR_LEFT_2});
inline MotorGroup baseR({DRIVE_MOTOR_RIGHT_1, DRIVE_MOTOR_RIGHT_2});

inline Controller controller;

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

void pidTurn(float set, QTime waitTime, int max_voltage);
void fwbwVoltage(int voltage);
void turnVoltage(int voltage);
void strafeVoltage(int voltage);
void stopDrive();
int calcVoltage(float volts);
void pidBase(float set, QTime waitTime, int max_voltage);

 const auto WHEEL_DIAMETER = 4_in;     //edit
 const auto CHASSIS_WIDTH = 12.75_in;  //edit
 inline auto driveController = ChassisControllerFactory::create(
    {DRIVE_MOTOR_RIGHT_1, DRIVE_MOTOR_RIGHT_2},
    {DRIVE_MOTOR_LEFT_1, DRIVE_MOTOR_LEFT_2},
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
 float windupmax;

 float iTerm=0;
 float error;
 float dt = 20;
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

  if (out > 12000)
  {out = 12000;}

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

    float get_Iterm()
    {return iTerm;}
};

inline PID armPID(60, 0, 13000, 2000 );  //object creation
inline PID basePID(60,10,26000, 2000);
//1740
//55, 1.7

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
//inline visionSensor vision; //object creation
