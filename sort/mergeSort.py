'''
	T(n) = 2T(n/2)+O(n)
	     = O(nlogn)
'''
def mergeSort(L):
	n = len(L)
	if n < 2:
		return L
	else:
		sorted_left = mergeSort(L[:n//2])
		sorted_right = mergeSort(L[n//2:])
		
		#merge
		output = []
		i = j = 0
		while i < len(sorted_left) and j < len(sorted_right):
			if sorted_left[i] < sorted_right[j]:
				output.append(sorted_left[i])
				i += 1
			else:
				output.append(sorted_right[j])
				j += 1
		
		#append the rest of the remaining list
		while i < len(sorted_left):
			output.append(sorted_left[i])
			i += 1
		while j < len(sorted_right):
			output.append(sorted_right[j])
			j += 1
		return output

print(mergeSort([1,3,43,12,1888,23]))
