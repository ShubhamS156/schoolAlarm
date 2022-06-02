#include <Arduino.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include<MenuData.h>
#include<MenuManager.h>
#include<RtcDS3231.h>
#include<TTP229.h>
#include<freertos/task.h>
#include<freertos/FreeRTOS.h>
#include<esp_system.h>


#define UP 4
#define DOWN 8
#define MENU 12
#define DELETE 13
#define BACK 15
#define ENT 16
#define TTP229_SDO 25
#define TTP229_SCL 26

#define countof(a) (sizeof(a) / sizeof(a[0]))

LiquidCrystal_I2C lcd(0x23,20,4);
RtcDS3231<TwoWire> rtc(Wire);
TTP229 ttp229;
MenuManager obj(sampleMenu_Root,menuCount(sampleMenu_Root));

int currentSelectionCmdId = mnuCmdHome;

int cursorRow = 0;
/*
 * Prints 4 items including the selected one.
 */
void printSelected(){
  const MenuItem* curr = obj.getMenuItem();
  int counter = 1;
  lcd.setCursor(0,0);
  if(obj.getCurrentItemIndex() < 4){
    //print top 4 items
    for(int i=0; i<4; i++){
      lcd.print(curr[i].name);
      Serial.println(curr[i].name);
      lcd.setCursor(0,counter++);
    }
    lcd.setCursor(0,obj.getCurrentItemIndex());
    Serial.printf("Setting cursor to %d\n",obj.getCurrentItemIndex());
  }
  else{
    //print the selected and above 3
    for(int i=obj.getCurrentItemIndex()-3; i<=obj.getCurrentItemIndex();i++){
      lcd.print(curr[i].name);
      lcd.setCursor(0,counter++);
    }
    lcd.setCursor(0,(obj.getCurrentItemIndex()%4));
    Serial.printf("Setting cursor to %d\n",(obj.getCurrentItemIndex()%4));
  }
    lcd.cursor();
    delay(300);
    lcd.noCursor();
    delay(300);
    lcd.cursor();
}

/*draw homescreen*/
void drawHome(RtcDateTime& dt){
  lcd.clear();
  lcd.setCursor(0,1);
  char datestring[20];
  snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(datestring);
    delay(1000); //delay here or in caller?
}


void keyChange()
{
    // A key press changed
    ttp229.keyChange = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  /*---------rtc init------------*/
  rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  if(!rtc.IsDateTimeValid()){
    if(rtc.LastError()!=0){
      Serial.print("RTC communicatins error= ");
      Serial.println(rtc.LastError());
    }
    else{
      Serial.println("RTC lost confidence in date and time");
      rtc.SetDateTime(compiled);
    }
  }
    if (!rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        rtc.SetIsRunning(true);
    }
  
  RtcDateTime now = rtc.GetDateTime();
  if(now < compiled){
    Serial.println("rtc is older than compile time. Updating");
    rtc.SetDateTime(compiled);
  }
  else{
    Serial.println("rtc time same or newer than compile time");
  }
  rtc.Enable32kHzPin(false);
  rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
  /*-----------lcd init---------*/
  lcd.begin();
  lcd.blink();
  /*-----------keypad------------*/
  ttp229.begin(TTP229_SCL,TTP229_SDO);
  attachInterrupt(digitalPinToInterrupt(TTP229_SDO), keyChange, FALLING);
//printSelected();
  
}

/*task to check for keypress
  it changes a flag which either tells loop() to do nothing 
  and task calls logic to handle current selection
  OR
  it tells loop() to run a logic continously. eg. draw homescreen and 
  keep updating time.
*/
void keyPressTask(void *pvParameters){
  if(ttp229.keyChange){
    int keyPressed = ttp229.GetKey16();
    Serial.printf("key pressed:%d\n",keyPressed);
    switch ((keyPressed))
    {
    case UP:
      if(obj.moveToPreviousItem()){
      Serial.println("going up");
      printSelected();
      }
      break;
    case DOWN:
      if(obj.moveToNextItem()){
      Serial.println("going down");
      printSelected();
      }
    default:
      break;
    }
  }
}

/*
- updates current time
- will loop logic or will wait for flag
*/
void loop() {
  // put your main code here, to run repeatedly:

    if (!rtc.IsDateTimeValid()) 
    {
        if (rtc.LastError() != 0)
        {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for 
            // what the number means
            Serial.print("RTC communications error = ");
            Serial.println(rtc.LastError());
        }
        else
        {
            // Common Causes:
            //    1) the battery on the device is low or even missing and the power line was disconnected
            Serial.println("RTC lost confidence in the DateTime!");
        }
    }

  RtcDateTime now = rtc.GetDateTime(); //getting the current time 
  switch (currentSelectionCmdId)
  {
  case mnuCmdHome:
    drawHome(now);
    break;
  
  default:
    break;
  }
}