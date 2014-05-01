# -*- encoding: utf-8 -*-
# permutasjoner.py
#2006-08-05 16:54:53 
import sys
def permute(list, runlength=2):
	"""Prints all permutations of a list"""

	largest= selection[-1] 
	lowest = selection[0]

	run=range(runlength)
	i=0
	while i<runlength:
		run[i] = lowest
		i=i+1


	print run
	i=0
	sel_idx=0
	while i < runlength:
		if run[i] == largest:
			i=i+1
			continue
		else:
			sel_idx=selection.index(run[i])+1
			run[i] = selection[sel_idx]
			#sett alle mindre siffer til laveste verdi
			while i>0:
				i=i-1
				run[i]=lowest

		#reverse a copy of the list for left-to-right printing
		revlist=[]
		revlist.extend(run)
		revlist.reverse()
		print revlist


if __name__ == "__main__":
	if len(sys.argv) > 1:
	#Behandle argumentene. Første argument er runlength. Resten er listen.
	    runlength = int(sys.argv[1])
	else:
	    runlength = 2

	if len(sys.argv) > 2:
		selection=[]
        	for elem in sys.argv[2:]:
	            selection.append(elem)
	else:
	    selection = ['A', 'B', 'C', 'D', 'E', 'F'] 
  	
	permute(selection, runlength)
