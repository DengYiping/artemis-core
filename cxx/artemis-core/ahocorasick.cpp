//
//  ahocorasick.cpp
//  artemis-core
//
//  Created by Scott Deng on 7/22/16.
//  Copyright © 2016 Artemis. All rights reserved.
//

#include "ahocorasick.hpp"
#include <queue>
#include <stack>
#include <iostream>


namespace util{
  void delete_tree(util::Node* garbage);
}


/**
 Initialize Ahocorasick dictionary tree root
 */
util::Ahocorasick::Ahocorasick(){
  root = new Node();
}

void util::Ahocorasick::insert(const char *chr, int length){
  Node* ptr = root;
  for(int i =0; i < length; ++i){
    int ascii = chr[i];
    if(ptr->child[ascii] == NULL){
      ptr->child[ascii] = new Node();
      ptr->child[ascii]->parent = ptr;
      ptr->child[ascii]->input = ascii;
    }
    ptr = ptr->child[ascii];
  }
  ptr->isLeaf = true;
}


void util::Ahocorasick::fix(){
  auto queue = std::queue<Node*>();
  //root's failure pointer NULL
  root->failure = NULL;
  
  
  queue.push(root);
  
  
  Node* node = NULL;
  while(queue.size() > 0){
    node = queue.front();
    queue.pop();
    Node* p = NULL;
    //If they have failure node
    for(int i = 0; i < CHAR_SET_SIZE; ++i){
        //go through all the possible children node(stop when hit the bottom)
      
        if(node->child[i] != NULL){
          queue.push(node->child[i]);
          p = node->failure;
          //root's children's failure pointer all point to root itself
          if(node==root){
            node->child[i]->failure=root;
          }
          else{
            //if current failure pointer has been set
            while(p){
              //if current failure node has a child that has same char
              if(p->child[i]){
                //point failure pointer there
                node->child[i]->failure = p->child[i];
                break;
              }
              //else go deeper into it's failure pointer until hit root(root->failure == NULL)
              p = p->failure;
            }
            
            //else child's failure pointer it to root
            if(p == NULL){
              node->child[i]->failure = root;
            }
          }
      }
    }
  }
}




std::vector<std::tuple<int, std::string>> util::Ahocorasick::query(const char *query, int length) const{
  Node* node = root;
  int index = 0;
  auto vector = std::vector<std::tuple<int, std::string>>();
  
  while(index < length){
    int chr = (int)query[index];
    //try to hit one until hit root
    /*
     automata state change code
     */
    while(node != root && node->child[chr] == NULL) {
      node = node->failure;
      //std::cout<<"node jump"<<std::endl;
    }
    node = node->child[chr];
    //if not hit, move back to root
    if(node == NULL){
      node = root;
    }
    

    if(node->isLeaf){
      //start recover the string
      int indexOfFind = index;
      Node* temp = node;
      auto string_stack = std::stack<char>();
      std::string string = std::string();
      while(temp != root){
        string_stack.push(temp->input);
        temp = temp->parent;
      }
      while(!string_stack.empty()){
        string.push_back(string_stack.top());
        string_stack.pop();
      }
      vector.push_back(std::tuple<int,std::string>(indexOfFind - string.size() + 1,string));
      
    }
    ++index;
  }
  return vector;
}




void util::delete_tree(util::Node* garbage){
  for(int i= 0; i < CHAR_SET_SIZE; ++i){
    if(garbage->child[i] != NULL)
      util::delete_tree(garbage->child[i]);
  }
  delete garbage;
  return;
}



util::Ahocorasick::~Ahocorasick(){
  util::delete_tree(this->root);
}


