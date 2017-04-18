/*
  ESP32_SD_EasyWebSocket.h - WebSocket for ESP-WROOM-32 ( ESP32 & SD microSDHC card use)
  Beta version 1.51

Copyright (c) 2017 Mgo-tec
This library improvement collaborator is Mr.Visyeii.
Part of WiFiServer.h(for ESP8266) library modified.

This library is used by the Arduino IDE(Tested in ver1.8.2).

Reference Blog --> https://www.mgo-tec.com

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Reference LGPL-2.1 license statement --> https://opensource.org/licenses/LGPL-2.1   

WiFi.h - Included WiFi library for esp32
Based on WiFi.h from Arduino WiFi shield library.
Copyright (c) 2011-2014 Arduino.  All right reserved.
Modified by Ivan Grokhotkov, December 2014
Licensed under the LGPL-2.1

WiFiClientSecure.h
Copyright (c) 2011 Adrian McEwen.  All right reserved.
Additions Copyright (C) 2017 Evandro Luis Copercini.
Licensed under the LGPL-2.1

@file sha1.h
@date 20.05.2015
@author Steve Reid <steve@edmweb.com>
SHA-1 in C
By Steve Reid <steve@edmweb.com>
from: http://www.virtualbox.org/svn/vbox/trunk/src/recompiler/tests/sha1.c
100% Public Domain

SD.h - Included SD card library for esp32
Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
Licensed under the Apache License, Version 2.0 (the "License");

Reference Apache License --> http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef _SD_EASYWEBSOCKET_H_INCLUDED
#define _SD_EASYWEBSOCKET_H_INCLUDED

//#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <SD.h>

enum HTTPClientStatus1 { HC_NONE1, HC_WAIT_READ1, HC_WAIT_CLOSE1 };
#define HTTP_MAX_DATA_WAIT 1000 //ms to wait for the client to send the request
#define HTTP_MAX_CLOSE_WAIT 2000 //ms to wait for the client to close the connection
class SD_EasyWebSocket
{
public:
  SD_EasyWebSocket();

  void AP_Connect(const char* ssid, const char* password);
  void SoftAP_setup(const char* ssid, const char* password);
  void handleClient();
  bool Get_Http_Req_Status();
  bool http_resp();
  void HandShake_timeout(uint8_t Num);
  void SD_Client_Write(File SDfile, const char* file_name);
  File SD_open(const char *filename);
  void EWS_HandShake_main(uint8_t sel, uint8_t cs_SD, const char* head_file1, const char* head_file2, const char* html_file1, const char* html_file2, IPAddress res_LIP, String res_html1, String res_html2, String res_html3, String res_html4, String res_html5, String res_html6, String res_html7);
  void EWS_HandShake(const char* HTML_file, String _res_html1, String _res_html2, String _res_html3, String _res_html4, String _res_html5, String _res_html6, String _res_html7);
  void EWS_Dev_HandShake(const char* HTML_head_file, const char* HTML_file1, String _res_html1, String _res_html2, String _res_html3, const char* HTML_file2);
  void EWS_Dev_AutoLIP_HandShake(const char* HTML_head_file1, IPAddress res_LIP, const char* HTML_head_file2, const char* HTML_file1, String res_html1, String res_html2, String res_html3, String res_html4, String res_html5, String res_html6, String res_html7, const char* HTML_file2);
	void EWS_HTTP_Responce();
  void Hash_Key(String h_req_key, char* h_resp_key);
  void EWS_ESP8266_Str_SEND(String str, String id);
  void EWS_ESP32_Str_SEND(String str, String id);
  void EWS_PING_SEND();
  String EWS_ESP8266CharReceive(uint16_t pTime);
  String EWS_ESP32CharReceive(uint16_t pTime);
  String EWS_ESP32DataReceive_SD_write(uint16_t pTime, uint8_t sd_cs, char bin_file[14]);
  void EWS_ESP32_Binary_Receive(uint8_t sd_cs, char bin_file[14]);
  String EWS_OnOff_Button(String button_id, uint16_t width, uint16_t height, uint8_t font_size, String f_color, String b_color);
  String EWS_On_Momentary_Button(String button_id, String text, uint16_t width, uint16_t height, uint8_t font_size, String f_color, String b_color);
  String EWS_Touch_Slider_BT(String slider_id, String vbox_id);
  String EWS_Touch_Slider_T(String slider_id, String txt_id);
  String EWS_Mouse_Slider_BT(String slider_id, String vbox_id);
  String EWS_Mouse_Slider_T(String slider_id, String txt_id);
  String EWS_Sl_BoxText(String vbox_id, uint16_t width, uint16_t height, uint8_t font_size, String color);
  String EWS_Sl_Text(String text_id, uint8_t font_size, String color);
  String EWS_Body_style(String text_color, String bg_color);
	String EWS_BrowserReceiveTextTag2(String id, String name, String b_color, uint8_t font_size, String fnt_col);
	String EWS_Close_Button2(String name, String BG_col, uint16_t width, uint16_t height, String font_col, uint8_t font_size);
  String EWS_Window_ReLoad_Button2(String name, String BG_col, uint16_t width, uint16_t height, String font_col, uint8_t font_size);
  String EWS_WebSocket_Reconnection_Button2(String name, String BG_col, uint16_t width, uint16_t height, String font_col, uint8_t font_size);
	String EWS_BrowserSendRate();
  String EWS_ESP32_SendRate(String button_id);
	String EWS_Status_Text2(String name, String b_color, uint8_t font_size, String f_color);
  String EWS_Canvas_Slider_T(String slider_id, uint16_t width, uint16_t height, String frame_col, String fill_col);
  String EWS_TextBox_Send(String id, String txt, String BT_txt);
  String EWS_Web_Get(char* host, String target_ip, uint8_t char_tag, String Final_tag, String Begin_tag, String End_tag, String Paragraph);
  String EWS_https_Web_Get(const char* host, String target_ip, char char_tag, String Final_tag, String Begin_tag, String End_tag, String Paragraph);
  bool HTTP_SD_Pic_Send(const char* Serv, const char* dir);
  void Favicon_Response(String str, uint8_t ws, uint8_t ini_htm, uint8_t up_f);
  void sha1( String data, uint8_t hash[20] );
  
private:
  uint8_t _Ini_html_on = 0;
  uint8_t _WS_on = 0;
  bool _Upgrade_first_on;
  char _Android_or_iPad;
  uint32_t _PingLastTime;
  uint32_t _PongLastTime;
  uint32_t _GetLoopTime = 0;
};

#endif
