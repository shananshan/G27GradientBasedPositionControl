// Constructor,initialise(),setPWM(float,float).

#ifndef _MOTORS_H
#define _MOTORS_H
#define L_PWM 10
#define L_DIR 16
#define R_PWM 9
#define R_DIR 15

#define MAX_PWM 50.0
#define L_F LOW
#define L_B HIGH
#define R_F LOW
#define R_B HIGH

class Motors_c {
  public:    // Constructor, must exist.
    Motors_c() {      // Leave empty. Ensure initialise() is called instead.
    }
    void initialise() {
      pinMode( L_PWM , OUTPUT );
      pinMode( L_DIR , OUTPUT );
      pinMode( R_PWM , OUTPUT );
      pinMode( R_DIR , OUTPUT );
      digitalWrite( L_DIR, LOW );
      digitalWrite( R_DIR, LOW );
      analogWrite( L_PWM , 0 );
      analogWrite( R_PWM , 0 );
    } // End of initialise()
    void setPWM( float left_pwr, float right_pwr ) {
      if ( left_pwr < 0 ) {
        digitalWrite( L_DIR, L_B );
      } else {
        digitalWrite( L_DIR, L_F );
      }
      if ( right_pwr < 0 ) {
        digitalWrite( R_DIR, R_B );
      } else {
        digitalWrite( R_DIR, R_F );
      }
      left_pwr = constrain(abs(left_pwr),0,MAX_PWM);
      right_pwr = constrain(abs(right_pwr),0,MAX_PWM);
      analogWrite( L_PWM, left_pwr );
      analogWrite( R_PWM, right_pwr );
      return;
    } // End of setPWM()
}; // End of Motors_c class definition.
#endif
