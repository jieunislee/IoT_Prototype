'use strict';

// Server_IP는 본인 서버 실행 컴퓨터 IP 기입
  const SERVER_IP = '143.248.109.186';






// Server_Port는 80으로 유지
const SERVER_PORT = 80;

const fs = require('fs');
const express = require('express');
const app = express();

// 서버를 실행 (https://143.248.109.205:80으로 접속)
const server = require('http').createServer(app);
server.listen(SERVER_PORT, function(){
});

// 서버에 접속한 클라이언트에게 index.html 파일을 전송하여 렌더링 해줌
app.use(express.static(__dirname));
app.get('/', function(req, res){
    res.sendFile(__dirname + '/index.html');
});

const io = require('socket.io').listen(server);


// 클라이언트가 웹페이지에 접속하였을 경우
io.on('connection', function(client){
    console.log("[LOG] Client Connected");
    console.log(client.id); // 클라이언트 ID가 필요한 경우 사용

    // 아두이노 클라이언트에서 웹으로 데이터가 전송된 경우
    client.on('fromarduino', function(data){
        console.log(client.id + "/ Received: " + JSON.stringify(data));
        io.sockets.emit('toweb', data);
    });

    // 웹 클라이언트에서 웹으로 아두이노로 전송된 경우
    client.on('fromweb', function(data){
        console.log(client.id + "/ Sending: " + JSON.stringify(data));
        io.sockets.emit('toarduino', data);
    });
});
