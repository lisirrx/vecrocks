# FreshDiskANN


## 1. How to install MKL in 2023

1. Go to `https://www.intel.com/content/www/us/en/developer/articles/guide/installing-free-libraries-and-python-apt-repo.html`
2. The Key `wget https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS-2019.PUB` has been expired, use `*-2023.PUB`
3. Install the version `intel-mkl-2019.4-070`

## 2. How to download dataset
Go to `http://corpus-texmex.irisa.fr/`
For example : `wget ftp://ftp.irisa.fr/local/texmex/corpus/siftsmall.tar.gz`

## 3. Startup

1. convert `fvec` format to `bin` which DiskANN uses.
```shell
./<build-dir>/tests/utils/fvecs_to_bin ../siftsmall/siftsmall_base.fvecs ../siftsmall/siftsmall_base.bin
```

2. split dataset
```shell
<type:float> <dataset size> <num_mem_indices:use 1> <insert cnt:not used> <delete cnt:not used> <cycle: 0>
./<build-dir>/tests/utils/seed_index_merger float 200 1 450 450 0 ../siftsmall/siftsmall_base.bin  ../siftsmall/dataset/siftsmall_dataset ../siftsmall/dataset/siftsmall_dataset_deleted.tags
```

You will get 2 files: `siftsmall_dataset.data` and `siftsmall_dataset.tags` which contain `dataset size` number of vectors.

3. build init disk index
```shell
build_stream_merger_disk_index float ../siftsmall/dataset/siftsmall_dataset_base.data ../siftsmall/dataset/index_2 64 75 1 4 1 ../siftsmall/dataset/siftsmall_dataset_base.tags 1
                                type                dataset                                   output prefix         R  L B M T     tags                                         single_file
```
R: out degree of graph
L: candidate list when search
B: bound on the memory footprint of the index at search time.
M: Limit on the memory allowed for building the index.
T: number of threads used by the index build process.

4. run test
```shell
test_concurr_merge_insert  float
./run_test_root/
../siftsmall/dataset/index_2_disk.index
merge_prefix
mem_prefix
75
1.0
75
1.0
../siftsmall/siftsmall_base_2.bin
1
.../siftsmall/siftsmall_query.bin
.../siftsmall/siftsmall_gt.bin
1
50
50
64
1
10
```

```shell
test_concurr_merge_insert  float
./run_test_root/
../siftsmall/dataset/index_2_disk.index
merge_prefix
mem_prefix
75 # L_mem
1.0 # alpha_mem
75 #L_disk
1.0 # alpha_disk
../siftsmall/siftsmall_base.bin
1 # singlefile
.../siftsmall/siftsmall_query.bin
.../siftsmall/siftsmall_gt.bin
1 # n_iters
50 # insert cnt
50 # delete cnt
64 # range: out degree
1 # recall
10 # search L
```