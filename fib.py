'''
	index 0 1 2 3 4 5 6
	fib_# 0 1 1 2 3 5 8
	
	run time:
		T(n) = T(n-1) + T(n-2) + O(1)
		     = O(fib_num) = 1.618^n    exponential time
'''
def fib_recursion(n):
	if n < 2:
		return n
	return fib_recursion(n-1) + fib_recursion(n-2)

'''
	1. express what you want to solve with a recurrence relation
	2. fill out an array
        run time:
                T(n) = O(1) + (n-1)*O(1) = O(n)
'''
def fib_dynamic_programming(n):
	if n < 2:
		return n
	else:
		prev1 = 1;
		prev2 = 0;
		thisNum = 0;
		for i in range (n-1):
			thisNum = prev1+prev2
			prev2 = prev1
			prev1 = thisNum
		return thisNum

'''
def fib_matrix(n):
        
'''
print(fib_recursion(3))
print(fib_dynamic_programming(2))
print(fib_dynamic_programming(5))
