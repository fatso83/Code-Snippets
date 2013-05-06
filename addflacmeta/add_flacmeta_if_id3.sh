#!/bin/bash
# Adds FLAC metadata to flac files that mistakenly have had added ID3 tags
# Known bugs: doesn't handle file names with consecutive spaces
#				e.g. "a  b.flac will mistakenly become "a b.flac" and thus result
# i		Generally not good with spaces, really. Limitation of BASH?
#					 in a "Not found error"
# First version: 2010-11-01
# Latest revision: 2010-11-07

OUT=${OUT:-/dev/null}

function usage()
{
	echo "Changes FLAC-files that lack proper metadata in the FLAC file"
	echo "by adding them based on any existing ID3v1 tags"
	echo "Usage: $0 name_of_dir_to_fix_tags"
	echo "Usage: $0 -f name_of_file_to_fix"
	exit -1
}

function fixify()
{
	FILE=$@ #All parameters starting from $1"
	if [[ ! -e "$FILE" ]]; then
		echo "No such file \"$FILE\""
		exit -1
	fi

	if has_id3 "$FILE"; then
		if has_meta "$FILE"; then
			echo "Has meta!"
			return
		else
			add_meta "$FILE"
		fi
	fi
}

function add_meta()
{
	echo Adding meta to "$@"
}

#returns 0 if file has ID3
#returns 1 else
function has_id3()
{
	#echo Tester fil: "$@"
	id3v2 -l "$@" |grep "No ID3 tag"	> "$OUT"
	status=$?
	if [[ $status = 0 ]]; then
		return 1; #no id3
	else
		return 0;  #has id3
	fi
}

function has_meta()
{
	metaflac --show-tag=TITLE "$@"|grep TITLE > "$OUT"
	return $?
}

#TESTS

TEST_has_id3_is_true_on_id3_file()
{
	echo -n "$FUNCNAME" : 
	if has_id3 ID3.flac; then
		echo .
	else
		echo FAIL
	fi
}

TEST_has_id3_is_false_on_no-id3_file()
{
	echo -n "$FUNCNAME" :
	if has_id3 NO_ID3.flac; then
		echo FAIL
	else
		echo .
	fi
}


run_tests()
{
	TEST_has_id3_is_true_on_id3_file
	TEST_has_id3_is_false_on_no-id3_file
}

#Testoppsett
if [[ x$TEST = xTRUE ]]; then
	run_tests
	exit
fi

#if number of arguments are less than 1, print the usage message
[[ $# < 1 ]] && usage;

#Single file use or directory scan?
if [[ $1 == '-f' ]]; then 
	shift 1
	#echo "Running fixify! with \$@=$@"
	fixify "$@"
else
	#Testing all flac files to check for missing metadata
	DIR="$@"
	if [[ ! -d "$DIR" ]]; then
		echo No such directory $DIR!
		exit -1
	fi

	find "$@" -type f -iname \*.flac -exec "$0" -f {} \;
fi

