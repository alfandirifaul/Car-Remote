#include <ESP8266WiFi.h>

// The Pin Configuration of Robot Car
#define leftFront 0
#define leftBack 1
#define rightFront 2
#define rightBack 3
#define buzzerHorn 4

// Configuration of Wifi
const char *ssid = "Pams-Robotics";
const char *password = "education";

// Configuration of Server
WiFiServer server(80);

//  S: Create the function to move forward
 void moveForward(){
     digitalWrite(leftFront, HIGH);
     digitalWrite(leftBack, LOW);
     digitalWrite(rightFront, HIGH);
     digitalWrite(rightBack, LOW);
 }
//  E: Create the function to move forward

// S: Create the function to move backward
void moveBackward(){
    digitalWrite(leftFront, LOW);
    digitalWrite(leftBack, HIGH);
    digitalWrite(rightFront, LOW);
    digitalWrite(rightBAck, HIGH);
}
// E: Create the function to move backward

// S: Create the function to move left
void moveLeft(){
    digitalWrite(leftFront, LOW);
    digitalWrite(leftBack, LOW);
    digitalWrite(rightFront, HIGH);
    digitalWrite(rightBack, HIGH);
}
// E: Create the function to move left

// S: Create the function to move right
void moveRight(){
    digitalWrite(leftFront, HIGH);
    digitalWrite(leftBack, HIGH);
    digitalWrite(rihghtFront, LOW);
    digitalWrite(rightBack, LOW);
}
// E: Create the function to move right

// S: Create function to turn on the buzzer
void buzzerOn(){
    digitalWrite(buzzerHorn, HIGH);
}
// E: Create function to turn on the buzzer

// S: Create function to turn off the buzzer
void buzzerOff(){
    digitalWrite(buzzerHorn, LOW);
}
// E: Create function to turn off the buzzer

// S: Create function to handle client request
void handleClient(){
    WiFiClient client = server.available();
    if(!client) return;
    Serial.println("New Client");

    String currentLine = "";
    while(client.connected()){
        if(client.available()){
            char command = client.read();
            Serial.print(command);
            if(command == '\n'){
                if(currentLine.length() == 0){
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println();
                    client.println("<!DOCTYPE HTML>");
                    client.println("<html><body>");
                    client.println("<h1>Remote Controller</h1>");
                    client.println("<h1>Pams Robotics Education</h1>");
                    client.println("<button onclick=\"location.href='forward'\">Forward</button>");
                    client.println("<button onclick=\"location.href='backward'\">Backward</button>");
                    client.println("<button onclick=\"location.href='left'\">Left</button>");
                    client.println("<button onclick=\"location.href='right'\">Right</button>");
                    client.println("<button onclick=\"location.href='buzzer'\">Buzzer</button>");
                    client.println("</body></html>");
                    client.println();
                    break;
                }
                else{
                    if(currentLine.startsWith("GET /forward")){
                        moveForward();
                    } 
                    else if(currentLine.startsWith("GET /backward")){
                        moveBackward();
                    }
                    else if(currentLine.startsWith("GET /left")){
                        moveLeft();
                    }
                    else if(currentLine.startsWith("GET /right")){
                        moveRight();
                    }
                    else if(currentLine.startsWith("GET /buzzer")){
                        buzzerOn();
                        delay(100);
                        buzzerOff();
                    }
                    currentLine = "";
                }
            }
            else if(command != '\r'){
                currentLine += command;
        }
    }    
    client.stop();
    Serial.println("Client Disconnected");
}

void setup(){
    // Start Serial Communication
    Serial.begin(115200);

    // Initialize the output pins
    pinMode(leftFront, OUTPUT);
    pinMode(leftBack, OUTPUT);
    pinMode(rightFront, OUTPUT);
    pinMode(rightBack, OUTPUT);
    pinMode(buzzerHorn, OUTPUT);

    // Create WiFi AP
    WiFi.softAP(ssid, password);
    char *IP = WiFi.softAPIP();
    Serial.print("AP IP Address: ");
    Serial.println(IP);

    // Start server
    server.begin();
    Serial.println("Server Started");
}


void loop(){
    handleClient();
}