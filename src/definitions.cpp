#include "definitions.hpp"
void pidTurn(float set, int maxVoltage)
{
  float armPos;
  arm_motor.tarePosition();
  armPID.set_error(10000);
  armPID.set_Dterm(10000);
  armPID.set_set_point(set);

  //while ((armPID.get_Dterm() > 0.0001) && (armPID.get_error() >= 0.001))

  while (armPID.get_Dterm()>0.5/*armPID.get_error()) >= 5*/)
  {
   pros::lcd::print(5, "WE'RE IN %f\n", armPID.get_set_point());
   armPos = (float) arm_motor.getPosition();

   if (armPID.output(armPos) >= maxVoltage)
   {arm_motor.moveVoltage(maxVoltage);}

   else
   arm_motor.move_voltage(armPID.output(armPos));
  }
   pros::lcd::print(5, "WE'RE OUT %f\n", armPID.get_set_point());
  arm_motor.move_voltage(0);
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
