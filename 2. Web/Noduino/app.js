'use strict';

// Server_IP는 본인 서버 실행 컴퓨터 IP 기입
// Arduino_IP는 아두이노가 연결된 공유기의 IP 기입
const SERVER_IP = '143.248.250.30';
// const ARDUINO_IP = '143.248.107.174';

// Server_Port는 80으로 유지
// UDP_Port는 아두이노 ino파일 속 localPort와 동일하게 기입 (ex. 4388)
const SERVER_PORT = 80;
// const UDP_PORT = 4388;

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
// var dgram = require('dgram');
// var udp = dgram.createSocket('udp4');


// 서버가 UDP 통신을 듣도록 함
// udp.bind(UDP_PORT, SERVER_IP, function(){
// });
//
// // 서버가 UDP 통신으로 어떠한 메시지를 받았을경우
// udp.on('message', function(message, remote) {
//     // 해당 메시지를 서버에 접속한 클라이언트가 보고있는 웹페이지로 전송
//     io.sockets.emit('message', message.toString('utf-8'));
// });


// 클라이언트가 웹페이지에 접속하였을 경우
io.on('connection', function(socket){
    console.log("[LOG] Client Connected")

    // 아두이노에게 통신이 시작되었음을 알려줌
    // udp.send("Begin", UDP_PORT, ARDUINO_IP, function(){
    // })

    // 클라이언트가 접속한 웹페이지에서 서버로 데이타가 전송되었을 경우
    socket.on('value', function(data){
        console.log(data);
        // udp.send(JSON.stringify(data), UDP_PORT, ARDUINO_IP, function(){
        // })
    });
});
