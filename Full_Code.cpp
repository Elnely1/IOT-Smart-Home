#include <WiFi.h>                                                                       // This library contains the Wifi function to run on ESP-32
#include <ESP32Servo.h>
                                                                                      // In these 4 lines we have defined the PWM details for LED
#define LEDC_TIMER_13_BIT  13
#define LEDC_BASE_FREQ     5000
#define LED_PIN            21
#define LED_PIN1           22
#define LED_PIN2           23
#define LED_PIN3           24
#define LED_PIN4           17
#define SERVO_PIN          16
#define LEDC_CHANNEL_0     0
#define Led_channel_1      1
#define Led_channel_2      2
#define Led_channel_4      4
#define min(a,b) ((a)<(b)?(a):(b));                                                     // Defining Min Function

const char* ssid     = "ELNELY";
const char* password = "1234567890";

WiFiServer server(80);
Servo myservo;

int brightness = 0;
int servoPosition = 0;
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255)          // function for analogWrite in Arduino 
{
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);
  ledcWrite(channel, duty);
}

void setup()
{
    Serial.begin(115200);                                                               //Baud Rate of ESP-32
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();                                                                     // Start the server
    ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);                       // PWM setup
    ledcAttachPin(LED_PIN, LEDC_CHANNEL_0);
    myservo.attach(SERVO_PIN);
    ledcSetup(Led_channel_1, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);                       // PWM setup
    ledcAttachPin(LED_PIN1, Led_channel_1);
    ledcSetup(Led_channel_2, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);                       // PWM setup
    ledcAttachPin(LED_PIN2, Led_channel_2);
    ledcSetup(Led_channel_4, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);                       // PWM setup
    ledcAttachPin(LED_PIN4, Led_channel_4);
      
}
int value = 0;
void loop(){
 WiFiClient client = server.available();                                                // listen for any incoming client
 
  if (client) {                                                                         // if you get any client,
    Serial.println("New Client.");                                                      // print a message on the serial monitor
    String currentLine = "";                                                            // make a String to hold incoming data from the client
    while (client.connected())                                                          // loop while the client's connected
    {                                                        
      if (client.available())                                                           // if there's bytes to read from the client
      {                                                         
        char c = client.read();                                                         // read a byte, then
        Serial.write(c);                                                                // print it out on the serial monitor
        if (c == '\n')                                                                  // if the byte is a newline character
        {                                                                
          if (currentLine.length() == 0)                                                // if the current line is blank, you got two newline characters in a row.
                                                                                        // that's the end of the client HTTP request, so send a response:
          { 
            client.println("HTTP/1.1 200 OK");                                          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            client.println("Content-type:text/html");                                   // and a content-type so the client knows what's coming,
            client.println();                                                           // then a blank line:
                                                                                        // the content of the HTTP response follows the header:
            client.print("<a href=\"/A\">Outdoor ON</a><br>");
            client.print("<a href=\"/B\">Outdoor OFF</a><br>");
            client.print("<a href=\"/C\">DOOR ON</a><br>");
            client.print("<a href=\"/D\">DOOR OFF</a><br>");
            client.print("<a href=\"/E\">Room1 ON</a><br>");
            client.print("<a href=\"/F\">Room1 OFF</a><br>");
            client.print("<a href=\"/G\">Room2 ON</a><br>");
            client.print("<a href=\"/H\">Room2 OFF</a><br>");
            client.print("<a href=\"/I\">OFF All</a><br>");
            // client.print("<a href=\"/J\">Level 5 Full</a><br>");
            client.println();                                                            // The HTTP response ends with another blank line:
            break;                                                                       // break out of the while loop: 
          } 
          else 
          {    
            currentLine = "";                                                            // if you got a newline, then clear currentLine:
          }
        } 
        else if (c != '\r')                                                              // if you got anything else but a carriage return character,
        {  
          currentLine += c;                                                              // add it to the end of the currentLine
        }
        if (currentLine.endsWith("GET /A"))                                              // Check to see if the client request was "GET /A" or "GET /B" and so on...:
        {
          ledcAnalogWrite(LEDC_CHANNEL_0, 255);   
        }
        if (currentLine.endsWith("GET /B"))                                              // Check to see if the client request was "GET /A" or "GET /B" and so on...:
        {
          ledcAnalogWrite(LEDC_CHANNEL_0, 0);   
        }
        if (currentLine.endsWith("GET /C")) 
        {
          myservo.write(180);
          ledcAnalogWrite(Led_channel_4, 255);
        }
        if (currentLine.endsWith("GET /D")) 
        {
          myservo.write(0);
          ledcAnalogWrite(Led_channel_4, 0);
        }
         if (currentLine.endsWith("GET /E"))        //room1 on                                      // Check to see if the client request was "GET /A" or "GET /B" and so on...:
        {
          ledcAnalogWrite(Led_channel_1, 255);   
        }
        if (currentLine.endsWith("GET /F"))           //room1 off                                   // Check to see if the client request was "GET /A" or "GET /B" and so on...:
        {
          ledcAnalogWrite(Led_channel_1, 0);   
        }
         if (currentLine.endsWith("GET /G"))             //room2 on                                 // Check to see if the client request was "GET /A" or "GET /B" and so on...:
        {
          ledcAnalogWrite(Led_channel_2, 255);   
        }
        if (currentLine.endsWith("GET /H"))             //room2 off                                // Check to see if the client request was "GET /A" or "GET /B" and so on...:
        {
          ledcAnalogWrite(Led_channel_2, 0);   
        }
        if (currentLine.endsWith("GET /I"))           //off all
        {
         ledcAnalogWrite(LEDC_CHANNEL_0, 0);
         myservo.write(0);
         ledcAnalogWrite(Led_channel_4, 0);
         ledcAnalogWrite(Led_channel_1, 0);
         ledcAnalogWrite(Led_channel_2, 0);
        }
      }
    }
    client.stop();                                                                         // close the connection:
    Serial.println("Client Disconnected.");
  }
}