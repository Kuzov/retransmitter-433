#include <RemoteReceiver.h>
#include <RemoteSwitch.h>
/*
* Demo for RF remote switch receiver.
* For details, see RemoteReceiver.h!
*
* This sketch shows the received signals on the serial port.
* Connect the receiver to digital pin 2.
*/

#define RF1ON   288663    //light on
#define RF1OFF  288659   //light off

#define RF2ON   288195    //sound on
#define RF2OFF  288191    //sound off

#define RF3ON   287223    //TK on
#define RF3OFF  287219    //TK off

#define PERIOD 149;


void setup() {
  Serial.begin(115200);
  
  //Initialize receiver on interrupt 0 (= digital pin 2), calls the callback "showCode"
  //after 3 identical codes have been received in a row. (thus, keep the button pressed
  //for a moment)
  //
  //See the interrupt-parameter of attachInterrupt for possible values (and pins)
  //to connect the receiver.
  RemoteReceiver::init(0, 3, showCode);
}

void loop() {
  while (Serial.available() == 0);

  int val = Serial.read() - '0';
  if (val == 1) transmit(RF1ON);  
  if (val == 2) transmit(RF1OFF);
  if (val == 3) transmit(RF2ON);  
  if (val == 4) transmit(RF2OFF);
  if (val == 9) transmit(RF3ON);  
  if (val == 0) transmit(RF3OFF);
  
}

//Callback function is called only when a valid code is received.
void showCode(unsigned long receivedCode, unsigned int period) {
  //Note: interrupts are disabled. You can re-enable them if needed.
  
  //Print the received code.
  Serial.print("Code: ");
  Serial.print(receivedCode);
  Serial.print(", period duration: ");
  Serial.print(period);
  Serial.println("us.");
}


void transmit(unsigned long rcode){
 unsigned long code = rcode;
  unsigned long period = PERIOD;
  code |= (unsigned long)period << 23;
  code |= 4L << 20; //(|= 4L) цифра перед (L), это (условное число), количества повторов посылаемого сигнала. (соответственно и паузы)
  RemoteSwitch::sendTelegram(code, 11); // RF transmitter pin - пин радио передатчика
   }

