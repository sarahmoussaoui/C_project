#define max 6
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 

#define MAX_HEAP_SIZE 100  // Maximum size of the heap

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

// the search returns the index of the element because we need it later
int searchInHeap(MaxHeap *heap, int number){  // O(n)
  for(int i=0;i < heap->size; i++){
    if(heap->data[i] == number){
      return i;
    }
  }
  return -1;
}
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

void deleteInHeap(MaxHeap *heap, int number){
  int index;
  index = searchInHeap(heap, number); //  we need to search for it first
  int size = heap->size;
  if(index != -1){
    swap(&heap->data[size-1],&heap->data[index]);
    heap->size --;
    // keep the heap property preserved
    heapify(heap, heap->size,index);

  }
  else{
    printf("Element doesnt exist in the heap!\n");
  }
  
}

void printHeap(MaxHeap *heap) {
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->data[i]);
    }
    printf("\n");
}


int main() {
    // we initially have a heap
    int T[max];
    MaxHeap heap;
    char input[256]; // so the user can enter multiple elements at once separeted by spaces
    initMaxHeap(&heap);

    int num_elements;  // Number of elements read

    printf("Enter the elements of the heap separated by spaces: ");
    // Read a line of input
    fgets(input, sizeof(input), stdin);

    // Parse the input into the array
    num_elements = 0;
    char *token = strtok(input, " ");
    while (token != NULL && num_elements < max) {
        T[num_elements++] = atoi(token);  // Convert to integer and store in the array
        token = strtok(NULL, " ");  // Get the next token
    }



    // affecting it to the heap
    heap.size = max;
    for (int i = 0; i < max; i++) {
        heap.data[i] = T[i]; // Assign the elements of T to the heap's data
    }

    printf("This is your array before any operation:   ");
    printHeap(&heap);
    int vouloir;
    int answer, number;
    printf("Do you want to perform an operation ? Tap 1 if yes :");
    scanf("%d",&vouloir);
    while(vouloir==1){
      printf("What operation do you want to perform? :\n");
      printf("  1 for insertion \n  2 for deletion \n  3 for searching \n");
      printf("Your answer: ");
      scanf("%d",&answer);

      if(answer==1){ // inserting
        printf("What number do you want to insert? :");
        scanf("%d",&number);
        insert(&heap, number);
      }

      else if(answer==2){ // deletion
        printf("What number do you want to delete? :");
        scanf("%d",&number);
        deleteInHeap(&heap,number);
        

      }
      else if(answer==3){ // searching
        printf("What number do you want to search? :");
        scanf("%d",&number);
        int index =searchInHeap(&heap,number);
        if(index != -1){
          printf("Your element exists at index %d!",index);
        }
        else{
          printf("Your element doesnt exist! \n");
        }
      }
      else{
            printf("Invalid operation. Please choose 1, 2, or 3.\n");
            continue;  // Go back to the start of the loop if input is invalid
      }
      printf("This is your heap after your operation:   ");
      printHeap(&heap);
      printf("\nDo you still want to perform an operation ? Tap 1 if yes :");
      scanf("%d",&vouloir);

    }
    return 0;
}