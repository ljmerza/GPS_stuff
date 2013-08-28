void setup() 
{ 
  Serial.begin(57600);
  Serial1.begin(57600);
} // void setup()

void loop()
{  
  // gps read and write info from serial port 1 to serial port 0
  if(Serial1.available() > 20)
  {
    Serial.write(Serial1.read());
  }

} // void loop


