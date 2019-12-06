#include "definitions.hpp"

void pidBaseTurn(float set, QTime waitTime, int max_voltage){
  float basePos;
  baseL.tarePosition();
  baseR.tarePosition();
  baseTurnPID.set_error(10000);
  baseTurnPID.set_Dterm(10000);
  baseTurnPID.set_set_point(abs(set));
//.
  Timer timer;
  timer.placeMark();
while(timer.getDtFromMark() < waitTime)
{
  basePos = (float) ((abs(baseL.getPosition())+abs(baseR.getPosition()))/2);
  pros::lcd::print(5, "WE'RE IN %f\n", basePos);

  if (set > 0)
  {
    if(baseTurnPID.output(basePos) > max_voltage)
    {
       baseL.moveVoltage(max_voltage);
       baseR.moveVoltage(-max_voltage);
    }
    else
    {
       baseL.moveVoltage(baseTurnPID.output(basePos));
       baseR.moveVoltage(-(baseTurnPID.output(basePos)));
    }
  }

  else
  {
    if(baseTurnPID.output(basePos) > max_voltage)
    {
       baseL.moveVoltage(-max_voltage);
       baseR.moveVoltage(max_voltage);
    }
    else
    {
       baseL.moveVoltage(-(baseTurnPID.output(basePos)));
       baseR.moveVoltage(baseTurnPID.output(basePos));
    }
  }
   if (baseTurnPID.get_error() > 15)
   {
     timer.clearMark();
     timer.placeMark();
   }
   pros::delay(20);
}
baseL.moveVoltage(0);
baseR.moveVoltage(0);
pros::lcd::print(6, "WE'RE OUT %f\n", baseTurnPID.get_error());
//pros::lcd::print(7,"Iterm %f\n", basePID.get_Iterm());
}
//

void pidBase(float set, QTime waitTime, int max_voltage)
{
  float basePos;
  baseL.tarePosition();
  baseR.tarePosition();
  basePID.set_error(10000);
  basePID.set_Dterm(10000);
  basePID.set_set_point(set);

  Timer timer;
  timer.placeMark();
while(timer.getDtFromMark() < waitTime)
{
   pros::lcd::print(5, "WE'RE IN %f\n", basePID.get_set_point());
   pros::lcd::print(7,"dterm %f\n", basePID.get_Dterm());
   baseL.moveVoltage(basePID.output(basePos));
   baseR.moveVoltage(basePID.output(basePos));
   basePos = (float) abs((baseL.getPosition()+baseR.getPosition())/2);
//
   if(basePID.output(basePos) > max_voltage )
   {baseL.moveVoltage(max_voltage);
    baseR.moveVoltage(max_voltage);}
   else
   {baseL.moveVoltage(basePID.output(basePos));
    baseR.moveVoltage(basePID.output(basePos));}
   if (basePID.get_error() > 15)
   {
     timer.clearMark();
     timer.placeMark();
   }
   pros::delay(20);
}
baseL.moveVoltage(0);
baseR.moveVoltage(0);
pros::lcd::print(6, "WE'RE OUT %f\n", basePID.get_error());
//pros::lcd::print(7,"Iterm %f\n", basePID.get_Iterm());
//
}
//
void pidTurn(float set, QTime waitTime, int max_voltage)
{
  float armPos;
  arm_motor.tarePosition();
  armPID.set_error(10000);
  armPID.set_Dterm(10000);
  armPID.set_set_point(set);
  Timer timer;
  timer.placeMark();
while(timer.getDtFromMark() < waitTime)
{
   pros::lcd::print(5, "WE'RE IN %f\n", armPID.get_set_point());
   pros::lcd::print(7,"dterm %f\n", armPID.get_Dterm());
   arm_motor.moveVoltage(armPID.output(armPos));
   armPos = (float) arm_motor.getPosition();


   if(armPID.output(armPos) > max_voltage )
   {arm_motor.moveVoltage(max_voltage);}
   else
   {arm_motor.moveVoltage(armPID.output(armPos));}
   if (armPID.get_error() > 15)
   {
     timer.clearMark();
     timer.placeMark();
   }
   pros::delay(20);
}
arm_motor.moveVoltage(0);
pros::lcd::print(6, "WE'RE OUT %f\n", armPID.get_error());
//pros::lcd::print(7,"Iterm %f\n", armPID.get_Iterm());
}


int calcVoltage(float volts){
  if(volts>=1.0){
    volts = 12000;
  } else if(volts<=-1.0){
    volts = -12000;
  } else{
    volts *= 12000;
  }

  return((int)volts);
}


void fwbwVoltage(int voltage)
{
  baseL.moveVoltage(voltage);
  baseR.moveVoltage(-voltage);
}
void turnVoltage(int voltage)
{
  baseL.moveVoltage(voltage);
  baseR.moveVoltage(voltage);
}
void strafeVoltage(int voltage)
{
  baseL.moveVoltage(voltage);
  baseR.moveVoltage(voltage);
}
void stopDrive()
{
  baseL.moveVoltage(0);
  baseR.moveVoltage(0);
}
