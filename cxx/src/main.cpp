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
void large_memory_mode();
void limited_memory_mode();



//log all the information
inline void log(int id, int count, std::string filename, int total){
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
}








int main(int argc, const char * argv[]) {
  // insert code here...
  limited_memory_mode();
  /*
  aho_corasick::trie s;
  s.insert("hell");
  s.insert("fuck");
  std::set<std::string> match_set;
  auto result = s.parse_text("hellofuck3412asdfuck2341fuck");
  std::cout<<result.size()<<std::endl;
  for(auto each:result){
    match_set.insert(each.get_keyword());
  }
  std::cout<<match_set.size()<<std::endl;
  */
  return 0;
}

/* run this function on large memory machine
*/
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
        //log(id, count, file);
      }
    }
  }
}




void limited_memory_mode(){
  std::string filename = "dump.dat";
  auto signList = util::getAllSignatures(filename);
  std::string path;

  int tree_count = 0;


  while(1){
    std::cout<<"Ready to start"<<std::endl;
    std::cin >> path;
    std::cout<<"Scanner Started..."<<std::endl;
    std::vector<std::tuple<std::string, std::string>> files;
    std::ifstream list_f("list");
    std::string buf;

    while(list_f.good() && !list_f.eof() && std::getline(list_f, buf)){
      files.push_back(std::make_tuple(buf, util::getHexString(buf)));
    }//read every file

    for(auto sign:signList){
      tree_count = 0;
      int id = std::get<0>(sign);
      aho_corasick::trie trie;

      for(auto entry: std::get<1>(sign)){
        if(10 < entry.size() && entry.size() < 10000){
          trie.insert(entry);
          ++tree_count;
        }
      }//iterate through every signatures in a components and build a trie tree

      if(tree_count != 0){

        for(const auto& file_tu: files){
          std::string file = std::get<0>(file_tu);
          std::unordered_set<std::string> match_set;
          std::string file_string = std::get<1>(file_tu);
          auto results = trie.parse_text(file_string);
          for(auto& result: results){
            if(!result.is_empty()){
              match_set.insert(result.get_keyword());
            }
          }
          int count = match_set.size();
          log(id, count, file, tree_count);

          
        }//end of file loop
      }
    }//end of components loop
  }

}
