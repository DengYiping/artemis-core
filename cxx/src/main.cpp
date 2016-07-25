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
  std::string filename = "dump.dat";
  auto signList = util::getAllSignatures(filename);
  std::vector<util::Ahocorasick*> acVector;
  for(auto sign:signList){
    util::Ahocorasick* ac = new util::Ahocorasick(std::get<0>(sign));
    for(auto entry: std::get<1>(sign)){
      std::cout<<"insert one: "<<entry<<std::endl;
      ac->insert(entry.c_str(), entry.size() - 1);
    }
    acVector.push_back(ac);
  }
  std::cout<<"AC number"<<std::endl;
  std::cout<<acVector.size()<<std::endl;
  std::string tmp1;
  std::cin>>tmp1;
  return 0;
}
