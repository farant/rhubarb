# Unicode 15.1.0 — provenientia

Fetched ONCE on 2026-09-28 from `https://www.unicode.org/Public/15.1.0/ucd/`
(Fran's OK: runae plan D3), checked in so nothing depends on the network
or rots. The licence is `LICENSE.txt` (Unicode License V3, from
`https://www.unicode.org/license.txt`), fetched the same day.

Version 15.1.0 was chosen to match the local oracle ICU4C 74.2 (plan
D2). A version bump is its own task: a new directory beside this one,
`RUNAE_VERSIO` in `include/runae.h`, and `./tools/runae_generare.sh`.

| file | upstream path | used by |
|---|---|---|
| DerivedGeneralCategory.txt | `extracted/` | width (U2) |
| EastAsianWidth.txt | root | width (U2) |
| DerivedCoreProperties.txt | root | width (DI), graphemes (InCB, U4) |
| GraphemeBreakProperty.txt | `auxiliary/` | width (RI, V, T, Prepend), graphemes (U4) |
| emoji-data.txt | `emoji/` | width (Emoji_Modifier), graphemes (Extended_Pictographic, U4) |
| GraphemeBreakTest.txt | `auxiliary/` | the U4 conformance oracle |

SHA-256:

```
f55d0db69123431a7317868725b1fcbf1eab6b265d756d1bd7f0f6d9f9ee108b  DerivedCoreProperties.txt
760720ac034f96b630a3055879a744e0907184e8aa811e89ba34583a7a487e85  DerivedGeneralCategory.txt
b08191401dc125f4e84ef262a95754faae6b737c79538e17ea9664a63434e94e  EastAsianWidth.txt
a7e52eee647e52dc210b8719b4d7037276f4b353810293d69377fc46374cec3f  GraphemeBreakProperty.txt
ed9c5e92fd0911ccbeeb63c97cb19c519ea272ff1112ce843abd991582dd848f  GraphemeBreakTest.txt
e7a93b009565cfce55919a381437ac4db883e9da2126fa28b91d12732bc53d96  LICENSE.txt
d7aef489c8fe4c14f09ea5695200277c6b93ac82ac60845cdd2161b0d6835cc1  emoji-data.txt
```
