#include "HttpHandler.h"

HttpHandler::HttpHandler(ESP8266WebServer& server) : server(server) { }

void HttpHandler::handleRoot() {
	if (captivePortal()) {
		return;
	}
	server.send(200, "text/plain", "juicy config: root");
}

void HttpHandler::handleConfig() {
	server.send(200, "text/plain", "juicy config: config");
}

void HttpHandler::handleConfigSave() {
	server.send(200, "text/plain", "juicy config: config save");
}

void HttpHandler::handleConfigSuccess() {
	server.send(200, "text/plain", "juicy config: config success");
}

void HttpHandler::handleNotFound() {
	if(captivePortal()) {
		return;
	}
	server.send(200, "text/plain", "juicy config: not found");
}

bool HttpHandler::captivePortal() {
	bool isRequestForIPAddress = isIPAddress(server.hostHeader());
	bool isRequestForConfigPage = server.hostHeader() == "juicy.local";
	if (!isRequestForIPAddress && !isRequestForConfigPage) {
		server.sendHeader("Location", String("http://") + toString(server.client().localIP()), true);
		server.send(302, "text/plain", "");
		server.client().stop();
		return true;
	}

	return false;
}

bool HttpHandler::isIPAddress(String str) {
	for (int i = 0; i < str.length(); i++) {
    	int c = str.charAt(i);
    	if (c != '.' && (c < '0' || c > '9')) {
      		return false;
    	}
  	}
  	return true;
}

String HttpHandler::toString(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
	  res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}
