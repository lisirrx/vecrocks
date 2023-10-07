//
// Created by dongdong on 9/27/23.
//

#pragma once

#include "options.h"
#include "vecrocks/id_generator.h"
#include "v2/merge_insert.h"
#include <string>

namespace Vecrocks {


  template<typename T, typename TagT>
  class DB {

   public:
    DB(diskann::MergeInsert<T, TagT>* merge_insert_index,
       float candidate_list_size_factor, IdGenerator<TagT>* id_generator);
    ~DB();

    // init the database with options
    static int Open(const Options& options, const std::string& path, DB** dbptr);

    // insert a vector to the database
    TagT insert(const TagT id, const  T* vec);

    TagT insert(const T* vec);

    // delete a vector by id
    void delete_by_id(const TagT id);

    // search the database with a query vector
    void search(const T* query, int topK, TagT* ids, float* dists);


   private:


    Vecrocks::IdGenerator<TagT> *_id_generator;

    float _candidate_list_size_factor;
    diskann::MergeInsert<T, TagT> *_merge_insert_index;

    std::atomic_bool _flush_thread_running;
    std::thread *_flush_thread;
    void _start_flush_thread(std::chrono::milliseconds check_interval);

    std::atomic_bool _merge_thread_running;
    std::thread *_merge_thread;
    void _start_merge_thread(std::chrono::milliseconds check_interval);
  };

}