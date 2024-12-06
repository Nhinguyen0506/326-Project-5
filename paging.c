#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

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

// Function to print memory state in table format
void printMemoryState(int memory[], int frameSize) {
    printf("| ");
    for (int i = 0; i < frameSize; i++) {
        if (memory[i] != -1) {
            printf("%2d | ", memory[i]);
        } else {
            printf(" - | ");
        }
    }
}

// FIFO Page Replacement Algorithm
void fifoPageReplacement(int pages[], int pageCount, int frameSize) {
    int memory[MAX_FRAMES];
    int front = 0, pageFaults = 0;

    // Initialize memory to -1 (empty)
    for (int i = 0; i < frameSize; i++) {
        memory[i] = -1;
    }

    printf("\n===== FIFO Algorithm ===== (Frames = %d)\n", frameSize);

    // Print table headers
    printf("| Step | Page  | Memory State      | Page Fault?  |\n");
    printf("---------------------------------------------------\n");

    for (int i = 0; i < pageCount; i++) {
        int currentPage = pages[i];
        int pageFault = 0;  // To track if there's a page fault in this step

        if (!isPageInMemory(memory, frameSize, currentPage)) {
            memory[front] = currentPage;  // Replace the oldest page
            front = (front + 1) % frameSize;
            pageFault = 1;  // Page fault occurred
            pageFaults++;
        }

        // Print the current step, page, memory state, and page fault status
        printf("| %-4d | %-5d | ", i + 1, currentPage);
        printMemoryState(memory, frameSize);
        printf("| %-12s |\n", pageFault ? "Yes" : "No");
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

    printf("\n===== LRU Algorithm ===== (Frames = %d)\n", frameSize);

    // Print table headers
    printf(" Step | Page  | Memory State      | Page Fault?  |\n");

    for (int i = 0; i < pageCount; i++) {
        int currentPage = pages[i];
        time++;
        int pageFault = 0;  // To track if there's a page fault in this step

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
            pageFault = 1;  // Page fault occurred
            pageFaults++;
        } else {
            // Update the usage time for the page
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
        printf("| %-12s |\n", pageFault ? "Yes" : "No");
    }

    printf("Total Page Faults (LRU): %d\n", pageFaults);
}

// Function to parse the page request sequence
int parsePageSequence(char input[], int pages[]) {
    int count = 0;
    char *token = strtok(input, "[, ]");
    while (token != NULL) {
        pages[count++] = atoi(token);
        token = strtok(NULL, "[, ]");
    }
    return count;
}

// Function to parse the frame sizes
int parseFrameSizes(char input[], int frames[]) {
    int count = 0;
    char *token = strtok(input, ", ");
    while (token != NULL) {
        frames[count++] = atoi(token);
        token = strtok(NULL, ", ");
    }
    return count;
}

int main() {
    int pages[MAX_PAGES], frames[MAX_FRAMES], pageCount, frameCount;
    char pageInput[256], frameInput[256];

    // Input for page request sequence
    printf("Enter the Page Request Sequence (e.g., [7, 0, 1, 2, 0, 3, ...]): ");
    fgets(pageInput, sizeof(pageInput), stdin);
    pageCount = parsePageSequence(pageInput, pages);

    // Input for frame sizes
    printf("Enter the Number of Frames (e.g., 2, 3, 4): ");
    fgets(frameInput, sizeof(frameInput), stdin);
    frameCount = parseFrameSizes(frameInput, frames);

    // Run algorithms for each frame size
    for (int i = 0; i < frameCount; i++) {
        int frameSize = frames[i];
        if (frameSize <= 0 || frameSize > MAX_FRAMES) {
            printf("Invalid frame size (%d). Skipping.\n", frameSize);
            continue;
        }

        fifoPageReplacement(pages, pageCount, frameSize);
        lruPageReplacement(pages, pageCount, frameSize);
    }

    return 0;
}
