#include <iostream>
#include <fstream>
#include <string>
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

  using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
  std::unordered_map<std::string,std::string> kvpairs;

  HttpServer server;
  server.config.port=80;

  server.resource["^/$"]["GET"]=[&kvpairs](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request){

    std::stringstream ss;
    ss<<"test.txt="<<kvpairs["test.txt"]<<std::endl;

    std::cerr << request->remote_endpoint().address().to_string() << ":" << request->remote_endpoint().port()<<std::endl;
    std::cerr << request->method << " " << request->path << " HTTP/" << request->http_version <<std::endl;

    response->write(ss);
  };


  float f;

  if (get("test.txt",&f)!=0){
    std::cerr<<"error reading file!"<<std::endl;
  } else {
    kvpairs["test.txt"]=std::to_string(f);
  }
  
  std::cout<<f<<":"<<kvpairs["test.txt"]<<std::endl;

  std::promise<unsigned short> server_port;
  std::thread server_thread([&server, &server_port]() {
    // Start server
    server.start([&server_port](unsigned short port) {
      server_port.set_value(port);
    });
  });
  std::cout << "Server listening on port " << server_port.get_future().get() << std::endl;


  server_thread.join();

  return 0;
}
