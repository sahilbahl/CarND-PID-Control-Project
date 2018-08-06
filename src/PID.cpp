#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;
  prevCte = 0.0;
  totalCte = 0.0;
  steps  = 0;
  twiddle_tolerance = 0.2;
  total_steps_to_twiddle = 100;
  runComplete = false;
  twiddle_step = 0;
  errors[0] = 1.0;
  errors[1] = 1.0;
  errors[2] = 1.0;
}

void PID::UpdateError(double cte) {
  double diffCte = cte - prevCte;
  prevCte = cte;
  steer_value = -Kp * cte - Kd * diffCte - Ki * totalCte;

  /*if(steps >= total_steps_to_twiddle){
    totalCte +=cte;
  }*/

  steps++;

  /*if(steps == total_steps_to_twiddle*2){
      runComplete = true;
      steps = 0;
      twiddle_step++;
  }*/


}

double PID::TotalError() {
  return totalCte/total_steps_to_twiddle;
}
