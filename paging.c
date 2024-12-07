#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FRAMES 10
#define MAX_PAGES 50

// Function to check if a page is in memory
bool isPageMemory(int memory[], int frameSize, int page) {
    for (int i = 0; i < frameSize; i++) {
        if (memory[i] == page) {
            return true;
        }
    }
    return false;
}

// Function to print memory state
void printMemoryState(int memory[], int frameSize) {
    printf("[");
    for (int i = 0; i < frameSize; i++) {
        if (memory[i] != -1) {
            printf("%2d", memory[i]);
        } else {
            printf(" -");
        }
        if (i < frameSize - 1) {
            printf(",");
        }
    }
    printf("] ");
}

// FIFO Page Replacement Algorithm
void fifoPageReplacement(int pages[], int pageCount, int frameSize) {
    int memory[MAX_FRAMES];
    int front = 0, pageFaults = 0;
    for (int i = 0; i < frameSize; i++) {
        memory[i] = -1;
    }
    int memoryStateWidth = frameSize * 5 + (frameSize - 1) * 2; 
    printf("\n===== FIFO Algorithm ===== (Frames = %d)\n", frameSize);
    printf("| Step | Page  | %-*s | Page Fault?  |\n", memoryStateWidth, "Memory State");
    printf("-----------------------------------------\n");

    for (int i = 0; i < pageCount; i++) {
        int currentPage = pages[i];
        int pageFault = 0;  // Track if there's a page fault in this step

        if (!isPageMemory(memory, frameSize, currentPage)) {
            memory[front] = currentPage;  // Replace the oldest page
            front = (front + 1) % frameSize;
            pageFault = 1;  // Page fault occurred
            pageFaults++;
        }
        printf("| %-4d | %-5d | ", i + 1, currentPage);
        printMemoryState(memory, frameSize);
        printf("     | %-12s\n", pageFault ? "Yes" : "No");
    }
    printf("Total Page Faults (FIFO): %d\n", pageFaults);
}

// LRU Page Replacement Algorithm
void lruPageReplacement(int pages[], int pageCount, int frameSize) {
    int memory[MAX_FRAMES];
    int usage[MAX_FRAMES];  // Track last used time for each frame
    int pageFaults = 0, time = 0;
    for (int i = 0; i < frameSize; i++) {
        memory[i] = -1;
        usage[i] = -1;
    }
    
    int memoryStateWidth = frameSize * 5 + (frameSize - 1) * 2; 
    printf("\n===== LRU Algorithm ===== (Frames = %d)\n", frameSize);
    printf("| Step | Page  | %-*s | Page Fault?  |\n", memoryStateWidth, "Memory State");
    printf("-----------------------------------------\n");

    for (int i = 0; i < pageCount; i++) {
        int currentPage = pages[i];
        time++;
        int pageFault = 0;  // Track if there's a page fault

        if (!isPageMemory(memory, frameSize, currentPage)) {
            // Find the least recently used frame
            int lruIndex = 0;
            for (int j = 1; j < frameSize; j++) {
                if (usage[j] < usage[lruIndex]) {
                    lruIndex = j;
                }
            }

            memory[lruIndex] = currentPage; 
            usage[lruIndex] = time; 
            pageFault = 1;  // Page fault occurred
            pageFaults++;
        } else {
            for (int j = 0; j < frameSize; j++) {
                if (memory[j] == currentPage) {
                    usage[j] = time;
                    break;
                }
            }
        }
        // Print the current step, page, memory state, and page fault status
        printf("| %-4d | %-5d | ", i + 1, currentPage);
        printMemoryState(memory, frameSize);
        printf("     | %-12s\n", pageFault ? "Yes" : "No");
    }

    printf("Total Page Faults (LRU): %d\n", pageFaults);
}

// Function to parse the page request sequence
int parsePageSequence(char input[], int pages[]) {
    int count = 0;
    char *token = strtok(input, "[,]");
    while (token != NULL) {
        pages[count++] = atoi(token);
        token = strtok(NULL, "[,]");
    }
    return count;
}

// Function to parse the frame sizes
int parseFrameSizes(char input[], int frames[]) {
    int count = 0;
    char *token = strtok(input, ",");
    while (token != NULL) {
        frames[count++] = atoi(token);
        token = strtok(NULL, ",");
    }
    return count;
}

int main() {
    int pages[MAX_PAGES], frames[MAX_FRAMES], pageCount, frameCount;
    char pageInput[256], frameInput[256];

    // Input for page request sequence
    printf("Enter the Page Request Sequence: ");
    fgets(pageInput, sizeof(pageInput), stdin);
    pageCount = parsePageSequence(pageInput, pages);

    // Input for frame sizes
    printf("Enter the Number of Frames:");
    fgets(frameInput, sizeof(frameInput), stdin);
    frameCount = parseFrameSizes(frameInput, frames);

    for (int i = 0; i < frameCount; i++) {
        int frameSize = frames[i];
        if (frameSize <= 0 || frameSize > MAX_FRAMES) {
            printf("Invalid frame size (%d). Skipping.\n", frameSize);
            continue;
        }
        // Run FIFO
        fifoPageReplacement(pages, pageCount, frameSize);
        // Run LRU
        lruPageReplacement(pages, pageCount, frameSize);
    }

    return 0;
}
