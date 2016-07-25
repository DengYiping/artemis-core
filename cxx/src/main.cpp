e //
//  main.cpp
//  artemis-core
//
//  Created by Scott Deng on 7/22/16.
//  Copyright © 2016 Artemis. All rights reserved.
//

#include <iostream>
#include "ahocorasick.hpp"
#include "data_access.hpp"
std::vector<std::tuple<int, util::Ahocorasick*>> make_acs(std::vector<std::tuple<int, std::vector<std::string>>>& raw);
int main(int argc, const char * argv[]) {
  // insert code here...
  auto fileLists = util::getFilenameVector("../");
  std::cout<<fileLists.size()<<std::endl;
  std::string filename = "dump.dat";
  auto signList = util::getAllSignatures(filename);
  auto acs = make_acs(signList);
  std::cout<<"AC number"<<std::endl;
  std::cout<<signList.size()<<std::endl;
  std::string tmp1;
  std::cin>>tmp1;
  return 0;
}

std::vector<util::AC> make_acs(std::vector<std::tuple<int, std::vector<std::string>>> raw){
  std::vector<util::AC> result;
  for(std::tuple<int, std::vector<std::string>> row : raw){
    result.push_back(util::AC(row));
  }
  return result;
}
