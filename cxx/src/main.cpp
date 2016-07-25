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
void large_memory_mode();
void limited_memory_mode();

int main(int argc, const char * argv[]) {
  // insert code here...
  return 0;
}

void large_memory_mode(){
  std::string filename = "dump.dat";
  auto signList = util::getAllSignatures(filename);
  std::vector<std::tuple<int, aho_corasick::trie*>> trees;
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
        int id = std::get<0>(tuple);
        std::string file_string = util::getHexString(file);
        aho_corasick::trie* tree = std::get<1>(tuple);
        auto result = tree->parse_text(file_string);
      }
    }
  }
}
