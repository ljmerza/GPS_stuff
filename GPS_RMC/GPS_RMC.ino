/*************************************************
Created by: Leonardo Merza
GPS GPRMC Data Extraction
Verison 0.9
*************************************************/

/*************************************************
Variables
*************************************************/
// rmc verification variables
char GPSchar;
char lineCheck[3] = {'R','M','C'};
boolean rmcDataFound = false;
// time variables
int hour;
int minute;
int second;
// gps coordinates
char latitude[10];
char longitude[10];
int latitudeInt = 0;
int longitudeInt = 0;
char latDirection;
char longDirection;
float convertedLat = 0;
float convertedLong = 0;
// date variables
char date[6];

/*************************************************
Starts serial0 for comm with laptop and Serial1
for comm with 
*************************************************/
void setup()
{
  Serial.begin(57600);
  Serial1.begin(57600);
} // void setup()

void loop()
{
  if(Serial1.available() > 60)
  {
    if(getGPRMC())
    {
      getTime();
      GPSchar = Serial1.read(); // active signal?
      GPSchar = Serial1.read(); // comma
      getCoordinates();
      convertCoordinates();
      getDate();
    } // if(getGPRMC())
  } // if(Serial1.available() > 60)
  
  if(rmcDataFound)
  {
    printData();
  } // if(rmcDataFound)
  
} // void loop()

void convertCoordinates()
{
  unsigned long degtemp = 0;
  float numtemp;
  char tempConvert[6];
    
  if(latitudeInt == 8)
  {
    degtemp = (latitude[0]-'0')*10;
    degtemp += (latitude[1]-'0');
    
    for(int i=0;i<7;i++)
    {
      tempConvert[i] = latitude[i+2];
    } // for(int i=0;i<7;i++)
    numtemp = atol(tempConvert);
    numtemp /= 600000;
    numtemp += degtemp;
    convertedLat = numtemp;
  } // if(latitudeInt == 8)
  else
  {
    degtemp = (latitude[0]-'0')*100;
    degtemp += (latitude[1]-'0')*10;
    degtemp += (latitude[2]-'0');
      
    for(int i=0;i<8;i++)
    {
      tempConvert[i] = latitude[i+3];
    } // for(int i=0;i<8;i++)
    numtemp = atol(tempConvert);
    numtemp /= 600000;
    numtemp += degtemp;
    convertedLat = numtemp;
  } // else
    
  if(latDirection == 'S')
  {
    convertedLat = -convertedLat;
  } // if(latDirection == 'S')    
   
  degtemp = 0;
  if(longitudeInt == 8)
  {
    degtemp = (longitude[0]-'0')*10;
    degtemp += (longitude[1]-'0');
   
    for(int i=0;i<7;i++)
    {
      tempConvert[i] = latitude[i+2];
    } // for(int i=0;i<7;i++)
    numtemp = atol(tempConvert);
    numtemp /= 600000;
    numtemp += degtemp;
    convertedLong = numtemp;
  } // if(longitudeInt == 8)
  else
  {
    degtemp = (longitude[0]-'0')*100;
    degtemp += (longitude[1]-'0')*10;
    degtemp += (longitude[2]-'0');
      
    for(int i=0;i<8;i++)
    {
      tempConvert[i] = longitude[i+3];
    } // for(int i=0;i<8;i++)
    numtemp = atol(tempConvert);
    numtemp /= 600000;
    numtemp += degtemp;
    convertedLong = numtemp;
  } // else 
    
  if(longDirection == 'W')
  {
    convertedLong = -convertedLong;
  } // if(longDirection == 'S')
  
} // void convertCoordinates()

void printData()
{
    rmcDataFound = false;
    Serial.print("The time is: ");
    if(hour < 10)
    {
      Serial.print("0");
    } // if(hour < 10)
    Serial.print(hour);
    Serial.print(":");
    if(minute < 10)
    {
      Serial.print("0");
    } // if(minute < 10)
    Serial.print(minute);
    Serial.print(":");
    if(second < 10)
    {
      Serial.print("0");
    } // if(second < 10)
    Serial.println(second);
    
    Serial.print("The date is (DDMMYY): ");
    for(int i=0;i<6;i++)
    {
      Serial.print(date[i]);
    } // for(int i=0;i<6;i++)
    Serial.println();
 
    Serial.print("The GPS coordinates are: ");
    Serial.print(convertedLat,4);
    Serial.print(",");
    Serial.println(convertedLong,4);  
    Serial.println(); 
}

void getTime()
{
  GPSchar = Serial1.read(); // get rid of comma
  hour = Serial1.read() - '0';
  hour *= 10;
  hour += Serial1.read()  - '0';
  hour -= 5;
  if(hour < 0)
  {
    hour = -1*hour;
    hour = 24 - hour;
  } // if(hour < 0)
         
  minute = Serial1.read()  - '0';
  minute *= 10;
  minute += Serial1.read()  - '0';
     
  second = Serial1.read()  - '0';
  second *= 10;
  second += Serial1.read()  - '0';
  
  GPSchar = Serial1.read(); // period
  GPSchar = Serial1.read(); // three digits for milliseconds
  GPSchar = Serial1.read();
  GPSchar = Serial1.read();
  GPSchar = Serial1.read(); // comma
} // void getTime()

boolean getGPRMC()
{
  latitudeInt = 0;
  longitudeInt = 0;
  

  GPSchar = Serial1.read();
  if(GPSchar == lineCheck[0])
  {
    GPSchar = Serial1.read();
    if(GPSchar == lineCheck[1])
    {
      GPSchar = Serial1.read();
      if(GPSchar == lineCheck[2])
      {
        rmcDataFound = true;
      } // if(GPSchar == lineCheck[2])
    } // if(GPSchar == lineCheck[1])
  } // if(GPSchar == lineCheck[0])
        
  return rmcDataFound;  
} // void getGPRMC()

void getCoordinates()
{
  GPSchar = Serial1.read();
          
  while(GPSchar != ',')
  {
    if(GPSchar != '.')
    {
      latitude[latitudeInt] = GPSchar;
      latitudeInt++;
    } // if(GPSchar != '.')
    //Serial.print(GPSchar);
    GPSchar = Serial1.read();
  } // while(GPSchar != ',')
  
  latDirection = Serial1.read();
  GPSchar = Serial1.read(); // comma
  GPSchar = Serial1.read();
  
  while(GPSchar != ',')
  {
    if(GPSchar != '.')
    {
      longitude[longitudeInt] = GPSchar;
      longitudeInt++;
    } // if(GPSchar != '.') 
    GPSchar = Serial1.read(); 
  } // while(GPSchar != ',')
  
  longDirection = Serial1.read();
}

void getDate()
{
  GPSchar = Serial1.read(); // comma
  GPSchar = Serial1.read();
  GPSchar = Serial1.read();
  GPSchar = Serial1.read();
  GPSchar = Serial1.read();
  GPSchar = Serial1.read(); // comma
  GPSchar = Serial1.read();
  GPSchar = Serial1.read();
  GPSchar = Serial1.read();
  GPSchar = Serial1.read();
  GPSchar = Serial1.read(); // comma
  GPSchar = Serial1.read();
  
  for(int i=0;i<6;i++)
  {
    GPSchar = Serial1.read();
    date[i] = GPSchar;
  } // for(int i=0;i<6;i++)
  
} // void getDate()
