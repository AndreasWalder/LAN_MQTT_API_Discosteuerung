#include <Arduino.h>

#include <LasPCF8574.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
IPAddress clientIP(192, 168, 8, 170);
IPAddress mqttServer(192, 168, 8, 115);
String DeviceName = "Disco";
String DeviceNameRelais1 = DeviceName + "/Relais1";

LasPCF8574Class relaisGruppe1(0x20);
LasPCF8574Class relaisGruppe2(0x21);
EthernetClient ethClient(8080);
PubSubClient mqttClient(ethClient);

int mqttZaehler = 0;
int StatusApi = 0;

char cmd[15];
char param1[15];
boolean systemArmed = true;

#define STRING_BUFFER_SIZE 128
char buffer[STRING_BUFFER_SIZE];

EthernetServer server(80);
EthernetClient client;
void reconnect() {
  // Loop until we're reconnected MQTT
  while (!mqttClient.connected()) {
    Serial.print("Verbindung MQTT");
    // Attempt to connect
    if (mqttClient.connect("arduinoClient")) {
      Serial.println(" verbunden");
      // Once connected, publish an announcement...
      String DeviceNameStatus = DeviceName + "/Status";
      mqttClient.publish((char*) DeviceNameStatus.c_str(),"verbunden");

      
      mqttClient.publish((char*) DeviceNameRelais1.c_str(),"verbunden");
      mqttClient.subscribe((char*) DeviceNameRelais1.c_str());
      
    } else {
      mqttZaehler ++;
      Serial.print(" / Verbindung fehlt, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" neuer Versuch in 5 Sekunden");
      // Wait 5 seconds before retrying
      delay(3000);
      Serial.print("Versuche: ");
      Serial.print(mqttZaehler);
      Serial.println(" / 3");

      if (mqttZaehler >= 3) {
        Serial.println("Software Reset!");
        delay(1000);
       // rstc_start_software_reset(RSTC);
        mqttZaehler = 0;
      }
    }   
  }

  
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Angekommene Meldung [");
  
  String Namen = String(topic);
  Serial.print(Namen);
  
  Serial.println("] ");
  
  String Auswertung = "";
  for (unsigned i = 0; i < length; i++) {
    Serial.print((char)payload[i]);  
    Auswertung = Auswertung + (char)payload[i];
  }


  if (Namen == DeviceNameRelais1)
  {
    if(Auswertung == "true") {
      relaisGruppe1.writeBit(Relais1, ein);
      } 
      if(Auswertung == "false") {
      relaisGruppe1.writeBit(Relais1, aus);
      } 
  }
}

void send404(EthernetClient client) {
     client.println("HTTP/1.1 404 OK");
     client.println("Content-Type: text/html");
     client.println("Connnection: close");
     client.println();
     client.println("<!DOCTYPE HTML>");
     client.println("<html><body>404 Error undefinierter Api Befehl :-( </body></html>");
}

void fatalError(EthernetClient client) {
     client.println("HTTP/1.1 404 OK");
     client.println("Content-Type: text/html");
     client.println("Connnection: close");
     client.println();
     client.println("<!DOCTYPE HTML>");
     client.println("<html><body>Systemfehler Api Neustart! </body></html>");

     delay(1000);
     //rstc_start_software_reset(RSTC);
}

void sendStatus(EthernetClient client) {

   // Send a standard http response header
   client.println("HTTP/1.1 200 OK");
   client.println("Content-Type: application/json");
   client.println("Connnection: close");
   client.println();
   client.println("{");
   client.print("\"Status:");
   client.print("Relais <");
   client.print(relaisGruppe1.readBackBit(Relais1));
   client.print(">");
   client.println("\n}");
}

void sendStatusApi(EthernetClient client) {
   StatusApi ++;
   // Send a standard http response header
   client.println("HTTP/1.1 200 OK");
   client.println("Content-Type: application/json");
   client.println("Connnection: close");
   client.println();
   client.println("{");
   client.print("\"Api:");
   client.print("Relais <");
   client.print(StatusApi);
   
   if (StatusApi == 1) {
    relaisGruppe1.writeBit(Relais1, ein);
   } 
   if (StatusApi == 2) {
    relaisGruppe1.writeBit(Relais1, aus);
   } 
   if (StatusApi == 3) {
    relaisGruppe1.writeBit(Relais1, ein);
   } 
   if (StatusApi == 4) {
    relaisGruppe1.writeBit(Relais1, aus);
   } 
   if (StatusApi == 5) {
    relaisGruppe1.writeBit(Relais1, ein);
   } 
   if (StatusApi == 6) {
    relaisGruppe1.writeBit(Relais1, aus);
   } 
   if (StatusApi == 7) {
    relaisGruppe1.writeBit(Relais1, ein);
   } 
   if (StatusApi == 8) {
    relaisGruppe1.writeBit(Relais1, aus);
   } 
   if (StatusApi == 9) {
    relaisGruppe1.writeBit(Relais1, ein);
   } 
   
   
   client.println("\n}");
}

/**
 * Parse the string and return an array which contains all path segments
 */
char** parse(char* str) {

  char ** messages;
  messages = (char**)malloc(sizeof(char *));
  char *p;
  p = strtok(str, " ");
  unsigned int i = 0;
  while (p != NULL) {
    p = strtok(NULL, "/");
    char *sp;
    boolean last = false;
    sp = strchr(p, ' ');
    if (sp != NULL) {
            *sp++ = '\0';
      last = true;
    }
    messages[i] = p;
    i++;
    if (last) {
      break;
    }
    messages = (char**)realloc(messages, sizeof(char *) * i + 1);
  }

  messages[i] = '\0';
  return messages;
}

void handleCommand(EthernetClient client,char* api, char* cmd) {
    if (strcmp(api, "api") == 0) {
      Serial.println("api Befehl erkannt");
      if (strcmp(cmd, "relais1ein") == 0) {
          relaisGruppe1.writeBit(Relais1, ein);
          sendStatus(client);
          return;
        }
        else if (strcmp(cmd, "relais1aus") == 0) {
          relaisGruppe1.writeBit(Relais1, aus);
          sendStatus(client);
          return;
        }
         send404(client);
      }
      
     else {
      fatalError(client);
    }
    
    /*
    else if (strcmp(cmd, "arm") == 0) {
      armSystem();
      sendStatus(client);
      Serial.println("arm");
    } else if (strcmp(cmd, "disarm") == 0) {
      disarmSystem();
      sendStatus(client);
      Serial.println("disarm");
    } else {
      send404(client);
    }
    */
}

int countSegments(char* str) {
  int p = 0;
  int count = 0;
  while (str[p] != '\0') {
    if (str[p] == '/') {
      count++;
    }
    p++;
  }
  // We don't want to count the / in 'HTTP/1.1'
  count--;
  return count;
}

void Api(EthernetClient client)
{
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {

        char c;
        int bufindex = 0; // reset buffer
        buffer[0] = client.read();
        buffer[1] = client.read();
        bufindex = 2;
        // Read the first line to determin the request page
        while (buffer[bufindex-2] != '\r' && buffer[bufindex-1] != '\n') {
            // read full row and save it in buffer
            c = client.read();
            if (bufindex<STRING_BUFFER_SIZE) {
              buffer[bufindex] = c;
            }
            bufindex++;
        }
        // Clean buffer for next row
        bufindex = 0;

        // Parse the query string
        int nSegments = countSegments(buffer);
        char **pathsegments = parse(buffer);

        int i = 0;
        for(i=0; i<nSegments; i++) {
          Serial.println(pathsegments[i]);
        }

        

        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }

        if (c == '\n' && currentLineIsBlank) {
          handleCommand(client, pathsegments[0], pathsegments[1]);
          break;
        }
      }
    }
}

void setup()
{
  Serial.begin(57600);
  Serial.print("Discosteuerung V1.0");
  Serial.println("");
  
  Ethernet.begin(mac, clientIP);
  // Allow the hardware to sort itself out
  delay(1500);

  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(callback);

  relaisGruppe1.begin();
  relaisGruppe2.begin();

  //Relais über I2C Status Aus
  relaisGruppe1.writeByte(255);
  relaisGruppe2.writeByte(255);

  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop()
 {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

   if (!client) {
    client = server.available();
   }
  
  if (client) {
     Api(client);
     delay(3);
     client.stop();
  }
  
}