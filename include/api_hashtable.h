/// @file hashtable.h
/// @copyright BSD 2-clause. See LICENSE.txt for the complete license text
/// @author Dane Larsen
/// @brief The primary header for the hashtable library. All public functions are defined here.

#ifndef API_HASHTABLE_H
#define API_HASHTABLE_H

#include "api.h"
#include <stdint.h>
#include <stddef.h>

typedef void (HashFunc)(const void *key, int len, uint32_t seed, void *out);

#define __WITH_MURMUR

#ifdef __WITH_MURMUR

HashFunc MurmurHash3_x86_32;
HashFunc MurmurHash3_x86_128;
HashFunc MurmurHash3_x64_128;
#endif

/// The initial size of the hash table.
#ifndef HT_INITIAL_SIZE
#define HT_INITIAL_SIZE 64
#endif //HT_INITIAL_SIZE

/// The hash_entry struct. This is considered to be private
typedef struct hash_entry hash_entry;

/// The primary hashtable struct
typedef struct hash_table_struct {
    // hash function for x86_32
    HashFunc *hashfunc_x86_32;

    // hash function for x86_128
    HashFunc *hashfunc_x86_128;

    // hash function for x64_128
    HashFunc *hashfunc_x64_128;

    /// The number of keys in the hash table.
    unsigned int key_count;

    /// The size of the internal array.
    unsigned int array_size;

    /// The internal hash table array.
    hash_entry **array;

    /// A count of the number of hash collisions.
    unsigned int collisions;

    /// Any flags that have been set. (See the ht_flags enum).
    int flags;

    /// The max load factor that is acceptable before an autoresize is triggered
    /// (where load_factor is the ratio of collisions to table size).
    double max_load_factor;

    /// The current load factor.
    double current_load_factor;

}api_hashtable_t;

/// Hashtable initialization flags (passed to api_hashtable_init)
typedef enum {

    /// No options set
    HT_NONE = 0,

    /// Constant length key, useful if your keys are going to be a fixed size.
    HT_KEY_CONST = 1,

    /// Constant length value.
    HT_VALUE_CONST = 2,

    /// Don't automatically resize hashtable when the load factor
    /// goes above the trigger value
    HT_NO_AUTORESIZE = 4

} api_hashtable_flags;

//----------------------------------
// HashTable functions
//----------------------------------

/// @brief Initializes the hash_table struct.
/// @param table A pointer to the hash table.
/// @param flags Options for the way the table behaves.
/// @param max_load_factor The ratio of collisions:table_size before an autoresize is triggered
///        for example: if max_load_factor = 0.1, the table will resize if the number
///        of collisions increases beyond 1/10th of the size of the table
API void api_hashtable_init(api_hashtable_t *table, api_hashtable_flags flags, double max_load_factor
#ifndef __WITH_MURMUR
        , HashFunc *for_x86_32, HashFunc *for_x86_128, HashFunc *for_x64_128
#endif //__WITH_MURMUR
        );

/// @brief Destroys the hash_table struct and frees all relevant memory.
/// @param table A pointer to the hash table.
API void api_hashtable_destroy(api_hashtable_t *table);

/// @brief Inserts the {key: value} pair into the hash table, makes copies of both key and value.
/// @param table A pointer to the hash table.
/// @param key A pointer to the key.
/// @param key_size The size of the key in bytes.
/// @param value A pointer to the value.
/// @param value_size The size of the value in bytes.
API void api_hashtable_insert(api_hashtable_t *table, void *key, size_t key_size, void *value, size_t value_size);

/// @brief Inserts an existing hash entry into the hash table.
/// @param table A pointer to the hash table.
/// @param entry A pointer to the hash entry.
API void api_hashtable_insert_he(api_hashtable_t *table, hash_entry *entry);

/// @brief Returns a pointer to the value with the matching key,
///         value_size is set to the size in bytes of the value
/// @param table A pointer to the hash table.
/// @param key A pointer to the key.
/// @param key_size The size of the key in bytes.
/// @param value_size A pointer to a size_t where the size of the return
///         value will be stored.
/// @returns A pointer to the requested value. If the return value
///           is NULL, the requested key-value pair was not in the table.
API void* api_hashtable_get(api_hashtable_t *table, void *key, size_t key_size, size_t *value_size);

/// @brief Removes the entry corresponding to the specified key from the hash table.
/// @param table A pointer to the hash table.
/// @param key A pointer to the key.
/// @param key_size The size of the key in bytes.
API void api_hashtable_remove(api_hashtable_t *table, void *key, size_t key_size);

/// @brief Used to see if the hash table contains a key-value pair.
/// @param table A pointer to the hash table.
/// @param key A pointer to the key.
/// @param key_size The size of the key in bytes.
/// @returns 1 if the key is in the table, 0 otherwise
API int api_hashtable_contains(api_hashtable_t *table, void *key, size_t key_size);

/// @brief Returns the number of entries in the hash table.
/// @param table A pointer to the table.
/// @returns The number of entries in the hash table.
API unsigned int api_hashtable_size(api_hashtable_t *table);

/// @brief Returns an array of all the keys in the hash table.
/// @param table A pointer to the hash table.
/// @param key_count A pointer to an unsigned int that
///        will be set to the number of keys in the returned array.
/// @returns A pointer to an array of keys.
/// TODO: Add a key_lengths return value as well?
API void** api_hashtable_keys(api_hashtable_t *table, unsigned int *key_count);

/// @brief Removes all entries from the hash table.
/// @param table A pointer to the hash table.
API void api_hashtable_clear(api_hashtable_t *table);

/// @brief Calulates the index in the hash table's internal array
///        from the given key (used for debugging currently).
/// @param table A pointer to the hash table.
/// @param key A pointer to the key.
/// @param key_size The size of the key in bytes.
/// @returns The index into the hash table's internal array.
API unsigned int api_hashtable_index(api_hashtable_t *table, void *key, size_t key_size);

/// @brief Resizes the hash table's internal array. This operation is
///        _expensive_, however it can make an overfull table run faster
///        if the table is expanded. The table can also be shrunk to reduce
///        memory usage.
/// @param table A pointer to the table.
/// @param new_size The desired size of the table.
API void api_hashtable_resize(api_hashtable_t *table, unsigned int new_size);

/// @brief Sets the global security seed to be used in hash function.
/// @param seed The seed to use.
API void api_hashtable_set_seed(uint32_t seed);

#endif /* ! API_HASHTABLE_H */

