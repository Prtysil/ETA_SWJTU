#ifndef _UDP_RTC_H_
#define _UDP_RTC_H_

//RTC
#include <WiFi.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <lvgl.h>

// NTP Servers:
static const char ntpServerName[] = "time1.aliyun.com";//阿里云的时间服务器
 
const int timeZone = 8;     // 时区
 
WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

void link_wifi(void);
void udp_int(void);
time_t getNtpTime();
char *num_week(uint8_t dayofweek,int Mode);//计算星期
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

void link_wifi(void)
{
  //联网
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED) {
    static int i = 0;
    i++;
    delay(500);
    Serial.print(".");
    if(i == 20) break;
  }
}

void udp_int(void)
{
  //UDP INT
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.remotePort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(100);
}
/*
@功能:判断星期并赋值
*/
char week1[10],week2[8],week3[2],week4[4];
char *num_week(uint8_t dayofweek,int Mode){
  switch(dayofweek)
  {
    case 1: 
    strcpy(week1,"Sunday");
    strcpy(week2,"周日");
    strcpy(week3,"Su");
    strcpy(week4,"日"); 
      break;
    case 2: 
    strcpy(week1,"Monday");
    strcpy(week2,"周一");
    strcpy(week3,"Mo");
    strcpy(week4,"一"); 
      break;
    case 3: 
    strcpy(week1,"Tuesday");
    strcpy(week2,"周二");
    strcpy(week3,"Tu");
    strcpy(week4,"二"); 
      break;
    case 4: 
    strcpy(week1,"Wednesday");
    strcpy(week2,"周三"); 
    strcpy(week3,"We");
    strcpy(week4,"三"); 
      break;
    case 5: 
    strcpy(week1,"Thursday");
    strcpy(week2,"周四"); 
    strcpy(week3,"Th");
    strcpy(week4,"四"); 
      break;
    case 6: 
    strcpy(week1,"Friday");
    strcpy(week2,"周五");
    strcpy(week3,"Fr"); 
    strcpy(week4,"五");
      break;
    case 7: 
    strcpy(week1,"Saturday");
    strcpy(week2,"周六"); 
    strcpy(week3,"Sa");
    strcpy(week4,"六");
      break;
    default:
    strcpy(week1,"NO");
    strcpy(week2,"无");
    strcpy(week3,"NO");
    strcpy(week4,"无");
      break; 
  }
  switch(Mode)
  {
    case 1: return week1; break;
    case 2: return week2; break;
    case 3: return week3; break;
    case 4: return week4; break;
    default: return " "; break;
  }
}
 
void digitalClockDisplay()
{
  now();
  // digital clock display of the time
  // Serial.print(year());
  // Serial.print("/");
  // Serial.print(month());
  // Serial.print("/");
  // Serial.print(day());
  // Serial.print("   ");
  // Serial.print(hour());yjt
  printDigits(minute());
  printDigits(second());
  // Serial.print("   星期");
  // Serial.print(num_week(weekday(),4));
  // Serial.println();
  char timedate[128]; 
  //sprintf(timedate, "%d",year()); 
  /*Fill the second column*/

  snprintf(date_str, DATE_LABEL_TEXT_LEN, "%04d-%02d-%02d", year(), month(), day());
  lv_label_set_text(date_label, date_str);


  snprintf(time_str, TIME_LABEL_TEXT_LEN, "%02d:%02d:%02d", hour(), minute(), second());
  lv_label_set_text(time_label, time_str);


  // lv_table_set_cell_value(table, 0, 0, timedate);sprintf(timedate, "%d",month());
  // lv_table_set_cell_value(table, 1, 0, timedate);sprintf(timedate, "%d",day());
  // lv_table_set_cell_value(table, 2, 0, timedate);
  // lv_table_set_cell_value(table, 3, 1, num_week(weekday(),1));sprintf(timedate, "%d",hour());
  // lv_table_set_cell_value(table, 4, 1, timedate);sprintf(timedate, "%d",minute());
  // lv_table_set_cell_value(table, 5, 1, timedate);sprintf(timedate, "%d\a",second());
  // lv_table_set_cell_value(table, 6, 1, timedate);
}
 
void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
 
/*-------- NTP code (下面不用看哦)----------*/
 
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
 
time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address
 
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}
 
// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}


#endif