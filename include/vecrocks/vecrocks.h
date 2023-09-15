//
// Created by dongdong on 9/27/23.
//

#pragma once


#include "options.h"
#include "vecrocks/id_generator.h"
#include "v2/merge_insert.h"
#include <string>



namespace Vecrocks {

  constexpr int ID_STEP = 100;

  template<typename T, typename TagT>
  class DB {
   public:
    DB(diskann::MergeInsert<T, TagT> *merge_insert_index,
       float candidate_list_size_factor) :
          _merge_insert_index(merge_insert_index), _candidate_list_size_factor(candidate_list_size_factor){
      _id_generator = new IdGenerator<TagT>(ID_STEP);
    }

    ~DB(){
      free(_id_generator);
      free(_merge_insert_index);
    }

    // init the database with options
    static int Open(const Options& options, const std::string& path, DB** dbptr);

    // insert a vector to the database
    TagT Insert(TagT id, const T* vec);

    TagT Insert(const T* vec);

    // delete a vector by id
    void Delete(TagT id);

    // search the database with a query vector
    void Search(const T* query, int topK, TagT* ids, float* dists);


   private:

    void _start_merge_thread(std::chrono::milliseconds check_interval);

    float _candidate_list_size_factor;
    diskann::MergeInsert<T, TagT> *_merge_insert_index;
    Vecrocks::IdGenerator<TagT> *_id_generator;
  };

}