# QR Library Worklog

## 2026-01-14: Initial Implementation

Created qr.h/qr.c implementing QR code generation for the mobile pairing use case.

### Implementation Notes

**Phases Completed:**
- Phase 1-3: Byte mode encoding, all ECC levels (L/M/Q/H), versions 1-10, full mask optimization

**Key Design Decisions:**

1. **Signedness**: Internal coordinate functions use `s32` (signed) because algorithms use negative offsets (e.g., finder pattern placement at `-3` to `+3` from center). Public API uses `i32` (unsigned). Casts at boundaries.

2. **Finder Pattern Logic**: The Chebyshev distance approach for finder patterns:
   - dist 0, 1: black (3x3 center)
   - dist 2: white (inner ring)
   - dist 3: black (outer ring)
   Initial implementation had this inverted (fixed).

3. **GF(256) Tables**: Using primitive polynomial 0x11D (x^8 + x^4 + x^3 + x^2 + 1). Tables initialized lazily on first use.

4. **Reed-Solomon**: Generator polynomial computed on-the-fly using temp arena. ECC codewords appended after data codewords.

5. **Mask Selection**: All 8 masks evaluated, penalty score computed (simplified - rules 1,2,4 but not full rule 3), lowest score wins.

**Known Limitations:**
- Versions 11-40 not tested (table only goes to V10)
- Mixed-mode encoding not implemented (always byte mode)
- No ECI mode support

**Testing:**
- 66 unit tests passing in probatio_qr.c
- Visual verification via probatio_qr_visual.c shows scannable codes

**Future Work:**
- Implement numeric mode (3 digits → 10 bits) for efficiency
- Implement alphanumeric mode (2 chars → 11 bits)
- Consider adding PNG/BMP export for delineare integration

---

## 2026-01-14: Added Alignment Patterns (V2+)

Added alignment pattern support for versions 2-10, making QR codes fully compliant with ISO/IEC 18004.

**Changes:**
- Added `_alignment_positiones` lookup table with center positions per version
- Added `_ponere_alignment()` - places single 5x5 alignment pattern (black outer, white ring, black center)
- Added `_ponere_alignment_patterns()` - places all alignment patterns for a version, skipping finder overlaps
- Updated `_est_reservatum()` to mark alignment pattern areas as reserved during data placement

**Pattern Structure (5x5):**
- dist 0: black (center)
- dist 1: white (ring)
- dist 2: black (outer)

**Alignment Pattern Positions (V2-V10):**
- V2: (18,18) - 1 pattern
- V3: (22,22) - 1 pattern
- V4: (26,26) - 1 pattern
- V5: (30,30) - 1 pattern
- V6: (34,34) - 1 pattern
- V7: 6 patterns at combinations of (6,22,38)
- V8: 6 patterns at combinations of (6,24,42)
- V9: 6 patterns at combinations of (6,26,46)
- V10: 6 patterns at combinations of (6,28,50)

Note: Patterns at corner positions that would overlap finders are skipped.

---

## 2026-08-01: Portatum in rhubarb + TRES defectus inventi oraculo

Portatum ex mobile-rhubarb pro QR pairing (desideratum 01KYYY01RY:
telephonum URL+clavem scannat). Portatio ipsa MUNDA: nullae
mutationes, examen ACCIPE, LXVI casus praeterierunt statim.

**Sed bibliotheca fracta erat, et probationes id videre NON
POTERANT.** Omnes LXVI casus STRUCTURALES sunt (amplitudines,
versiones, formae inventorii). Nullus BITS legit. Codex QR cum
formis perfectis et datis corruptis omnes praeteriret - quod
exacte accidebat.

### Oraculum (quod deerat)

probationes/probatio_qr_gyrus.sh: qr.c BMP scribit, CoreImage
(Apple, de codice nostro nescium) decodat. Textus decodatus
ingresso aequet. Independens, non se-consistens.

Experimentum REGIMINIS primum: CoreImage QR SUUM generavit et
relegit - ne oraculo falso fideremus. (Bene factum: duo defectus
MEI ante hoc apparuerunt - numerus ObjC non-JS, et ansa i32
descendens quae numquam finivit et imaginem 5.9 GB scripsit. i32
INSIGNATUS est; 'y >= 0' semper verum.)

### Tres defectus, singuli a limite versionis revelati

**I. data_codewords ex tabula CAPACITATIS.** `_capacitas_octeti`
capacitatem NUNTII fert (characteres), non codewords. Differentia
= caput modi+numeri: II octeti V1-V9, III pro V10 (mensuratum per
omnes XL compositiones). ECC ergo in offset falso sedebat.

**II. INTERTEXTIO BLOCCORUM absens.** Nullum vestigium bloccorum
in tota plagula. Codewords data+ECC per bloccos intertexenda sunt
(ISO 18004 §8.6); appensio simplex recta est bloco UNO solo.
Limes mensuratus: V1-V3 (gradu M) legebantur, V4+ non - exacte
ubi bloci plures fiunt.

**III. INFORMATIO VERSIONIS absens (V7+).** Post I+II emendatos,
V1-V6 legebantur et V7+ non. Versiones VII-XL aream XVIII bitorum
bis positam poscunt; V1-VI eam non habent. Tabula D.1 pro V7-V10.

Emendationes: tabula `_bloci` SOLA nova (ceterae mensurae
derivantur - tabula quinque columnarum manu transcripta erraret);
numerus characterum XVI bitis pro V10; `_ponere_versio_info` post
larvam (ut informatio formati, larvanda non est).

### Post emendationem

XXV/XXV gyri legibiles, versiones I-X trans gradus L/M/Q/H omnes.
Oraculum CALIBRATUM: informatione versionis inhibita, VIII casus
V7+ cadunt et nullus infra. (Conatus calibrationis primus ne
COMPILAVIT quidem - `-Wunused-function` de sede vocationis
remota clamavit. Compilator ipse porta erat.)

probatio_qr auctum: `qr_structura` (API nova, introspectio) +
casus qui XL compositiones verificat arithmetica - totales
INDEPENDENTER in probatione scripti, quia probatio quae numeros
ex eadem tabula haurit nihil probat. LXVI -> CCXXIX casus.

### Lex

Probationes STRUCTURALES formam probant, non veritatem. Ubi
artefactum a lectore ALIENO legendum est (QR, imago, protocollum,
forma limae), oraculum gyri necessarium est - aliter "probationes
praetereunt" significat "figura recta est", quod non idem est.
Nota Ianuaria "visual verification shows scannable codes" vera
erat - sed chorda brevis (V1-V3), ergo sub-sampled.
