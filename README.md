# Advanced Database Organization - Assignment 01
## Disk Storage and Buffer Manager

### Overview

Implement two fundamental database components:

1. **Storage Manager**: Manages fixed-size pages on disk
2. **Buffer Manager**: Caches disk pages in memory using LRU replacement

---

### Learning Objectives

- Implement disk-based page storage
- Design and manage an in-memory buffer pool
- Apply the Least Recently Used (LRU) replacement strategy
- Safely manage dynamic memory in C
- Understand why buffer management is critical for DBMS performance

---

### System Architecture

```
   Client (Test Driver)
          |
          v
   +----------------+
   | Buffer Manager |  <- Caches pages in memory (LRU)
   +----------------+
          |
          v
   +----------------+
   | Storage Manager|  <- Reads/writes pages to disk
   +----------------+
          |
          v
      Page File (4096 bytes per page)
```

---

### Grading Rubric (100 points)

| Category | Points |
|----------|--------|
| Storage Manager correctness | 25 |
| Buffer Manager correctness | 35 |
| LRU replacement | 20 |
| Testing & robustness | 10 |
| Memory safety (Valgrind) | 10 |
| **Total** | **100** |

---

### Files to Implement

| File | Description |
|------|-------------|
| `storage_mgr.c` | Storage Manager implementation |
| `buffer_mgr.c` | Buffer Manager implementation |

**Do NOT modify** header files (`.h`) or provided helper files.

---

### Key Requirements

#### Storage Manager
- Page size: **4096 bytes**
- Create files with initial empty page
- Support reading/writing by page number
- Auto-extend file when writing new pages
- Track current page position

#### Buffer Manager
- Fixed number of frames (configurable)
- **LRU (Least Recently Used)** replacement policy
- **Pinned pages must NOT be evicted**
- **Dirty pages MUST be written back before eviction**
- Track fix counts, dirty flags, and I/O statistics

---

### Critical Implementation Notes

#### LRU Replacement
```
IMPORTANT: Update LRU timestamp on EVERY pin operation!

When a page is pinned:
- If already in buffer: UPDATE its timestamp (make it "recently used")
- If loading from disk: Set new timestamp

Common mistake: Only setting timestamp when loading from disk
```

#### Dirty Page Write-back
```
IMPORTANT: Write dirty pages to disk BEFORE evicting!

When evicting a page:
1. Check if page is dirty
2. If dirty: write to disk first
3. Then load new page into frame

Common mistake: Overwriting dirty data without saving
```

---

### Testing Requirements (per FPA1.docx)

Your implementation must handle:
- Reading and writing **at least 100 pages**
- Correct LRU eviction order
- Dirty pages written back correctly
- **Zero memory leaks** (verified with Valgrind)

---

### Building and Testing

```bash
# Build
make

# Run storage tests
make test-storage

# Run buffer tests
make test-buffer

# Run all tests
make test

# Memory check
make memcheck-storage
make memcheck-buffer

# Clean
make clean
```

---

### Submission

Submit a **ZIP file** containing only:
- `storage_mgr.c`
- `buffer_mgr.c`

**Do NOT include**: headers, test files, binaries, or other files.

---

### Common Mistakes

1. **Not updating LRU on re-pin** (costs 5-10 points)
   - Must update timestamp when pinning already-buffered page

2. **Not writing dirty pages on eviction** (costs 10-20 points)
   - Always check and write dirty pages before replacing

3. **Memory leaks** (costs 10 points)
   - Free all allocated memory in shutdown
   - Use Valgrind: `valgrind --leak-check=full ./test`

4. **Evicting pinned pages** (costs 5 points)
   - Only evict pages with fixCount == 0

5. **Not handling 100+ pages** (costs 5-10 points)
   - Test with large files, small buffer pools

---

### API Reference

#### Storage Manager
```c
void initStorageManager(void);
RC createPageFile(char *fileName);
RC openPageFile(char *fileName, SM_FileHandle *fHandle);
RC closePageFile(SM_FileHandle *fHandle);
RC destroyPageFile(char *fileName);
RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
RC appendEmptyBlock(SM_FileHandle *fHandle);
RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle);
// Plus navigation functions: readFirstBlock, readLastBlock, etc.
```

#### Buffer Manager
```c
RC initBufferPool(BM_BufferPool *bm, const char *pageFileName,
                  int numPages, ReplacementStrategy strategy, void *stratData);
RC shutdownBufferPool(BM_BufferPool *bm);
RC forceFlushPool(BM_BufferPool *bm);
RC pinPage(BM_BufferPool *bm, BM_PageHandle *page, PageNumber pageNum);
RC unpinPage(BM_BufferPool *bm, BM_PageHandle *page);
RC markDirty(BM_BufferPool *bm, BM_PageHandle *page);
RC forcePage(BM_BufferPool *bm, BM_PageHandle *page);
// Statistics: getFrameContents, getDirtyFlags, getFixCounts, getNumReadIO, getNumWriteIO
```

---

Good luck!
