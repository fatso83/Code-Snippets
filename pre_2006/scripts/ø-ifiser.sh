#!/bin/sh
#BUGS
#Funker bare på en ø av gangen
#Pga av at \x9b blir tolket som \n...

# ø å, mangler æ,Æ,Ø,Å
FRA='\x9b\x86'
TIL="øå"


sed -n '
#kan ikke bruke metakarakterer i en klasse [...].
#altså ikke [\x86\x9b]. derfor må vi ha to "if"-er og en "exit"

#if ødelagt ø, gå til main:
/\x9b/b main
#else if ødelagt å, gå til main
/\x86/b main
#else goto END
b

:main
p
	# add ./ if there is no path, only a filename
	/\//! s/^/.\//

	#hold space
	h

	# remove path
	s/.*\///


	#bytt spesialbokstavene
	y/'$FRA'/'$TIL'/

	#bytt hold og pattern space. 
	#nå er er nytt filnavn i hold space
	x

	#legg til nytt filnavn til linja, som nå 
	#inneholder /bane/Gammeltnavn\nNyttnavn
	G

	#splitt opp, og sett inn mv
	s/^\(.*\/\)\(.*\x9b.*\)\n\(.*\)$/mv "\1\2" "\1\3"/p
	#s/\(.*\)/\1/p
	#s/\(.*\x9b\)//p
'
