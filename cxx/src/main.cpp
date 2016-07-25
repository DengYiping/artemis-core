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
#include <set>
#include "json.hpp"
void large_memory_mode();
void limited_memory_mode();

void log(int id, int count){
  using nlohmann::json;
  json js;
  js["id"] = id;
  js["count"] = count;
  std::cout<<js<<std::endl;
}

int main(int argc, const char * argv[]) {
  // insert code here...
  limited_memory_mode();
  return 0;
}

void large_memory_mode(){
  std::string filename = "dump.dat";
  auto signList = util::getAllSignatures(filename);
  std::vector<std::tuple<int, aho_corasick::trie*>> trees;
  
  
  
  //build all the trees
  for(auto sign:signList){
    int id = std::get<0>(sign);
    aho_corasick::trie* trie = new aho_corasick::trie();
    for(auto entry: std::get<1>(sign)){
      trie->insert(entry);
    }
    trees.push_back(std::make_tuple(id,trie));
  }
  
  
  
  
  std::string path;
  while(1){
    std::cin >> path;
    std::vector<std::string> files = util::getFilenameVector(path);
    for(std::string file: files){
      for(std::tuple<int,aho_corasick::trie*> tuple: trees){
        std::set<std::string> match_set;
        int id = std::get<0>(tuple);
        std::string file_string = util::getHexString(file);
        aho_corasick::trie* tree = std::get<1>(tuple);
        auto results = tree->parse_text(file_string);
        for(auto result: results){
          if(!result.is_empty()){
            match_set.insert(result.get_keyword());
          }
        }
        int count = match_set.size();
        log(id, count);
      }
    }
  }
}




void limited_memory_mode(){
  std::string filename = "dump.dat";
  auto signList = util::getAllSignatures(filename);
  std::string path;
  
  while(1){
    std::cin >> path;
    std::vector<std::string> files = util::getFilenameVector(path);
    for(auto sign:signList){
      int id = std::get<0>(sign);
      aho_corasick::trie trie;
      for(auto entry: std::get<1>(sign)){
        trie.insert(entry);
      }
      for(std::string file: files){
        std::set<std::string> match_set;
        std::string file_string = util::getHexString(file);
        auto results = trie.parse_text(file_string);
        for(auto& result: results){
          if(!result.is_empty()){
            match_set.insert(result.get_keyword());
          }
        }
        int count = match_set.size();
        log(id, count);
      }
    }
  }
  
}
