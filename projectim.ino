/*********
 Resul Caliskan
 B191210002
 25/12/22
 IOT Projesi :Akıllı Led RGB
*********/


#include <ESP8266WiFi.h>

const char* ssid     = "Mi";
const char* password = "Resul1234";

// port 80 e ayarlaniyor
WiFiServer server(80);

// http den gelen degerleri parse edip alacaz
String redString = "0";
String greenString = "0";
String blueString = "0";
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;

// gelen istegi burada tutacaz
String header;

// piinlerin cikis degerleri
const int redPin = 4;     // GPIO4 D2 ye esit
const int greenPin = 2;   // GPIO2 D4 e esit
const int bluePin = 12;    //GPIO12 D6 ya esit

// cozunurluk icin
const int resolution = 256;


unsigned long simdikiZaman = millis();

unsigned long oncekiZaman = 0; 
//zaman asimi 2 saniye olarak tanimlandi
const long zamanAsimi = 2000;

void setup() {
  Serial.begin(115200);
  
  // analog pinleri ayarlama
  analogWriteRange(resolution);
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  
  //wifi a baglanma

  Serial.println(ssid);
  Serial.print(": Agina baglaniyor");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi Baglandi :)");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // client lar icin dinleniyor
  if (client) {                             // eger yeni client baglandiysa
    simdikiZaman = millis();
    oncekiZaman = simdikiZaman;
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected() && simdikiZaman - oncekiZaman <= zamanAsimi) {            
      simdikiZaman = millis();
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    
          //eger gelen karakter bos ise 
          // baglantiyi kapatacaz
          if (currentLine.length() == 0) {
           
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
                   
            // html sayfasi
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\">");
            client.println("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script>");
            client.println("</head><body><div class=\"row\" style=\"background-color:black;color:white\"><h1>| AKILLI LED</h1></div>");
            client.println("<div class=\"row\" style=\"background-color:black \"><h4 style=\"color: aliceblue;font-style: italic;\">| |  Resul Çalışkan B191210002<br><a href=\"https://github.com/Resul-Caliskan/Smart-Led-with-ESP-WebServer\">| | github_Linkim.com</a</h4></div><div class=\"container\">");
            client.println("<br><a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color\" role=\"button\">Renk Değiştir</a> ");
            client.println("<input class=\"jscolor {onFineChange:'update(this)'}\" id=\"rgb\"></div>");
            client.println("<br><div class=\"row\"><div class=\"col-2\"></div><div class=\"col-2\"><a class=\"btn btn-primary btn-lg \" href=\"/?r0g255b255&\" role=\"button \"style=\"border:solid;border-color:red;background-color: rgb(255, 255, 255);color:red;\">Romantik</a></div>");
            client.println("<div class=\"col-2\"><a class=\"btn btn-primary btn-lg \" href=\"/?r0g226b45&\" role=\"button \"style=\"border:solid;border-color:rgb(134, 99, 78);background-color: rgb(255, 255, 255);color:rgb(134, 99, 78);\">Kitap Okuma</a></div>");
            client.println("<div class=\"col-2\"><a class=\"btn btn-primary btn-lg \" href=\"/?r255g255b0&\" role=\"button \"style=\"border:solid;border-color:blue;background-color: rgb(255, 255, 255);color:blue;\">Gökyüzü</a></div>");
            client.println("<div class=\"col-2\"><a class=\"btn btn-primary btn-lg \" href=\"/?r255g0b255&\" role=\"button \"style=\"border:solid;border-color:green;background-color: rgb(255, 255, 255);color:green;\">Orman</a></div><div class=\"col-2\"></div></div>");
            client.println("<script>function update(picker) {var red=(-1)*(picker.rgb[0]-255);var green=(-1)*(picker.rgb[1]-255);var blue=(-1)*(picker.rgb[2]-255);document.getElementById('rgb').innerHTML = Math.round(picker.rgb[0]) + ', ' +  Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]);");
            client.println("document.getElementById(\"change_color\").href=\"?r\" + Math.round(red) + \"g\" +  Math.round(green) + \"b\" + Math.round(blue) + \"&\";}</script></body></html>");
           

            client.println();

            
            // gelen requesti parcaliyoruz
            if(header.indexOf("GET /?r") >= 0) {
              pos1 = header.indexOf('r');
              pos2 = header.indexOf('g');
              pos3 = header.indexOf('b');
              pos4 = header.indexOf('&');
              redString = header.substring(pos1+1, pos2);
              greenString = header.substring(pos2+1, pos3);
              blueString = header.substring(pos3+1, pos4);
              //daha sonra analog cikisa veriyoruz degerleri
              analogWrite(redPin, redString.toInt());
              analogWrite(greenPin, greenString.toInt());
              analogWrite(bluePin, blueString.toInt());
            }
            
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }
    //basligi temizliyoruz
    header = "";
    //baglantiyi kapatiyoruz
    client.stop();
    Serial.println("Baglanti sonlandi.");
    Serial.println("");
  }
}
