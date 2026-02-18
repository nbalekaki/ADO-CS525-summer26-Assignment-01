/*******************************************************************************
 * Advanced Database Organization - Assignment 01
 * Buffer Manager Basic Tests (Student Version)
 *
 * These are BASIC tests. The autograder runs more comprehensive hidden tests.
 * Passing these tests does NOT guarantee full marks.
 *
 * Grading (hidden tests): 65 points (Buffer Manager + LRU + Robustness)
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "storage_mgr.h"
#include "buffer_mgr.h"
#include "buffer_mgr_stat.h"
#include "dberror.h"
#include "test_helper.h"

char *testName;
int testsPassed = 0;
int testsFailed = 0;

#define TESTPF "test_buffer.bin"

void cleanup(void) {
    remove(TESTPF);
}

void createTestPages(int numPages) {
    SM_FileHandle fh;
    SM_PageHandle ph = (SM_PageHandle)malloc(PAGE_SIZE);

    createPageFile(TESTPF);
    openPageFile(TESTPF, &fh);
    ensureCapacity(numPages, &fh);

    for (int i = 0; i < numPages; i++) {
        memset(ph, 0, PAGE_SIZE);
        sprintf(ph, "Page-%d", i);
        writeBlock(i, &fh, ph);
    }

    closePageFile(&fh);
    free(ph);
}

/* Test: Basic pool operations */
void testBasicPoolOps(void) {
    BM_BufferPool *bm = MAKE_POOL();

    testName = "Basic Pool Operations";
    printf(">> TEST: %s\n", testName);

    cleanup();
    createTestPages(20);

    TEST_CHECK(initBufferPool(bm, TESTPF, 5, RS_FIFO, NULL));
    ASSERT_EQUALS_INT(5, bm->numPages, "pool has 5 frames");
    ASSERT_TRUE(bm->strategy == RS_FIFO, "strategy is FIFO");

    TEST_CHECK(shutdownBufferPool(bm));

    TEST_CHECK(initBufferPool(bm, TESTPF, 3, RS_LRU, NULL));
    ASSERT_TRUE(bm->strategy == RS_LRU, "strategy is LRU");
    TEST_CHECK(shutdownBufferPool(bm));

    cleanup();
    free(bm);

    TEST_DONE();
}

/* Test: Pin and unpin */
void testPinUnpin(void) {
    BM_BufferPool *bm = MAKE_POOL();
    BM_PageHandle *h = MAKE_PAGE_HANDLE();

    testName = "Pin/Unpin Pages";
    printf(">> TEST: %s\n", testName);

    cleanup();
    createTestPages(20);
    initBufferPool(bm, TESTPF, 5, RS_FIFO, NULL);

    TEST_CHECK(pinPage(bm, h, 0));
    ASSERT_TRUE(strncmp(h->data, "Page-0", 6) == 0, "page 0 content");

    int *fixCounts = getFixCounts(bm);
    ASSERT_EQUALS_INT(1, fixCounts[0], "fix count is 1");
    free(fixCounts);

    /* Pin same page again */
    TEST_CHECK(pinPage(bm, h, 0));
    fixCounts = getFixCounts(bm);
    ASSERT_EQUALS_INT(2, fixCounts[0], "fix count is 2 after second pin");
    free(fixCounts);

    TEST_CHECK(unpinPage(bm, h));
    TEST_CHECK(unpinPage(bm, h));
    fixCounts = getFixCounts(bm);
    ASSERT_EQUALS_INT(0, fixCounts[0], "fix count is 0 after unpins");
    free(fixCounts);

    shutdownBufferPool(bm);
    cleanup();
    free(h);
    free(bm);

    TEST_DONE();
}

/* Test: Dirty page write-back (CRITICAL TEST) */
void testDirtyPages(void) {
    BM_BufferPool *bm = MAKE_POOL();
    BM_PageHandle *h = MAKE_PAGE_HANDLE();

    testName = "Dirty Page Write-back (CRITICAL)";
    printf(">> TEST: %s\n", testName);

    cleanup();
    createTestPages(20);
    initBufferPool(bm, TESTPF, 3, RS_FIFO, NULL);

    /* Pin, modify, mark dirty */
    TEST_CHECK(pinPage(bm, h, 0));
    sprintf(h->data, "MODIFIED-DATA");
    TEST_CHECK(markDirty(bm, h));
    TEST_CHECK(unpinPage(bm, h));

    /* Force eviction */
    TEST_CHECK(pinPage(bm, h, 1)); unpinPage(bm, h);
    TEST_CHECK(pinPage(bm, h, 2)); unpinPage(bm, h);
    TEST_CHECK(pinPage(bm, h, 3)); unpinPage(bm, h);  /* Evicts page 0 */

    shutdownBufferPool(bm);

    /* Verify modification persisted */
    initBufferPool(bm, TESTPF, 3, RS_FIFO, NULL);
    TEST_CHECK(pinPage(bm, h, 0));

    ASSERT_TRUE(strncmp(h->data, "MODIFIED-DATA", 13) == 0,
                "DIRTY PAGE MUST BE WRITTEN BACK ON EVICTION!");

    unpinPage(bm, h);
    shutdownBufferPool(bm);
    cleanup();
    free(h);
    free(bm);

    TEST_DONE();
}

/* Test: LRU replacement (CRITICAL TEST) */
void testLRU(void) {
    BM_BufferPool *bm = MAKE_POOL();
    BM_PageHandle *h = MAKE_PAGE_HANDLE();

    testName = "LRU Replacement (CRITICAL)";
    printf(">> TEST: %s\n", testName);

    cleanup();
    createTestPages(20);
    initBufferPool(bm, TESTPF, 3, RS_LRU, NULL);

    /* Fill buffer: 0, 1, 2 */
    pinPage(bm, h, 0); unpinPage(bm, h);
    pinPage(bm, h, 1); unpinPage(bm, h);
    pinPage(bm, h, 2); unpinPage(bm, h);

    /* Access page 0 to make it recently used */
    /* CRITICAL: This must update page 0's LRU timestamp! */
    pinPage(bm, h, 0); unpinPage(bm, h);

    /* LRU order: 1 (oldest), 2, 0 (newest) */
    /* Pin page 3 - should evict page 1, NOT page 0 */
    pinPage(bm, h, 3); unpinPage(bm, h);

    PageNumber *contents = getFrameContents(bm);
    int found0 = 0, found1 = 0;
    for (int i = 0; i < 3; i++) {
        if (contents[i] == 0) found0 = 1;
        if (contents[i] == 1) found1 = 1;
    }
    free(contents);

    ASSERT_TRUE(found0, "Page 0 should still be in buffer (was recently used)");
    ASSERT_TRUE(!found1, "Page 1 should be evicted (LRU - update timestamp on re-pin!)");

    shutdownBufferPool(bm);
    cleanup();
    free(h);
    free(bm);

    TEST_DONE();
}

/* Test: Pinned page protection */
void testPinnedProtection(void) {
    BM_BufferPool *bm = MAKE_POOL();
    BM_PageHandle *h = MAKE_PAGE_HANDLE();
    BM_PageHandle *h2 = MAKE_PAGE_HANDLE();

    testName = "Pinned Page Protection";
    printf(">> TEST: %s\n", testName);

    cleanup();
    createTestPages(20);
    initBufferPool(bm, TESTPF, 3, RS_FIFO, NULL);

    /* Fill and keep all pinned */
    pinPage(bm, h, 0);
    pinPage(bm, h, 1);
    pinPage(bm, h, 2);

    /* Should fail - all pinned */
    RC result = pinPage(bm, h2, 3);
    ASSERT_TRUE(result != RC_OK, "should fail when all frames pinned");

    /* Unpin one */
    h->pageNum = 0;
    unpinPage(bm, h);

    /* Now should work */
    TEST_CHECK(pinPage(bm, h2, 3));

    h->pageNum = 1; unpinPage(bm, h);
    h->pageNum = 2; unpinPage(bm, h);
    unpinPage(bm, h2);

    shutdownBufferPool(bm);
    cleanup();
    free(h);
    free(h2);
    free(bm);

    TEST_DONE();
}

/* Test: 100+ pages (per FPA1.docx) */
void testStress100Pages(void) {
    BM_BufferPool *bm = MAKE_POOL();
    BM_PageHandle *h = MAKE_PAGE_HANDLE();
    int numPages = 120;

    testName = "Stress Test (100+ pages)";
    printf(">> TEST: %s\n", testName);

    cleanup();
    createTestPages(numPages);
    initBufferPool(bm, TESTPF, 5, RS_LRU, NULL);

    /* Access all pages, modify each */
    for (int i = 0; i < numPages; i++) {
        TEST_CHECK(pinPage(bm, h, i));
        sprintf(h->data, "STRESS-%d", i);
        markDirty(bm, h);
        unpinPage(bm, h);
    }

    shutdownBufferPool(bm);

    /* Verify */
    initBufferPool(bm, TESTPF, 5, RS_LRU, NULL);
    int checkPages[] = {0, 50, 100, 119};
    for (int c = 0; c < 4; c++) {
        pinPage(bm, h, checkPages[c]);
        char expected[32];
        sprintf(expected, "STRESS-%d", checkPages[c]);
        ASSERT_TRUE(strncmp(h->data, expected, strlen(expected)) == 0,
                    "stress test page content");
        unpinPage(bm, h);
    }

    shutdownBufferPool(bm);
    cleanup();
    free(h);
    free(bm);

    TEST_DONE();
}

int main(void) {
    printf("============================================================\n");
    printf("     BUFFER MANAGER BASIC TESTS (Student Version)\n");
    printf("     Note: Hidden tests are more comprehensive!\n");
    printf("============================================================\n\n");

    initStorageManager();

    testBasicPoolOps();
    testPinUnpin();
    testDirtyPages();
    testLRU();
    testPinnedProtection();
    testStress100Pages();

    cleanup();

    printf("\n============================================================\n");
    printf("Tests Passed: %d\n", testsPassed);
    printf("Tests Failed: %d\n", testsFailed);
    printf("\nCRITICAL REMINDERS:\n");
    printf("  1. Update LRU timestamp on EVERY pin (including re-pins)\n");
    printf("  2. Write dirty pages to disk BEFORE evicting\n");
    printf("  3. Run Valgrind to check for memory leaks\n");
    printf("============================================================\n");

    return (testsFailed == 0) ? 0 : 1;
}
