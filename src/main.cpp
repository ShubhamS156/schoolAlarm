#include <Arduino.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include<MenuData.h>
#include<MenuManager.h>
#include<NTPClient.h>
#include<TTP229.h>


#define UP 4
#define DOWN 8
#define MENU 12
#define DELETE 13
#define BACK 15
#define ENT 16
#define TTP229_SDO 25
#define TTP229_SCL 26

LiquidCrystal_I2C lcd(0x23,20,4);
TTP229 ttp229;
MenuManager obj(sampleMenu_Root,menuCount(sampleMenu_Root));

int cursorRow = 0;
/*
 * Prints 4 items including the selected one.
 */

void printSelected(){
  const MenuItem* curr = obj.getMenuItem();
  int counter = 1;
  if(obj.getCurrentItemIndex() < 4){
    //print top 4 items
    for(int i=0; i<4; i++){
      lcd.print(curr[i].name);
      Serial.println(curr[i].name);
      lcd.setCursor(0,counter++);
    }
    lcd.setCursor(0,obj.getCurrentItemIndex()+1);
    Serial.printf("Setting cursor to %d\n",obj.getCurrentItemIndex()+1);
  }
  else{
    //print the selected and above 3
    for(int i=obj.getCurrentItemIndex()-3; i<=obj.getCurrentItemIndex();i++){
      lcd.print(curr[i].name);
      lcd.setCursor(0,counter++);
    }
    lcd.setCursor(0,(obj.getCurrentItemIndex()%4)+1);
    Serial.printf("Setting cursor to %d\n",(obj.getCurrentItemIndex()%4)+1);
  }
}

uint8_t getKey(){
  return ttp229.GetKey16();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.begin();
  ttp229.begin(TTP229_SCL,TTP229_SDO);
  printSelected();
  Serial.println("bruh");
}

void loop() {
  // put your main code here, to run repeatedly:
}