//
//  ahocorasick.hpp
//  artemis-core
//
//  Created by Scott Deng on 7/22/16.
//  Copyright © 2016 Artemis. All rights reserved.
//

#ifndef ahocorasick_hpp
#define ahocorasick_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <tuple>
#define CHAR_SET_SIZE 128
namespace util{
  
  
  struct Node{
    Node* child[CHAR_SET_SIZE];
    Node* parent;
    Node* failure;
    char input;
    bool isLeaf;
    Node(){
      for(int i = 0; i < CHAR_SET_SIZE; ++i){
        child[i] = NULL;
      }
      failure = NULL;
      parent = NULL;
      input = NULL;
      isLeaf = false;
    }
  };
  
  
  
  class Ahocorasick{
  private:
    Node* root;
  public:
    Ahocorasick();
    
    //insert function
    void insert(const char* chr, int length);
    inline void insert(const std::string& string){
      insert(string.c_str(),string.size());
    }
    
    //make ahocorasick automata
    void fix();
    
    //find query
    std::vector<std::tuple<int, std::string>> query(const char* query, int length) const;
    inline std::vector<std::tuple<int, std::string>> query(const std::string string) const{
      return query(string.c_str(), string.size());
    }
    
    ~Ahocorasick();
  };
}



#endif /* ahocorasick_hpp */
