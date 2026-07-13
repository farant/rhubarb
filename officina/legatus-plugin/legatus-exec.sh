#!/bin/bash
# legatus-exec.sh - plugin -> launcher repositorii (valida sub
# --plugin-dir, ubi plica plugin IN repositorio vivit; involucrum
# marketplace = non-propositum v0). Garrulitas aedificationis ad
# stderr; stdout purus protocollo.
exec "$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)/legatus.sh"
