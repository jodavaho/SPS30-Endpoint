#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <sstream>
#include "SimpleWebServer/server_http.hpp"


int get(std::string location, float* out_val){
  std::ifstream file(location.c_str());

  if (!file){
    std::cerr<<"Error opening:"<<location<<std::endl;
    return 1;
  } 

  file >> *out_val;
  
  if (!file){
    std::cerr<<"Unable to read value from file:"<<location<<std::endl;
    return 2;
  }

  file.close();
  return 0;
}

int main (){

  std::array<std::string,4> endpoints={
    "/sys/bus/i2c/devices/1-0069/iio:device0/in_massconcentration_pm1_input",
    "/sys/bus/i2c/devices/1-0069/iio:device0/in_massconcentration_pm2p5_input",
    "/sys/bus/i2c/devices/1-0069/iio:device0/in_massconcentration_pm4_input",
    "/sys/bus/i2c/devices/1-0069/iio:device0/in_massconcentration_pm10_input",
  };

  std::array<std::string,4> keynames={
    "pm_010",
    "pm_025",
    "pm_040",
    "pm_100",
  };

  std::array<std::string,4> values={
    "0",
    "0",
    "0",
    "0",
  };

  using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
  std::unordered_map<std::string,std::string> kvpairs;

  HttpServer server;
  server.config.port=8050;

  server.resource["^/$"]["GET"]=[&keynames,&values](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request){

    std::stringstream ss;
    for (size_t i=0;i<keynames.size();i++){
      ss<<keynames[i]<<"="<<values[i]<<std::endl;
    }

    std::cerr << request->remote_endpoint().address().to_string() << ":" << request->remote_endpoint().port()<<std::endl;
    std::cerr << request->method << " " << request->path << " HTTP/" << request->http_version <<std::endl;

    response->write(ss);
  };



  std::promise<unsigned short> server_port;
  std::thread server_thread([&server, &server_port]() {
    // Start server
    server.start([&server_port](unsigned short port) {
      server_port.set_value(port);
    });
  });
  std::cout << "Server listening on port " << server_port.get_future().get() << std::endl;


  while(1){
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cerr<<"Reading ... "<<std::endl;
    for (size_t i=0;i<endpoints.size();i++){
      float f;
      if (get(endpoints[i],&f)!=0){
        std::cerr<<"error reading file!"<<std::endl;
      } else {
        values[i]=std::to_string(f);
        std::cout<<endpoints[i]<<":"<<values[i]<<std::endl;
      }
    }
  }

  return 0;
}
