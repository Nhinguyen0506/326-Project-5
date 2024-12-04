#include <stdio.h>
#include <stdbool.h>

#define MAX_FRAMES 10
#define MAX_PAGES 50

// Function to check if a page is in memory
bool isPageInMemory(int memory[], int frameSize, int page) {
    for (int i = 0; i < frameSize; i++) {
        if (memory[i] == page) {
            return true;
        }
    }
    return false;
}

// Function to print memory state
void printMemoryState(int memory[], int frameSize) {
    printf("[ ");
    for (int i = 0; i < frameSize; i++) {
        if (memory[i] != -1) {
            printf("%d ", memory[i]);
        } else {
            printf("- ");
        }
    }
    printf("]\n");
}

// FIFO Page Replacement Algorithm
void fifoPageReplacement(int pages[], int pageCount, int frameSize) {
    int memory[MAX_FRAMES];
    int front = 0, pageFaults = 0;

    // Initialize memory to -1 (empty)
    for (int i = 0; i < frameSize; i++) {
        memory[i] = -1;
    }

    printf("\n===== FIFO Algorithm =====\n");

    for (int i = 0; i < pageCount; i++) {
        int currentPage = pages[i];

        if (!isPageInMemory(memory, frameSize, currentPage)) {
            memory[front] = currentPage;  // Replace the oldest page
            front = (front + 1) % frameSize;
            pageFaults++;
            printf("Page %d caused a fault: ", currentPage);
        } else {
            printf("Page %d found in memory: ", currentPage);
        }

        printMemoryState(memory, frameSize);
    }

    printf("Total Page Faults (FIFO): %d\n", pageFaults);
}

// LRU Page Replacement Algorithm
void lruPageReplacement(int pages[], int pageCount, int frameSize) {
    int memory[MAX_FRAMES];
    int usage[MAX_FRAMES];  // Track last used time for each frame
    int pageFaults = 0, time = 0;

    // Initialize memory and usage arrays to -1
    for (int i = 0; i < frameSize; i++) {
        memory[i] = -1;
        usage[i] = -1;
    }

    printf("\n===== LRU Algorithm =====\n");

    for (int i = 0; i < pageCount; i++) {
        int currentPage = pages[i];
        time++;

        if (!isPageInMemory(memory, frameSize, currentPage)) {
            // Find the least recently used frame
            int lruIndex = 0;
            for (int j = 1; j < frameSize; j++) {
                if (usage[j] < usage[lruIndex]) {
                    lruIndex = j;
                }
            }

            memory[lruIndex] = currentPage;  // Replace the least recently used page
            usage[lruIndex] = time;          // Update the usage time
            pageFaults++;
            printf("Page %d caused a fault: ", currentPage);
        } else {
            // Update the usage time for the page
            for (int j = 0; j < frameSize; j++) {
                if (memory[j] == currentPage) {
                    usage[j] = time;
                    break;
                }
            }
            printf("Page %d found in memory: ", currentPage);
        }

        printMemoryState(memory, frameSize);
    }

    printf("Total Page Faults (LRU): %d\n", pageFaults);
}

int main() {
    int pages[MAX_PAGES], pageCount, frameSize;

    // Input
    printf("Enter the number of pages (max %d): ", MAX_PAGES);
    scanf("%d", &pageCount);

    if (pageCount <= 0 || pageCount > MAX_PAGES) {
        printf("Invalid number of pages. Exiting.\n");
        return 1;
    }

    printf("Enter the page reference sequence: ");
    for (int i = 0; i < pageCount; i++) {
        scanf("%d", &pages[i]);
    }

    printf("Enter the number of page frames (max %d): ", MAX_FRAMES);
    scanf("%d", &frameSize);

    if (frameSize <= 0 || frameSize > MAX_FRAMES) {
        printf("Invalid number of frames. Exiting.\n");
        return 1;
    }

    // Run FIFO algorithm
    fifoPageReplacement(pages, pageCount, frameSize);

    // Run LRU algorithm
    lruPageReplacement(pages, pageCount, frameSize);

    return 0;
}