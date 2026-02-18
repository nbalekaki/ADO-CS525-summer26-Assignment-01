/*******************************************************************************
 * Advanced Database Organization - Assignment 01
 * Storage Manager Implementation
 *
 * Student Name: [YOUR NAME]
 * Student ID:   [YOUR ID]
 *
 * INSTRUCTIONS:
 * - Implement all functions declared in storage_mgr.h
 * - Do not modify the function signatures
 * - Use the provided dberror.h return codes
 * - Test your implementation using the provided test files
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage_mgr.h"
#include "dberror.h"

/******************************************************************************
 * INITIALIZATION
 ******************************************************************************/

/**
 * Initialize the storage manager.
 * Called once when the system starts.
 */
void initStorageManager(void) {
    /* TODO: Implement initialization if needed */
}

/******************************************************************************
 * FILE OPERATIONS
 ******************************************************************************/

/**
 * Create a new page file with a single empty page.
 * The file should be PAGE_SIZE bytes (4096 bytes).
 * All bytes in the initial page should be zero.
 *
 * @param fileName Name of the file to create
 * @return RC_OK on success, error code on failure
 */
RC createPageFile(char *fileName) {
    /* TODO: Implement this function */
    return RC_OK;
}

/**
 * Open an existing page file.
 * Populate the file handle with file information.
 *
 * @param fileName Name of the file to open
 * @param fHandle Pointer to file handle to populate
 * @return RC_OK on success, RC_FILE_NOT_FOUND if file doesn't exist
 */
RC openPageFile(char *fileName, SM_FileHandle *fHandle) {
    /* TODO: Implement this function */
    return RC_FILE_NOT_FOUND;
}

/**
 * Close an open page file.
 *
 * @param fHandle Pointer to file handle
 * @return RC_OK on success, error code on failure
 */
RC closePageFile(SM_FileHandle *fHandle) {
    /* TODO: Implement this function */
    return RC_OK;
}

/**
 * Delete a page file from disk.
 *
 * @param fileName Name of the file to delete
 * @return RC_OK on success, RC_FILE_NOT_FOUND if file doesn't exist
 */
RC destroyPageFile(char *fileName) {
    /* TODO: Implement this function */
    return RC_FILE_NOT_FOUND;
}

/******************************************************************************
 * READ OPERATIONS
 ******************************************************************************/

/**
 * Read a specific page from the file into memory.
 *
 * @param pageNum The page number to read (0-indexed)
 * @param fHandle Pointer to file handle
 * @param memPage Buffer to store the page data (must be PAGE_SIZE bytes)
 * @return RC_OK on success, RC_READ_NON_EXISTING_PAGE if page doesn't exist
 */
RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
    /* TODO: Implement this function */
    return RC_READ_NON_EXISTING_PAGE;
}

/**
 * Get the current page position.
 *
 * @param fHandle Pointer to file handle
 * @return Current page position
 */
int getBlockPos(SM_FileHandle *fHandle) {
    /* TODO: Implement this function */
    return 0;
}

/**
 * Read the first page (page 0).
 */
RC readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    /* TODO: Implement this function */
    return RC_READ_NON_EXISTING_PAGE;
}

/**
 * Read the previous page relative to current position.
 */
RC readPreviousBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    /* TODO: Implement this function */
    return RC_READ_NON_EXISTING_PAGE;
}

/**
 * Read the current page.
 */
RC readCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    /* TODO: Implement this function */
    return RC_READ_NON_EXISTING_PAGE;
}

/**
 * Read the next page relative to current position.
 */
RC readNextBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    /* TODO: Implement this function */
    return RC_READ_NON_EXISTING_PAGE;
}

/**
 * Read the last page.
 */
RC readLastBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    /* TODO: Implement this function */
    return RC_READ_NON_EXISTING_PAGE;
}

/******************************************************************************
 * WRITE OPERATIONS
 ******************************************************************************/

/**
 * Write a page to the file.
 *
 * @param pageNum The page number to write (0-indexed)
 * @param fHandle Pointer to file handle
 * @param memPage Buffer containing the page data
 * @return RC_OK on success, RC_WRITE_FAILED on failure
 */
RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
    /* TODO: Implement this function */
    return RC_WRITE_FAILED;
}

/**
 * Write to the current page position.
 */
RC writeCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    /* TODO: Implement this function */
    return RC_WRITE_FAILED;
}

/**
 * Append an empty page to the end of the file.
 *
 * @param fHandle Pointer to file handle
 * @return RC_OK on success, error code on failure
 */
RC appendEmptyBlock(SM_FileHandle *fHandle) {
    /* TODO: Implement this function */
    return RC_WRITE_FAILED;
}

/**
 * Ensure the file has at least numberOfPages pages.
 * If the file has fewer pages, append empty pages.
 *
 * @param numberOfPages Minimum number of pages required
 * @param fHandle Pointer to file handle
 * @return RC_OK on success, error code on failure
 */
RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle) {
    /* TODO: Implement this function */
    return RC_OK;
}
