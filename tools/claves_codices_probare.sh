#!/bin/sh
# claves_codices_probare.sh - tabulam CODICES_LITTERARUM contra
# auctoritatem systematis (Carbon kVK_ANSI_*) probare.
#
# CUR SEORSUM ET NON IN SUITA: tabula in lib/fenestra_macos.m
# NUMERIS scripta est, non constantibus Carbon, et hoc consulto -
# Carbon ab Apple iam pridem deprecatum est, et arbor haec saecula
# spectat. Codices ipsi ABI sunt: ab anno MCMLXXXIV immoti, et
# mutari non possunt sine omni agitatore claviaturae frangendo.
#
# Ergo periculum non est mutatio, sed TRANSCRIPTIO mea. Hoc scriptum
# eam semel probat et probabilem manere facit. Si Carbon aliquando
# evanescet, hoc scriptum solum perit - tabula manet.
#
# Usus: ./tools/claves_codices_probare.sh   (0 = congruunt)

set -e
cd "$(dirname "$0")/.."

# SOLAE lineae numerorum: 'i32' in declaratione ipsa '32' ferret, et
# numerus superfluus tabulam pravam simularet. (Deprehensum ab hoc
# ipso scripto, prima vice quo cucurrit - custodia numeri egit.)
TABULA=$(sed -n '/kVK_ANSI_\* pro a\.\.z/,/};/p' lib/fenestra_macos.m \
    | grep '^[[:space:]]*[0-9][0-9,[:space:]]*$' \
    | grep -o '[0-9]\+' | tr '\n' ',' | sed 's/,$//')

if [ -z "$TABULA" ]; then
    echo "claves: tabulam in lib/fenestra_macos.m invenire non potui" >&2
    exit 2
fi

OPUS=$(mktemp -d)
trap 'rm -rf "$OPUS"' EXIT

cat > "$OPUS/probatio.m" <<EOF
#include <Carbon/Carbon.h>
#include <stdio.h>
int main(void){
  int mei[] = {$TABULA};
  int veri[26] = {kVK_ANSI_A,kVK_ANSI_B,kVK_ANSI_C,kVK_ANSI_D,kVK_ANSI_E,
    kVK_ANSI_F,kVK_ANSI_G,kVK_ANSI_H,kVK_ANSI_I,kVK_ANSI_J,kVK_ANSI_K,
    kVK_ANSI_L,kVK_ANSI_M,kVK_ANSI_N,kVK_ANSI_O,kVK_ANSI_P,kVK_ANSI_Q,
    kVK_ANSI_R,kVK_ANSI_S,kVK_ANSI_T,kVK_ANSI_U,kVK_ANSI_V,kVK_ANSI_W,
    kVK_ANSI_X,kVK_ANSI_Y,kVK_ANSI_Z};
  int i, malae = 0;
  int n = (int)(sizeof(mei)/sizeof(mei[0]));
  if (n != 26) { printf("tabula XXVI litteras non habet (%d)\n", n); return 2; }
  for (i = 0; i < 26; i++) {
    if (mei[i] != veri[i]) {
      printf("DISCREPAT %c: tabula=%d systema=%d\n", 'a'+i, mei[i], veri[i]);
      malae++;
    }
  }
  printf("%s (%d discrepantiae ex XXVI)\n",
         malae ? "PRAVA" : "OMNES CONGRUUNT", malae);
  return malae ? 1 : 0;
}
EOF

clang "$OPUS/probatio.m" -o "$OPUS/probatio"
"$OPUS/probatio"
