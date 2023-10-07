//
// Created by dongdong on 9/28/23.
//
#include "vecrocks/options.h"
#include "vecrocks/vecrocks.h"
#include "v2/index_merger.h"
#include "v2/merge_insert.h"

namespace Vecrocks {
  template<typename T, typename TagT>
  int DB<T, TagT>::Open(const Options &options, const std::string &path,
                        DB **dbptr) {
    // todo @lh configurable
    diskann::DistanceL2 dist_cmp;
    diskann::Metric     metric = diskann::Metric::L2;
    diskann::Parameters* paras = new diskann::Parameters();
    paras->Set<unsigned>("L_mem", options.mem_l_index);
    paras->Set<unsigned>("R_mem", options.range);
    paras->Set<float>("alpha_mem", options.mem_alpha);
    paras->Set<unsigned>("L_disk", options.merge_l_index);
    paras->Set<unsigned>("R_disk", options.range_disk);
    paras->Set<float>("alpha_disk", options.alpha_disk);
    paras->Set<unsigned>("C", options.merge_maxc);
    paras->Set<unsigned>("beamwidth", options.beam_width);
    paras->Set<unsigned>("nodes_to_cache", options.nodes_to_cache);
    paras->Set<unsigned>("num_search_threads", options.num_search_thread);
    paras->Set<bool>("skip_disk_search", options.skip_disk_search);
    paras->Set<unsigned>("merge_th", options.max_record);

    diskann::MergeInsert<T, TagT>* merge_insert = new diskann::MergeInsert<T, TagT>(
        paras, 2, "mem", "base", "merge", &dist_cmp, metric,
        true, path);

    IdGenerator<TagT> *id_generator = new IdGenerator<TagT>(path, 100);
    *dbptr = new DB<T, TagT>(merge_insert, options.search_candidate_list_size_factor, id_generator);

    (*dbptr)->_start_merge_thread(std::chrono::milliseconds(options.merge_check_interval));
    // todo @lh flush job
    return 0;
  }

  template<typename T, typename TagT>
  TagT DB<T, TagT>::insert(const TagT id, const T *vec) {
    _merge_insert_index->insert(vec, id);
    // todo @lh save to kvstore
    return id;
  }

  template<typename T, typename TagT>
  TagT DB<T, TagT>::insert(const T *vec) {
    TagT id = _id_generator->next();
    // todo @lh save to kvstore
    _merge_insert_index->insert(vec, id);
    return id;
  }

  template<typename T, typename TagT>
  void DB<T, TagT>::delete_by_id(const TagT id) {
    _merge_insert_index->lazy_delete(id);
  }

  template<typename T, typename TagT>
  void DB<T, TagT>::search(const T *query, int topk, TagT *ids, float *dists) {
    diskann::QueryStats stats;
    _merge_insert_index->search_sync(query, topk, _candidate_list_size_factor * topk ,ids, dists, &stats);
  }

  template<typename T, typename TagT>
  void DB<T, TagT>::_start_flush_thread(std::chrono::milliseconds check_interval) {
    std::thread *merge_trigger = new std::thread([&](){
      while (1){
        if (!this->_merge_thread_running.load()){
          return ;
        }
        this->_merge_insert_index->trigger_flush();
        std::this_thread::sleep_for(check_interval);
      }
    });

    this->_merge_thread = merge_trigger;
  }
  template<typename T, typename TagT>
  DB<T, TagT>::DB(diskann::MergeInsert<T, TagT> *merge_insert_index,
     float candidate_list_size_factor, Vecrocks::IdGenerator<TagT> *id_generator) :
        _merge_insert_index(merge_insert_index), _candidate_list_size_factor(candidate_list_size_factor), _id_generator(id_generator){
    _merge_thread_running.store(true);
  }

  template<typename T, typename TagT>
  DB<T, TagT>::~DB(){
    _merge_thread_running.store(false);
    this->_merge_thread->join();

    free(_id_generator);
    free(_merge_insert_index);
  }

  template class DB<float, uint32_t>;
  template class DB<float, int64_t>;
  template class DB<float, uint64_t>;
}  // namespace Vecrocks