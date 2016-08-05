#include "worker.hpp"
#include "aho_corasick.hpp"
#include <unordered_set>

namespace thread{
  void thread_main(threadtool::Threadsafe_queue<std::tuple<int, std::vector<std::string>>>& queue,
    std::function<void(int, int, std::string, int)> callback,
    std::vector<std::tuple<std::string, std::string>>& files)
    {
      int tree_count = 0;
      while(1){
        std::tuple<int, std::vector<std::string>> component;
        queue.wait_pop(component);
        int id = std::get<0>(component);
        std::vector<std::string> signatures = std::get<1>(component);
        aho_corasick::trie trie;

        tree_count = 0;

        for(auto& signature: signatures){
          if(10 < signature.size() && signature.size() < 10000){
            trie.insert(signature);
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

            //use callback function
            callback(id, count, file, tree_count);
          }//end of file loop
      }
  }//end of main loop
}
}
