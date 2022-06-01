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

void printSelected(int start, int end){
  cursorRow = 0;
  if(start<0)
    start = 0;
  const MenuItem* curr = obj.getMenuItem(); //current menu
  for(int i=start; i<=end; i++){
    lcd.print(curr[obj.getCurrentItemIndex()].name);
    lcd.setCursor(cursorRow++,0);
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
  printSelected(obj.getCurrentItemIndex()-3,obj.getCurrentItemIndex());
  Serial.println("halohalo");
}

void loop() {
  // put your main code here, to run repeatedly:
}