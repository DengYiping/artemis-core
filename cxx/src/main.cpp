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
int main(int argc, const char * argv[]) {
  // insert code here...
  std::string temp = "0123456789worldsdflo123hello12340fasf40123";
  util::Ahocorasick* ac = new util::Ahocorasick();
  ac->insert("world");
  ac->insert("hello");
  ac->insert("lo123");
  ac->fix();
  
  auto querys = ac->query(temp);
  for(auto query:querys){
    std::cout<<std::get<0>(query)<<std::get<1>(query)<<std::endl;
  }
  delete ac;
  auto fileLists = util::getFilenameVector("../");
  std::cout<<fileLists.size()<<std::endl;
  for(std::string& path:fileLists){
    std::cout<<path<<std::endl;
  }
  auto signList = util::getAllSignatures(std::string("dump.dat"));
  std::cout<<signList.size()<<std::endl;  
  return 0;
}
