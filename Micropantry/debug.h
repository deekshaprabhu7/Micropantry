#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG_PRINT(x)  if(Serial) { Serial.print(x); }
#define DEBUG_PRINTLN(x)  if(Serial) { Serial.println(x); }

#endif
