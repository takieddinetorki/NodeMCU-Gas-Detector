/*
  ESP32 publish telemetry data to VOne Cloud (MQ2 Sensor)
*/

#include "VOneMqttClient.h"
float gasValue;

//defining devices id
const char* PIRsensor = "44fc79f4-710b-44bb-a9cf-b0e56fd1a21d";  
const char* MQ2sensor = "8538fbc0-0432-451e-aac1-2372158c6b20"; 
//Used Pins
const int MQ2pin = 34;
const int motionSensor = 17;                                                                                                                                                                                                                                                                                                                                                                                   

//input sensor
// Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;

#define timeSeconds 3



// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement() {
  startTimer = true;
  lastTrigger = millis();
}


//Create an instance of VOneMqttClient
VOneMqttClient voneClient;

//last message time
unsigned long lastMsgTime = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void processCommand(String command) {
  // Compare the command with the expected commands
  if (command == "ON") {
    // Turn on the LED
    
    digitalWrite(22, HIGH);
  } else if (command == "OFF") {
    // Turn off the LED
    digitalWrite(22, LOW);
  }
}
void setup() {
  // Initialize the serial communication
  Serial.begin(115200);
  setup_wifi();
  voneClient.setup();
  pinMode(motionSensor, INPUT);
  pinMode(22, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);
  Serial.println("Gas sensor warming up!");
  delay(20000); // allow the MQ-2 to warm up
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readString();
    processCommand(command);
  }
  if (!voneClient.connected()) {
    voneClient.reconnect();
    voneClient.publishDeviceStatusEvent(MQ2sensor, true);
    voneClient.publishDeviceStatusEvent(PIRsensor, true);
  }else {
    unsigned long cur = millis();
    if (cur - lastMsgTime > INTERVAL) {
      lastMsgTime = cur;

      now = millis();
      // Turn off the LED after the number of seconds defined in the timeSeconds variable
      if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
        startTimer = false;
        gasValue = analogRead(MQ2pin);
      voneClient.publishTelemetryData(PIRsensor, "Motion", startTimer);
      voneClient.publishTelemetryData(MQ2sensor, "Gas detector", gasValue);
      }

      //Sample sensor fail message
      //String errorMsg = "DHTSensor Fail";
      //voneClient.publishDeviceStatusEvent(DHT22Sensor, false, errorMsg.c_str());
    }
  }

}
