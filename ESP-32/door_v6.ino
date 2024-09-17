#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <SocketIoClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <string.h>
#include <ESP32_Servo.h>

#define LED 32
#define TRIG 33
#define ECHO 26
#define BUZZER 14
#define THRESHOLD 50
#define SERVO_PIN 13

int door_open, door_open_time, user_control;
// door_open holds the status of door (1 means open, 0 means closed)
// door_open_time holds the time at which the door opened
// user_control controls the mode of operation (1 means manual, 0 means automatic)
float duration_us, distance_cm;  // used to measure distance from the ultrasonic sensor
unsigned long tim, start_time;   // used to keep track of time
const int wait_time = 1000 * 10; // 10 seconds
const int dela = 500;            // 0.5 seconds

char *temp = (char *)malloc(3 * sizeof(char));

const char *ssid = "Door";            // AccessPoint Wifi
const char *password = "password123"; // AcessPoint password

AsyncWebServer server(80); // for HTTP requests
SocketIoClient webSocket;  // for Socket.IO
Servo servoMotor;          // creating servo object

// Function to handle requests for non-existing pages
void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

// Function to print a message when connected to the server
void print_connect(const char *payload, size_t length)
{
  Serial.printf("connected to server");
}

// Function to print a message when disconnected from the server
void print_disconnect(const char *payload, size_t length)
{
  Serial.printf("disconnected from server");
}

void setup()
{

  servoMotor.attach(SERVO_PIN); // attaching servo
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // initially door is closed and mode of operation is automatic
  door_open = 0;
  user_control = 0;

  // begin serial communication
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  // creating AcessPoint
  if (!WiFi.softAP(ssid, password))
  {
    log_e("Soft AP creation failed.");
    while (1); 
  }

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  Serial.println("Server started");

  // handling socket events from server
  webSocket.on("message", event);
  webSocket.on("connect", print_connect);
  webSocket.on("disconnect", print_disconnect);
  webSocket.on("command_device", handle_user);
  webSocket.begin("192.168.4.2", 3000);

  // listening to HTTP GET requests on '/' and '/status'
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "Smart door"); });
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(door_open)); });
  server.onNotFound(notFound);
  // starting asynchronous HTTP server
  server.begin();
}

void loop()
{

  // listening to Socket.IO events
  webSocket.loop();

  // calculating distance from the ultrasonic sensor
  tim = millis();
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration_us = pulseIn(ECHO, HIGH);
  // distance_cm holds the distance of the nearest obstacle
  distance_cm = 0.017 * duration_us;

  // automatic mode of operation
  if (user_control == 0)
  {
    if (distance_cm < THRESHOLD)
    {
      if (door_open == 0)
      {
        // if door is not open, opens the door and emits an event to the server
        webSocket.emit("door_status", "\"opened\"");
        // door is opened using servo
        servoMotor.write(90);
        start_time = millis();
        // visual indication using led and audible indicating using buzzer
        digitalWrite(LED, HIGH);
        digitalWrite(BUZZER, HIGH);
      }
      // door status is set to open
      door_open = 1;
    }
    else
    {
      // time in milliseconds since the door opened is calculated 
      door_open_time = tim - start_time;
      // if door is kept open for more than 10s then door is closed and server is notified
      if (door_open == 1 && door_open_time >= wait_time)
      {
        webSocket.emit("door_status", "\"closed\"");
        servoMotor.write(0);
        door_open = 0;
        digitalWrite(LED, LOW);
      }
    }
  }

  // Control buzzer after a delay(0.5s)
  int x = tim - start_time;
  if (x >= dela)
  {
    digitalWrite(BUZZER, LOW);
  }

  delay(500); // Delay to stabilize sensor readings
}

// Function to handle user commands received over WebSocket
void handle_user(const char *payload, size_t length)
{

  strcpy(temp, payload);
  char *task = strtok(temp, "\"");
  Serial.print("\nUser request : ");
  Serial.print(task);
  Serial.print("\n");
  if (strcmp(task, "on") == 0)
  {
    door_open = 1;
    user_control = 1;
    Serial.print("\nusercontrol : ");
    Serial.print(user_control);
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(30);
    digitalWrite(BUZZER, LOW);
    delay(30);
    digitalWrite(BUZZER, HIGH);
    delay(30);
    digitalWrite(BUZZER, LOW);
    delay(30);
    webSocket.emit("door_status", "\"opened\"");
    servoMotor.write(130);
    digitalWrite(LED, HIGH);
  }
  else
  {
    user_control = 0;
    door_open = 0;
    servoMotor.write(0);
    webSocket.emit("door_status", "\"closed\"");
    digitalWrite(LED, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(BUZZER, HIGH);
    delay(30);
    digitalWrite(BUZZER, LOW);
    delay(1000);
    digitalWrite(BUZZER, HIGH);
    delay(30);
    digitalWrite(BUZZER, LOW);
    delay(30);
  }
}