#!/usr/bin/env python3
"""tomllib_tagatum.py - plagulam TOML per tomllib (Python, bibliotheca
ordinaria) legere et ut JSON signatum toml-test reddere.

Usus: tomllib_tagatum.py <via>
       tomllib_tagatum.py -aurum <radix>   (vias ex stdin -> clausulae aurei)
Exitus: 0 + JSON una linea (valida) · 1 + 'INVALIDUM: <causa>' · 2 usus.

Oraculum secundum clientis toml (planum toml-arbor Q1): aureum
toml/probationes/fixa/tomllib/aurum.txt ex hoc generatur
(toml/tomllib_aurum.sh). Forma signata = toml-test: tabula -> obiectum,
series -> tabulatum, scalaris -> {"type": T, "value": V} (V chorda).
"""
import datetime
import json
import math
import os
import sys
import tomllib


def signare(valor):
    if isinstance(valor, dict):
        return {clavis: signare(v) for clavis, v in valor.items()}
    if isinstance(valor, list):
        return [signare(v) for v in valor]
    if isinstance(valor, bool):
        return {"type": "bool", "value": "true" if valor else "false"}
    if isinstance(valor, int):
        return {"type": "integer", "value": str(valor)}
    if isinstance(valor, float):
        if math.isnan(valor):
            textus = "nan"
        elif math.isinf(valor):
            textus = "inf" if valor > 0 else "-inf"
        else:
            textus = repr(valor)
        return {"type": "float", "value": textus}
    if isinstance(valor, str):
        return {"type": "string", "value": valor}
    if isinstance(valor, datetime.datetime):
        genus = "datetime" if valor.tzinfo is not None else "datetime-local"
        return {"type": genus, "value": valor.isoformat()}
    if isinstance(valor, datetime.date):
        return {"type": "date-local", "value": valor.isoformat()}
    if isinstance(valor, datetime.time):
        return {"type": "time-local", "value": valor.isoformat()}
    raise TypeError("genus ignotum: %r" % type(valor))


def aureum(radix):
    """-aurum: vias (una per lineam) ex stdin; pro quaque clausulam
    aurei scribit. Via relativa ad 'radix' legitur; '~' domum dicit."""
    for linea in sys.stdin:
        via = linea.rstrip("\n")
        if not via:
            continue
        plena = via
        if via.startswith("~"):
            plena = os.path.expanduser(via)
        elif not os.path.isabs(via):
            plena = os.path.join(radix, via)
        print("#### %s" % via)
        try:
            octeti = open(plena, "rb").read()
            documentum = tomllib.loads(octeti.decode("utf-8"))
        except (tomllib.TOMLDecodeError, UnicodeDecodeError) as e:
            print("## STATUS: INVALIDUM")
            print("## CAUSA: %s" % str(e).replace("\n", " "))
            print("## FINIS")
            continue
        except OSError as e:
            print("## STATUS: ABEST")
            print("## FINIS")
            continue
        print("## STATUS: VALIDUM")
        print(json.dumps(signare(documentum), sort_keys=True,
                         ensure_ascii=False))
        print("## FINIS")
    return 0


def principale(argumenta):
    if len(argumenta) == 3 and argumenta[1] == "-aurum":
        return aureum(argumenta[2])
    if len(argumenta) != 2:
        sys.stderr.write("usus: tomllib_tagatum.py <via> | -aurum <radix>\n")
        return 2
    octeti = open(argumenta[1], "rb").read()
    try:
        documentum = tomllib.loads(octeti.decode("utf-8"))
    except (tomllib.TOMLDecodeError, UnicodeDecodeError) as e:
        print("INVALIDUM: %s" % str(e).replace("\n", " "))
        return 1
    print(json.dumps(signare(documentum), sort_keys=True,
                     ensure_ascii=False))
    return 0


if __name__ == "__main__":
    sys.exit(principale(sys.argv))
