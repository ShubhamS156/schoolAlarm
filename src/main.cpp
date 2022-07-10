#include "DFRobotDFPlayerMini.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <MenuData.h>
#include <MenuManager.h>
#include <RtcDS3231.h>
#include <TTP229.h>
#include <Wire.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Preferences.h>

#define RELEASE 0
#define ONE 1
#define TWO 2
#define THREE 3
#define UP 4
#define FOUR 5
#define FIVE 6
#define SIX 7
#define DOWN 8
#define SEVEN 9
#define EIGHT 10
#define NINE 11
#define MENU 12
#define DELETE 13
#define ZERO 14
#define BACK 15
#define ENT 16
#define TTP229_SDO 25
#define TTP229_SCL 26
#define PROGSCHEDSIZE 24

#define countof(a) (sizeof(a) / sizeof(a[0]))

enum Modes
{
  SUMMER = 0,
  WINTER,
  EXAM,
  UNDEFINED
};

typedef struct
{
  int hour;
  int min;
  int file;
} Bell __packed;

// TODO: store instances of these in eeprom
typedef struct
{
  int countBells = 0;
  Bell *bells; // dynamic array ptr to num of bells
} ProgSched __packed ;

byte verticalLine[8] = {B00100, B00100, B00100, B00100,
                        B00100, B00100, B00100, B00100};

byte char2[8] = {B00000, B00000, B00000, B11100,
                 B00100, B00100, B00100, B00100};

byte char1[8] = {0b00000, 0b00000, 0b00000, 0b00111,
                 0b00100, 0b00100, 0b00100, 0b00100};

byte char3[8] = {0b00100, 0b00100, 0b00100, 0b00111,
                 0b00000, 0b00000, 0b00000, 0b00000};

byte char4[8] = {0b00100, 0b00100, 0b00100, 0b11100,
                 0b00000, 0b00000, 0b00000, 0b00000};

uint8_t arrow[8] = {0x00, 0x04, 0x06, 0x1f,
                    0x06, 0x04, 0x00}; // Send 0,4,6,1F,6,4,0 for the arrow

int currentSelectionCmdId = mnuCmdHome;
int currentMode = UNDEFINED;
int cursorRow = 0;
int currentSchedule = 0;
// we have 24 schedule. 8 for sum,wint,exm
ProgSched schedules[PROGSCHEDSIZE];
ProgSched* currSchedPtr = NULL;

HardwareSerial mySoftwareSerial(2);
DFRobotDFPlayerMini myDFPlayer;
LiquidCrystal_I2C lcd(0x27, 20, 4);
RtcDS3231<TwoWire> rtc(Wire);
TTP229 ttp229;
MenuManager obj(sampleMenu_Root, menuCount(sampleMenu_Root));
RtcDateTime now; // global to get current time.
Preferences preferences;

void createCustomCharacters()
{
  lcd.createChar(0, verticalLine);
  lcd.createChar(1, char1);
  lcd.createChar(2, char2);
  lcd.createChar(3, char3);
  lcd.createChar(4, char4);
  lcd.createChar(5, arrow);
}
/*
 * Prints 4 items including the selected one.
 */
void printSelected()
{
  const MenuItem *curr = obj.getMenuItem();
  if (curr != sampleMenu_Root)
  {
    lcd.clear();
  }
  int counter = 1;
  lcd.setCursor(0, 0);
  if (obj.getCurrentItemIndex() < 4)
  {
    // print top 4 items
    for (int i = 0; i < 4; i++)
    {
      lcd.print(curr[i].name);
      Serial.println(curr[i].name);
      lcd.setCursor(0, counter++);
    }
    lcd.setCursor(0, obj.getCurrentItemIndex());
    Serial.printf("Setting cursor to %d\n", obj.getCurrentItemIndex());
  }
  else
  {
    // print the selected and above 3
    for (int i = obj.getCurrentItemIndex() - 3; i <= obj.getCurrentItemIndex();
         i++)
    {
      lcd.print(curr[i].name);
      lcd.setCursor(0, counter++);
    }
    // lcd.setCursor(0, (obj.getCurrentItemIndex() % 4));
    lcd.setCursor(0, 3);
    // Serial.printf("Setting cursor to %d\n", (obj.getCurrentItemIndex()-1));
  }
  lcd.write(5);
}

void printFrame()
{
  lcd.setCursor(1, 0);
  lcd.print("------------------");
  lcd.setCursor(1, 3);
  lcd.print("------------------");
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  lcd.setCursor(0, 2);
  lcd.write(byte(0));
  lcd.setCursor(19, 1);
  lcd.write(byte(0));
  lcd.setCursor(19, 2);
  lcd.write(byte(0));
  lcd.setCursor(0, 0);
  lcd.write(byte(1));
  lcd.setCursor(19, 0);
  lcd.write(byte(2));
  lcd.setCursor(0, 3);
  lcd.write(byte(3));
  lcd.setCursor(19, 3);
  lcd.write(byte(4));
  lcd.setCursor(2, 1);
}
void printTime(RtcDateTime &tm)
{
  lcd.setCursor(2, 1);
  lcd.print(tm.Month());
  lcd.print("/");
  lcd.print(tm.Day());
  lcd.print("/");
  lcd.print((tm.Year()));

  String seconds, minutes;
  lcd.setCursor(2, 2);
  lcd.print(tm.Hour());
  lcd.print(":");
  if (tm.Minute() < 10)
  {
    minutes = "0" + String(tm.Minute());
    lcd.print(minutes);
  }
  else
  {
    lcd.print(tm.Minute());
  }
  lcd.print(":");
  if (tm.Second() < 10)
  {
    seconds = "0" + String(tm.Second());
    lcd.print(seconds);
  }
  else
  {
    lcd.print(tm.Second());
  }
}

/*draw homescreen*/
void drawHome(RtcDateTime &dt)
{
  lcd.blink_off();
  lcd.noCursor();
  printTime(dt);
  lcd.setCursor(13, 1);
  lcd.print("Mode");
  lcd.setCursor(14, 2);
  if (currentMode == SUMMER)
  {
    lcd.print("Sum");
  }
  else if (currentMode == WINTER)
  {
    lcd.print("Win");
  }
  else if (currentMode == EXAM)
  {
    lcd.print("Exm");
  }
  else if (currentMode == UNDEFINED)
  {
    lcd.print("N/a");
  }
  // lcd.setCursor(2, 1);
  // char datestring[20];
  // snprintf_P(datestring, countof(datestring),
  //            PSTR("%02u/%02u/%04u %02u:%02u:%02u"), dt.Month(), dt.Day(),
  //            dt.Year(), dt.Hour(), dt.Minute(), dt.Second());
  // Serial.print(datestring);
  // lcd.print(datestring);
  delay(1000); // delay here or in caller?
}

void keyChange()
{
  // A key press changed
  ttp229.keyChange = true;
}

void gotoRoot()
{
  currentSelectionCmdId = -1; // root screen
  obj.reset();
  lcd.clear();
  printSelected();
}

void parseKeys(int buffer[], int counter, int actionKey)
{
  switch (actionKey)
  {
  case ONE:
    buffer[counter] = 1;
    break;
  case TWO:
    buffer[counter] = 2;
    break;
  case THREE:
    buffer[counter] = 3;
    break;
  case FOUR:
    buffer[counter] = 4;
    break;
  case FIVE:
    buffer[counter] = 5;
    break;
  case SIX:
    buffer[counter] = 6;
    break;
  case SEVEN:
    buffer[counter] = 7;
    break;
  case EIGHT:
    buffer[counter] = 8;
    break;
  case NINE:
    buffer[counter] = 9;
    break;
  case ZERO:
    buffer[counter] = 0;
    break;
  default:
    buffer[counter] = -1;
  }
}

void storeScheduleEprom(int schedCounter){
  String key = "p"+String(schedCounter+1);
  preferences.putBytes(key.c_str(),(void*)(&schedules[schedCounter]),sizeof(ProgSched));
  Serial.printf("stored schedule=%d\n",schedCounter);
  
}
// get time from user, set it into rtc.
void handleSetDateTime()
{
  int h = 0, m = 0;
  int timeArr[4] = {0, 0, 0, 0};
  int actionKey = -1;
  int keyPressed;
  String msg = "00:00";
  lcd.clear();
  lcd.print(msg);
  lcd.setCursor(0, 0);
  bool exit = false;
  int counter = 0;
  while (!exit)
  {
    if (ttp229.keyChange)
    {
      keyPressed = ttp229.GetKey16();
      if (keyPressed != 0)
      {
        actionKey = keyPressed;
      }
      switch (keyPressed)
      {
      case RELEASE:
        if (actionKey == ENT)
        {
          h = 10 * timeArr[0] + timeArr[1];
          m = 10 * timeArr[2] + timeArr[3];
          now = rtc.GetDateTime();
          RtcDateTime toSet(now.Year(), now.Month(), now.Day(), h, m, 0);
          rtc.SetDateTime(toSet);
          exit = true;
        }
        else if (actionKey == DELETE)
        {
          counter = 0;
          lcd.clear();
          lcd.print(msg);
          lcd.setCursor(0, 0);
        }
        else if (actionKey != -1 && actionKey != UP && actionKey != DOWN && actionKey != MENU && actionKey != BACK)
        {
          if (counter < 4)
          {
            Serial.println("actionKey=" + actionKey);
            parseKeys(timeArr, counter, actionKey);
            lcd.print(String(timeArr[counter]));
            if (counter == 1)
            {
              lcd.print(":");
            }
            counter++;
          }
        }
        actionKey = -1;
        break;
      }
    }
    else
    {
      Serial.println("no key");
      delay(100);
    }
  }
}

void handleProgSched()
{
  // number of schedules fixed to 24.
  Serial.println("Starting handleProgSched");
  lcd.clear();
  lcd.setCursor(0, 0);
  String base = "P-";
  String counterStr = "";
  int schedCounter = 0;
  lcd.print(base);
  int actionKey;
  bool exit = false;
  while (!exit)
  {
    if (ttp229.keyChange)
    {
      int keyPressed = ttp229.GetKey16();
      lcd.setCursor(2, 0);
      if (keyPressed != 0)
      {
        actionKey = keyPressed;
      }
      switch (keyPressed)
      {
      case RELEASE:
        if (actionKey == UP)
        {
          --schedCounter;
          if (schedCounter < 1)
          {
            schedCounter = 1;
          }
          counterStr = String(schedCounter);
          lcd.print(counterStr);
          actionKey = 0;
        }
        else if (actionKey == DOWN)
        {
          ++schedCounter;
          if (schedCounter > PROGSCHEDSIZE)
          {
            schedCounter = PROGSCHEDSIZE;
          }
          counterStr = String(schedCounter);
          lcd.print(counterStr);
          actionKey = 0;
        }
        else if (actionKey == ENT)
        {
          Serial.printf("Program=%d\n", schedCounter + 1);
          currentSchedule = schedCounter;
          // get the number of bells for the selected schedule.
          bool bellCountDone = false;
          int i = 0;
          int bellPressed = -1;
          int bellKey = -1;
          int bellBuff[2]; // need not more than 99 bells?
          lcd.clear();
          lcd.print("Bells=");
          while (!bellCountDone)
          {
            if (ttp229.keyChange)
            {
              bellPressed = ttp229.GetKey16();
              if (bellPressed != 0)
              {
                bellKey = bellPressed;
                Serial.printf("BellKey=%d\n", bellKey);
              }
              switch (bellPressed)
              {
              case RELEASE:
                if (bellKey != -1)
                {
                  parseKeys(bellBuff, i, bellKey);
                  lcd.print(String(bellBuff[i]));
                  i++;
                  if (i == 2)
                  {
                    bellCountDone = true;
                  }
                }
                bellKey = -1;
                break;
              }
            }
            else
            {
              Serial.println("no key");
              delay(10); // test for this
            }
          }
          schedules[schedCounter].countBells = bellBuff[0] * 10 + bellBuff[1] * 1;
          Serial.printf("set bells=%d for schedule=%d \n", schedules[schedCounter].countBells, schedCounter + 1);
          // alloc memory for number of bells given.
          schedules[schedCounter].bells = (Bell *)(malloc(sizeof(Bell) * schedules[schedCounter].countBells));
          Serial.println("memory allocated");
          int setBellCounter = 0;
          int bellTimePressed = -1;
          int bellTimeKey = -1;
          int bellTimeBuff[4]; // hhmm
          while (setBellCounter < schedules[schedCounter].countBells)
          {
            lcd.clear();
            lcd.print("Bell=");
            lcd.print(String(setBellCounter + 1));
            Serial.printf("Processing Bell=%d\n", setBellCounter + 1);
            lcd.setCursor(1, 1); // 2nd row to display time to be entered.
            lcd.print("Enter Time: ");
            bool bellCurrentDone = false;
            int i = 0;
            while (!bellCurrentDone)
            {
              if (ttp229.keyChange)
              {
                bellTimePressed = ttp229.GetKey16();
                if (bellTimePressed != 0)
                {
                  bellTimeKey = bellTimePressed;
                }
                if (bellTimePressed == RELEASE)
                {
                  if (bellTimeKey != -1)
                  {
                    parseKeys(bellTimeBuff, i, bellTimeKey);
                    lcd.print(String(bellTimeBuff[i]));
                    if (i == 1)
                    {
                      lcd.print(":");
                    }
                    if (bellTimeKey == ENT && i > 3)
                    {
                      bellCurrentDone = true;
                    }
                    i++;
                  }
                  bellTimeKey = -1;
                }
              }
              else
              {
                delay(10);
              }
            }

            // got time, set in the current bell
            schedules[schedCounter].bells[setBellCounter].hour = bellTimeBuff[0] * 10 + bellTimeBuff[1] * 1;
            schedules[schedCounter].bells[setBellCounter].min = bellTimeBuff[2] * 10 + bellTimeBuff[3] * 1;
            Serial.printf("Time= %d:%d\n", schedules[schedCounter].bells[setBellCounter].hour, schedules[schedCounter].bells[setBellCounter].min);
            lcd.clear();
            lcd.print("FILE=");
            Serial.println("Get file");
            i = 0;
            bool bellFileDone = false;
            int fileCount = myDFPlayer.readFileCounts();
            int bellFileKey = -1;
            int bellFileCounter = 0;
            while (!bellFileDone)
            {
              if (ttp229.keyChange)
              {
                lcd.setCursor(5, 0);
                int bellFilePressed = ttp229.GetKey16();
                if (bellFilePressed != 0)
                {
                  bellFileKey = bellFilePressed;
                }
                if (bellFilePressed == RELEASE)
                {
                  if (bellFileKey == UP)
                  {
                    bellFileCounter--;
                    if (bellFileCounter < 0)
                    {
                      bellFileCounter = 0;
                    }
                    lcd.print(String(bellFileCounter));
                  }
                  else if (bellFileKey == DOWN)
                  {
                    bellFileCounter++;
                    if (bellFileCounter > fileCount)
                    {
                      bellFileCounter = fileCount;
                    }
                    lcd.print(String(bellFileCounter));
                  }
                  else if (bellFileKey == ENT)
                  {
                    schedules[schedCounter].bells[setBellCounter].file = bellFileCounter;
                    Serial.printf("Bell=%d File=%d\n", setBellCounter + 1, bellFileCounter);
                    bellFileDone = true;
                    //store here in eeprom?
                    storeScheduleEprom(schedCounter);
                  }
                  bellFileKey = -1;
                }
              }
              else
              {
                delay(10);
              }
            }
            setBellCounter++;
          }
          lcd.clear();
          lcd.print("P-");
        }
        else if (actionKey == BACK)
        {
          Serial.println("exiting programming mode handler");
          exit = true;
          lcd.clear();
        }
        actionKey = -1;
        break;
      }
    }
  }
}
void handleManualMode()
{
  lcd.clear();
  lcd.blink_off();
  String msg = "FILE-x";
  String counterStr = "";
  int fileCount = myDFPlayer.readFileCounts();
  int counter = 0;
  lcd.setCursor(0, 0);
  lcd.print(msg);
  bool exit = false;
  int actionKey;
  while (!exit)
  {
    if (ttp229.keyChange)
    {
      int keyPressed = ttp229.GetKey16();
      Serial.println(keyPressed);
      lcd.setCursor(5, 0);
      if (keyPressed != 0)
      {
        actionKey = keyPressed;
      }
      switch (keyPressed)
      {
      case RELEASE:
        if (actionKey == UP)
        {
          --counter;
          if (counter < 1)
          {
            counter = 1;
          }
          Serial.printf("counter=%d\n", counter);
          counterStr = String(counter);
          lcd.print(counterStr);
          actionKey = 0;
        }
        else if (actionKey == DOWN)
        {
          ++counter;
          if (counter > fileCount)
          {
            counter = fileCount;
          }
          Serial.printf("counter=%d\n", counter);
          counterStr = String(counter);
          lcd.print(counterStr);
          actionKey = 0;
        }
        else if (actionKey == ENT)
        {
          myDFPlayer.play(counter);
          actionKey = 0;
        }
        break;
      case BACK:
        exit = true;
        obj.ascendToParentMenu();
        lcd.clear();
      default:
        break;
      }
    }
  }
  printSelected();
}

/*task to check for keypress
  it changes a flag which either tells loop() to do nothing
  and task calls logic to handle current selection
  OR
  it tells loop() to run a logic continously. eg. draw homescreen and
  keep updating time.
*/
void keyPressTask(void *pvParameters)
{
  int currId;
  int actionKey = -1;
  while (1)
  {
    if (ttp229.keyChange)
    {
      int keyPressed = ttp229.GetKey16();
      if (keyPressed != 0)
      {
        actionKey = keyPressed;
      }
      switch ((keyPressed))
      {
      case RELEASE:
        switch (actionKey)
        {
        case UP:
          if (currentSelectionCmdId ==
              mnuCmdHome)
          { // nothing happends on pressing up down in selected
            // home
            break;
          }
          if (obj.moveToPreviousItem())
          {
            Serial.println("going up");
            printSelected();
          }
          break;
        case DOWN:
          if (currentSelectionCmdId == mnuCmdHome)
          {
            break;
          }
          if (obj.moveToNextItem())
          {
            Serial.println("going down");
            printSelected();
          }
          break;
        case ENT:
          currId = obj.getCurrentItemCmdId();
          Serial.printf("currId=%d\n", currId);
          currentSelectionCmdId = currId;
          if (obj.currentItemHasChildren())
          {
            lcd.clear();
            obj.descendToChildMenu();
            printSelected();
          }
          if (currId == mnuCmdHome)
          {
            lcd.clear();
          }
          else if (currId == mnuCmdManual)
          {
            // call function to select mp3 file and play it.
            Serial.println("calling handleManualMode");
            handleManualMode();
          }
          else if (currId == mnuCmdSummer)
          { // summer selected in mode
            // selection
            currentMode = SUMMER;
            Serial.printf("mode=%d\n", currentMode);
            gotoRoot();
          }
          else if (currId == mnuCmdWinter)
          { // summer selected in mode
            // selection
            currentMode = WINTER;
            Serial.printf("mode=%d\n", currentMode);
            gotoRoot();
          }
          else if (currId == mnuCmdExam)
          {
            currentMode = EXAM;
            Serial.printf("mode=%d\n", currentMode);
            gotoRoot();
          }
          else if (currId == mnuCmdOFF)
          {
            currentMode = UNDEFINED;
            Serial.printf("mode=%d\n", currentMode);
            gotoRoot();
          }
          else if (currId == mnuCmdSetDateTime)
          {
            Serial.println("Calling handleSetDateTime()");
            handleSetDateTime();
            printSelected();
          }
          else if (currId == mnuCmdProgSched)
          {
            Serial.println("Calling handleProgSched");
            handleProgSched();
            printSelected();
          }
          break;
        case BACK:
          if (currId == mnuCmdHome)
          {
            // kuch ni
          }
          currentSelectionCmdId = -1; // clear selection
          if (obj.currentMenuHasParent())
          {
            obj.ascendToParentMenu();
            lcd.clear();
            lcd.setCursor(0, 0);
            printSelected();
          }
          else
          {
            lcd.clear();
            currentSelectionCmdId = mnuCmdHome; // goto home on back from root
          }
          break;
        case MENU:
          gotoRoot();
          break;
        default:
          break;
        }
        actionKey = -1;
      }
    }
    currId = -1;
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void alarmTask(void *pvParameters){
  // if schedule is activated, start with curr bell and match time , if match move to next bell, reset to belll 0 after last bell. 
  //optimisations: after last bell sleep task for a FIXED time.
  Serial.println("Starting Alarm Task");
  int currBell = 0;
  int currSched = currentSchedule;
  ProgSched* activeSchedPtr  = currSchedPtr;
  while(1){
    Serial.println("Getting Time");
    int h = now.Hour();
    int m = now.Minute();
    if(activeSchedPtr != NULL){
      if(currBell < activeSchedPtr->countBells){
        Serial.println("trying match");
        if(activeSchedPtr->bells[currBell].hour ==h && activeSchedPtr->bells[currBell].min == m){
          Serial.printf("Ringing bell %d\n",currBell);
          myDFPlayer.play(activeSchedPtr->bells[currBell].file);
          currBell++;
        }
        else{
          Serial.println("no match");
        }
      }
      else{
        Serial.println("Invalid Bell");
      }
    }
    else{
      Serial.println("Invalid ptr to scheudle");
    }
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  /*---------------Preferences--------------------*/
  preferences.begin("schedules",false);
  String key = "p"+String(currentSchedule+1);
  void* buf;
  int ret = preferences.getBytes(key.c_str(),buf,sizeof(ProgSched));
  if(ret > 0){
    currSchedPtr = (ProgSched*)buf;
  }
  else{
    Serial.println("Failed to read schedule from eeprom");
  }
  /*-----------------Serial----------------------*/
  mySoftwareSerial.begin(9600, SERIAL_8N1, 16, 17);
  lcd.begin();
  /*---------------software serial and dfplayer init-----------------*/
  if (!myDFPlayer.begin(mySoftwareSerial))
  {
    Serial.println(myDFPlayer.readType(), HEX);
    lcd.setCursor(0,0); 
    lcd.print("DF MODULE ER.-");
    Serial.println(F("Unable to begin:"));
    lcd.setCursor(0,1);
    lcd.print("INSERT SD CARD"); 
    Serial.println(F("1.Please recheck the conCTN"));
    lcd.setCursor(0,2); 
    lcd.print("OR CHECK CONCTN");
    Serial.println(F("2.Please insert the SD card!"));
    while (true)
      ;
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.setTimeOut(500); // Set serial communictaion time out 500ms
  myDFPlayer.volume(26);      // Set volume value (0~30).
  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_AUX);
  /*---------rtc init------------*/
  rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  if (!rtc.IsDateTimeValid())
  {
    if (rtc.LastError() != 0)
    {
      Serial.print("RTC communicatins error= ");
      Serial.println(rtc.LastError());
    }
    else
    {
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
  if (now < compiled)
  {
    Serial.println("rtc is older than compile time. Updating");
    // rtc.SetDateTime(compiled); //NOTE: not updating date time
  }
  else
  {
    Serial.println("rtc time same or newer than compile time");
  }
  rtc.Enable32kHzPin(false);
  rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
  /*-----------lcd init---------*/
  createCustomCharacters(); // creating custom characters
  lcd.blink();
  /*-----------keypad------------*/
  ttp229.begin(TTP229_SCL, TTP229_SDO);
  attachInterrupt(digitalPinToInterrupt(TTP229_SDO), keyChange, RISING);
  /*-------------keyPress Task----------*/
  xTaskCreate(keyPressTask, "keypress", 4096, NULL, 3, NULL);
  xTaskCreate(alarmTask,"alarm",1024,NULL,2,NULL);
}
/*
- updates current time
- will loop logic or will wait for flag
*/
void loop()
{
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
      //    1) the battery on the device is low or even missing and the power
      //    line was disconnected
      Serial.println("RTC lost confidence in the DateTime!");
    }
  }

  now = rtc.GetDateTime(); // getting the current time
  switch (currentSelectionCmdId)
  {
  case mnuCmdHome:
    printFrame();
    drawHome(now);
    break;
  default:
    break;
  }
  delay(500);
}