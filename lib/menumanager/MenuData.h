#ifndef _sampleMenu_
#define _sampleMenu_
#include "MenuManager.h"

/*

Generated using LCD Menu Builder at
https://lcd-menu-bulder.cohesivecomputing.co.uk/ For more information, visit
https://www.cohesivecomputing.co.uk/hackatronics/arduino-lcd-menu-library/

All our hackatronics projects are free for personal use. If you find them
helpful or useful, please consider making a small donation to our hackatronics
fund using the donate buttons on our web pages. Thank you.

*/

enum sampleMenuCommandId {
  mnuCmdBack = 0,
  mnuCmdHome,
  mnuCmdManual,
  mnuCmdModeSelect,
  mnuCmdSummer,
  mnuCmdWinter,
  mnuCmdExam,
  mnuCmdOFF,
  mnuCmdProgram,
  mnuCmdSetDateTime,
  mnuCmdProgSched,
  mnuCmdSelectSchedule,
  mnuCmdTotalBell,
  mnuCmdSummerWorkDays,
  mnuCmdSumMon,
  mnuCmdSumTue,
  mnuCmdSumWed,
  mnuCmdSumThu,
  mnuCmdSumFri,
  mnuCmdSumSat,
  mnuCmdSumSun,
  mnuCmdWinterWorkDays,
  mnuCmdWinMon,
  mnuCmdWinTue,
  mnuCmdWinWed,
  mnuCmdWinThu,
  mnuCmdWinFri,
  mnuCmdWinSat,
  mnuCmdWinSun,
  mnuCmdExams,
  mnuCmdExMon,
  mnuCmdExTue,
  mnuCmdExWed,
  mnuCmdExThu,
  mnuCmdExFri,
  mnuCmdExSat,
  mnuCmdExSun,
  mnuCmdProgMonth,
  mnuCmdOff,
  mnuCmdLastDay,
  mnuCmdSecondSat,
  mnuCmdDate,
  mnuCmdCalendarHoliday,
  mnuCmdJan,
  mnuCmdFeb,
  mnuCmdMarch,
  mnuCmdApr,
  mnuCmdMay,
  mnuCmdJun,
  mnuCmdJul,
  mnuCmdAug,
  mnuCmdSep,
  mnuCmdOct,
  mnuCmdNov,
  mnuCmdDec
};

const char sampleMenu_back[] = "Back";
const char sampleMenu_exit[] = "Exit";

const char sampleMenu_4_2_1[] = "4.2.1 Select Schedule";
const char sampleMenu_4_2_2[] = "4.2.2 TotalBells";
const MenuItem sampleMenu_List_4_2[] = {
    {mnuCmdSelectSchedule, sampleMenu_4_2_1},
    {mnuCmdTotalBell, sampleMenu_4_2_2},
    {mnuCmdBack, sampleMenu_back}};

const char sampleMenu_4_3_1[] = " Monday";
const char sampleMenu_4_3_2[] = " Tuesday";
const char sampleMenu_4_3_3[] = " Wednesday";
const char sampleMenu_4_3_4[] = " Thursday";
const char sampleMenu_4_3_5[] = " Friday";
const char sampleMenu_4_3_6[] = " Saturday";
const char sampleMenu_4_3_7[] = " Sunday";
const MenuItem sampleMenu_List_4_3[] = {
    {mnuCmdSumMon, sampleMenu_4_3_1}, {mnuCmdSumTue, sampleMenu_4_3_2},
    {mnuCmdSumWed, sampleMenu_4_3_3}, {mnuCmdSumThu, sampleMenu_4_3_4},
    {mnuCmdSumFri, sampleMenu_4_3_5}, {mnuCmdSumSat, sampleMenu_4_3_6},
    {mnuCmdSumSun, sampleMenu_4_3_7}, {mnuCmdBack, sampleMenu_back}};

const char sampleMenu_4_4_1[] = " Monday";
const char sampleMenu_4_4_2[] = " Tuesday";
const char sampleMenu_4_4_3[] = " Wednesday";
const char sampleMenu_4_4_4[] = " Thursday";
const char sampleMenu_4_4_5[] = " Friday";
const char sampleMenu_4_4_6[] = " Saturday";
const char sampleMenu_4_4_7[] = " Sunday";
const MenuItem sampleMenu_List_4_4[] = {
    {mnuCmdWinMon, sampleMenu_4_4_1}, {mnuCmdWinTue, sampleMenu_4_4_2},
    {mnuCmdWinWed, sampleMenu_4_4_3}, {mnuCmdWinThu, sampleMenu_4_4_4},
    {mnuCmdWinFri, sampleMenu_4_4_5}, {mnuCmdWinSat, sampleMenu_4_4_6},
    {mnuCmdWinSun, sampleMenu_4_4_7}, {mnuCmdBack, sampleMenu_back}};

const char sampleMenu_4_5_1[] = " Monday";
const char sampleMenu_4_5_2[] = " Tuesday";
const char sampleMenu_4_5_3[] = " Wednesday";
const char sampleMenu_4_5_4[] = " Thursday";
const char sampleMenu_4_5_5[] = " Friday";
const char sampleMenu_4_5_6[] = " Saturday";
const char sampleMenu_4_5_7[] = " Sunday";
const MenuItem sampleMenu_List_4_5[] = {
    {mnuCmdExMon, sampleMenu_4_5_1}, {mnuCmdExTue, sampleMenu_4_5_2},
    {mnuCmdExWed, sampleMenu_4_5_3}, {mnuCmdExThu, sampleMenu_4_5_4},
    {mnuCmdExFri, sampleMenu_4_5_5}, {mnuCmdExSat, sampleMenu_4_5_6},
    {mnuCmdExSun, sampleMenu_4_5_7}, {mnuCmdBack, sampleMenu_back}};

const char sampleMenu_4_6_1[] = " Off";
const char sampleMenu_4_6_2[] = " Last Day of Month";
const char sampleMenu_4_6_3[] = " Second Saturday";
const char sampleMenu_4_6_4[] = " Select Date";
const MenuItem sampleMenu_List_4_6[] = {{mnuCmdOff, sampleMenu_4_6_1},
                                        {mnuCmdLastDay, sampleMenu_4_6_2},
                                        {mnuCmdSecondSat, sampleMenu_4_6_3},
                                        {mnuCmdDate, sampleMenu_4_6_4},
                                        {mnuCmdBack, sampleMenu_back}};

const char sampleMenu_4_7_1[] = " Jan";
const char sampleMenu_4_7_2[] = " Feb";
const char sampleMenu_4_7_3[] = " March";
const char sampleMenu_4_7_4[] = " Apr";
const char sampleMenu_4_7_5[] = " May";
const char sampleMenu_4_7_6[] = " Jun";
const char sampleMenu_4_7_7[] = " Jul";
const char sampleMenu_4_7_8[] = " Aug";
const char sampleMenu_4_7_9[] = " Sep";
const char sampleMenu_4_7_10[] = " Oct";
const char sampleMenu_4_7_11[] = " Nov";
const char sampleMenu_4_7_12[] = " Dec";
const MenuItem sampleMenu_List_4_7[] = {
    {mnuCmdJan, sampleMenu_4_7_1},   {mnuCmdFeb, sampleMenu_4_7_2},
    {mnuCmdMarch, sampleMenu_4_7_3}, {mnuCmdApr, sampleMenu_4_7_4},
    {mnuCmdMay, sampleMenu_4_7_5},   {mnuCmdJun, sampleMenu_4_7_6},
    {mnuCmdJul, sampleMenu_4_7_7},   {mnuCmdAug, sampleMenu_4_7_8},
    {mnuCmdSep, sampleMenu_4_7_9},   {mnuCmdOct, sampleMenu_4_7_10},
    {mnuCmdNov, sampleMenu_4_7_11},  {mnuCmdDec, sampleMenu_4_7_12},
    {mnuCmdBack, sampleMenu_back}};

const char sampleMenu_3_1[] = " Summer";
const char sampleMenu_3_2[] = " Winter";
const char sampleMenu_3_3[] = " Exam";
const char sampleMenu_3_4[] = " NA";
const MenuItem sampleMenu_List_3[] = {{mnuCmdSummer, sampleMenu_3_1},
                                      {mnuCmdWinter, sampleMenu_3_2},
                                      {mnuCmdExam, sampleMenu_3_3},
                                      {mnuCmdOFF, sampleMenu_3_4}};

const char sampleMenu_4_1[] = " SET DATE TIME";
const char sampleMenu_4_2[] = " PROGRAM SCHEDULE";
const char sampleMenu_4_3[] = " SUMMER WORK DAYS";
const char sampleMenu_4_4[] = " WINTER WORK DAYS";
const char sampleMenu_4_5[] = " EXAM DAYS";
const char sampleMenu_4_6[] = " PROGRAM MONTHLY";
const char sampleMenu_4_7[] = " CALENDAR HOLIDAY";
const MenuItem sampleMenu_List_4[] = {
    {mnuCmdSetDateTime, sampleMenu_4_1},
    {mnuCmdProgSched, sampleMenu_4_2, sampleMenu_List_4_2,
     menuCount(sampleMenu_List_4_2)},
    {mnuCmdSummerWorkDays, sampleMenu_4_3, sampleMenu_List_4_3,
     menuCount(sampleMenu_List_4_3)},
    {mnuCmdWinterWorkDays, sampleMenu_4_4, sampleMenu_List_4_4,
     menuCount(sampleMenu_List_4_4)},
    {mnuCmdExams, sampleMenu_4_5, sampleMenu_List_4_5,
     menuCount(sampleMenu_List_4_5)},
    {mnuCmdProgMonth, sampleMenu_4_6, sampleMenu_List_4_6,
     menuCount(sampleMenu_List_4_6)},
    {mnuCmdCalendarHoliday, sampleMenu_4_7, sampleMenu_List_4_7,
     menuCount(sampleMenu_List_4_7)},
};

const char sampleMenu_1[] = " HOME";
const char sampleMenu_2[] = " MANUAL";
const char sampleMenu_3[] = " MODE SELECTION";
const char sampleMenu_4[] = " PROGRAMMINGMODE";
const MenuItem sampleMenu_Root[] = {
    {mnuCmdHome, sampleMenu_1},
    {mnuCmdManual, sampleMenu_2},
    {mnuCmdModeSelect, sampleMenu_3, sampleMenu_List_3,
     menuCount(sampleMenu_List_3)},
    {mnuCmdProgram, sampleMenu_4, sampleMenu_List_4,
     menuCount(sampleMenu_List_4)}};

/*
case mnuCmdHome :
        break;
case mnuCmdManual :
        break;
case mnuCmdSummer :
        break;
case mnuCmdWinter :
        break;
case mnuCmdExam :
        break;
case mnuCmdOFF :
        break;
case mnuCmdSetDateTime :
        break;
case mnuCmdSelectSchedule :
        break;
case mnuCmdTotalBell :
        break;
case mnuCmdSumMon :
        break;
case mnuCmdSumTue :
        break;
case mnuCmdSumWed :
        break;
case mnuCmdSumThu :
        break;
case mnuCmdSumFri :
        break;
case mnuCmdSumSat :
        break;
case mnuCmdSumSun :
        break;
case mnuCmdWinMon :
        break;
case mnuCmdWinTue :
        break;
case mnuCmdWinWed :
        break;
case mnuCmdWinThu :
        break;
case mnuCmdWinFri :
        break;
case mnuCmdWinSat :
        break;
case mnuCmdWinSun :
        break;
case mnuCmdExMon :
        break;
case mnuCmdExTue :
        break;
case mnuCmdExWed :
        break;
case mnuCmdExThu :
        break;
case mnuCmdExFri :
        break;
case mnuCmdExSat :
        break;
case mnuCmdExSun :
        break;
case mnuCmdOff :
        break;
case mnuCmdLastDay :
        break;
case mnuCmdSecondSat :
        break;
case mnuCmdDate :
        break;
case mnuCmdJan :
        break;
case mnuCmdFeb :
        break;
case mnuCmdMarch :
        break;
case mnuCmdApr :
        break;
case mnuCmdMay :
        break;
case mnuCmdJun :
        break;
case mnuCmdJul :
        break;
case mnuCmdAug :
        break;
case mnuCmdSep :
        break;
case mnuCmdOct :
        break;
case mnuCmdNov :
        break;
case mnuCmdDec :
        break;
*/

/*
<?xml version="1.0"?><RootMenu
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xmlns:xsd="http://www.w3.org/2001/XMLSchema"><Config IdPrefix="mnuCmd"
VarPrefix="sampleMenu" UseNumbering="true" IncludeNumberHierarchy="true"
          MaxNameLen="25" MenuBackFirstItem="false" BackText="Back"
ExitText="Exit" AvrProgMem="false" /><MenuItems><Item Id="Home"
Name="Home"/><Item Id="Manual" Name="Manual"/><Item Id="ModeSelect" Name="Mode
Selection"><MenuItems><Item Id="Summer" Name="Summer"></Item><Item Id="Winter"
Name="Winter"></Item><Item Id="Exam" Name="Exam"></Item><Item Id="OFF"
Name="NA"></Item></MenuItems></Item><Item Id="Program"
Name="ProgrammingMode"><MenuItems><Item Id="SetDateTime" Name="Set Date
Time"/><Item Id="ProgSched" Name="Program Schedule"><MenuItems><Item
Id="SelectSchedule" Name="Select Schedule"/><Item Id="TotalBell"
Name="TotalBells"/></MenuItems></Item><Item Id="SummerWorkDays" Name="Summer
Work Days"><MenuItems><Item Id="SumMon" Name="Monday"/><Item Id="SumTue"
Name="Tuesday"/><Item Id="SumWed" Name="Wednesday"/><Item Id="SumThu"
Name="Thursday"/><Item Id="SumFri" Name="Friday"/><Item Id="SumSat"
Name="Saturday"/><Item Id="SumSun" Name="Sunday"/></MenuItems></Item><Item
Id="WinterWorkDays" Name="Winter Work Days"><MenuItems><Item Id="WinMon"
Name="Monday"/><Item Id="WinTue" Name="Tuesday"/><Item Id="WinWed"
Name="Wednesday"/><Item Id="WinThu" Name="Thursday"/><Item Id="WinFri"
Name="Friday"/><Item Id="WinSat" Name="Saturday"/><Item Id="WinSun"
Name="Sunday"/></MenuItems></Item><Item Id="Exams" Name="Exam
Days"><MenuItems><Item Id="ExMon" Name="Monday"/><Item Id="ExTue"
Name="Tuesday"/><Item Id="ExWed" Name="Wednesday"/><Item Id="ExThu"
Name="Thursday"/><Item Id="ExFri" Name="Friday"/><Item Id="ExSat"
Name="Saturday"/><Item Id="ExSun" Name="Sunday"/></MenuItems></Item><Item
Id="ProgMonth" Name="Program Monthly"><MenuItems><Item Id="Off"
Name="Off"/><Item Id="LastDay" Name="Last Day of Month"/><Item Id="SecondSat"
Name="Second Saturday"/><Item Id="Date" Name="Select
Date"/></MenuItems></Item><Item Id="CalendarHoliday" Name="Calendar
Holiday"><MenuItems><Item Id="Jan" Name="Jan"/><Item Id="Feb" Name="Feb"/><Item
Id="March" Name="March"/><Item Id="Apr" Name="Apr"/><Item Id="May"
Name="May"/><Item Id="Jun" Name="Jun"/><Item Id="Jul" Name="Jul"/><Item Id="Aug"
Name="Aug"/><Item Id="Sep" Name="Sep"/><Item Id="Oct" Name="Oct"/><Item Id="Nov"
Name="Nov"/><Item Id="Dec"
Name="Dec"/></MenuItems></Item></MenuItems></Item></MenuItems></RootMenu>
*/
#endif
