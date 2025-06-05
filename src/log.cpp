#include <Arduino.h>
#include <string>

void _debug_log(const char* file, int line, const char* str) 
{
  #if DEBUG == 1
  std::string s = std::string("[") + file + ":" + std::to_string(line) + "] " + str;
  Serial.print(s.c_str());
  #endif
  (void)file, (void)line, (void)str; // this is to silence potential compiler errors.
}
