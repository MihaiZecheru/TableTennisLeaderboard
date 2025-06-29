#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include "Secrets.h"

using namespace websockets;
WebsocketsClient socket;
bool websocket_initialized = false;

void StartWebSocket(const char* p1_name, const char* p2_name) {
  Serial.println("START OF WEBSOCKET FUNC");
  if (!websocket_initialized)
  {
    socket.onMessage([](WebsocketsMessage message) {
      // The ESP32 shouldn't be receiving any messages
      // This is for receiving the keep-alive pings
      Serial.print("Message received: ");
      Serial.println(message.data());
    });

    socket.onEvent([](WebsocketsEvent event, String data) {
      if (event == WebsocketsEvent::ConnectionOpened)
        Serial.println("Connected to WebSocket server.");
      else if (event == WebsocketsEvent::ConnectionClosed)
        Serial.println("Disconnected from WebSocket server.");
    });

    socket.setCACert(PUBLIC_SSL_CERT_FOR_SERVER);
    websocket_initialized = true;
  }

  String ws_url = String(WEBSOCKET_LIVE_SCORE_URL) + "?client=ESP32&p1_name=" +
    p1_name + "&p2_name=" + p2_name;
  Serial.println("right before connection");
  socket.connect(ws_url);
  Serial.println("Websocket connected");
}

void PollWebsocket()
{
  socket.poll();
}

void CloseWebsocket()
{
  if (socket.available())
  {
    socket.close();
  }
}

void _send_websocket_message(const String &message)
{
  if (socket.available())
  {
    socket.send(message);
    Serial.println("Sent: " + message);
  }
  else
  {
    Serial.println("WebSocket not connected. Cannot send: " + message);
  }
}

void SendWebsocketMessage_IncrementP1Score()
{
  _send_websocket_message("p1+1");
}

void SendWebsocketMessage_IncrementP2Score()
{
  _send_websocket_message("p2+1");
}

void SendWebsocketMessage_DecrementP1Score()
{
  _send_websocket_message("p1-1");
}

void SendWebsocketMessage_DecrementP2Score()
{
  _send_websocket_message("p2-1");
}

void SendWebsocketMessage_ResetGame()
{
  _send_websocket_message("reset");
}

#endif