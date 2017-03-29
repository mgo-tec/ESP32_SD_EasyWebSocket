# ESP32_SD_EasyWebSocket BETA 1.50
Simple library for WebSocket communication with ESP-WROOM-32(ESP32) and smartphone browser.  
This requires the Arduino core for ESP32 library.  
https://github.com/espressif/arduino-esp32  
This use SD card (SPI).  

My Blog: https://www.mgo-tec.com
# Change log:
(1.50)  
Changed for ESP32 based on SD_EasyWebSocket BETA 1.47(for ESP8266).
I used the sha1 source file (License: Public Domain) made by Steve Reid.
Thank you Steve Reid.
Add String res_html variable of WS_Dev_AutoLIP_HandShake function to seven.  
Added EWS_ESP32_SendRate function to adjust transfer rate of data sent from ESP32.  
Other unnecessary functions were deleted.  
  
  
【更新履歴】(Japanese)
(1.50)  
SD_EasyWebSocket BETA 1.47(ESP8266用)を基に ESP32用に移植。  
Steve Reidさん作のsha1ソースファイル(License:Public Domain)を使用させていただきました。
Steve Reidさん ありがとうございます。
WS_Dev_AutoLIP_HandShake関数のString res_html変数を７つにしました。
ESP32から送信するデータの転送レートを調節するための、EWS_ESP32_SendRate関数を追加しました。 
その他、不要な関数を削除しました。  

# Credits and license
*Licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 2.1  
  
Copyright (c) 2017 Mgo-tec  
This library improvement collaborator is Mr.Visyeii.  
Part of WiFiServer.h(for ESP8266) library modified.  
  
This library is used by the Arduino IDE(Tested in ver1.8.2).  
  
Reference Blog --> https://www.mgo-tec.com  
  
This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.  
  
This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
See the GNU Lesser General Public License for more details.  
  
You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA  
  
Reference LGPL-2.1 license statement --> https://opensource.org/licenses/LGPL-2.1  
  
WiFi.h - Included WiFi library for esp32  
  
ESP8266WiFi.h - esp8266 Wifi support.  
WiFiServer.h - esp8266 Wifi support.  
Based on WiFi.h from Arduino WiFi shield library.  
Copyright (c) 2011-2014 Arduino.  All right reserved.  
Modified by Ivan Grokhotkov, December 2014  
  
sha1.h - Included SHA-1 library.  
author: Steve Reid  
from: http://www.virtualbox.org/svn/vbox/trunk/src/recompiler/tests/sha1.c  
100% Public Domain  
  
SD.h - Included SD card library for esp32  
Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD  
Licensed under the Apache License, Version 2.0 (the "License");  
  
Reference Apache License --> http://www.apache.org/licenses/LICENSE-2.0  
  
# My Blog: 
Other usage is Japanese, please visit my blog.  
https://www.mgo-tec.com