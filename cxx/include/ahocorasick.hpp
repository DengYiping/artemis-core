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

    //make ahocorasick automata
    void fix();

    //find query
    std::vector<std::tuple<int, std::string>> query(const char* query, int length) const;

    ~Ahocorasick();
  };


  class AC{
  public:
    int id;
    Ahocorasick* ac;
    AC(int id_, Ahocorasick* ac_){
      id = id_;
      ac = ac_;
    }

    AC(std::tuple<int, std::vector<std::string>>& raw){
      id = std::get<0>(raw);
      std::vector<std::string> sign_strings = std::get<1>(raw);
      ac = new util::Ahocorasick();
      for(std::string& s:sign_strings){
        if(s.size()){
          ac->insert(s.c_str(), s.size());
        }
      }
      ac->fix();
    }
    ~AC(){
      delete ac;
    }
  };
}



#endif /* ahocorasick_hpp */
