#!/bin/bash
# probare.sh - porta natalis pythonicae (stdlib python3 systematis)
# <porta titulus="pythonica" gradus="fusio" tegit="pythonica/*.py" legit="*"/>
cd "$(dirname "$0")/.." || exit 2
python3 pythonica/probatio_silva.py
