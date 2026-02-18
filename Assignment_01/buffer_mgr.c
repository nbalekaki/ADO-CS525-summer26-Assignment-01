/*******************************************************************************
 * Advanced Database Organization - Assignment 01
 * Buffer Manager Implementation
 *
 * Student Name: [YOUR NAME]
 * Student ID:   [YOUR ID]
 *
 * INSTRUCTIONS:
 * - Implement all functions declared in buffer_mgr.h
 * - You MUST implement FIFO and LRU replacement strategies
 * - Use the Storage Manager to read/write pages to disk
 * - Do not modify the function signatures
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer_mgr.h"
#include "storage_mgr.h"
#include "dberror.h"

/******************************************************************************
 * DATA STRUCTURES
 *
 * Define your internal data structures here.
 * Suggested: Frame structure to track each buffer frame.
 ******************************************************************************/

/* Example frame structure (modify as needed):
typedef struct Frame {
    PageNumber pageNum;     // Page number stored in this frame (-1 if empty)
    char *data;             // Page data buffer
    bool dirty;             // Has the page been modified?
    int fixCount;           // Number of clients using this page
    // Add fields for replacement strategy (timestamps, etc.)
} Frame;

typedef struct BM_MgmtData {
    Frame *frames;          // Array of frames
    SM_FileHandle fh;       // File handle for the page file
    int numReadIO;          // Count of read operations
    int numWriteIO;         // Count of write operations
    // Add additional bookkeeping fields
} BM_MgmtData;
*/

/******************************************************************************
 * BUFFER POOL OPERATIONS
 ******************************************************************************/

/**
 * Initialize a buffer pool.
 *
 * @param bm Pointer to buffer pool structure to initialize
 * @param pageFileName Name of the page file to manage
 * @param numPages Number of frames in the buffer pool
 * @param strategy Replacement strategy (RS_FIFO or RS_LRU)
 * @param stratData Strategy-specific data (can be NULL)
 * @return RC_OK on success, error code on failure
 */
RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName,
                  const int numPages, ReplacementStrategy strategy,
                  void *stratData) {
    /* TODO: Implement this function
     *
     * Steps:
     * 1. Allocate management data structure
     * 2. Allocate array of frames
     * 3. Initialize each frame (allocate data buffer, set defaults)
     * 4. Open the page file using Storage Manager
     * 5. Initialize counters and bookkeeping
     * 6. Set up the BM_BufferPool fields
     */
    return RC_OK;
}

/**
 * Shutdown a buffer pool.
 * Write all dirty pages to disk and free resources.
 *
 * @param bm Pointer to buffer pool
 * @return RC_OK on success, RC_BM_PAGE_PINNED if pages still pinned
 */
RC shutdownBufferPool(BM_BufferPool *const bm) {
    /* TODO: Implement this function
     *
     * Steps:
     * 1. Check that no pages are pinned (fixCount > 0)
     * 2. Flush all dirty pages to disk
     * 3. Close the page file
     * 4. Free all allocated memory
     */
    return RC_OK;
}

/**
 * Write all dirty pages with fixCount 0 to disk.
 *
 * @param bm Pointer to buffer pool
 * @return RC_OK on success
 */
RC forceFlushPool(BM_BufferPool *const bm) {
    /* TODO: Implement this function */
    return RC_OK;
}

/******************************************************************************
 * PAGE ACCESS OPERATIONS
 ******************************************************************************/

/**
 * Mark a page as dirty.
 *
 * @param bm Pointer to buffer pool
 * @param page Page handle identifying the page
 * @return RC_OK on success
 */
RC markDirty(BM_BufferPool *const bm, BM_PageHandle *const page) {
    /* TODO: Implement this function */
    return RC_OK;
}

/**
 * Unpin a page (decrease fix count).
 *
 * @param bm Pointer to buffer pool
 * @param page Page handle identifying the page
 * @return RC_OK on success
 */
RC unpinPage(BM_BufferPool *const bm, BM_PageHandle *const page) {
    /* TODO: Implement this function */
    return RC_OK;
}

/**
 * Force a specific page to disk.
 *
 * @param bm Pointer to buffer pool
 * @param page Page handle identifying the page
 * @return RC_OK on success
 */
RC forcePage(BM_BufferPool *const bm, BM_PageHandle *const page) {
    /* TODO: Implement this function */
    return RC_OK;
}

/**
 * Pin a page into the buffer pool.
 * If the page is already in the pool, increase its fix count.
 * Otherwise, load it from disk (potentially evicting another page).
 *
 * @param bm Pointer to buffer pool
 * @param page Page handle to populate
 * @param pageNum Page number to pin
 * @return RC_OK on success, RC_BM_NO_FREE_FRAME if all pages are pinned
 */
RC pinPage(BM_BufferPool *const bm, BM_PageHandle *const page,
           const PageNumber pageNum) {
    /* TODO: Implement this function
     *
     * Steps:
     * 1. Check if page is already in buffer (search frames)
     *    - If found: increment fixCount, update page handle, update LRU info
     * 2. If not found, need to load from disk:
     *    a. Find an empty frame OR select victim using FIFO/LRU
     *    b. If victim is dirty, write it to disk first
     *    c. Read the requested page from disk
     *    d. Update frame metadata
     *    e. Set up page handle
     *
     * IMPORTANT: Update LRU timestamp on EVERY access (pin of existing page)
     */
    return RC_OK;
}

/******************************************************************************
 * STATISTICS FUNCTIONS
 ******************************************************************************/

/**
 * Get array of page numbers stored in each frame.
 * Returns NO_PAGE (-1) for empty frames.
 *
 * @param bm Pointer to buffer pool
 * @return Array of PageNumbers (caller should NOT free this)
 */
PageNumber *getFrameContents(BM_BufferPool *const bm) {
    /* TODO: Implement this function */
    return NULL;
}

/**
 * Get array of dirty flags for each frame.
 *
 * @param bm Pointer to buffer pool
 * @return Array of bools (caller should NOT free this)
 */
bool *getDirtyFlags(BM_BufferPool *const bm) {
    /* TODO: Implement this function */
    return NULL;
}

/**
 * Get array of fix counts for each frame.
 *
 * @param bm Pointer to buffer pool
 * @return Array of ints (caller should NOT free this)
 */
int *getFixCounts(BM_BufferPool *const bm) {
    /* TODO: Implement this function */
    return NULL;
}

/**
 * Get total number of read I/O operations since pool initialization.
 *
 * @param bm Pointer to buffer pool
 * @return Number of disk reads
 */
int getNumReadIO(BM_BufferPool *const bm) {
    /* TODO: Implement this function */
    return 0;
}

/**
 * Get total number of write I/O operations since pool initialization.
 *
 * @param bm Pointer to buffer pool
 * @return Number of disk writes
 */
int getNumWriteIO(BM_BufferPool *const bm) {
    /* TODO: Implement this function */
    return 0;
}
