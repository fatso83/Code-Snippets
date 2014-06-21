#runlength_comb.py
#A program to list all selections of size N from selection
#StartDate:2005-06-26
#Author: Carl-Erik Kopseng

#NOT WORKING
###############################################################################
selection = ['A', 'B', 'C', 'D', 'E', 'F'] 
runlength = 3

largest= selection[-1] 
lowest = selection[0]

#initiate 'run' to the lowest letter/digit in the selection
run=range(runlength)
i=0
while i<runlength:
	run[i] = lowest
	i=i+1

i=0
while i < runlength:
	
	j = 0
	while j <= i:
	
		symbol_idx = 0
		while symbol_idx < len(selection):
			if run[j] != largest:
				run[j] = selection[symbol_idx]
				symbol_idx = symbol_idx + 1
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
				while selection[tmp] != run[idx]:
					tmp = tmp + 1
				#Insert next char/value
				run[idx] = selection[tmp+1]
				print run
					
				
		j = j +1
	i = i+1
