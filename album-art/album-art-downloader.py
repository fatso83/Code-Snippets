#!/usr/bin/env python

##########################################################################
# Copyright 2009 Broadcom Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License"); 
# you may not use this file except in compliance with the License. 
# You may obtain a copy of the License at 
# 
#      http://www.apache.org/licenses/LICENSE-2.0 
#
# Unless required by applicable law or agreed to in writing, software 
# distributed under the License is distributed on an "AS IS" BASIS, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
# See the License for the specific language governing permissions and 
# limitations under the License. 
###########################################################################

# Purpose:



from __future__ import with_statement

import logging
import sys
import os
import subprocess
import mutagen
#from mutagen.easyid3 import EasyID3
from mutagen.flac import FLAC

myname = os.path.basename(sys.argv[0])

global loglevel
loglevel = logging.DEBUG
logging.basicConfig(level=loglevel, format=myname + ":%(levelname)s: %(message)s")


import re

# given a string, executes it as an executable, and returns the STDOUT
# as a string
def get_shell_cmd_output(cmd):
	try:
		proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
		return proc.stdout.read().rstrip('\n')
	except KeyboardInterrupt:
		logging.info("... interrupted")

	except Exception, e:
		logging.info(e)
		logging.error("Failed trying to execute '%s'", cmd)


def get_artwork(filename):
	if not filename.endswith(".flac"):
		print "This program is strictly intended for use with flac files" 
		exit(1)
	
	flacinfo = FLAC(filename) 
	if 'album' not in flacinfo.keys() or 'album' not in flacinfo.keys(): 
		print("Missing album or artist" + filename)
		
	artist=flacinfo["artist"][0].encode('utf-8')
	album=flacinfo["album"][0].encode('utf-8')
	print artist,album

	#cmd = "glyrc cover --artist {0} --album {1} --write cover".format(unicode(artist,'utf-8'),  unicode(album,'utf-8'))
	folder  =  os.path.dirname(os.path.realpath(filename))

	coverpath = "".join([folder, os.path.sep, "cover.jpg"])
	if os.path.isfile(coverpath): return

	cmd = "glyrc cover --artist \'{0}\' --album \'{1}\' --write \'{2}\'".format(artist,  album, coverpath)
	out = get_shell_cmd_output(cmd)
	print(out)


get_artwork(sys.argv[1])
