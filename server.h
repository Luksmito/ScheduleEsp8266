#include <ESP8266WebServer.h>
#include "schedule.h"

extern ESP8266WebServer server;
ESP8266WebServer server(80);

void handleRoot() {
  // Home page
  String html = "<html><body>";
  html += "<h1>agenda cafeteira</h1>";
  html += "<form method='post' action='/config'>";
  html += "Selecione um day:<br>";
  html += "<select name='day'>";
  html += "<option value='0'>Domingo</option>";
  html += "<option value='1'>Segunda</option>";
  html += "<option value='2'>Terca</option>";
  html += "<option value='3'>Quartaarta</option>";
  html += "<option value='4'>Quinta</option>";
  html += "<option value='5'>Sexta</option>";
  html += "<option value='6'>Sabado</option>";
  html += "</select><br>";
  html += "hour:<br>";
  html += "<input type='number' name='hour'><br>";
  html += "minute:<br>";
  html += "<input type='number' name='minute'><br>";
  html += "<input type='submit' value='Enviar'>";
  html += "</form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleConfig() {
  // Handle the data received
  if (server.hasArg("day") && server.hasArg("hour") && server.hasArg("minute")) {
    String day = server.arg("day");
    String hour = server.arg("hour");
    String minute = server.arg("minute");

    int dayN = day.toInt();
    int hourN = hour.toInt();
    int minuteN = minute.toInt();
    
    Schedule::setHourandMinuteForDay(dayN, hourN, minuteN);
  
  }
  // redirects back to the home page
  server.sendHeader("Location", "/");
  server.send(303);

}

void initializeServer() {
  // Configura as rotas para as páginas da web
  server.on("/", HTTP_GET, handleRoot);
  server.on("/config", HTTP_POST, handleConfig);

  // Inicia o servidor
  server.begin();
 
}