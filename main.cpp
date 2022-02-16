/*
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "../lib/ArduinoJson/ArduinoJson.h"
#include<PubSubClient.h>



//	WiFi的初始化和连接
void WiFi_Connect()
{
	WiFi.begin("CMCC-9JCs", "f4cz5hka");
	while (WiFi.status() != WL_CONNECTED)
	{ //这里是阻塞程序，直到连接成功
		delay(2500);
		Serial.print("wait.\r\n");
	}
	Serial.println("WiFi connected");
}

void setup()
{

	Serial.begin(115200); // open the serial port at 115200 bps;
	delay(100);

	

	WiFi_Connect();

	

	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());


}





// bilibili api: follower
String UID = "32512116";
String followerUrl = "http://api.bilibili.com/x/relation/stat?vmid=" + UID; // 粉丝数

long follower = 0; // 粉丝数

DynamicJsonDocument doc(1024);

//	获取粉丝数
void getBiliBiliFollower()
{
	HTTPClient http;
	http.begin(followerUrl); //HTTP begin
	int httpCode = http.GET();

	if (httpCode > 0)
	{
		// httpCode will be negative on error
		Serial.printf("HTTP Get Code: %d\r\n", httpCode);

		if (httpCode == HTTP_CODE_OK) // 收到正确的内容
		{
			String resBuff = http.getString();

			//	输出示例：{"mid":123456789,"following":226,"whisper":0,"black":0,"follower":867}}
			Serial.println(resBuff);

			//	使用ArduinoJson_6.x版本，具体请移步：https://github.com/bblanchon/ArduinoJson
			deserializeJson(doc, resBuff); //开始使用Json解析
			follower = doc["data"]["follower"];
			Serial.printf("Follers: %ld \r\n", follower);
		}
	}
	else
	{
		Serial.printf("HTTP Get Error: %s\n", http.errorToString(httpCode).c_str());
	}

	http.end();
}


void loop()
{

	//getBiliBiliFollower();
	delay(5000);
	delay(5000);
}
*/



#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "../lib/ArduinoJson/ArduinoJson.h"
#include<PubSubClient.h>

const char* ssid ="CMCC-9JCs";
const char* password ="f4cz5hka";
const char* mqttServer ="120.79.133.42";
const int mqttPort=1883;
const char* mqttUser ="admin";
const char* mqttPassword ="public";

//	WiFi的初始化和连接
void WiFi_Connect()
{
	WiFi.begin("CMCC-9JCs", "f4cz5hka");
	while (WiFi.status() != WL_CONNECTED)
	{ //这里是阻塞程序，直到连接成功
		delay(2500);
		Serial.print("wait.\r\n");
	}
	Serial.println("WiFi connected\r\n");
}

WiFiClient espClient;                     // 定义wifiClient实例
PubSubClient client(espClient);          // 定义PubSubClient的实例

void callback(char*topic, byte* payload, unsigned int length) 
{
    Serial.print("message from your topic:");             
    Serial.println(topic);                
    Serial.print("message：");              
    for (int i = 0; i< length; i++)        //使用循环打印接收到的信息
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void mqtt_Connect()
{
  while(!client.connected())
  {
    Serial.println("mqtt connecting...\r\n");
    if(client.connect("ESP32",mqttUser,mqttPassword))
    {
      Serial.println("mqtt connected\r\n");
    }
    else
    {
      Serial.print("mqtt connect fail\r\n");            
      Serial.print(client.state());       //重新连接函数
      Serial.println("\n");
      delay(2000);  
    }
  }
}
void setup()
{

	Serial.begin(115200); // open the serial port at 115200 bps;
	delay(100);	

	WiFi_Connect();

	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

  	client.setServer(mqttServer,mqttPort);//MQTT服务器连接函数（服务器IP，端口号）

 	client.setCallback(callback);
  	mqtt_Connect();

	 client.subscribe("topic"); 
	 Serial.println("connected topic ");
	 client.publish("/World","hello from esp32");


}

void loop()
{
  client.loop();
}






