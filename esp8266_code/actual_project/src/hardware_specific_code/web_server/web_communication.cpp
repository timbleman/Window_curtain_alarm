/********************************** Includes **********************************/
#include "web_communication.h"
#ifdef ENABLE_LOCAL_WEB_SITE
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "arduinoWebSockets/src/WebSocketsServer.h"
#include <Hash.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "../../configuration.h"


/********************************** Defines ***********************************/
#define WEB_SCKT_BUF_LEN 256


/********************************* Constants **********************************/
/*
static const char PROGMEM INDEX_HTML11[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
<title>ESP8266 WebSocket Demo</title>
<style>
"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }"
</style>
<script>
var websock;
function start() {
  websock = new WebSocket('ws://' + window.location.hostname + ':81/');
  websock.onopen = function(evt) { console.log('websock open'); };
  websock.onclose = function(evt) { console.log('websock close'); };
  websock.onerror = function(evt) { console.log(evt); };
  websock.onmessage = function(evt) {
    console.log(evt);
    var e = document.getElementById('ledstatus');
    if (evt.data === 'ledon') {
      e.style.color = 'red';
    }
    else if (evt.data === 'ledoff') {
      e.style.color = 'black';
    }
    else {
      console.log('unknown event');
    }
  };
}
function buttonclick(e) {
  websock.send(e.id);
}
</script>
</head>
<body onload="javascript:start();">
<h1>ESP8266 WebSocket Demo</h1>
<div id="ledstatus"><b>LED</b></div>
<button id="ledon"  type="button" onclick="buttonclick(this);">On</button> 
<button id="ledoff" type="button" onclick="buttonclick(this);">Off</button>
</body>
</html>
)rawliteral";
*/

// This may break in the future 
static const char PROGMEM INDEX_HTML[]
#include "web_page.html"
//#include "test_html.html"
;


/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
void handleRoot();
void handleNotFound();
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
int fetch_web_data(char *buf, int buf_max_len);
int setup_web_comm();
int get_web_in();
user_action_t fetch_web_action();
int get_web_in(char *buf, int buf_max_len);
int respond_to_web_user(char *buf, int buf_max_len);


/**************************** Variable definitions ****************************/
MDNSResponder mdns;

//ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

char web_socket_buf[WEB_SCKT_BUF_LEN] = {};
bool data_available = false;


/**************************** Function definitions ***************************/
/* 
 * Setup an input_handler_t. Better way to handle multiple input sources.
 * 
 * @param inh: An input_handler_t to initialize.
 * @retval: Success status.
 */
int setup_web_input_handler_t(input_handler_t *inh)
{
    inh->setup = setup_web_comm;
    inh->input_available = get_web_in;
    inh->fetch_action = fetch_web_action;
    inh->respond_to_user = respond_to_web_user;
    strncpy(inh->tag, "web", TAG_LEN);

    return inh->setup();
}

void handleRoot()
{
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  Serial.printf("webSocketEvent(%d, %d, ...)\r\n", num, type);
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\r\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\r\n", num, payload);

      if (length > 0)
      {
        strncpy(web_socket_buf, (const char *)payload, min((size_t)WEB_SCKT_BUF_LEN, length));
        printf("web_socket_buf %s \n\r", web_socket_buf);
        data_available = true;
      }

      break;
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\r\n", num, length);
      hexdump(payload, length);

      // echo data back to browser
      webSocket.sendBIN(num, payload, length);
      break;
    default:
      Serial.printf("Invalid WStype [%d]\r\n", type);
      break;
  }
}

/* 
 * Setup the web server.
 * 
 * return:  Success status.
 */
int setup_web_comm()
{
    if (mdns.begin("espWebSock", WiFi.localIP()))
    {
        Serial.println("MDNS responder started");
        mdns.addService("http", "tcp", 80);
        mdns.addService("ws", "tcp", 81);
    }
    else 
    {
        Serial.println("MDNS.begin failed");
    }
    Serial.print("Connect to http://espWebSock.local or http://");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.onNotFound(handleNotFound);

    server.begin();

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    return 0;
}

/* 
 * Handle clients and web socket.
 * Needs to be called in a loop.
 * 
 * @return:  Success status.
 */
int get_web_in()
{
  webSocket.loop();
  server.handleClient();

  if (data_available)
    return 0;
  else
    return 1;
}

/* 
 * Fetch an action if available. 
 * Pass by value with copying is used, should not be a problem for this small
 * size.
 * 
 * @return: A user_action_t.
 */
user_action_t fetch_web_action()
{
  data_available = false;

  user_action_t new_act = {};
  new_act = get_action(web_socket_buf);

  if (strlen(web_socket_buf) > 0)
    memset(web_socket_buf, 0, strlen(web_socket_buf) * sizeof(char));

  return new_act;
}

/* 
 * Copy the received data into the buffer.
 * @param buf:          Chars to send.
 * @param buf_max_len:  Buffer size.
 * @return:             Success status.
 */
int fetch_web_data(char *buf, int buf_max_len)
{
    data_available = false;

    if (strlen(buf) > strlen(web_socket_buf))
        memset(buf, 0, strlen(buf) * sizeof(char));

    strncpy(buf, web_socket_buf, min(WEB_SCKT_BUF_LEN, buf_max_len));
    return 0;
}

/* 
 * Respond to all connected clients.
 * 
 * @param buf:          Chars to send.
 * @param buf_max_len:  Buffer size.
 * @return:             Success status.
 */
int respond_to_web_user(char *buf, int buf_max_len)
{
    bool status = webSocket.broadcastTXT(buf, buf_max_len);

    return (int)!status;
}

#endif // ENABLE_LOCAL_WEB_SITE
