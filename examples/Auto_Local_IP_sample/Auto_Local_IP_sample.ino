/*
ESP32_SD_WebSocket for ESP-WROOM-32(ESP32) Sample Sketch
for Beta version 1.50
---> https://www.mgo-tec.com
Please rewrite their own the ssid and password.

Use the SD card (SPI), please upload the LIPhead1.txt, LIPhead2.txt, dummy.txt, file to SD/EWS/ folder.
*/
#include <ESP32_SD_EasyWebSocket.h>
#include <WiFi.h>
#include <SD.h>

const char* ssid = "xxxx"; //ご自分のルーターのSSID
const char* password = "xxxx"; //ご自分のルーターのパスワード

const char* HTM_head_file1 = "/EWS/LIPhead1.txt"; //HTMLヘッダファイル1
const char* HTM_head_file2 = "/EWS/LIPhead2.txt"; //HTMLヘッダファイル2
const char* HTML_body_file = "/EWS/dummy.txt"; //HTML body要素ファイル（ここではダミーファイルとしておく）
const char* dummy_file = "/EWS/dummy.txt"; //HTMLファイル連結のためのダミーファイル

SD_EasyWebSocket ews;

IPAddress LIP; //ローカルIPアドレス自動取得用

String html_str1 = ""; //ブラウザへ送信するHTML文字列初期化
String html_str2 = "";
String html_str3 = "";
String html_str4 = "";
String html_str5 = "";
String html_str6 = "";
String html_str7 = "";

String ret_str; //ブラウザから送られてくる文字列格納用
String txt = "text??";

int PingSendTime = 10000; //ESP32からブラウザへPing送信する間隔(ms)

long ESP32_send_LastTime;
int ESP32_send_Rate = 300;

byte cnt = 0;

#define ledPin1 12 //GPIO #12
#define ledPin2 13 //GPIO #13
#define ledPin3 14 //GPIO #14

void setup() 
{
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  
  //HMTLは基本的にSDカード内のファイルに記述することをお勧めする
  html_str1 = "<body style='background:#000; color:#fff;'>\r\n";
  html_str1 += "<font size=3>\r\n";
  html_str1 += "ESP-WROOM-32(ESP32)\r\n";
  html_str1 += "<br>\r\n";
  html_str1 += "SD_EasyWebSocket Beta1.48 Sample\r\n";
  html_str1 += "</font><br>\r\n";
  html_str1 += ews.EWS_BrowserSendRate();
  html_str1 += "<br>\r\n";
  html_str1 += ews.EWS_ESP32_SendRate("!esp32t-Rate");
  html_str1 += "<br>\r\n";
  html_str1 += ews.EWS_BrowserReceiveTextTag2("wroomTXT", "from ESP32 DATA", "#555", 20,"green");
  html_str1 += "<br>\r\n";
  html_str1 += ews.EWS_Status_Text2("WebSocket Status","#555", 20,"#FF00FF");
  html_str1 += "<br>\r\n";
  html_str1 += ews.EWS_TextBox_Send("txt1", "ESP32 EasyWebSocket Beta1.48","Submit");
  html_str1 += "<br><br>\r\n";
  html_str1 += "LED \r\n";
  html_str1 += ews.EWS_On_Momentary_Button("ALL", "ALL-ON", 80,25,15,"#000000","#AAAAAA");
  html_str1 += ews.EWS_On_Momentary_Button("OUT", "ALL-OFF", 80,25,15,"#FFFFFF","#555555");
  html_str1 += "<br>\r\n";
  
  html_str2 += "<br>LED BLUE... Dim\r\n";
  html_str2 += ews.EWS_Canvas_Slider_T("BLUE",200,40,"#777777","#0000ff"); //CanvasスライダーはString文字列に２つまでしか入らない
  html_str2 += "<br>LED GREEN Dim\r\n";
  html_str2 += ews.EWS_Canvas_Slider_T("GREEN",200,40,"#777777","#00ff00"); //CanvasスライダーはString文字列に２つまでしか入らない
  
  html_str3 += "<br>LED RED..... Dim\r\n";
  html_str3 += ews.EWS_Canvas_Slider_T("RED",200,40,"#777777","#ff0000"); //CanvasスライダーはString文字列に２つまでしか入らない
  html_str3 += "<br>LED RGB..... Dim\r\n";
  html_str3 += ews.EWS_Canvas_Slider_T("_RGB",200,40,"#777777","#ffff00");
  html_str4 += "<br><br>\r\n";
  html_str4 += ews.EWS_WebSocket_Reconnection_Button2("WS-Reconnect", "grey", 200, 40, "black" , 17);
  html_str4 += "<br><br>\r\n";  
  html_str4 += ews.EWS_Close_Button2("WS CLOSE", "#bbb", 150, 40, "red", 17);
  html_str4 += ews.EWS_Window_ReLoad_Button2("ReLoad", "#bbb", 150, 40, "blue", 17);
  html_str4 += "</body></html>";
  
  ews.AP_Connect(ssid, password);
  delay(1000);
  
  LIP = WiFi.localIP(); //ESP32のローカルIPアドレスを自動取得

  Serial.println(); Serial.println("Initializing SD card...");

  if (!SD.begin(SS, SPI, 40000000, "/sd")) {
    Serial.println("Card failed, or not present");
    return;
  }

  Serial.println("card initialized. OK!");

  sigmaDeltaSetup(0, 312500);
  sigmaDeltaSetup(1, 312500);
  sigmaDeltaSetup(2, 312500);
  sigmaDeltaAttachPin(ledPin1,0);
  sigmaDeltaAttachPin(ledPin2,1);
  sigmaDeltaAttachPin(ledPin3,2);
  sigmaDeltaWrite(0, 0);
  sigmaDeltaWrite(1, 0);
  sigmaDeltaWrite(2, 0);
  
  ESP32_send_LastTime = millis();
}

void loop() {
  ews.EWS_Dev_AutoLIP_HandShake(HTM_head_file1, LIP, HTM_head_file2, HTML_body_file, html_str1, html_str2, html_str3, html_str4, html_str5, html_str6, html_str7, dummy_file);

  String str;

  if(ret_str != "_close"){
    if(millis()-ESP32_send_LastTime > ESP32_send_Rate){
      if(cnt > 3){
        cnt = 0;
      }
      switch(cnt){
        case 0:
          str = txt;
          break;
        case 1:
          str = "ESP32_WebSockets";
          break;
        case 2:
          str = "Hello!!";
          break;
        case 3:
          str = "World!!";
          break;
      }
      ews.EWS_ESP8266_Str_SEND(str, "wroomTXT");
      ESP32_send_LastTime = millis();
      cnt++;
    }

    ret_str = ews.EWS_ESP8266CharReceive(PingSendTime);
    if(ret_str != "\0"){
      Serial.println(ret_str);
      if(ret_str != "Ping"){
        if(ret_str[0] != 't'){
          int ws_data = (ret_str[0]-0x30)*100 + (ret_str[1]-0x30)*10 + (ret_str[2]-0x30);
          switch(ret_str[4]){
            case '!':
              ESP32_send_Rate = ws_data;
              break;
            case 'B':
              LED_PWM(1, 0, floor(ws_data/2));
              break;
            case 'G':
              LED_PWM(1, 1, floor(ws_data/2));
              break;
            case 'R':
              LED_PWM(1, 2, floor(ws_data/2));
              break;
            case '_':
              LED_PWM(2, 0, floor(ws_data/2));
              break;
            case 'A':
              LED_PWM(3, 0, ws_data);
              break;
            case 'O':
              LED_PWM(4, 0, ws_data);
              break;
          }
        }else if(ret_str[0] == 't'){
          txt = ret_str.substring(ret_str.indexOf('|')+1, ret_str.length()-1);
          Serial.println(txt);
        }
      }
    }
  }else if(ret_str == "_close"){
    ESP32_send_LastTime = millis();
    ret_str = "";
  }
	yield(); //これ重要かも
}

//**************************************************************
void LED_PWM(byte Led_gr, byte channel, int data_i)
{
  Serial.println(data_i);
  switch(Led_gr){
    case 1:
      sigmaDeltaWrite(channel, floor(data_i*2.5));
      break;
    case 2:
      if(data_i < 34){
        sigmaDeltaWrite(0, data_i*7);
        sigmaDeltaWrite(1, 0);
        sigmaDeltaWrite(2, 0);
      }else if(data_i > 33 && data_i < 67){
        sigmaDeltaWrite(1, (data_i-33)*7);
        sigmaDeltaWrite(0, 0);
        sigmaDeltaWrite(2, 0);
      }else if(data_i > 66){
        sigmaDeltaWrite(2, (data_i-66)*7);
        sigmaDeltaWrite(0, 0);
        sigmaDeltaWrite(1, 0);
      }
      break;
    case 3:
      sigmaDeltaWrite(0, 255);
      sigmaDeltaWrite(1, 255);
      sigmaDeltaWrite(2, 255);
      break;
    case 4:
      sigmaDeltaWrite(0, 0);
      sigmaDeltaWrite(1, 0);
      sigmaDeltaWrite(2, 0);
      break;
  }
}