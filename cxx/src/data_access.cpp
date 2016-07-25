//
//  data_access.cpp
//  artemis-core
//
//  Created by Scott Deng on 7/22/16.
//  Copyright © 2016 Artemis. All rights reserved.
//

#include "data_access.hpp"
#include "json.hpp"
#include <fstream>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <tuple>
namespace util{
  /* 
   Some helper function
   */


  /* Convert string to hex string
   */
  std::string string_to_hex(const std::string& input)
  {
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    
    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
      const unsigned char c = input[i];
      output.push_back(lut[c >> 4]);
      output.push_back(lut[c & 15]);
    }
    return output;
  }
  
  /* Convert hex string to string
   */
  std::string hex_to_string(const std::string& input)
  {
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    if (len & 1) throw std::invalid_argument("odd length");
    
    std::string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2)
    {
      char a = input[i];
      const char* p = std::lower_bound(lut, lut + 16, a);
      if (*p != a) throw std::invalid_argument("not a hex digit");
      
      char b = input[i + 1];
      const char* q = std::lower_bound(lut, lut + 16, b);
      if (*q != b) throw std::invalid_argument("not a hex digit");
      
      output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
  }

  /*recursively get file name
   */
  void GetFileListing(std::vector<std::string> &files, std::string dir) {
    DIR *d;
    if ((d = opendir(dir.c_str())) == NULL) return;
    if (dir.at(dir.length() - 1) != '/') dir += "/";
    
    struct dirent *dent;
    struct stat st;
    
    while ((dent = readdir(d)) != NULL) {
      std::string path = dir;
      
      if (std::string(dent->d_name) != "." && std::string(dent->d_name) != "..") {
        path += std::string(dent->d_name);
        const char *p = path.c_str();
        lstat(p, &st);
        
        if (S_ISDIR(st.st_mode)) {
          GetFileListing(files, (path + std::string("/")).c_str());
        } else {
          files.push_back(path);
        }
      }
    }
    
    
    closedir(d);
  }
  
  /* get hex string from file
   */
  std::string getHexString(const std::string& filename){
    std::ifstream t(filename);
    std::string str;
    
    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    
    str.assign((std::istreambuf_iterator<char>(t)),
               std::istreambuf_iterator<char>());
    
    return string_to_hex(str);
  }
  
  /* get file list
   */
  std::vector<std::string> getFilenameVector(std::string folder_name){
    std::vector<std::string> tmp = std::vector<std::string>();
    GetFileListing(tmp, folder_name);
    return tmp;
  }
  
  std::vector<std::tuple<int, std::vector<std::string>>> getAllSignatures(std::string& filename){
    std::ifstream f(filename);
    std::string buf;
    std::vector<std::tuple<int, std::vector<std::string>>> result;
    
    while(f.good() && !f.eof() && std::getline(f,buf)){
      using nlohmann::json;
      
      if(buf.size() != 0){
        json js = buf;
        std::vector<std::string> signs;
        int name = js["name"].get<int>();
        json signs_js = js["signatures"];
        for(json::iterator it = signs_js.begin(); it != signs_js.end(); ++it){
          signs.push_back(it->get<std::string>());
        }
        result.push_back(std::make_tuple(name, signs));
      }
      
      
    }
    
    return result;
  }
  
}