#!/bin/sh

#PRESET="16 "
DESTDIR=.
WORKING_FOLDER=$1

if test "x$PRESET" = "x"; then
  PRESET="fast extreme"
fi

if test "x$WORKING_FOLDER" = "x"; then
  WORKING_FOLDER=.
fi

cd "$WORKING_FOLDER"

if ! ls *.flac  > /dev/null 2>&1 ;then echo "NO FLAC" > /dev/stderr; exit 1;fi

for a in *.flac
do
  
      OUTF=`echo ".banshee-dap-$a" | sed s/\.flac/.mp3/g`

      echo
      echo "Source=`pwd`/$a Destination=$DESTDIR/$OUTF"
      echo

#hva hvis det ikke er noe metablokk? kanskje det er lagret som id3v1!
      ARTIST=`metaflac "$a" --show-tag=ARTIST | sed s/.*=//g`
      TITLE=`metaflac "$a" --show-tag=TITLE | sed s/.*=//g`
      ALBUM=`metaflac "$a" --show-tag=ALBUM | sed s/.*=//g`
      GENRE=`metaflac "$a" --show-tag=GENRE | sed s/.*=//g`
      TRACKNUMBER=`metaflac "$a" --show-tag=TRACKNUMBER | sed s/.*=//g`
      YEAR=`metaflac "$a" --show-tag=DATE | sed s/.*=//g | cut -b -4`

      echo
      echo "Launching: flac -c -d $a | lame --preset $PRESET - $DESTDIR/$OUTF"
      echo

      flac -c -d "$a" | lame --preset $PRESET - "$DESTDIR/$OUTF"

      echo
      echo "Setting id3 ($TITLE, $TRACKNUMBER, $ARTIST, $ALBUM, $GENRE, $YEAR)"
      echo

      if test "x$TITLE" != "x"; then
        id3 -t "$TITLE" "$DESTDIR/$OUTF" > /dev/null
      fi

      if test "x$TRACKNUMBER" != "x"; then
        id3 -T "$TRACKNUMBER" "$DESTDIR/$OUTF" > /dev/null
      fi

      if test "x$ARTIST" != "x"; then
        id3 -a "$ARTIST" "$DESTDIR/$OUTF" > /dev/null
      fi

      if test "x$ALBUM" != "x"; then
       id3 -A "$ALBUM" "$DESTDIR/$OUTF" > /dev/null
      fi

      if test "x$GENRE" != "x"; then
        id3 -g "$GENRE" "$DESTDIR/$OUTF"
      fi

      if test "x$YEAR" != "x"; then
        id3 -y "$YEAR" "$DESTDIR/$OUTF"
      fi
done
