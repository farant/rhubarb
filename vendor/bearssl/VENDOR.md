# BearSSL - vendored

- Fons: https://www.bearssl.org/git/BearSSL (clonus ../bearssl)
- Commissio: 7bea48e5e850ab4cafbe68d3765cdaba13a86d6f (master, 2026-04-06,
  v0.6-39-g7bea48e) - "Fixed bug in handling incoming records with invalid
  length"
- Licentia: MIT (LICENSE.txt), Thomas Pornin
- Arbor: `git archive` totus (CDLXVI fasciculi), octetim idem, numquam
  manu mutatus. Renovatio = directorium totum substituere, haec plagula
  renovanda, `tools/bearssl_amalgamare.sh`, porta `bearssl`.
- Amalgama: vendor/bearssl_amalgama.c + .h (GENERATUM,
  tools/bearssl_amalgamare.py - titulus proprius, non bearssl.h:
  instrumenta silvae capita per basename resolvunt); compilata laxe
  (aedilis.stml <vendor>, compile_tests.sh VENDOR_FLAGS).
- Summa: SHA-256 super lineas `shasum -a 256` omnium fasciculorum
  (praeter hanc), ordine LC_ALL=C (tools/bearssl_probare.sh -summa).

summa: 6d653367387b8e6608adf67badd5c63529635ccb9b9beb12b41909999a6a76b5
