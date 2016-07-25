//
//  data_access.hpp
//  artemis-core
//
//  Created by Scott Deng on 7/22/16.
//  Copyright © 2016 Artemis. All rights reserved.
//

#ifndef data_access_hpp
#define data_access_hpp

#include <stdio.h>
#include <string>
#include <vector>
namespace util{
  /* @filename: pass a name of a file
   @return: return a hex string of the file
   */
  std::string getHexString(const std::string& filename);
  
  /*@folder_name: Name of the folder
   @return: relative path of the file
   */
  std::vector<std::string> getFilenameVector(std::string folder_name);
}
#endif /* data_access_hpp */
