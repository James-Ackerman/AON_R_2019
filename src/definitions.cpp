#include "definitions.hpp"
void pidTurn(float set, QTime waitTime, int max_voltage)
{
  float armPos;
  arm_motor.tarePosition();
  armPID.set_error(10000);
  armPID.set_Dterm(10000);
  armPID.set_set_point(set);

//timer
Timer timer;
timer.placeMark();
while(timer.getDtFromMark() < waitTime)
{
   pros::lcd::print(5, "WE'RE IN %f\n", armPID.get_set_point());
   arm_motor.move_voltage(armPID.output(armPos));
   armPos = (float) arm_motor.getPosition();

   if(armPID.output(armPos) > max_voltage )
   {arm_motor.move_voltage(max_voltage);}
   else
   {arm_motor.move_voltage(armPID.output(armPos));}
   if (armPID.get_error() > 0.01)
   {
     timer.clearMark();
     timer.placeMark();
   }
   pros::delay(20);
}
arm_motor.move_voltage(0);
pros::lcd::print(6, "WE'RE OUT %f\n", armPID.get_error());
pros::lcd::print(7,"Iterm %f\n", armPID.get_Iterm());
}
//
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
  baseLF.moveVoltage(voltage);
  baseLB.moveVoltage(voltage);
  baseRF.moveVoltage(-voltage);
  baseRB.moveVoltage(-voltage);
}
void turnVoltage(int voltage)
{
  baseLF.moveVoltage(voltage);
  baseLB.moveVoltage(voltage);
  baseRF.moveVoltage(voltage);
  baseRB.moveVoltage(voltage);
}
void strafeVoltage(int voltage)
{
  baseLF.moveVoltage(voltage);
  baseLB.moveVoltage(-voltage);
  baseRF.moveVoltage(-voltage);
  baseRB.moveVoltage(voltage);
}
void stopDrive()
{
  baseLF.moveVoltage(0);
  baseLB.moveVoltage(0);
  baseRF.moveVoltage(0);
  baseRB.moveVoltage(0);
}
