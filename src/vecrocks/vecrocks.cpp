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
    diskann::Parameters paras;
    paras.Set<unsigned>("L_mem", options.mem_l_index);
    paras.Set<unsigned>("R_mem", options.range);
    paras.Set<float>("alpha_mem", options.mem_alpha);
    paras.Set<unsigned>("L_disk", options.merge_l_index);
    paras.Set<unsigned>("R_disk", options.range_disk);
    paras.Set<float>("alpha_disk", options.alpha_disk);
    paras.Set<unsigned>("C", options.merge_maxc);
    paras.Set<unsigned>("beamwidth", options.beam_width);
    paras.Set<unsigned>("nodes_to_cache", options.nodes_to_cache);
    paras.Set<unsigned>("num_search_threads", options.num_search_thread);
    paras.Set<bool>("skip_disk_search", options.skip_disk_search);
    paras.Set<unsigned>("merge_th", options.max_record);

    diskann::MergeInsert<T, TagT> merge_insert(
        paras, 2, "mem", "base", "merge", &dist_cmp, metric,
        true, options.working_path);

    *dbptr = new DB<T, TagT>(&merge_insert, options.search_candidate_list_size_factor);

    (*dbptr)->_start_merge_thread(std::chrono::milliseconds(options.merge_check_interval));

    return 0;
  }
  template<typename T, typename TagT>
  TagT DB<T, TagT>::Insert(TagT id, const T *vec) {
    _merge_insert_index->insert(vec, id);
    // todo @lh save to kvstore
    return id;
  }
  template<typename T, typename TagT>
  TagT DB<T, TagT>::Insert(const T *vec) {
    TagT id = _id_generator->next();
    // todo @lh save to kvstore
    _merge_insert_index->insert(vec, id);
    return id;
  }
  template<typename T, typename TagT>
  void DB<T, TagT>::Delete(TagT id) {
    _merge_insert_index->lazy_delete(id);
  }
  template<typename T, typename TagT>
  void DB<T, TagT>::Search(const T *query, int topk, TagT *ids, float *dists) {
    _merge_insert_index->search_sync(query, topk, _candidate_list_size_factor * topk ,ids, dists);
  }

  template<typename T, typename TagT>
  void DB<T, TagT>::_start_merge_thread(std::chrono::milliseconds check_interval) {
    std::thread merge_trigger([&](){
      while (1){
        this->_merge_insert_index->trigger_merge();
        std::this_thread::sleep_for(check_interval);
      }
    });
  }
}  // namespace Vecrocks