import heapq
import random
#  this generate a max heap
def generate_max_heap_to_file(num_elements, file_name):
    # Generate random numbers
    elements = [random.randint(1, 20000) for _ in range(num_elements)]
    
    # Convert the list to a max heap
    # Negate values to use Python's min-heap as max-heap
    max_heap = [-elem for elem in elements]
    heapq.heapify(max_heap)
    
    # Transform back to original max-heap values
    max_heap = [-elem for elem in max_heap]
    
    # Write to file
    with open(file_name, "w") as file:
        file.write(" ".join(map(str, max_heap)))

# Specify the number of elements
num_elements = 19000
generate_max_heap_to_file(num_elements, "heap_elements.txt")

