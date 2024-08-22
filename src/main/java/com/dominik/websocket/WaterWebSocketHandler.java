package com.dominik.websocket;

import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.client.WebSocketClient;
import org.springframework.web.socket.client.standard.StandardWebSocketClient;
import org.springframework.web.socket.handler.TextWebSocketHandler;

import java.net.URI;

public class WaterWebSocketHandler extends TextWebSocketHandler {
    @Override
    protected void handleTextMessage(WebSocketSession session, TextMessage message) throws Exception {

        if (message.getPayload().equals("WATER")) {
            WebSocketClient client = new StandardWebSocketClient();
            URI arduinoUri = new URI("ws://ipaddress:81");

            client.doHandshake(new TextWebSocketHandler() {
                @Override
                public void afterConnectionEstablished(WebSocketSession arduinoSession) throws Exception {
                    arduinoSession.sendMessage(new TextMessage("WATER"));
                    arduinoSession.close();
                }
            }, String.valueOf(arduinoUri));
            session.sendMessage(new TextMessage("Watering started"));
        }
    }
}
