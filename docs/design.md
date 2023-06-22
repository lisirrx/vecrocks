# Vecrocks Docs

Vecrocks is built on Rocksdb and faiss. We use RockDB as the storage backend, and build vector index by faiss.

To simplify the implementation, here are some assumptions.

1. The data modal we supported is like a table with 3 columns: `[id int64, vec float[], tag string]`,
   and the database have a collection named DEFAULT, with an index of the `vec` column named DEFAULT.
2. Only support FLAT index as an in-memory index.(But store the index in rocksdb for rebuilding every time).
3. Use json as the serde lib.

## 0. Example

You can run the code in [example.cpp](../example/example.cpp)
Let's say we have four vectors and a vector for lookup.

```c++
float a[2] = {0, 1.0};
float b[2] = {0, 2.0};
float c[2] = {0, -2.0};
float d[2] = {0, -1.0};
```

And create a vecrocks db object.

```c++
auto db = new Vecrocks::DB(DIM);
db->Init(PATH);
```

We can put these vectors into the db.

```c++
db->Put(1, a, "First");
db->Put(2, b, "Second");
db->Put(3, c, "Third");
db->Put(4, d, "Forth");
```

And get some of them

```c++
auto res = db_for_read->Get(1);
// Get the (0, 1) vec
assert(res->vec[0] == 0.);
assert(res->vec[1] == 1.0);
```

We can search the most `k` similar vector of a vec q

```c++
int k = 2;
float q[2] = {0, 0};

auto search_result = db->Search(q, k);

// search_result have 2 elements, one is the float a[] with id 1, another is the float d[] with id 4
```

We can delete the `1` and `4` row, and search top 2 similar vector again.

```c++
db->Del(1);
db->Del(4);

search_result = db->Search(q, k);
// now we get the row of 2 and 3
```

## 1. Storage Layout and Index

To support a `collection - field - value` structure like [milvus](https://milvus.io/docs/create_collection.md),
We designed a method for using KV store to represent the database model, which is inspired
by [TiDB](https://cn.pingcap.com/blog/tidb-internal-2).

When we use Rocksdb's seek by prefix feature, we can get an ordered map sorted by key.
So if we design the key carefully, all keys with same prefix can by accessed quickly.

For example:
We can store our raw data by such key.

`t_c${collection_name}_pk${id} -> value`

If we want to locate a specific key, we simply construct it by concat a string.

The important thing is how to build index. We can design the index like this:

`t_c${collection_name}_idx${idx_name}_v${encoded_vec}_${id} -> null`

If we want to get all id by a vector, we can just build a key prefix like `t_cDEFAULT_idxDEFAULT_[1.0, 1.0]_`,
and use this prefix to seek in the rocksdb.

Actually, the key should
be [memcomparable](https://github.com/facebook/mysql-5.6/wiki/MyRocks-record-format#memcomparable-format),
but due to time limited, we simply serialize the key as a string.

## 2. Unfinished features

- [ ] full metadata support
- [ ] concurrent operation(read/write)
- [ ] different index support(how to persistent IVF index to RocksDB)
