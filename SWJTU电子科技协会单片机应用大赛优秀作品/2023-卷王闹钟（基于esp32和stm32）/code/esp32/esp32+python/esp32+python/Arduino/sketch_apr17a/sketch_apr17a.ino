#include <esp32cam.h>
#include <WebServer.h>
#include <WiFi.h>
#include "Arduino.h"
#include <WiFiUdp.h>
#include <string.h>

const char* WIFI_SSID = "HONOR70";  // 改成自己的wifi名称
const char* WIFI_PASS = "88888888";  // 改成自己的wifi密码

const IPAddress serverIP(192,168,226,42); //欲访问的服务端IP地址192.168.226.42
uint16_t serverPort = 61234;         //服务端口号
WiFiClient client; //声明一个ESP32客户端对象，用于与服务器进行连接

const int Buzzer = 2;
const int LED = 33;

WebServer server(80);

static auto loRes = esp32cam::Resolution::find(320, 240);
static auto hiRes = esp32cam::Resolution::find(800, 600);

/////////////////
void handleRoot(){
  server.send(200, "text/html", "<form action=\"/led\" method=\"POST\"> <input type=\"submit\" value=\"button\"></form>");
  }
void handleled(){
  //控制led状态并返回至首页
  digitalWrite(Buzzer,!digitalRead(Buzzer));
  digitalWrite(LED,!digitalRead(LED));
  server.sendHeader("Location","/");
  server.send(303);
  }

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found");
  }
////////////////
void handleBmp(){
  if (!esp32cam::Camera.changeResolution(loRes)) {
    Serial.println("SET-LO-RES FAIL");
  }

  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()));

  if (!frame->toBmp()) {
    Serial.println("CONVERT FAIL");
    server.send(503, "", "");
    return;
  }
  Serial.printf("CONVERT OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()));

  server.setContentLength(frame->size());
  server.send(200, "image/bmp");
  WiFiClient client = server.client();
  frame->writeTo(client);
  }

void serveJpg(){
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()));

  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
  WiFiClient client = server.client();
  frame->writeTo(client);
  }

void handleJpgLo(){
  if (!esp32cam::Camera.changeResolution(loRes)) {
    Serial.println("SET-LO-RES FAIL");
  }
  serveJpg();
  }

void handleJpgHi()
  {
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
  }

void handleJpg(){
  server.sendHeader("Location", "/cam-hi.jpg");
  server.send(302, "", "");
  }

void handleMjpeg(){
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HI-RES FAIL");
  }

  Serial.println("STREAM BEGIN");
  WiFiClient client = server.client();
  auto startTime = millis();
  int res = esp32cam::Camera.streamMjpeg(client);
  if (res <= 0) {
    Serial.printf("STREAM ERROR %d\n", res);
    return;
  }
  auto duration = millis() - startTime;
  Serial.printf("STREAM END %dfrm %0.2ffps\n", res, 1000.0 * res / duration);
  }

void setup()
{
  pinMode(Buzzer, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(Buzzer, HIGH);
  digitalWrite(LED, HIGH);

  Serial.begin(115200);
  Serial.println();

  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }

  {
    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false); ////////////////关闭STA模式下wifi休眠，提高响应速度
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
      delay(200);
      Serial.print(".");
     }

    Serial.print("http://");
    Serial.println(WiFi.localIP());
    Serial.println("  /cam.bmp");
    Serial.println("  /cam-lo.jpg");
    Serial.println("  /cam-hi.jpg");
    Serial.println("  /cam.mjpeg");

    server.on("/",HTTP_GET, handleRoot);      //服务器首页
    server.on("/led",HTTP_POST,handleled );    //led控制页

    server.on("/cam.bmp", handleBmp);
    server.on("/cam-lo.jpg", handleJpgLo);
    server.on("/cam-hi.jpg", handleJpgHi);
    server.on("/cam.jpg", handleJpg);
    server.on("/cam.mjpeg", handleMjpeg);

    server.onNotFound(handleNotFound);      //404页面

    server.begin();
  }
  
  // Serial.println("尝试访问服务器");
  // if (client.connect(serverIP, serverPort))  //尝试访问目标地址
  // {
  //   Serial.println("访问成功");

  //   client.print("Connected");                    //向服务器发送“hello world”
  //   while (client.connected()) //如果处于连接状态
  //   {
  //     if(client.available())
  //     {
  //       Serial.println("查询信息:");
  //       if (client.available()) //如果有数据可读取
  //       {
  //         String line = client.readStringUntil('\n'); //读取数据到换行符
  //         String line_Buzzer1 = "Buzzer_ON";
  //         String line_Buzzer2 = "Buzzer_ON_OK";
  //         Serial.print("收到消息：");
  //         Serial.println(line);

  //         if(line.compareTo(line_Buzzer1) == 0)//如果闭眼
  //         {
  //           digitalWrite(Buzzer, LOW);  //蜂鸣器鸣叫
  //           digitalWrite(LED, LOW); //LED亮
  //           client.write(line_Buzzer2.c_str()); //发送反馈
  //           //c_str()函数返回一个指向正规C字符串的指针, 内容与本string串相同  
  //         }
  //       }
  //     }
  //   }
  //   Serial.println("关闭当前连接");
  //   client.stop(); //关闭客户端
  // }
  // else
  // {
  //     Serial.println("访问失败");
  //     client.stop(); //关闭客户端
  // }
}

void loop()
{
///////////////////
  Serial.println("尝试访问服务器");
  if (client.connect(serverIP, serverPort)) //尝试访问目标地址
  {
    Serial.println("访问成功");
    client.print("This is esp32");                    //向服务器发送数据
    //while (client.connected() || client.available()) //如果已连接或有收到的未读取的数据
    //{
      if (client.available()) //如果有数据可读取
      {
        String line = client.readStringUntil('\n'); //读取数据到换行符
        String line_Buzzer = "Buzzer_ON";
        Serial.print("读取到数据：");
        Serial.println(line);
        
        if(line.compareTo(line_Buzzer) == 0)//如果闭眼
        {
          digitalWrite(Buzzer, LOW);

           if(digitalRead(Buzzer) == 0)
           {
              digitalWrite(LED, LOW);
           }
        }

        client.write(line.c_str()); //将收到的数据回发
      }
    //}
    Serial.println("关闭当前连接");
    client.stop(); //关闭客户端
  }
  else
  {
    Serial.println("访问失败");
    client.stop(); //关闭客户端
  }
////////////////
  server.handleClient();//处理来自客户端的请求
}
