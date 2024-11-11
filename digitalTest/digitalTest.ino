#define EMIT_PIN    11    // Documentation says 11.
#define LS_LEFT_PIN A2   // Complete for DN3 pin


void setup() {

  // Set some initial pin modes and states
  pinMode( EMIT_PIN, INPUT ); // Set EMIT as an input (off)
  pinMode( LS_LEFT_PIN, INPUT );     // Set line sensor pin to input

  // Start Serial, wait to connect, print a debug message.
  Serial.begin(9600);
  delay(1500);
  Serial.println("***RESET***");

} // End of setup()


void loop() {

  pinMode( EMIT_PIN, OUTPUT );
  digitalWrite( EMIT_PIN, HIGH );

  pinMode( LS_LEFT_PIN, OUTPUT );
  digitalWrite( LS_LEFT_PIN, HIGH );
  delayMicroseconds( 10 );

  pinMode( LS_LEFT_PIN, INPUT );

  unsigned long start_time = micros();

  while( digitalRead( LS_LEFT_PIN ) == HIGH ) {
      
  }

  unsigned long end_time = micros();
  
  pinMode( EMIT_PIN, INPUT );

  unsigned long elapsed_time = end_time - start_time;

  // Print what this sensor measurement was
  Serial.println( elapsed_time );

  delay(20);

} // End of loop()
