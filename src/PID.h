#ifndef PID_H
#define PID_H

class PID {
public:
  /*
  * Errors
  */
  double errors[3];

  /*
  * Coefficients
  */
  double Kp;
  double Ki;
  double Kd;

  double prevCte;
  double totalCte;
  double steer_value;
  int steps ;
  double twiddle_tolerance ;
  int total_steps_to_twiddle ;
  bool runComplete;
  int twiddle_step;

  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();
};

#endif /* PID_H */
