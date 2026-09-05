#!/bin/sh
# aedificare.sh - salve aedificare (a silice genitum)
set -e
cd "$(dirname "$0")"
mkdir -p bin
clang -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Iinclude "fontes/salve.c" "fontes/salve_regiones.c" lib/*.c -o "bin/salve"
echo "aedificatum: bin/salve"
