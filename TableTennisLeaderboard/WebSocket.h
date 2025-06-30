#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include "Secrets.h"

using namespace websockets;

WebsocketsClient socket;
bool websocket_initialized = false;

// ----------- ASYNC SEND TASK -----------
void _send_websocket_task(void* param) {
  String* message = static_cast<String*>(param);

  if (socket.available()) {
    socket.send(*message);
    // Serial.println("Sent: " + *message);
  } else {
    Serial.println("WebSocket not connected. Cannot send: " + *message);
  }

  delete message; // Free memory
  vTaskDelete(NULL); // End task
}

void SendWebsocketMessageAsync(const String& msg) {
  // Heap-allocate a copy so the task can safely access it
  String* message_copy = new String(msg);
  xTaskCreate(_send_websocket_task, "ws_send", 4096, message_copy, 1, NULL);
}

// ----------- CORE WEBSOCKET SETUP -----------
void StartWebSocket(const char* p1_name, const char* p2_name, const uint8_t first_server) {
  if (!websocket_initialized) {
    socket.onMessage([](WebsocketsMessage message) {
      Serial.print("WebSocket message received: ");
      Serial.println(message.data());
    });

    socket.onEvent([](WebsocketsEvent event, String data) {
      if (event == WebsocketsEvent::ConnectionOpened)
        Serial.println("Connected to WebSocket server");
      else if (event == WebsocketsEvent::ConnectionClosed)
        Serial.println("Disconnected from WebSocket server");
    });

    socket.setCACert(PUBLIC_SSL_CERT_FOR_SERVER);
    websocket_initialized = true;
  }

  String ws_url = String(WEBSOCKET_LIVE_SCORE_URL) + "?client=ESP32&p1_name=" +
                  p1_name + "&p2_name=" + p2_name + "&current_server=" + first_server;

  socket.connect(ws_url);
}

void PollWebsocket() {
  socket.poll();
}

void CloseWebsocket() {
  if (socket.available()) {
    socket.close();
  }
}

// ----------- MESSAGE FUNCTIONS -----------
void SendWebsocketMessage_IncrementP1Score(uint8_t current_server) {
  SendWebsocketMessageAsync(String("p1+1;") + String(current_server));
}

void SendWebsocketMessage_IncrementP2Score(uint8_t current_server) {
  SendWebsocketMessageAsync(String("p2+1;") + String(current_server));
}

void SendWebsocketMessage_DecrementP1Score(uint8_t current_server) {
  SendWebsocketMessageAsync(String("p1-1;") + String(current_server));
}

void SendWebsocketMessage_DecrementP2Score(uint8_t current_server) {
  SendWebsocketMessageAsync(String("p2-1;") + String(current_server));
}

void SendWebsocketMessage_ResetGame() {
  SendWebsocketMessageAsync("reset");
}

#endif
