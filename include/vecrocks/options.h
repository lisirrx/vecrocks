//
// Created by dongdong on 9/27/23.
//
#pragma once

#include <string>

namespace Vecrocks {

  struct Options {
    // disk ann params
    std::string working_path = "./db";

    // the size of search list we maintain during index building. Typical values
    // are between 75 to 200.
    unsigned mem_l_index = 75;

    // max degree of index: larger is typically better, range (50-150).
    // Preferrably ensure that L is at least R.
    unsigned range = 50;

    // float value which determines how dense our overall graph will be, and
    // diameter will be log of n base alpha (roughly). Typical values are
    // between 1 to 1.5
    float mem_alpha = 1.2f;

    // the size of search list we maintain during index building. Typical values
    // are between 75 to 200.
    unsigned merge_l_index = 75;

    // max degree of index: larger is typically better, range (50-150).
    // Preferrably ensure that L is at least R.
    unsigned range_disk = 50;

    // float value which determines how dense our overall graph will be, and
    // diameter will be log of n base alpha (roughly). Typical values are
    // between 1 to 1.5
    float alpha_disk = 1.2f;

    // max num of candidates to consider while pruning
    unsigned merge_maxc = 125;

    // BW for search on disk_in
    unsigned beam_width = 4;

    // our program stores the entire graph on disk. For faster search
    // performance, we provide the support to cache a few nodes (which are
    // closest to the starting point) in memory.
    unsigned nodes_to_cache = 100;

    unsigned num_search_thread = 2;

    // dim of vectors
    unsigned dim;

    // mem index
    unsigned max_record = 18000000;

    bool skip_disk_search;

    unsigned search_candidate_list_size_factor = 2;

    // in millsecond
    unsigned merge_check_interval = 100;

  };

}  // namespace Vecrocks