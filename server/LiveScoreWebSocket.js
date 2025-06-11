const ws_connections = {};
const ws_data = {};

function HandleKeepAlivePings(ws) {
  const interval = setInterval(() => {
    if (ws.readyState === ws.OPEN) ws.ping();
  }, 30000);
  
  ws.on('close', () => {
    clearInterval(interval);
  });
}

function HandleESP32ClientConnected(ws, req) {
  ws_data['p1_name'] = req.query.p1_name;
  ws_data['p2_name'] = req.query.p2_name;

  if (!ws_data['p1_name']) {
    return ws.close(1000, 'Missing p1_name query property in ESP32 client request');
  } else if (!ws_data['p2_name']) {
    return ws.close(1000, 'Missing p2_name query property in ESP32 client request');
  }

  ws_data['p1_score'] = 0;
  ws_data['p2_score'] = 0;
  
  // If the Web client is already connected, send the initial data to it (this is for sending the names mostly; the score should be 0-0 when the ESP32 connects).
  if (ws_connections['Web']) {
    ws_connections['Web'].send(JSON.stringify({ p1_name: ws_data['p1_name'], p2_name: ws_data['p2_name'], p1_score: ws_data['p1_score'], p2_score: ws_data['p2_score'] }));
  }

  ws.on('close', () => {
    console.log("ESP32 client disconnected");
    ws_connections['Web']?.send("ESP32 client disconnected");
    delete ws_connections['ESP32'];
    delete ws_data['p1_name'];
    delete ws_data['p2_name'];
    delete ws_data['p1_score'];
    delete ws_data['p2_score'];
  });
}

function HandleWebClientConnected(ws) {
  // The Web client will receive the initial data from the ESP32 client if the ESP32 client is already connected.
  // If the ESP32 client is not connected, the data will be sent once the ESP32 client connects.
  if (ws_connections['ESP32']) {
    ws.send(JSON.stringify({ p1_name: ws_data['p1_name'], p2_name: ws_data['p2_name'], p1_score: ws_data['p1_score'], p2_score: ws_data['p2_score'] }));
  }
  
  ws.on('close', () => {
    console.log("Web client disconnected");
    delete ws_connections['Web'];
  });
}

function HandleIncomingESP32Message(msg, ws) {
  // Messages can only be received from the ESP32 client.
  if (msg === 'p1+1') {
    ws_data['p1_score']++;
  } else if (msg === 'p2+1') {
    ws_data['p2_score']++;
  } else if (msg === 'p1-1') {
    ws_data['p1_score'] = Math.max(0, ws_data['p1_score'] - 1);
  } else if (msg === 'p2-1') {
    ws_data['p2_score'] = Math.max(0, ws_data['p2_score'] - 1);
  } else if (msg === 'reset') {
    delete ws_data['p1_name'];
    delete ws_data['p2_name'];
    delete ws_data['p1_score'];
    delete ws_data['p2_score'];
  } else {
    return ws.close(1000, 'Invalid message. Must be "p1+1", "p2+1", "p1-1", "p2-1", or "reset"');
  }

  // Forward the ESP32 message to the Web client
  if (ws_connections['Web']) {
    ws_connections['Web']?.send(msg);
  }
}

export default function LiveScoreWebSocket(ws, req) {
  HandleKeepAlivePings(ws);

  // --- 'WHEN CLIENT FIRST CONNECTING' ---
  const client_name = req.query.client;
  console.log("Client connected:", client_name);
  if (client_name !== 'ESP32' && client_name !== 'Web') return ws.close(1000, 'Invalid client name. Must be "ESP32" or "Web"');
  ws_connections[client_name] = ws;

  if (client_name === 'ESP32') {
    HandleESP32ClientConnected(ws, req);
  } else if (client_name === 'Web') {
    HandleWebClientConnected(ws);
  }
  // --- END 'WHEN FIRST CONNECTING' ---

  ws.on('message', (msg) => {
    if (client_name !== 'ESP32') return ws.close(1000, 'Only the ESP32 can send messages to the server');
    HandleIncomingESP32Message(msg, ws)
  });
}
