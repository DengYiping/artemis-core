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













namespace util_{
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <queue>

using std::string;
using std::map;
using std::vector;
using std::queue;
using std::cin;
using std::cout;
using std::endl;

class BorNode;
typedef map<const char, BorNode *> LinksMap;


class BorNode {
public:
    LinksMap links;
    BorNode *fail;
    BorNode *term;
    int out;

public:
    BorNode(BorNode *fail_node = NULL)
        : fail(fail_node)
        , term(NULL)
        , out(-1)
    { }

    BorNode* getLink(const char c) const
    {
        LinksMap::const_iterator iter = links.find(c);
        if (iter != links.cend()) {
            return iter->second;
        }
        else {
            return NULL;
        }
    }

    bool isTerminal() const
    {
        return (out >= 0);
    }
};

class AhoCorasick
{
public:
    typedef void (*Callback) (const char* substr);
    BorNode root;
    vector<string> words;
    BorNode* current_state;

public:
    void addString(const char* const str)
    {
        BorNode *current_node = &root;
        for(const char *cp = str; *cp; ++cp) {
            BorNode *child_node = current_node->getLink(*cp);
            if (!child_node) {
                child_node = new BorNode(&root);
                current_node->links[*cp] = child_node;
            }
            current_node = child_node;
        }
        current_node->out = words.size();
        words.push_back(str);
    }

    void init()
    {
        queue<BorNode *> q;
        q.push(&root);
        while (!q.empty()) {
            BorNode *current_node = q.front();
            q.pop();
            for (LinksMap::const_iterator iter = current_node->links.cbegin();
                 iter != current_node->links.cend(); ++iter)
            {
                const char symbol = iter->first;
                BorNode *child = iter->second;


                BorNode *temp_node = current_node->fail;
                while (temp_node) {
                    BorNode *fail_candidate = temp_node->getLink(symbol);
                    if (fail_candidate) {
                        child->fail = fail_candidate;
                        break;
                    }
                    temp_node = temp_node->fail;
                }


                    if (child->fail->isTerminal()) {
                        child->term = child->fail;
                    }
                    else {
                        child->term = child->fail->term;
                    }
                q.push(child);
            }
        }
    }

    void step(const char c)
    {
        while (current_state) {
            BorNode *candidate = current_state->getLink(c);
            if (candidate) {
                current_state = candidate;
                return;
            }
            current_state = current_state->fail;
        }
        current_state = &root;
    }

    void printTermsForCurrentState(Callback callback) const
    {
        if (current_state->isTerminal()) {
            callback(words[current_state->out].c_str());
        }
        BorNode *temp_node = current_state->term;
        while (temp_node) {
            callback(words[temp_node->out].c_str());
            temp_node = temp_node->term;
        }
    }

    void search(const char* str, Callback callback)
    {
        current_state = &root;
        for(; *str; ++str) {
            cout << *str << ':' << endl;
            step(*str);
            printTermsForCurrentState(callback);
         }
    }
};
}
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
    }
    for(auto sign:signList){
      tree_count = 0;
      int id = std::get<0>(sign);
      aho_corasick::trie trie;
      for(auto entry: std::get<1>(sign)){
        if(10 < entry.size() < 10000){
          trie.insert(entry);
          ++tree_count;
        }
      }
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
        }
      }
    }
  }

}
