''' fixHeap: O(logn) makeHeap: O(n) heapSort: O(nlogn)
	
	in binary heap:
		child_i = 2*parent_i+1 or 2*parent_i+2
'''
# swap values at index a and index b
def swap(L,a,b):
	temp = L[a]
	L[a] = L[b]
	L[b] = temp

	
# fix heap property at index i
#	assumption is only value at index i is incorrect
def fixHeap(L,i):
	n = len(L)
	while i < n:
		leftChild_i = 2*i+1
		rightChild_i = 2*i+2

		if leftChild_i >= n: # no child
                    break
        elif leftChild_i == n-1:# only left child
            if L[leftChild_i] < L[i]:
                swap(L,leftChild_i,i)
                i = leftChild_i
            else: # has both childre
				if L[leftChild_i] < L[i]:
					if L[leftChild_i] <= L[rightChild_i]:
                        swap(L,leftChild_i,i)
                        i = leftChild_i
                    else:
                        swap(L,rightChild_i,i)
                        i = rightChild_i
                    elif L[rightChild_i] < L[i]:
                        swap(L,rightChild_i,i)
                        i = rightChild_i
                                
                                
L = [1,2,5,3,6]
fixHeap(L,2)
print(L)
				
	
