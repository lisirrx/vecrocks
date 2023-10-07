//
// Created by neole on 10/5/2023.
//
#include "pq_flash_index.h"
#include "linux_aligned_file_reader.h"

int main(){
  std::shared_ptr<AlignedFileReader> reader = nullptr;
  reader.reset(new LinuxAlignedFileReader());
  diskann::PQFlashIndex<float, uint32_t> disk_index(diskann::Metric::L2, reader, false, true);

}