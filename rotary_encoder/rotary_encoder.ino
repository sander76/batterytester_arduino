/* Read encoder values.
 * 
 *  
 *  
 *  Encoder Library - Basic Example
   http://www.pjrc.com/teensy/td_libs_Encoder.html

   This example code is in the public domain.


*/

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <Incoming.h>

Incoming incoming;
char received_char = -1;
bool command_finished = false;

unsigned long previousMillis=0;
const long interval = 200; // 200 milliseconds send interval.


Encoder myEnc(2, 3);
//   avoid using pins with LEDs attached

void setup() {
  Serial.begin(1115200);
  Serial.println("Basic Encoder Test:");
}

long oldPosition  = -999;

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >=interval){
    previousMillis = currentMillis;
      send_position();
    }
  
  read_incoming();
}

long position=-999;
void parse_string(){
  switch (incoming.command){
    case 'p':
      send_position();
      break;
    case 'r':
      Serial.println("{Resetting to 0}");
      myEnc.write(0);
      break;
    default:
      break;
    }
  }

void send_position(){
    position = myEnc.read();
      Serial.print("{p:");
      Serial.print(position);
      Serial.println("}");
  }

void read_incoming()
{
  received_char = Serial.read();
  if (received_char > -1) {
    command_finished = incoming.update(received_char);
    if (command_finished) {
      parse_string();
    }
  }
}
