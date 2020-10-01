
/*******************************************************

 year

 *******************************************************/
int year(int tm_year) {
  return tm_year + 1900;
}

/*******************************************************

 month

 *******************************************************/
int month(int tm_month) {
  return tm_month + 1;
}

/*******************************************************

 minute

 *******************************************************/
void minute(int tm_minute, char buffer[]) {

  buffer[0] = 0;

  if (tm_minute < 10) {
    sprintf(buffer, "0%d", tm_minute);
  } else {
    sprintf(buffer, "%d", tm_minute);
  }

}

/*******************************************************

 hour12

 *******************************************************/
int hour12(int tm_hour) { // the hour for the given time in 12 hour format

  if ( tm_hour == 0 ) {
    return 12; // 12 midnight
  } else if ( tm_hour  > 12) {
    return tm_hour - 12 ;
  } else {
    return tm_hour ;
  }
}

/*******************************************************

 day

 *******************************************************/
void day(int dayNum, char buffer[]) {
  buffer[0] = 0;
  switch (dayNum) {
    case 0:
      sprintf(buffer, "Sun");

      break;
    case 1:
      sprintf(buffer, "Mon");

      break;
    case 2:
      sprintf(buffer, "Tue");

      break;
    case 3:
      sprintf(buffer, "Wed");

      break;
    case 4:
      sprintf(buffer, "Thu");

      break;
    case 5:
      sprintf(buffer, "Fri");

      break;
    case 6:
      sprintf(buffer, "Sat");

      break;
    default:
      sprintf(buffer, "-");
  }
}

/*******************************************************

 setClockTime

 *******************************************************/
void setClockTime(struct tm  *timeinfo) {

  String displayTime;
  //displayTime += hour12(timeinfo->tm_hour);
  displayTime += (timeinfo->tm_hour);

  currentHour = (timeinfo->tm_hour);

  if(displayTime.length() < 2)
  {
    displayTime = "0" + displayTime;
  }
  
  displayTime += ":";

  char minuteBuffer[9];
  minute(timeinfo->tm_min, minuteBuffer);

  String minuteString = String(minuteBuffer);
  if(minuteString.length() < 2)
  {
    minuteString = "0" + minuteString;
  }
  
  //displayTime += String(minuteBuffer);
  displayTime += minuteString;

  Serial.println(displayTime);
  updateText(displayTime, timeText);

  if(oldHour != currentHour)
  {
    hourChanged = true;
    oldHour = currentHour;
  }

}
/*******************************************************

 setClockDate

 *******************************************************/
void setClockDate(struct tm  *timeinfo) {

  String displayDate, MM, YY, dd;
  
  displayDate += month(timeinfo->tm_mon);
  displayDate += "-";
  displayDate += timeinfo->tm_mday;
  displayDate += "-";
  displayDate += year(timeinfo->tm_year);

  dd = timeinfo->tm_mday;
  dd = (dd.length() < 2)? "0" + dd : dd;
  MM = month(timeinfo->tm_mon);
  MM = (MM.length() < 2)? "0" + MM : MM;
  YY = year(timeinfo->tm_year);
  YY = (YY.length() < 2)? "0" + YY : YY;

  //Serial.println(displayDate);
  //updateText(displayDate, dateText);
  Serial.println(dd + "/" + MM + "/" + YY);
  //updateText(dd + "/" + MM + "/" + YY, dateText);
  updateText(dd + "/" + MM , dateText);

}
/*******************************************************

 setClockDay

 *******************************************************/
void setClockDay(struct tm  *timeinfo) {

  String displayDay;
  char dayBuffer[11];
  day(timeinfo->tm_wday, dayBuffer);
  displayDay += String(dayBuffer);

  Serial.println(displayDay);
  updateText(displayDay, dayText);

}

void updateText(String &newText, struct text &textStruct) {

  if (newText != textStruct.str) {
    textStruct.oldStr = textStruct.str; //save old str so we can clear it from lcd.
    textStruct.str = newText;
    textStruct.updated = true;
  }
}
