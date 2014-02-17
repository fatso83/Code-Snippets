#!/bin/sh
# @author Carl-Erik.Kopseng@makingwaves.no
# @date 2013-01-15
#
# Instruksjoner:
# 1. Pytt i hjemmemappa di
# 2. Åpne en terminal
# 3. Skriv følgende (ikke fnutter): "sh lag_kontonummer.sh 12345678"
# (12345678 er bare et eksempel på starten på et kontonummer)

params=$@

function main() {
  eight_digits=`first_eight $params`
  if ! ok_input; then usage; fi

  generer_kontonummer | filtrer_ut_gyldige_kontonummer
}

function ok_input() {
  len=`echo $eight_digits | wc -c | strip_space`
  if [[ $len < 8+1 ]]; then 
    return 1
  else
    return 0
  fi
}

function strip_space() {
  sed -n 's/ \{0,\}\([0-9]\{1,\}\)/\1/p'
}

function first_eight() {
echo $1 |sed -n 's/\([0-9]\{8\}\).*/\1/p'
}

function usage() {
  echo "Genererer mange kontonummer basert på ett kontonummer"
  echo "Bruk: $0 <kontonummer>"
  exit 1
}

function filtrer_ut_gyldige_kontonummer {
   grep -E '^[0-9]{11,11}$'
}

function lag_sjekksiffer {
python - $1 << EOF
import sys

def kid_mod11_wiki(a):
    cross = sum([int(val)*[2,3,4,5,6,7][idx%6] for idx,val in enumerate(list(str(a))[::-1])])
    return "%s%s" % (a,cross % 11 == 10 and '-' or 11-(cross % 11))

if __name__ == "__main__":
    print( kid_mod11_wiki(sys.argv[1]))

EOF
}

function generer_kontonummer {
  for i in `seq 10 99` ; do lag_sjekksiffer ${eight_digits}${i} ; done
}

main
