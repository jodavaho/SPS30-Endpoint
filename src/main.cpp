#include <iostream>
#include <libconfig.h>
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
    return 2;
  } 

  file >> *out_val;
  
  if (!file){
    std::cerr<<"Unable to read value from file:"<<location<<std::endl;
    return 3;
  }

  file.close();
  return 0;
}

int main (){
  int sleep_s;
  std::vector<std::string> endpoints;
  std::vector<std::string> keynames;
  std::vector<std::string> values;

  //I like the C API better
  config_t config; config_init(&config);
  if (config_read_file(&config, "test.config") != CONFIG_TRUE)
  {
    std::cerr<<"problem in config file!"<<std::endl;
    std::cerr
      <<config_error_file(&config)
      <<":"<<config_error_line(&config)
      <<" \""<<config_error_text(&config)<<"\""
      <<std::endl;
    return 1;
  }

  auto config_sleep = config_lookup(&config,"sleep");
  if (!config_sleep){
    std::cerr<<"'sleep' not found in config. Defaulting to 5s"<<std::endl;
    sleep_s = 5;
  } else {
    auto sleep_desired =  config_setting_get_int(config_sleep) ;
    if (sleep_desired == 0){
      sleep_desired = std::floor(config_setting_get_float(config_sleep));
    }
    std::cerr<<"Read: sleep:"<<sleep_desired<<std::endl;
    if (sleep_desired==0){
      std::cerr<<"'sleep'="<<sleep_desired<<" not valid!"<<std::endl;
      return 1;
    }
    sleep_s = sleep_desired;
  }
  

  auto config_endpoints= config_lookup(&config,"endpoints");
  if (!config_endpoints){
    std::cerr<<"endpoints not found in config file!"<<std::endl;
    return 1;
  }
  int elem_counter=0;
  config_setting_t *config_endpoint_i;
  while ( (config_endpoint_i = config_setting_get_elem(config_endpoints,elem_counter)) ){

    std::cerr<<"Setting up endpoint "<<elem_counter<<std::endl;

    auto key_name_ptr = config_setting_get_member(config_endpoint_i,"key");
    if (!key_name_ptr){
      std::cerr<<"Could not locate key name in config!"<<std::endl;
      return 1;
    }
    auto key_name = config_setting_get_string(key_name_ptr);
    if (!key_name){
      std::cerr<<"Could not locate key name in config!"<<std::endl;
      return 1;
    }
    auto endpoint_ptr = config_setting_get_member(config_endpoint_i,"node");
    if (!endpoint_ptr){
      std::cerr<<"Could not find node location in config!"<<std::endl;
      return 1;
    }
    auto node = config_setting_get_string(endpoint_ptr);
    if (!key_name){
      std::cerr<<"Could not find node location in config!"<<std::endl;
      return 1;
    }

    endpoints.push_back(node);
    keynames.push_back(key_name);
    values.push_back("0");
    
    elem_counter++;
  }
    

  config_destroy(&config);

  //Now, set up webserver to serve the data


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
    std::this_thread::sleep_for(std::chrono::seconds(sleep_s));
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
