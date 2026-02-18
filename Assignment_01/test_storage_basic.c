/*******************************************************************************
 * Advanced Database Organization - Assignment 01
 * Storage Manager Basic Tests (Student Version)
 *
 * These are BASIC tests. The autograder runs more comprehensive hidden tests.
 * Passing these tests does NOT guarantee full marks.
 *
 * Grading (hidden tests): 25 points
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "storage_mgr.h"
#include "dberror.h"
#include "test_helper.h"

char *testName;
int testsPassed = 0;
int testsFailed = 0;

#define TESTPF "test_pagefile.bin"

void cleanup(void) {
    remove(TESTPF);
}

/* Test: File operations */
void testFileOperations(void) {
    SM_FileHandle fh;

    testName = "File Operations";
    printf(">> TEST: %s\n", testName);

    cleanup();

    TEST_CHECK(createPageFile(TESTPF));
    TEST_CHECK(openPageFile(TESTPF, &fh));

    ASSERT_TRUE(strcmp(fh.fileName, TESTPF) == 0, "filename correct");
    ASSERT_TRUE(fh.totalNumPages == 1, "new file has 1 page");
    ASSERT_TRUE(fh.curPagePos == 0, "position starts at 0");

    TEST_CHECK(closePageFile(&fh));
    TEST_CHECK(destroyPageFile(TESTPF));

    ASSERT_TRUE(openPageFile(TESTPF, &fh) != RC_OK, "destroyed file should not open");

    TEST_DONE();
}

/* Test: Read and write pages */
void testReadWritePages(void) {
    SM_FileHandle fh;
    SM_PageHandle ph;

    testName = "Read/Write Pages";
    printf(">> TEST: %s\n", testName);

    cleanup();
    ph = (SM_PageHandle)malloc(PAGE_SIZE);

    TEST_CHECK(createPageFile(TESTPF));
    TEST_CHECK(openPageFile(TESTPF, &fh));

    /* Read first page - should be zeros */
    TEST_CHECK(readFirstBlock(&fh, ph));
    for (int i = 0; i < PAGE_SIZE; i++) {
        if (ph[i] != 0) {
            printf("FAILED: initial page should be zeros\n");
            testsFailed++;
            free(ph);
            return;
        }
    }
    testsPassed++;

    /* Write and read back */
    for (int i = 0; i < PAGE_SIZE; i++) {
        ph[i] = (i % 26) + 'A';
    }
    TEST_CHECK(writeBlock(0, &fh, ph));

    memset(ph, 0, PAGE_SIZE);
    TEST_CHECK(readBlock(0, &fh, ph));

    for (int i = 0; i < PAGE_SIZE; i++) {
        if (ph[i] != (i % 26) + 'A') {
            printf("FAILED: data mismatch at byte %d\n", i);
            testsFailed++;
            free(ph);
            return;
        }
    }
    testsPassed++;

    TEST_CHECK(closePageFile(&fh));
    TEST_CHECK(destroyPageFile(TESTPF));
    free(ph);

    TEST_DONE();
}

/* Test: Multiple pages (100+ pages per FPA1.docx) */
void testMultiplePages(void) {
    SM_FileHandle fh;
    SM_PageHandle ph;
    int numPages = 110;  /* More than 100 */

    testName = "Multiple Pages (100+)";
    printf(">> TEST: %s\n", testName);

    cleanup();
    ph = (SM_PageHandle)malloc(PAGE_SIZE);

    TEST_CHECK(createPageFile(TESTPF));
    TEST_CHECK(openPageFile(TESTPF, &fh));

    TEST_CHECK(ensureCapacity(numPages, &fh));
    ASSERT_EQUALS_INT(numPages, fh.totalNumPages, "should have 110 pages");

    /* Write to some pages */
    for (int p = 0; p < numPages; p += 10) {
        memset(ph, 'A' + (p % 26), PAGE_SIZE);
        *((int*)ph) = p;
        TEST_CHECK(writeBlock(p, &fh, ph));
    }

    /* Read back and verify */
    for (int p = 0; p < numPages; p += 10) {
        TEST_CHECK(readBlock(p, &fh, ph));
        int stored = *((int*)ph);
        ASSERT_EQUALS_INT(p, stored, "page number matches");
    }

    TEST_CHECK(closePageFile(&fh));
    TEST_CHECK(destroyPageFile(TESTPF));
    free(ph);

    TEST_DONE();
}

/* Test: Error handling */
void testErrorHandling(void) {
    SM_FileHandle fh;
    SM_PageHandle ph;

    testName = "Error Handling";
    printf(">> TEST: %s\n", testName);

    ph = (SM_PageHandle)malloc(PAGE_SIZE);

    /* Non-existent file */
    ASSERT_ERROR(openPageFile("nonexistent.bin", &fh), "open non-existent should fail");
    ASSERT_ERROR(destroyPageFile("nonexistent.bin"), "destroy non-existent should fail");

    /* Invalid page access */
    cleanup();
    TEST_CHECK(createPageFile(TESTPF));
    TEST_CHECK(openPageFile(TESTPF, &fh));

    ASSERT_ERROR(readBlock(100, &fh, ph), "read non-existing page should fail");
    ASSERT_ERROR(readBlock(-1, &fh, ph), "read negative page should fail");

    TEST_CHECK(closePageFile(&fh));
    TEST_CHECK(destroyPageFile(TESTPF));
    free(ph);

    TEST_DONE();
}

int main(void) {
    printf("============================================================\n");
    printf("     STORAGE MANAGER BASIC TESTS (Student Version)\n");
    printf("     Note: Hidden tests are more comprehensive!\n");
    printf("============================================================\n\n");

    initStorageManager();

    testFileOperations();
    testReadWritePages();
    testMultiplePages();
    testErrorHandling();

    cleanup();

    printf("\n============================================================\n");
    printf("Tests Passed: %d\n", testsPassed);
    printf("Tests Failed: %d\n", testsFailed);
    printf("============================================================\n");

    return (testsFailed == 0) ? 0 : 1;
}
