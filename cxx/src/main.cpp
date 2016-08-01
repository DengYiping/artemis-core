//
//  main.cpp
//  artemis-core
//
//  Created by Scott Deng on 7/22/16.
//  Copyright © 2016 Artemis. All rights reserved.
//

#include <iostream>
#include "ahocorasick.hpp"
#include "data_access.hpp"
#include "aho_corasick.hpp"
#include <unordered_set>
#include "json.hpp"
#include <fstream>
#include <tuple>
#include "worker.hpp"
#include <thread>
#include "threadtool.h"

void limited_memory_mode();






int main(int argc, const char * argv[]) {
  // insert code here...
  limited_memory_mode();
  return 0;
}

/* run this function on large memory machine
*/



void limited_memory_mode(){
  std::string filename = "dump.dat";
  auto components = util::getAllSignatures(filename);
  std::string path;
  std::vector<std::tuple<std::string, std::string>> files;
  threadtool::Threadsafe_queue<std::tuple<int, std::vector<std::string>>> queue;

  auto callback = [](int id, int count, std::string filename, int total){
    using nlohmann::json;
    if(count > 5 && (count / (double)total) > 0.01){
      json js;
      js["id"] = id;
      js["count"] = count;
      js["file"] = filename;
      js["total"] = total;
      js["percentage"] = count / (double)total;
      std::cout<<js<<std::endl;
    }
  };

  std::thread t1(thread::thread_main, std::ref(queue), callback, std::ref(files));
  threadtool::Thread_guard g1(t1);
  std::thread t2(thread::thread_main, std::ref(queue), callback, std::ref(files));
  threadtool::Thread_guard g2(t2);
  std::thread t3(thread::thread_main, std::ref(queue), callback, std::ref(files));
  threadtool::Thread_guard g3(t3);
  std::thread t4(thread::thread_main, std::ref(queue), callback, std::ref(files));
  threadtool::Thread_guard g4(t4);

    if(queue.empty()){
      std::ifstream list_f("list");
      std::string buf;
      while(list_f.good() && !list_f.eof() && std::getline(list_f, buf)){
        files.push_back(std::make_tuple(buf, util::getHexString(buf)));
      }//read every file
      for(auto& component: components){
        queue.push(component);
      }
    }
}
