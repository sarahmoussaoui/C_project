#define max 13000
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 
#define MAX_HEAP_SIZE 15000  // Maximum size of the heap

typedef struct {
    int size;                // Current size of the heap
    int data[MAX_HEAP_SIZE]; // Array to store heap elements
} MaxHeap;

// Function to initialize a max heap
void initMaxHeap(MaxHeap *heap) {
    heap->size = 0; // Initialize size to 0
}

// Function to get the parent index
int parent(int index) {
    return (index -1) / 2; // -1 because index starts at 0 in C language 
}

// Function to get the left child index
int leftChild(int index) {
    return (2 * index + 1);
}

// Function to get the right child index
int rightChild(int index) {
    return (2 * index + 2);
}

// Function to swap two elements
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to insert an element into the min heap
void insert(MaxHeap *heap, int value) {
    if (heap->size >= MAX_HEAP_SIZE) {
        printf("Heap is full!\n");
        return;
    }
    
    // Insert the new element at the end
    heap->data[heap->size] = value;
    heap->size++;

    // Move the new element up to maintain the max heap property
    int index = heap->size - 1;
    while (index > 0 && heap->data[parent(index)] < heap->data[index]) {
        swap(&heap->data[parent(index)], &heap->data[index]);
        index = parent(index);
    }
}

// Function to extract the minimum element from the min heap
int extractMax(MaxHeap *heap) {
    if (heap->size == 0) {
        printf("Heap is empty!\n");
        return -1; // Return an invalid value
    }

    // Get the minimum value (the root of the heap)
    int max_element = heap->data[0];
    
    // Replace the root with the last element
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;

    // Move the new root down to maintain the max heap property
    int index = 0;
    while (leftChild(index) < heap->size) {
    int largerChild = leftChild(index); // Start with the left child
    // Compare right child with left child to find the larger child
    if (rightChild(index) < heap->size && 
        heap->data[rightChild(index)] > heap->data[largerChild]) {
        largerChild = rightChild(index); // Update to the right child if it's larger
    }

    // If the new root is greater than both children, break
    if (heap->data[index] >= heap->data[largerChild]) {
        break; // The max-heap property is satisfied
    }

    // Swap the new root with the larger child
    swap(&heap->data[index], &heap->data[largerChild]);
    index = largerChild; // Move down to the larger child
}

    return max_element; // Return the minimum value
}

// construct a heap using heapify --> O(n)
void heapify(MaxHeap *heap, int size, int index) {
    int largest = index; // Initialize largest as the root
    int left = leftChild(index); // Left child index
    int right = rightChild(index); // Right child index

    // Check if left child exists and is greater than the root
    if (left < size && heap->data[left] > heap->data[largest]) {
        largest = left;
    }

    // Check if right child exists and is greater than the largest so far
    if (right < size && heap->data[right] > heap->data[largest]) {
        largest = right;
    }

    // If the largest is not the root, swap and continue heapifying
    if (largest != index) {
        swap(&heap->data[index], &heap->data[largest]);
        heapify(heap, size, largest); // Recursively heapify the affected subtree
    }
}


void printHeap(MaxHeap *heap) {
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->data[i]);
    }
    printf("\n");
}

int main() {
    // we initially have an unsorted array
    int T[max];
    MaxHeap heap;
    initMaxHeap(&heap);


    int num_elements;  // Number of elements read

    // Parse the input into the array
    num_elements = 0;
    FILE *file = fopen("numbers.txt", "r");
    if (file == NULL) {
        perror("Error opening numbers.txt");
        return 1;
    }

    char input[1000000];  // Large buffer for reading file contents
    if (fgets(input, sizeof(input), file) != NULL) {  // Read the entire line
        // Parse the input into the array
        char *token = strtok(input, " ");
        while (token != NULL && num_elements < MAX_HEAP_SIZE) {
            T[num_elements++] = atoi(token);  // Convert to integer and store in the array
            token = strtok(NULL, " ");  // Get the next token
        }
    }

    fclose(file);

    setvbuf(stdout, NULL, _IOFBF, 1024 * 1024);  // Set larger buffer size because of huge heaps not printing to output.txt


    if (freopen("output.txt", "w", stdout) == NULL) {
        perror("Error opening output.txt");
        return 1;
    } // to put all my results into a file
    // step1 : inserting all elements --> MIN HEAP


    // method nlogn
    clock_t start = clock();
    for (int i = 0; i < max; i++) {  
        insert(&heap, T[i]);
    }
    clock_t end = clock();
    printf("Max Heap elements before Sorting Method nLog(n):\n ");
    printHeap(&heap);
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Execution time for nlog(n) method of construction: %f seconds\n", time_taken);

    printf("*********************\n\n\n\n\n\n\n");  


    // method 2 : O(n)
    MaxHeap heap2;
    initMaxHeap(&heap2);
    heap2.size = max;
    for (int i = 0; i < max; i++) {
        heap2.data[i] = T[i]; // Assign the elements of T to the heap's data
    }
    clock_t start2 = clock();
    for (int i = max- 1; i >= 0; i--) {
        heapify(&heap2, max,i);
    }
    clock_t end2 = clock();
    
    printf("Max Heap elements before Sorting Method O(n): \n");
    printHeap(&heap2);
    double time_taken2 = ((double)(end2 - start2)) / CLOCKS_PER_SEC;
    printf("Execution time for O(n) method of construction: %f seconds\n", time_taken2);
    
    printf("*********************\n\n\n\n\n\n\n");

    // deleting all elements successively starting from root (MIN)
    // and inserting into the free space of the array to have a final sorted array
    // we know that heap has max elements, we'll insert from  max-1 to index 0
    int i = max-1;
    int max_element;
    while(i >= 0){
      max_element = extractMax(&heap);
      heap.data[i] = max_element;
      i--;
    }
    heap.size = max;
    printf("Max Heap elements After Sorting Ascendently the heap constructed nlog(n):\n ");
    printHeap(&heap);

    i = max-1;
    while(i >= 0){
      max_element = extractMax(&heap2);
      heap2.data[i] = max_element;
      i--;
    }

    printf("*********************\n\n\n\n\n\n\n");

    heap2.size = max;
    printf("Max Heap elements After Sorting Ascendently the heap constructed O(n): \n");
    printHeap(&heap2);

    fclose(stdout);




    return 0;
}


