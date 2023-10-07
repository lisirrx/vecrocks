//
// Created by neole on 9/6/2023.
//
#include "v2/index_merger.h"
#include "v2/merge_insert.h"
#include "vecrocks/id_generator.h"
#include "vecrocks/vecrocks.h"
int main() {



      diskann::DistanceL2 dist_cmp;
        diskann::Metric               metric = diskann::Metric::L2;
        diskann::Parameters paras;
        paras.Set<unsigned>("L_mem", 50);
        paras.Set<unsigned>("R_mem", 50);
        paras.Set<float>("alpha_mem", 1.0);
        paras.Set<unsigned>("L_disk", 50);
        paras.Set<unsigned>("R_disk",50);
        paras.Set<float>("alpha_disk", 1.0);
        paras.Set<unsigned>("C", 750);
        paras.Set<unsigned>("beamwidth", 0);
        paras.Set<unsigned>("nodes_to_cache", 10);
        paras.Set<unsigned>("num_search_threads", 2);
        paras.Set<bool>("skip_disk_search", true);
        paras.Set<unsigned>("merge_th", 1);


        diskann::MergeInsert<float, uint32_t> merge_insert(
            &paras, 2, "mem_prefix", "base_prefix", "merge_prefix", &dist_cmp,
            metric, true, "./working");

        float a[2] {1.0, 0.0};
        float b[2] {1.0, 1.0};
        merge_insert.insert(a, 1);
        merge_insert.insert(b, 2);

        merge_insert.trigger_flush();
        float q[2] {0.0, 0.0};
        uint32_t tag[10];
        float distance[10];
        diskann::QueryStats queryStats;
        merge_insert.search_sync(q, 1, 5, tag, distance, &queryStats);

        printf("%f\n", distance[0]);
        printf("%d\n", tag[0]);
  }
  // namespace diskann