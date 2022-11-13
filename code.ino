#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#define led_a 25
#define led_b 26
#define led_c 27
#define touchPin 35


 #define LEDR 19   
 #define LEDG 18    
 #define LEDB 5   
 #define R_channel 0  
 #define G_channel 1  
 #define B_channel 2  
 #define pwm_Frequency 5000   
 #define pwm_resolution 8   

int buttonState = 0;

const char* ssid = "NPT-CEO_2.4Ghz";
const char* password = "2021020100";





unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

// Set your Static IP address
IPAddress local_IP(10, 19, 181, 49);
// Set your Gateway IP address
IPAddress gateway(10, 19, 181, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(128, 100, 119, 37);   //optional
IPAddress secondaryDNS(128, 100, 119, 8); //optional

String json_array;
String json_array_status;

 int deLayBtn;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(led_a,OUTPUT);
  pinMode(led_b,OUTPUT);
  pinMode(led_c,OUTPUT);
  pinMode(touchPin,INPUT);


ledcAttachPin(LEDR, R_channel);  
  ledcAttachPin(LEDG, G_channel);  
  ledcAttachPin(LEDB, B_channel);   
  ledcSetup(R_channel, pwm_Frequency, pwm_resolution);  
  ledcSetup(G_channel, pwm_Frequency, pwm_resolution);  
  ledcSetup(B_channel, pwm_Frequency, pwm_resolution);   

    // Configures static IP address
//  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
//    Serial.println("STA Failed to configure");
//  }
  while (!Serial); // wait for serial port to connect. Needed for Leonardo only
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  double temperature_v;
  double humidity_v;
  int clickBtn = 0;

  buttonState = digitalRead(touchPin);
  Serial.print("Touch : ");
      Serial.println(buttonState);
  if(buttonState == LOW){
    clickBtn = 0;
  }else{
    clickBtn = 1;
  }

  //Click For close void and set to green led.
  if(clickBtn == 0){
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
              //Your Domain name with URL path or IP address with path
     String  server  = "http://10.20.7.102/GetData.php?deviceID=6";
//     String  server_getstatus  = "http://10.20.7.102/GetStatus.php";

//      json_array_status = GET_Request(server_getstatus.c_str());
      json_array = GET_Request(server.c_str());
//      Serial.println(json_array_status);
      Serial.println(json_array);
      JSONVar my_obj = JSON.parse(json_array);
//      JSONVar my_obj_status = JSON.parse(json_array_status);
  
      if (JSON.typeof(my_obj) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
//      if (JSON.typeof(my_obj_status) == "undefined") {
//        Serial.println("Parsing input failed!");
//        return;
//      }
    
      Serial.print("JSON object = ");
      Serial.println(my_obj);
//      Serial.print("JSON object Status = ");
//      Serial.println(my_obj_status);
      Serial.print("Temperature: ");
      Serial.println(my_obj["Data"][0]["Temperature"]);
      Serial.print("Humidity: ");
      Serial.println(my_obj["Data"][0]["Humidity"]);

      //set value for compare temperature and humidity
      double green_start_tem = atof(my_obj["Spec"][0]["T_start"]);
      double green_end_tem = atof(my_obj["Spec"][0]["T_end"]);
      double green_start_hum = atof(my_obj["Spec"][0]["H_start"]);
      double green_end_hum = atof(my_obj["Spec"][0]["H_end"]);

       double yellow_start_tem = atof(my_obj["Spec"][1]["T_start"]);
      double yellow_end_tem = atof(my_obj["Spec"][1]["T_end"]);
      double yellow_start_hum = atof(my_obj["Spec"][1]["H_start"]);
      double yellow_end_hum = atof(my_obj["Spec"][1]["H_end"]);

       double red_start_tem = atof(my_obj["Spec"][2]["T_start"]);
      double red_end_tem = atof(my_obj["Spec"][2]["T_end"]);
      double red_start_hum = atof(my_obj["Spec"][2]["H_start"]);
      double red_end_hum = atof(my_obj["Spec"][2]["H_end"]);

       deLayBtn = atoi(my_obj["Spec"][0]["set_delay_btn"]);



      temperature_v = atof(my_obj["Data"][0]["Temperature"]);
      humidity_v = atof(my_obj["Data"][0]["Humidity"]);
//       humidity_v = 73.5;
         if(temperature_v >= green_start_tem && temperature_v <= green_end_tem){
          Serial.println("Green light");
          
                digitalWrite(led_a,HIGH);
          digitalWrite(led_c,LOW);
          digitalWrite(led_b,LOW);
              
          
        }
        else{
            if(temperature_v <= red_start_tem || temperature_v >= red_end_tem){
              Serial.println("Red light And Alarm noise");
              digitalWrite(led_b,LOW);
              digitalWrite(led_a,LOW);
              
                //lastMillis = millis();
                digitalWrite(led_c,HIGH);
                   delay(500);
               digitalWrite(led_c,LOW);
               delay(300);
              
              
            }else{
              Serial.println("Yellow light");
              
                 digitalWrite(led_b,HIGH);
                 delay(500);
                 digitalWrite(led_b,LOW);
                 delay(300);
              digitalWrite(led_a,LOW);
              digitalWrite(led_c,LOW);
              
             
            }
        }

          //Humidy
          if(humidity_v >= green_start_hum && humidity_v <= green_end_hum){
            Serial.println("Green light");
            RGB_Color(0,255,0);
            
          }
          else{
              if(humidity_v <= red_start_hum || humidity_v >= red_end_hum){
                Serial.println("Red light And Alarm noise");
                 RGB_Coloroff();
                RGB_Color(255,0,0); // RED color  
                     delay(500);
                      RGB_Coloroff(); // RED color  
                  
                 delay(300);
                
                
              }else{
                Serial.println("Yellow light");
                 RGB_Coloroff();
         RGB_Color(0,0,255);
                   delay(500);
                   RGB_Coloroff();
                   delay(300);
                
               
              }
        }
        
        
          
            
          
        }
        else {
            Serial.println("WiFi Disconnected");
          }
          lastTime = millis();
      }else{
          digitalWrite(led_a,HIGH);
          digitalWrite(led_c,LOW);
          digitalWrite(led_b,LOW);
      
          RGB_Color(0,255,0);
          delay(deLayBtn);
          clickBtn = 0;
        }
}

 void RGB_Color(int i, int j, int k)  
 {  
  ledcWrite(R_channel, i);   
  ledcWrite(G_channel, j);  
  ledcWrite(B_channel, k);   
 }  

 void RGB_Coloroff()  
 {  
  ledcWrite(R_channel, 0);   
  ledcWrite(G_channel, 0);  
  ledcWrite(B_channel, 0);   
 }  



String GET_Request(const char* server) {
  HTTPClient http;    
  http.begin(server);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.GET();
  Serial.println(httpResponseCode);
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();


  return payload;
}
