
#ifndef _KV_H
#define _KV_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include "postgres.h"
#include "kv_posix.h"
#include "access/attnum.h"


/**
 * C wrapper
 */

#ifdef VIDARDB
void* Open(char* path, bool useColumn, int attrCount);
#else
void* Open(char* path);
#endif
void Close(void* db);

uint64 Count(void* db);

void* GetIter(void* db);
void DelIter(void* it);
bool Next(void* db, void* iter, char* buffer);
bool ReadBatch(void* db, void* iter, char* buf, size_t* bufLen);

bool Get(void* db, char* key, size_t keyLen, char** val, size_t* valLen);
bool Put(void* db, char* key, size_t keyLen, char* val, size_t valLen);
bool Delete(void* db, char* key, size_t keyLen);

/* at least buf[10] is expected to pass in, return encoding length */
uint8 EncodeVarintLength(uint64 len, char* buf);

/*
 * pass in start of the buf, limit is the one after the last position of buf,
 * decoded length is stored in len, return the real encoded length.
 */
uint8 DecodeVarintLength(char* start, char* limit, uint64* len);


#ifdef VIDARDB
typedef struct RangeQueryOptions {
    size_t startLen;
    char* start;
    size_t limitLen;
    char* limit;
    int attrCount;
    AttrNumber* attrs;
    size_t batchCapacity;
} RangeQueryOptions;

void ParseRangeQueryOptions(RangeQueryOptions* queryOptions, void** range,
                            void** readOptions);

bool RangeQuery(void* db, void* range, void** readOptions, size_t* bufLen,
                void** result);

void ParseRangeQueryResult(void* result, char* buf);

void ClearRangeQueryMeta(void* range, void* readOptions);
#endif


#if defined(__cplusplus)
}
#endif

#endif
