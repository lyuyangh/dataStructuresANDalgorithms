'''
	O(n^2)
'''
def selectionSort(L):
	sortedList = []
	minNum = L[0]
	minIndex = 0
	while len(L) != 0:
		for i in range(len(L)):
			if L[i] < min_num:
				minNum = L[i]
				minIndex = i
		sortedList.append(L.pop(minIndex))
	return L