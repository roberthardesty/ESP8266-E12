#ifndef HttpComm_h
#define HttpComm_h
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


#define NODE_RED_HOME_HOST "https://newsapi.org/v1/articles?source=techcrunch&sortBy=latest&apiKey=50d43fae948b4cb4931a8681fc9e50d6"
#define NODE_RED_PORT 80

class HttpComm {
  public: 
    String defaultGET();

  private: 
    HTTPClient http;
};
#endif

