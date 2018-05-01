/*  ESP32_SD_WebSocket for ESP-WROOM-32(ESP32) Sample Sketch
for Beta version 1.60
STA mode only
---> https://www.mgo-tec.com/blog-entry-easywebsocket-library-beta151-esp32-esp8266.html
Please rewrite their own the ssid and password.

Use the SD card (SPI), please upload the LIPhead1.txt, LIPhead2.txt, dummy.txt, file to SD/EWS/ folder.
*/
#include <ESP32_SD_EasyWebSocket.h> //beta ver 1.60
  
const char* ssid = "xxxx"; //ご自分のルーターのSSIDに書き換えてください
const char* password = "xxxx"; //ご自分のルーターのパスワードに書き換えてください
  
const char* HTM_head_file1 = "/EWS/LIPhead1.txt"; //HTMLヘッダファイル1
const char* HTM_head_file2 = "/EWS/LIPhead2.txt"; //HTMLヘッダファイル2
const char* HTML_body_file = "/EWS/dummy.txt"; //HTML body要素ファイル（ここではダミーファイルとしておく）
const char* dummy_file = "/EWS/dummy.txt"; //HTMLファイル連結のためのダミーファイル
  
const uint8_t cs_SD = 5; //SDcard CS(ChipSelect)
 
SD_EasyWebSocket ews;
  
IPAddress LIP; //ローカルIPアドレス自動取得用
  
String ret_str; //ブラウザから送られてくる文字列格納用
String txt = "text send?"; //ブラウザから受信した文字列を ESP32から再送信する文字列
  
int PingSendTime = 10000; //ESP32からブラウザへPing送信する間隔(ms)
bool get_http_req_status = false; //ブラウザからGETリクエストがあったかどうかの判定変数
  
long ESP32_send_LastTime;
int ESP32_send_Rate = 300;
byte cnt = 0;
  
#define ledPin1 12 //GPIO #12
#define ledPin2 13 //GPIO #13
#define ledPin3 14 //GPIO #14
  
void setup() 
{
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
      
  ews.AP_Connect(ssid, password);
  delay(1000);
    
  LIP = WiFi.localIP(); //ESP32のローカルIPアドレスを自動取得
  
  Serial.println(); Serial.println("Initializing SD card...");
  
  if (!SD.begin(cs_SD, SPI, 40000000, "/sd")) {
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
  sigmaDeltaWrite(0, 0); // LED OFF
  sigmaDeltaWrite(1, 0); // LED OFF
  sigmaDeltaWrite(2, 0); // LED OFF
    
  ESP32_send_LastTime = millis();
}
  
void loop() {
  websocket_handshake();

  ret_str = ews.EWS_ESP32CharReceive(PingSendTime);

  if(ret_str != "_close"){
    if(millis()-ESP32_send_LastTime > ESP32_send_Rate){
      if(cnt > 3){
        cnt = 0;
      }
      websocket_send(cnt, txt);
      cnt++;
      ESP32_send_LastTime = millis();
    }

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
}
//**************************************************************
void LED_PWM(byte Led_gr, byte channel, int data_i)
{
  Serial.println(data_i);
  switch(Led_gr){
    case 1: //１つのLEDを調光制御
      sigmaDeltaWrite(channel, floor(data_i*2.5));
      break;
    case 2: //３つのLEDを順番に点灯、消灯
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
    case 3: //３つのLED全点灯
      sigmaDeltaWrite(0, 255);
      sigmaDeltaWrite(1, 255);
      sigmaDeltaWrite(2, 255);
      break;
    case 4: //３つのLED全消灯
      sigmaDeltaWrite(0, 0);
      sigmaDeltaWrite(1, 0);
      sigmaDeltaWrite(2, 0);
      break;
  }
}
//*********************************************
void websocket_send(uint8_t count, String txt1){
  String str;

  switch(cnt){
    case 0:
      str = txt1;
      break;
    case 1:
      str = "WebSockets";
      break;
    case 2:
      str = "Hello!!";
      break;
    case 3:
      str = "World!!";
      break;
  }
    
  ews.EWS_ESP32_Str_SEND(str, "wroomTXT"); //ブラウザに文字列を送信
}
//************************* Websocket handshake **************************************
void websocket_handshake(){  
  if(ews.Get_Http_Req_Status()){ //ブラウザからGETリクエストがあったかどうかの判定
    String html_str1="", html_str2="", html_str3="", html_str4="", html_str5="", html_str6="", html_str7="";
 
    //※String変数一つにEWS_Canvas_Slider_T関数は２つまでしか入らない
    html_str1 += "<body style='background:#000; color:#fff;'>\r\n";
    html_str1 += "<font size=3>\r\n";
    html_str1 += "ESP-WROOM-32(ESP32)\r\n";
    html_str1 += "<br>\r\n";
    html_str1 += "SD_EasyWebSocket Beta1.60 Sample\r\n";
    html_str1 += "</font><br>\r\n";
    html_str1 += ews.EWS_BrowserSendRate();
    html_str1 += "<br>\r\n";
    html_str1 += ews.EWS_ESP32_SendRate("!esp32t-Rate");
    html_str1 += "<br>\r\n";
    html_str1 += ews.EWS_BrowserReceiveTextTag2("wroomTXT", "from ESP32 DATA", "#555", 20,"#00FF00");
    html_str1 += "<br>\r\n";
    html_str1 += ews.EWS_Status_Text2("WebSocket Status","#555", 20,"#FF00FF");
    html_str1 += "<br><br>\r\n";

    html_str2 += ews.EWS_TextBox_Send("txt1", "Hello Easy WebSocket Beta1.51","送信");
    html_str2 += "<br><br>\r\n";
    html_str2 += "LED \r\n";
    html_str2 += ews.EWS_On_Momentary_Button("ALL", "ALL-ON", 80,25,15,"#000000","#AAAAAA");
    html_str2 += ews.EWS_On_Momentary_Button("OUT", "ALL-OFF", 80,25,15,"#FFFFFF","#555555");
    html_str2 += "<br>\r\n";

    html_str3 += "<br>LED BLUE... Dim\r\n";
    html_str3 += ews.EWS_Canvas_Slider_T("BLUE",200,40,"#777777","#0000ff"); //CanvasスライダーはString文字列に２つまでしか入らない
    html_str3 += "<br>LED GREEN Dim\r\n";
    html_str3 += ews.EWS_Canvas_Slider_T("GREEN",200,40,"#777777","#00ff00"); //CanvasスライダーはString文字列に２つまでしか入らない

    html_str4 += "<br>LED RED..... Dim\r\n";
    html_str4 += ews.EWS_Canvas_Slider_T("RED",200,40,"#777777","#ff0000"); //CanvasスライダーはString文字列に２つまでしか入らない
    html_str4 += "<br>LED RGB..... Dim\r\n";
    html_str4 += ews.EWS_Canvas_Slider_T("_RGB",200,40,"#777777","#ffff00");

    html_str5 += "<br><br>\r\n";
    html_str5 += ews.EWS_WebSocket_Reconnection_Button2("WS-Reconnect", "grey", 200, 40, "black" , 17);
    html_str5 += "<br><br>\r\n";  
    html_str5 += ews.EWS_Close_Button2("WS CLOSE", "#bbb", 150, 40, "red", 17);
    html_str5 += ews.EWS_Window_ReLoad_Button2("ReLoad", "#bbb", 150, 40, "blue", 17);
    html_str5 += "</body></html>";

    //WebSocket ハンドシェイク関数
    ews.EWS_HandShake_main(3, cs_SD, HTM_head_file1, HTM_head_file2, HTML_body_file, dummy_file, LIP, html_str1, html_str2, html_str3, html_str4, html_str5, html_str6, html_str7);
  }
}
