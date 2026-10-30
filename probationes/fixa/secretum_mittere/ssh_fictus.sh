#!/bin/bash
# ssh_fictus.sh - ssh FICTUM pro probatio_secretum_mittere.c: servum
# remotum in directorio $STUB_RADIX simulat. Argumenta omnia in $STUB_LOG
# scribuntur (probatio asserit clavem numquam ibi esse). Mandatum
# remotum (argumentum ultimum): 'sudo -n ' tollitur, '/var/lib/' in
# $STUB_RADIX/var/lib/ vertitur, 'install -o X -g Y' sine possessore
# (non radix) agitur; sha256sum per shasum (macOS). $STUB_CORRUMPERE:
# post installationem octetus additur (probatio 'differt').
set -u
printf '%s\n' "$*" >> "$STUB_LOG"
mandatum="${!#}"
m="${mandatum//sudo -n /}"
m="${m//\/var\/lib\//$STUB_RADIX/var/lib/}"
m="$(printf '%s' "$m" | sed -E 's/install -o [^ ]+ -g [^ ]+ /install /; s/^sha256sum /shasum -a 256 /')"
case "$m" in
    install*)
        dest="${m##* }"
        mkdir -p "$(dirname "$dest")"
        ;;
esac
eval "$m"
rc=$?
if [ -n "${STUB_CORRUMPERE:-}" ] && [ "${m%% *}" = "install" ]; then
    printf 'x' >> "${m##* }"
fi
exit $rc
