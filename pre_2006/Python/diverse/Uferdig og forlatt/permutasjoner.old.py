#runlength_comb.py
#A program to list all selections of size N from sel
#Date 2005-06-26
#Author: Carl-Erik Kopseng


###############################################################################
#Variable declaration

#the letters/digits to be manipulated (whatever!)
sel = ['A', 'B', 'C', 'D', 'E', 'F'] 
#sel = ['1', '0' ]
runlength = 5

#For comparison
largest= sel[-1] 
lowest = sel[0]
next = lowest


#loop counters
i = j = k = 0

run=[]

#initiate 'run' to the lowest letter/digit in the selection
while i < runlength:
	run.append(lowest)
	i = i + 1
###############################################################################

#main body
i = 0
while i < runlength:
	
	j = 0
	while j <= i:
	
		k = 0
		while k < len(sel):
			if run[j] != largest:
				run[j] = sel[k]
				k = k + 1
				print run
			else:
				#Check for specials like CFFF
				#Becomes DAAA
				idx = j+1
				while run[idx] == largest:
					idx = idx + 1
					

				# lowest i run[0]-run[j]
				x = 0
				while x < idx:
					run[x] = lowest
					x = x + 1
				#Get current value of run[idx]
				tmp = 0
				while sel[tmp] != run[idx]:
					tmp = tmp + 1
				#Insert next char/value
				run[idx] = sel[tmp+1]
				print run
					
				
		j = j +1
	i = i+1
