heap.c : contains inserting, researching and deleting elements procedures.
heap_sort.c : transforms a given array T having max elements into a Max-heap using 2 algorithms :
  1- O( nlog(n) ) --> inserting each time then re-arranging to maintaing max heap property
  2- O(n) --> using heapify method ( children to root)
  at last sorts the 2 resulted heaps by deleting successively and inserting at end of heap after reducing its size
  result : array sorted ascendant order.
  
