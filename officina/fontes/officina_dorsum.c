/* officina_dorsum.c - dorsum nativum aarch64 (M5 T7)
 *
 * FIGURA COMPILATIONIS functionis unius:
 *   I.   probatio: quod nondum emittitur recusatur nominatim;
 *   II.  vita registrorum (medulla_fluxus) -> allocatio linearis:
 *        x9-x15 (temporaria) intervallis quae vocationem non
 *        transeunt, x19-x28 (servata) ceteris et superfluis; sine
 *        registro libero intervallum fine remotissimo in loculum
 *        tabulati effunditur;
 *   III. emissio in ordine dorsi (medulla_fluxus_ordo): prologus,
 *        blocci cum notis, epilogus in quoque 'redde';
 *   IV.  codex in aream scribitur et sigillatur; mappa pc -> index
 *        planus servatur.
 *
 * TABULATUM (sp post prologum immotus):
 *   [x29]      x29, x30 vocantis   (stp x29,x30,[sp,#-16]!)
 *   [sp + 8n)  registra servata usa
 *   [sp + 0)   loculi effusionis (VIII octeti quisque)
 *
 * LATITUDO: typi XXXII bitorum et minores per formas W (pars superior
 * registri zephyra); typi LXIV per formas X. Valor canonicus
 * machinulae (signo extensus in LXIV) non servatur - registrum signo
 * minore scriptum in usu LXIV recusatur (census E17).
 *
 * x16/x17: registra operis (fontes effusi, immediata, destinatio
 * effusa). x18 numquam.
 */

#include "officina_dorsum.h"
#include "officina_medulla_fluxus.h"
#include "officina_medulla_textus.h"
#include "officina_effugium.h"
#include "officina_machinula.h"
#include "codificator_a64.h"

#include <stdio.h>
#include <string.h>

#define TEMPORARIA_NUMERUS  VII     /* x9 .. x15 */
#define TEMPORARIA_PRIMUM   IX
#define SERVATA_NUMERUS     X       /* x19 .. x28 */
#define SERVATA_PRIMUM      XIX
#define PHYSICA_NUMERUS     (TEMPORARIA_NUMERUS + SERVATA_NUMERUS)
#define R_OPERIS_A          XVI
#define R_OPERIS_B          XVII
#define R_FP                XXIX
#define R_LR                XXX
#define R_SP                XXXI    /* in ADD/SUB_IMM et basi memoriae */
#define R_NULLUM                XXXI    /* alibi */
#define SEDES_NULLA         (-I)    /* registrum numquam vivit */
#define TABULATUM_MAXIMUM   0x100000 /* sub sp duobus imm12 */

nomen structura {
            vacuum* introitus;       /* NIHIL = non compilata */
    memoriae_index  initium;         /* offset in area */
    memoriae_index  finis;
               i32* mappa_offset;    /* octeti ab initio, crescentes */
               i32* mappa_index;     /* index planus medullae */
               i32  mappa_numerus;
               b32  in_opere;        /* compilatio pendet (recursio
                                      * mutua agnoscitur) */
} DorsumFunctio;

structura OfficinaDorsum {
                 Piscina* piscina;
        constans Conexio* conexio;
    MemoriaExsecutabilis* area;
          memoriae_index  cursor;    /* offset liber in area */
                     i32  numerus;
           DorsumFunctio* functiones;
    /* symbolum globale -> index functionis conexionis (-I: non
     * functio interpretata); in creare lectum, ante omnem versionem */
                     s32* functio_symboli;
                     i32  numerus_symbolorum;
};

/* intervallum vitae registri medullae */
nomen structura {
    s32 index;        /* registrum medullae */
    i32 initium;
    i32 finis;
    b32 transit;      /* vocationem transit: servatum postulat */
    s32 pondus;       /* profunditas ansae maxima usuum */
    s32 physicum;     /* index in tabula physicorum; -I = effusum */
} Intervallum;

/* status compilationis functionis unius */
nomen structura {
             OfficinaDorsum* d;
                    Piscina* piscina;
    constans MedullaFunctio* functio;
              MedullaFluxus* fluxus;
             CodificatorA64* c;
                     chorda* causa_out;
                        b32  fractum;
    /* sedes registri medullae: >= 0 registrum A64; <= -II loculus
     * (-(loculus + II)); SEDES_NULLA numquam vivit */
                        s32* sedes;
                        s32  numerus_registrorum;
                        s32  numerus_loculorum;
                        b32  servata_usa[SERVATA_NUMERUS];
                        s32  numerus_servatorum;
                        s64  tabulatum;      /* octeti sub x29 */
    /* arcae (T8): locellus staticus per instructionem arca, supra
     * loculos et servata; offset ab sp per indicem planum (-I: non
     * arca) */
                        s32* arcae_offset;
    /* area pontis (T8): verba argumentorum aedificati cum statu,
     * inter servata et arcas (offset parvus: str [sp,#imm]) */
                        s64 pons_basis;
    /* argumenta exeuntia in stiva (T8 gradus III): ima tabulati
     * ([sp + 0)), ut AAPCS64 poscit; loculi supra */
                        s64 exitus_magnitudo;
                        s32 pons_verba;
                        s64 arcae_basis;
                        s64 arcae_magnitudo;
    /* aggregata (T8 gradus IV): locellus XVI octetorum per
     * parametrum aggregatum parvum in registris acceptum et per
     * %reditus parvum (offset in arcis; -I: nullus); reditus parvus
     * in 'redde' inde in x0:x1 oneratur */
                        s32* parametri_arca;
                        s64  reditus_arca;
                        s32  reditus_octeti;
                        i32  numerus_instructionum;
                        s32* notae;          /* nota per bloccum */
                        i32* basis_plana;    /* index planus primi
                                              * instructionis blocci */
                        i32* mappa_offset;
                        i32* mappa_index;
                        i32  mappa_numerus;
    /* emissio: index instructionis in ordine dorsi (positiones
     * fluxus: 2n usus, 2n + 1 definitio), bloccus sequens in ordine
     * (-I ultimus), conditio comparationis in ramum plicatae (-I
     * nulla) */
                        i32 index_dorsi;
                        s32 bloccus_sequens;
                        s32 conditio_plicata;
                        s32 functio_index;
                        s32 modulus_index;
} Compilatio;


/* ==================================================
 * Adiumenta
 * ================================================== */

interior vacuum
_compilationem_recusare (
            Compilatio* k,
    constans character* nuntius)
{
    si (!k->fractum && k->causa_out != NIHIL)
    {
        *k->causa_out = chorda_ex_literis(nuntius, k->piscina);
    }
    k->fractum = VERUM;
}

interior s32
_latitudo (
    s32 typus)
{
    redde (typus == MEDULLA_TYPUS_I64 || typus == MEDULLA_TYPUS_S64)
        ? LXIV : XXXII;
}

interior b32
_typus_integer_latus (
    s32 typus)
{
    redde typus == MEDULLA_TYPUS_I32 || typus == MEDULLA_TYPUS_S32
        || typus == MEDULLA_TYPUS_I64 || typus == MEDULLA_TYPUS_S64;
}

interior b32
_typus_integer (
    s32 typus)
{
    redde typus >= MEDULLA_TYPUS_I8 && typus <= MEDULLA_TYPUS_S64;
}

/* typus signatus XXXII bitorum aut minor: registrum eo scriptum
 * partem superiorem canonicam (signum extensum) NON fert */
interior b32
_typus_signatus_angustus (
    s32 typus)
{
    redde typus == MEDULLA_TYPUS_S8 || typus == MEDULLA_TYPUS_S16
        || typus == MEDULLA_TYPUS_S32;
}

/* valor canonicus machinulae (verbum LXIV) immediati sub typo */
interior s64
_canonicum_typi (
    s64 valor,
    s32 typus)
{
    i64 v = (i64)valor;

    commutatio (typus)
    {
    casus MEDULLA_TYPUS_I8:
        redde (s64)(v & (i64)0xFF);
    casus MEDULLA_TYPUS_I16:
        redde (s64)(v & (i64)0xFFFF);
    casus MEDULLA_TYPUS_I32:
        redde (s64)(v & (i64)0xFFFFFFFF);
    casus MEDULLA_TYPUS_S8:
        v &= (i64)0xFF;
        redde (s64)((v & (i64)0x80) ? (v | ~(i64)0xFF) : v);
    casus MEDULLA_TYPUS_S16:
        v &= (i64)0xFFFF;
        redde (s64)((v & (i64)0x8000) ? (v | ~(i64)0xFFFF) : v);
    casus MEDULLA_TYPUS_S32:
        v &= (i64)0xFFFFFFFF;
        redde (s64)((v & (i64)0x80000000) ? (v | ~(i64)0xFFFFFFFF)
            : v);
    ordinarius:
        redde valor;
    }
}

interior s32
_physicum_ad_registrum (
    s32 physicum)
{
    redde (physicum < TEMPORARIA_NUMERUS)
        ? TEMPORARIA_PRIMUM + physicum
        : SERVATA_PRIMUM + (physicum - TEMPORARIA_NUMERUS);
}

/* instructionem A64 emittit; defectus codificationis = fractura */
interior vacuum
_a64 (
    Compilatio* k,
           s32  forma,
           s32  latitudo,
           s32  rd,
           s32  rn,
           s32  rm,
           s64  immediatum,
           s32  translatio)
{
    CodificatorA64Instructio i;

    si (k->fractum)
    {
        redde;
    }
    memset(&i, ZEPHYRUM, magnitudo(i));
    i.forma       = forma;
    i.latitudo    = latitudo;
    i.rd          = rd;
    i.rn          = rn;
    i.rm          = rm;
    i.immediatum  = immediatum;
    i.translatio  = translatio;
    si (!codificator_a64_emittere(k->c, &i))
    {
        _compilationem_recusare(k,
            "dorsum: instructio non codificabilis");
    }
}

/* mov rd, rn (latitudo data) */
interior vacuum
_registrum_movere (
    Compilatio* k,
           s32  latitudo,
           s32  rd,
           s32  rn)
{
    si (rd != rn)
    {
        _a64(k, CODIFICATOR_A64_ORR_REG, latitudo, rd, R_NULLUM, rn, 0,
            0);
    }
}

/* valorem immediatum in rd ponit (movz/movn + movk) */
interior vacuum
_constantem_ponere (
    Compilatio* k,
           s64  valor,
           s32  latitudo,
           s32  rd)
{
    i64 v       = (i64)valor;
    s32 partes  = (latitudo == LXIV) ? IV : II;
    b32 primum  = VERUM;
    s32 p;

    si (latitudo == XXXII)
    {
        v &= (i64)0xFFFFFFFF;
    }
    alioquin si ((~v) < (i64)0x10000)
    {
        /* negativa parva: movn */
        _a64(k, CODIFICATOR_A64_MOVN, LXIV, rd, 0, 0, (s64)(~v), 0);
        redde;
    }
    per (p = ZEPHYRUM; p < partes; p++)
    {
        i64 pars = (v >> (p * XVI)) & (i64)0xFFFF;

        si (pars == ZEPHYRUM)
        {
            perge;
        }
        _a64(k, primum ? CODIFICATOR_A64_MOVZ : CODIFICATOR_A64_MOVK,
            latitudo, rd, 0, 0, (s64)pars, p * XVI);
        primum = FALSUM;
    }
    si (primum)
    {
        _a64(k, CODIFICATOR_A64_MOVZ, latitudo, rd, 0, 0, 0, 0);
    }
}

interior s64
_offset_loculi (
    constans Compilatio* k,
                    s32  sedes)
{
    redde k->exitus_magnitudo + (s64)(-(sedes + II)) * VIII;
}

interior s32
_octeti_typi (
    s32 typus);

/* locus argumenti integri in stiva (Apple AAPCS64: magnitudine et
 * ordinatione NATURALI, non verbis VIII octetorum); cursor
 * promovetur */
interior s64
_stivae_locum_capere (
    s64* cursor,
    s32  typus)
{
    s64 mag = (s64)_octeti_typi(typus);
    s64 offset;

    *cursor  = (*cursor + mag - I) & ~(mag - I);
    offset   = *cursor;
    *cursor  += mag;
    redde offset;
}

/* sedes liminis in vocatione (Apple AAPCS64, classis integra) */
#define LIMEN_REGISTRUM  ZEPHYRUM    /* x[index_x] (verbum I aut II) */
#define LIMEN_STIVA      I           /* [stiva + offset] */
#define LIMEN_X_OCTO     II          /* %reditus maior: inscriptio x8 */
#define LIMEN_NULLUM     III         /* %reditus parvus: nihil transit,
                                      * valor in x0:x1 redit */

nomen structura {
    s32 genus;
    s32 index_x;    /* registrum x primum */
    s32 octeti;     /* aggregatum parvum per VALOREM (aut %reditus):
                     * magnitudo; scalare et inscriptio: zephyrus */
    s32 typus;      /* scalaris (inscriptio aggregati maioris: I64) */
    s64 offset;
} LimenSedes;

nomen structura {
    s32 registra;   /* x proximum liberum (0 - VIII) */
    s64 stiva;      /* octetus proximus liber stivae argumentorum */
} LimenCursor;

/* limen proximum collocat et cursorem promovet:
 *  - %reditus <= XVI octetorum: nihil transit; maior: x8;
 *  - aggregatum <= XVI octetorum: verba I aut II in registris x
 *    CONTINUIS; si non capiunt, TOTUM in stivam (ordinatio VIII) et
 *    registra x reliqua VACANT;
 *  - aggregatum maius: inscriptio copiae, ut i64;
 *  - scalare: registrum x, aut stiva magnitudine NATURALI. */
interior LimenSedes
_limen_collocare (
                     LimenCursor* cursor,
    constans MedullaTypusLiminis* limen)
{
    LimenSedes s;

    memset(&s, ZEPHYRUM, magnitudo(s));
    s.typus = limen->typus;
    si (limen->est_reditus)
    {
        s.typus   = MEDULLA_TYPUS_I64;
        s.octeti  = (s32)limen->magnitudo_octetorum;
        s.genus   = (limen->magnitudo_octetorum <= XVI)
            ? LIMEN_NULLUM : LIMEN_X_OCTO;
        redde s;
    }
    si (limen->typus == MEDULLA_TYPUS_AGGREGATUM)
    {
        s.typus = MEDULLA_TYPUS_I64;
        si (limen->magnitudo_octetorum <= XVI)
        {
            s32 verba = ((s32)limen->magnitudo_octetorum + VII) / VIII;

            s.octeti = (s32)limen->magnitudo_octetorum;
            si (cursor->registra + verba <= VIII)
            {
                s.genus           = LIMEN_REGISTRUM;
                s.index_x         = cursor->registra;
                cursor->registra  += verba;
                redde s;
            }
            cursor->registra  = VIII;
            cursor->stiva     = (cursor->stiva + VII) & ~(s64)VII;
            s.genus           = LIMEN_STIVA;
            s.offset          = cursor->stiva;
            cursor->stiva     += (s64)verba * VIII;
            redde s;
        }
    }
    si (cursor->registra < VIII)
    {
        s.genus    = LIMEN_REGISTRUM;
        s.index_x  = cursor->registra;
        cursor->registra++;
        redde s;
    }
    s.genus   = LIMEN_STIVA;
    s.offset  = _stivae_locum_capere(&cursor->stiva, s.typus);
    redde s;
}

/* limen quod dorsum transferre scit; FALSUM = recusatio posita */
interior b32
_limen_sustentum (
                      Compilatio* k,
    constans MedullaTypusLiminis* limen)
{
    si (limen->typus != MEDULLA_TYPUS_AGGREGATUM && !limen->est_reditus)
    {
        redde VERUM;
    }
    si (limen->hfa_typus >= ZEPHYRUM)
    {
        _compilationem_recusare(k,
            "dorsum: aggregatum fluitans (Opus IX)");
        redde FALSUM;
    }
    si (limen->magnitudo_octetorum == ZEPHYRUM)
    {
        _compilationem_recusare(k,
            "dorsum: aggregatum sine magnitudine");
        redde FALSUM;
    }
    redde VERUM;
}

/* typus sub quo operandum liminis legitur (aggregatum et %reditus:
 * inscriptio) */
interior s32
_typus_liminis (
    constans MedullaTypusLiminis* limen)
{
    redde (limen->typus == MEDULLA_TYPUS_AGGREGATUM
        || limen->est_reditus)
        ? (s32)MEDULLA_TYPUS_I64 : limen->typus;
}

/* fontem in registrum A64 reddit: registrum allocatum ipsum, aut
 * registrum operis oneratum (loculus, immediatum) */
interior s32
_fons (
                   Compilatio* k,
    constans MedullaOperandum* operandum,
                          s32  latitudo,
                          s32  operis)
{
    si (operandum->genus == MEDULLA_OPERANDUM_REGISTRUM)
    {
        s32 sedes = (operandum->datum.index >= ZEPHYRUM
                && operandum->datum.index < k->numerus_registrorum)
            ? k->sedes[operandum->datum.index] : SEDES_NULLA;

        si (sedes >= ZEPHYRUM)
        {
            redde sedes;
        }
        si (sedes == SEDES_NULLA)
        {
            _compilationem_recusare(k,
                "dorsum: registrum sine sede lectum");
            redde operis;
        }
        _a64(k, CODIFICATOR_A64_LDR_IMM, VIII, operis, R_SP, 0,
            _offset_loculi(k, sedes), 0);
        redde operis;
    }
    si (operandum->genus == MEDULLA_OPERANDUM_IMMEDIATUM)
    {
        _constantem_ponere(k, operandum->datum.immediatum, latitudo,
            operis);
        redde operis;
    }
    _compilationem_recusare(k, "dorsum: operandum non sustentum");
    redde operis;
}

/* registrum A64 in quod destinatio scribitur (allocatum aut
 * R_OPERIS_A si effusum - _destinationem_condere deinde servat) */
interior s32
_destinatio (
    Compilatio* k,
           s32  index)
{
    s32 sedes = (index >= ZEPHYRUM && index < k->numerus_registrorum)
        ? k->sedes[index] : SEDES_NULLA;

    redde (sedes >= ZEPHYRUM) ? sedes : R_OPERIS_A;
}

interior vacuum
_destinationem_condere (
    Compilatio* k,
           s32  index,
           s32  registrum_a64)
{
    s32 sedes = (index >= ZEPHYRUM && index < k->numerus_registrorum)
        ? k->sedes[index] : SEDES_NULLA;

    si (sedes <= -II)
    {
        _a64(k, CODIFICATOR_A64_STR_IMM, VIII, registrum_a64, R_SP, 0,
            _offset_loculi(k, sedes), 0);
    }
    /* SEDES_NULLA: destinatio numquam lecta - valor abicitur */
}


/* ==================================================
 * I. Probatio: quid dorsum emittit
 * ================================================== */

interior b32
_op_sustenta (
    s32 op)
{
    commutatio (op)
    {
    casus MEDULLA_OP_MOVERE:
    casus MEDULLA_OP_ADDERE:
    casus MEDULLA_OP_SUBTRAHERE:
    casus MEDULLA_OP_MULTIPLICARE:
    casus MEDULLA_OP_DIVIDERE:
    casus MEDULLA_OP_RESIDUUM:
    casus MEDULLA_OP_SISTERE:
    casus MEDULLA_OP_NEGARE:
    casus MEDULLA_OP_ET:
    casus MEDULLA_OP_VEL:
    casus MEDULLA_OP_AUT:
    casus MEDULLA_OP_COMPLEMENTUM:
    casus MEDULLA_OP_SINISTRORSUM:
    casus MEDULLA_OP_DEXTRORSUM:
    casus MEDULLA_OP_AEQUALIS:
    casus MEDULLA_OP_INAEQUALIS:
    casus MEDULLA_OP_MINOR:
    casus MEDULLA_OP_MINOR_AEQUALIS:
    casus MEDULLA_OP_MAIOR:
    casus MEDULLA_OP_MAIOR_AEQUALIS:
    casus MEDULLA_OP_SALIRE:
    casus MEDULLA_OP_RAMUS:
    casus MEDULLA_OP_EXTENDERE:
    casus MEDULLA_OP_AMPUTARE:
    casus MEDULLA_OP_VOCARE:
    casus MEDULLA_OP_LEGERE:
    casus MEDULLA_OP_SCRIBERE:
    casus MEDULLA_OP_ARCA:
    casus MEDULLA_OP_LOCUS:
    casus MEDULLA_OP_COPIA:
    casus MEDULLA_OP_REDDE:
        redde VERUM;
    ordinarius:
        redde FALSUM;
    }
}

interior vacuum
_operationem_recusare (
    Compilatio* k,
           s32  op)
{
             character  nuntius[XCVI];
    constans character* titulus = medulla_op_titulus(op);

    sprintf(nuntius, "operatio %.40s nondum nativa",
        (titulus != NIHIL) ? titulus : "ignota");
    _compilationem_recusare(k, nuntius);
}

/* aedificatum a ut valor vocati negativus; -I recusatio manet */
#define VOCATUS_AEDIFICATUM(a)  (-(a) - X)
#define AEDIFICATUM_VOCATI(v)   (-(v) - X)
#define PONS_ARGUMENTA_MAXIMA   XVI
#define VOCATUS_INDIRECTUS      (-II)
#define ARGUMENTA_NATIVA_MAXIMA XXXII

/* vocatus directus: index functionis conexionis (>= 0), aedificatum
 * (VOCATUS_AEDIFICATUM, <= -X), aut -I + recusatio nominata */
interior s32
_vocatum_resolvere (
                    Compilatio* k,
    constans MedullaInstructio* ins)
{
    constans ConexioSymbolum* symbolum;
                   character  nuntius[CXXVIII];
                         s32  globale;

    si (ins->a.genus == MEDULLA_OPERANDUM_REGISTRUM)
    {
        redde VOCATUS_INDIRECTUS;   /* per descriptorem in registro */
    }
    si (ins->a.genus != MEDULLA_OPERANDUM_SYMBOLUM)
    {
        _compilationem_recusare(k, "dorsum: vocatus insanus");
        redde -I;
    }
    globale = conexio_symbolum_globale(k->d->conexio, k->modulus_index,
        ins->a.datum.index);
    si (globale < ZEPHYRUM || (i32)globale >= k->d->numerus_symbolorum)
    {
        _compilationem_recusare(k, "dorsum: vocatus insanus");
        redde -I;
    }
    si (k->d->functio_symboli[globale] >= ZEPHYRUM)
    {
        redde k->d->functio_symboli[globale];
    }
    symbolum = conexio_symbolum_obtinere(k->d->conexio, globale);
    /* aedificatum machinulae (decipula in AEDIFICATUM versa): index
     * eius ut valor negativus codificatur */
    si (   symbolum->genus == CONEXIO_SYMBOLUM_DECIPULA
        && symbolum->sedes != NIHIL
        && ((constans ConexioDescriptor*)symbolum->sedes)->signum
            == CONEXIO_SIGNUM_AEDIFICATUM)
    {
        redde VOCATUS_AEDIFICATUM((s32)((constans ConexioDescriptor*)
            symbolum->sedes)->index);
    }
    sprintf(nuntius, "vocat $%.*s interpretatum",
        (symbolum->titulus.mensura < LXXX)
            ? (int)symbolum->titulus.mensura : LXXX,
        (constans character*)symbolum->titulus.datum);
    _compilationem_recusare(k, nuntius);
    redde -I;
}

/* limen argumenti j vocationis; NIHIL si typi desunt */
interior constans MedullaTypusLiminis*
_limen_argumenti (
                    Compilatio* k,
    constans MedullaInstructio* ins,
                           s32  j)
{
    constans MedullaFunctio* f = k->functio;

    si (   f->operanda_typi == NIHIL
        || (i32)(ins->extra_index + j) >= (i32)xar_numerus(
               f->operanda_typi))
    {
        redde NIHIL;
    }
    redde xar_obtinere(f->operanda_typi, (i32)(ins->extra_index + j));
}

/* aedificatum PURUM directe (inscriptio libc) vocari potest si
 * vocatio AAPCS64 integra est: non variadica, <= VIII argumenta
 * integra, reditus integer aut nullus. Aliter per pontem (pura
 * fluitantia: donec dorsum registra fluitantia habeat). */
interior b32
_aedificatum_directum (
                    Compilatio* k,
    constans MedullaInstructio* ins,
                           s32  aedificatum)
{
    s32 j;

    si (   !machinula_aedificatum_purum(aedificatum)
        || machinula_aedificatum_inscriptio(aedificatum) == NIHIL
        || ins->argumenta_fixa >= ZEPHYRUM || ins->extra_numerus > VIII
        || (   ins->typus != MEDULLA_TYPUS_NIHIL
            && !_typus_integer(ins->typus)))
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < ins->extra_numerus; j++)
    {
        constans MedullaTypusLiminis* limen = _limen_argumenti(k, ins,
            j);

        si (   limen == NIHIL || !_typus_integer(limen->typus)
            || limen->est_reditus)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* typi instructionis unius; FALSUM = recusatio posita */
interior b32
_typi_instructionis_probare (
                    Compilatio* k,
    constans MedullaInstructio* ins)
{
    s32 j;

    commutatio (ins->op)
    {
    casus MEDULLA_OP_REDDE:
    casus MEDULLA_OP_SALIRE:
    casus MEDULLA_OP_RAMUS:
    casus MEDULLA_OP_SISTERE:
    casus MEDULLA_OP_COPIA:
        redde VERUM;
    casus MEDULLA_OP_ARCA:
        /* locellus staticus: magnitudo et ordinatio immediatae
         * (lex machinulae E14); ordinatio ultra XVI tabulatum
         * aliter ordinatum posceret */
        si (   ins->a.genus != MEDULLA_OPERANDUM_IMMEDIATUM
            || ins->b.genus != MEDULLA_OPERANDUM_IMMEDIATUM
            || ins->a.datum.immediatum < ZEPHYRUM
            || ins->a.datum.immediatum > (s64)TABULATUM_MAXIMUM)
        {
            _compilationem_recusare(k,
                "dorsum: arca sine magnitudine immediata");
            redde FALSUM;
        }
        si (ins->b.datum.immediatum > XVI)
        {
            _compilationem_recusare(k,
                "dorsum: arca ordinatione ultra XVI");
            redde FALSUM;
        }
        redde VERUM;
    casus MEDULLA_OP_LOCUS:
        si (   ins->a.genus != MEDULLA_OPERANDUM_SYMBOLUM
            || conexio_symbolum_globale(k->d->conexio,
                   k->modulus_index, ins->a.datum.index) < ZEPHYRUM)
        {
            _compilationem_recusare(k, "dorsum: locus sine symbolo");
            redde FALSUM;
        }
        redde VERUM;
    casus MEDULLA_OP_LEGERE:
    casus MEDULLA_OP_SCRIBERE:
    casus MEDULLA_OP_MOVERE:
        si (!_typus_integer(ins->typus))
        {
            _operationem_recusare(k, ins->op);
            redde FALSUM;
        }
        redde VERUM;
    casus MEDULLA_OP_EXTENDERE:
    casus MEDULLA_OP_AMPUTARE:
        si (   !_typus_integer(ins->typus)
            || !_typus_integer(ins->typus_secundus))
        {
            _operationem_recusare(k, ins->op);
            redde FALSUM;
        }
        redde VERUM;
    casus MEDULLA_OP_VOCARE:
    {
        s32 vocatus = _vocatum_resolvere(k, ins);
        b32 per_pontem;

        si (vocatus == -I)
        {
            redde FALSUM;
        }
        per_pontem = vocatus <= -X
            && !_aedificatum_directum(k, ins,
            AEDIFICATUM_VOCATI(vocatus));
        si (   ins->typus != MEDULLA_TYPUS_NIHIL
            && !_typus_integer(ins->typus))
        {
            _compilationem_recusare(k,
                "dorsum: reditus vocati non integer (Opus IX)");
            redde FALSUM;
        }
        si (!per_pontem && ins->argumenta_fixa >= ZEPHYRUM)
        {
            _compilationem_recusare(k,
                "dorsum: vocatio variadica (Opus VIII)");
            redde FALSUM;
        }
        si (ins->extra_numerus > (per_pontem ? PONS_ARGUMENTA_MAXIMA
                : ARGUMENTA_NATIVA_MAXIMA))
        {
            _compilationem_recusare(k, "dorsum: argumenta nimia");
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < ins->extra_numerus; j++)
        {
            constans MedullaTypusLiminis* limen = _limen_argumenti(k,
                ins, j);
            constans MedullaOperandum* operandum = xar_obtinere(
                k->functio->operanda, (i32)(ins->extra_index + j));

            si (limen == NIHIL || limen->typus < ZEPHYRUM)
            {
                _compilationem_recusare(k,
                    "dorsum: argumentum sine typo");
                redde FALSUM;
            }
            si (per_pontem)
            {
                /* pons verba LXIV accipit: integra, inscriptio
                 * aggregati, immediatum fluitans ut figura bitorum;
                 * registrum fluitans dorsum nondum habet */
                si (   (   limen->typus == MEDULLA_TYPUS_F32
                        || limen->typus == MEDULLA_TYPUS_F64)
                    && operandum->genus
                        != MEDULLA_OPERANDUM_IMMEDIATUM_F)
                {
                    _compilationem_recusare(k,
                        "dorsum: argumentum fluitans (Opus IX)");
                    redde FALSUM;
                }
                si (limen->est_reditus)
                {
                    _compilationem_recusare(k, "dorsum: argumentum"
                        " non integrum (Opus VIII)");
                    redde FALSUM;
                }
                perge;
            }
            si (!_limen_sustentum(k, limen))
            {
                redde FALSUM;
            }
            si (!_typus_integer(_typus_liminis(limen)))
            {
                _compilationem_recusare(k,
                    "dorsum: argumentum fluitans (Opus IX)");
                redde FALSUM;
            }
        }
        redde VERUM;
    }
    ordinarius:
        si (!_typus_integer_latus(ins->typus))
        {
            _operationem_recusare(k, ins->op);
            redde FALSUM;
        }
        redde VERUM;
    }
}

/* VERUM si operandum registrum 'angustum' (signo minore scriptum)
 * sub typo LXIV legitur */
interior b32
_lectio_lata_angusti (
                 constans b32* angusta,
                          s32  numerus,
    constans MedullaOperandum* operandum,
                          s32  typus)
{
    redde operandum->genus == MEDULLA_OPERANDUM_REGISTRUM
        && operandum->datum.index >= ZEPHYRUM
        && operandum->datum.index < numerus
        && angusta[operandum->datum.index]
        && (typus == MEDULLA_TYPUS_I64 || typus == MEDULLA_TYPUS_S64);
}

/* census E17: registrum typo signato XXXII aut minore scriptum
 * verbum canonicum (signo in LXIV extensum) non fert; lectio eius
 * sub typo LXIV recusatur. Exempta: numerus gyri (A64 eum modulo
 * latitudinis sumit), ramus (zephyrus aut non). Fructus
 * comparationum, extendere et movere immediati verba plena sunt. */
interior vacuum
_latitudines_probare (
    Compilatio* k)
{
    constans MedullaFunctio* f = k->functio;
                        b32* angusta;
                        i32  b;
                        i32  j;
                        s32  a;
                        s32  transitus;

    angusta = piscina_allocare(k->piscina, (memoriae_index)
        (k->numerus_registrorum + I) * magnitudo(b32));
    si (angusta == NIHIL)
    {
        _compilationem_recusare(k, "dorsum: memoria deest");
        redde;
    }
    memset(angusta, ZEPHYRUM, (memoriae_index)
        (k->numerus_registrorum + I) * magnitudo(b32));
    per (b = ZEPHYRUM; b < (i32)xar_numerus(f->parametra); b++)
    {
        constans MedullaParametrum* p = xar_obtinere(f->parametra, b);

        si (   _typus_signatus_angustus(p->typus)
            && p->index >= ZEPHYRUM
            && p->index < k->numerus_registrorum)
        {
            angusta[p->index] = VERUM;
        }
    }
    /* transitus 0: scripturae notantur; transitus 1: lectiones
     * probantur (registra multi-definita: ordo non refert) */
    per (transitus = ZEPHYRUM; transitus < II; transitus++)
    {
        per (b = ZEPHYRUM; b < (i32)xar_numerus(f->blocci); b++)
        {
            constans MedullaBloccus* bloccus = xar_obtinere(f->blocci,
                b);

            si (!medulla_fluxus_attingibilis(k->fluxus, (s32)b))
            {
                perge;
            }
            per (j = ZEPHYRUM;
                 j < (i32)xar_numerus(bloccus->instructiones); j++)
            {
                constans MedullaInstructio* ins = xar_obtinere(
                    bloccus->instructiones, j);
                                       s32 typus_fructus  = ins->typus;
                                       b32 lata           = FALSUM;

                si (transitus == ZEPHYRUM)
                {
                    si (   ins->destinatio < ZEPHYRUM
                        || ins->destinatio >= k->numerus_registrorum)
                    {
                        perge;
                    }
                    commutatio (ins->op)
                    {
                    casus MEDULLA_OP_AEQUALIS:
                    casus MEDULLA_OP_INAEQUALIS:
                    casus MEDULLA_OP_MINOR:
                    casus MEDULLA_OP_MINOR_AEQUALIS:
                    casus MEDULLA_OP_MAIOR:
                    casus MEDULLA_OP_MAIOR_AEQUALIS:
                    casus MEDULLA_OP_EXTENDERE:
                    casus MEDULLA_OP_LEGERE:   /* onus signo in LXIV
                                                * extensum */
                    casus MEDULLA_OP_ARCA:
                    casus MEDULLA_OP_LOCUS:
                        perge;
                    casus MEDULLA_OP_MOVERE:
                        si (ins->a.genus
                            == MEDULLA_OPERANDUM_IMMEDIATUM)
                        {
                            perge;
                        }
                        frange;
                    casus MEDULLA_OP_AMPUTARE:
                        typus_fructus = ins->typus_secundus;
                        frange;
                    ordinarius:
                        frange;
                    }
                    si (_typus_signatus_angustus(typus_fructus))
                    {
                        angusta[ins->destinatio] = VERUM;
                    }
                    perge;
                }
                commutatio (ins->op)
                {
                casus MEDULLA_OP_SALIRE:
                casus MEDULLA_OP_RAMUS:
                casus MEDULLA_OP_SISTERE:
                casus MEDULLA_OP_ARCA:
                casus MEDULLA_OP_LOCUS:
                    frange;
                casus MEDULLA_OP_LEGERE:
                    lata = _lectio_lata_angusti(angusta,
                        k->numerus_registrorum, &ins->a,
                        MEDULLA_TYPUS_I64);
                    frange;
                casus MEDULLA_OP_SCRIBERE:
                    lata = _lectio_lata_angusti(angusta,
                               k->numerus_registrorum, &ins->a,
                               MEDULLA_TYPUS_I64)
                        || _lectio_lata_angusti(angusta,
                               k->numerus_registrorum, &ins->b,
                               ins->typus);
                    frange;
                casus MEDULLA_OP_COPIA:
                    lata = _lectio_lata_angusti(angusta,
                               k->numerus_registrorum, &ins->a,
                               MEDULLA_TYPUS_I64)
                        || _lectio_lata_angusti(angusta,
                               k->numerus_registrorum, &ins->b,
                               MEDULLA_TYPUS_I64)
                        || _lectio_lata_angusti(angusta,
                               k->numerus_registrorum, &ins->c,
                               MEDULLA_TYPUS_I64);
                    frange;
                casus MEDULLA_OP_REDDE:
                    lata = _lectio_lata_angusti(angusta,
                        k->numerus_registrorum, &ins->a,
                        f->typus_reditus);
                    frange;
                casus MEDULLA_OP_SINISTRORSUM:
                casus MEDULLA_OP_DEXTRORSUM:
                    lata = _lectio_lata_angusti(angusta,
                        k->numerus_registrorum, &ins->a, ins->typus);
                    frange;
                casus MEDULLA_OP_VOCARE:
                    /* descriptor vocati indirecti: inscriptio */
                    lata = _lectio_lata_angusti(angusta,
                        k->numerus_registrorum, &ins->a,
                        MEDULLA_TYPUS_I64);
                    per (a = ZEPHYRUM; a < ins->extra_numerus; a++)
                    {
                        constans MedullaTypusLiminis* limen =
                            _limen_argumenti(k, ins, a);

                        si (   limen != NIHIL
                            && _lectio_lata_angusti(angusta,
                                   k->numerus_registrorum,
                                   xar_obtinere(f->operanda,
                                       (i32)(ins->extra_index + a)),
                                   _typus_liminis(limen)))
                        {
                            lata = VERUM;
                        }
                    }
                    frange;
                ordinarius:
                    lata = _lectio_lata_angusti(angusta,
                               k->numerus_registrorum, &ins->a,
                               ins->typus)
                        || _lectio_lata_angusti(angusta,
                               k->numerus_registrorum, &ins->b,
                               ins->typus);
                    frange;
                }
                si (lata)
                {
                    _compilationem_recusare(k,
                        "registrum s32 in usu 64");
                    redde;
                }
            }
        }
    }
}

interior vacuum
_functionem_probare (
    Compilatio* k)
{
    constans MedullaFunctio* f = k->functio;
                        i32  b;
                        i32  j;

    si (f->est_variadica)
    {
        _compilationem_recusare(k, "functio variadica");
        redde;
    }
    si (xar_numerus(f->parametra) > ARGUMENTA_NATIVA_MAXIMA)
    {
        _compilationem_recusare(k, "dorsum: parametra nimia");
        redde;
    }
    si (   f->typus_reditus != MEDULLA_TYPUS_NIHIL
        && !_typus_integer(f->typus_reditus))
    {
        _compilationem_recusare(k,
            "dorsum: reditus non integer (Opus IX)");
        redde;
    }
    per (b = ZEPHYRUM; b < (i32)xar_numerus(f->parametra); b++)
    {
        constans MedullaParametrum* p = xar_obtinere(f->parametra, b);

        si (!_limen_sustentum(k, &p->limen))
        {
            redde;
        }
        si (   !_typus_integer(p->typus)
            || !_typus_integer(_typus_liminis(&p->limen)))
        {
            _compilationem_recusare(k,
                "dorsum: parametrum non integrum (Opus IX)");
            redde;
        }
    }
    per (b = ZEPHYRUM; b < (i32)xar_numerus(f->blocci); b++)
    {
        constans MedullaBloccus* bloccus = xar_obtinere(f->blocci, b);

        si (!medulla_fluxus_attingibilis(k->fluxus, (s32)b))
        {
            perge;
        }
        per (j = ZEPHYRUM; j < (i32)xar_numerus(bloccus->instructiones);
             j++)
        {
            constans MedullaInstructio* ins = xar_obtinere(
                bloccus->instructiones, j);

            si (!_op_sustenta(ins->op))
            {
                _operationem_recusare(k, ins->op);
                redde;
            }
            si (!_typi_instructionis_probare(k, ins))
            {
                redde;
            }
        }
    }
    si (!k->fractum)
    {
        _latitudines_probare(k);
    }
}


/* ==================================================
 * II. Allocatio linearis
 * ================================================== */

interior vacuum
_intervalla_ordinare (
    Intervallum* v,
            s32  n)
{
    s32 i;
    s32 j;

    per (i = I; i < n; i++)
    {
        Intervallum t = v[i];

        j = i;
        dum (j > ZEPHYRUM && v[j - I].initium > t.initium)
        {
            v[j] = v[j - I];
            j--;
        }
        v[j] = t;
    }
}

interior vacuum
_pondus_notare (
                          s32* pondera,
                          s32  numerus,
    constans MedullaOperandum* operandum,
                          s32  profunditas)
{
    si (   operandum->genus       == MEDULLA_OPERANDUM_REGISTRUM
        && operandum->datum.index >= ZEPHYRUM
        && operandum->datum.index < numerus
        && pondera[operandum->datum.index] < profunditas)
    {
        pondera[operandum->datum.index] = profunditas;
    }
}

/* pondus registri = profunditas ansae maxima inter instructiones quae
 * id legunt aut scribunt (effusio extra ansas praefertur) */
interior s32*
_pondera_computare (
    Compilatio* k)
{
    constans MedullaFunctio* f = k->functio;
                        s32* pondera;
                        i32  b;
                        i32  j;

    pondera = piscina_allocare(k->piscina, (memoriae_index)
        (k->numerus_registrorum + I) * magnitudo(s32));
    si (pondera == NIHIL)
    {
        redde NIHIL;
    }
    memset(pondera, ZEPHYRUM, (memoriae_index)
        (k->numerus_registrorum + I) * magnitudo(s32));
    per (b = ZEPHYRUM; b < (i32)xar_numerus(f->blocci); b++)
    {
        constans MedullaBloccus* bloccus = xar_obtinere(f->blocci, b);
                            s32  profunditas;

        si (!medulla_fluxus_attingibilis(k->fluxus, (s32)b))
        {
            perge;
        }
        profunditas = medulla_fluxus_profunditas_ansae(k->fluxus,
            (s32)b);
        per (j = ZEPHYRUM; j < (i32)xar_numerus(bloccus->instructiones);
             j++)
        {
            constans MedullaInstructio* ins = xar_obtinere(
                bloccus->instructiones, j);

            _pondus_notare(pondera, k->numerus_registrorum, &ins->a,
                profunditas);
            _pondus_notare(pondera, k->numerus_registrorum, &ins->b,
                profunditas);
            _pondus_notare(pondera, k->numerus_registrorum, &ins->c,
                profunditas);
            si (   ins->destinatio >= ZEPHYRUM
                && ins->destinatio < k->numerus_registrorum
                && pondera[ins->destinatio] < profunditas)
            {
                pondera[ins->destinatio] = profunditas;
            }
        }
    }
    redde pondera;
}

interior vacuum
_registra_allocare (
    Compilatio* k)
{
    Intervallum* v;
    Intervallum* activa[PHYSICA_NUMERUS];
            s32* pondera            = _pondera_computare(k);
            s32  numerus_activorum  = ZEPHYRUM;
            b32  occupatum[PHYSICA_NUMERUS];
            s32  n = ZEPHYRUM;
            s32  r;
            s32  i;
            s32  a;

    si (pondera == NIHIL)
    {
        _compilationem_recusare(k, "dorsum: memoria deest");
        redde;
    }
    memset(occupatum, ZEPHYRUM, magnitudo(occupatum));
    v = piscina_allocare(k->piscina, (memoriae_index)
        (k->numerus_registrorum + I) * magnitudo(Intervallum));
    si (v == NIHIL)
    {
        _compilationem_recusare(k, "dorsum: memoria deest");
        redde;
    }
    per (r = ZEPHYRUM; r < k->numerus_registrorum; r++)
    {
        i32 initium;
        i32 finis;

        k->sedes[r] = SEDES_NULLA;
        si (!medulla_fluxus_intervallum(k->fluxus, r, &initium, &finis))
        {
            perge;
        }
        v[n].index    = r;
        v[n].initium  = initium;
        v[n].finis    = finis;
        v[n].transit = medulla_fluxus_transit_vocationem(k->fluxus,
            r);
        v[n].physicum  = -I;
        v[n].pondus    = pondera[r];
        n++;
    }
    _intervalla_ordinare(v, n);

    per (i = ZEPHYRUM; i < n; i++)
    {
        Intervallum* hoc      = &v[i];
                s32  electum  = -I;
                s32  p;

        /* exspirata liberantur */
        a = ZEPHYRUM;
        dum (a < numerus_activorum)
        {
            si (activa[a]->finis < hoc->initium)
            {
                occupatum[activa[a]->physicum] = FALSUM;
                activa[a] = activa[numerus_activorum - I];
                numerus_activorum--;
            }
            alioquin
            {
                a++;
            }
        }
        /* registrum liberum: temporaria primum nisi transit */
        per (p = hoc->transit ? TEMPORARIA_NUMERUS : ZEPHYRUM;
             p < PHYSICA_NUMERUS && electum < ZEPHYRUM; p++)
        {
            si (!occupatum[p])
            {
                electum = p;
            }
        }
        si (electum < ZEPHYRUM)
        {
            /* effusio: inter activa quorum registrum huic aptum est
             * et hoc ipsum, effunditur quod in ansa minime profunda
             * legitur; inter paria, cuius finis remotissimus */
            Intervallum* victima = NIHIL;

            per (a = ZEPHYRUM; a < numerus_activorum; a++)
            {
                si (   hoc->transit
                    && activa[a]->physicum < TEMPORARIA_NUMERUS)
                {
                    perge;
                }
                si (   victima == NIHIL
                    || activa[a]->pondus < victima->pondus
                    || (   activa[a]->pondus == victima->pondus
                        && activa[a]->finis > victima->finis))
                {
                    victima = activa[a];
                }
            }
            si (   victima != NIHIL
                && (   victima->pondus < hoc->pondus
                    || (   victima->pondus == hoc->pondus
                        && victima->finis > hoc->finis)))
            {
                electum            = victima->physicum;
                victima->physicum  = -I;
                per (a = ZEPHYRUM; a < numerus_activorum; a++)
                {
                    si (activa[a] == victima)
                    {
                        activa[a] = activa[numerus_activorum - I];
                        numerus_activorum--;
                        frange;
                    }
                }
            }
        }
        si (electum >= ZEPHYRUM)
        {
            hoc->physicum              = electum;
            occupatum[electum]         = VERUM;
            activa[numerus_activorum]  = hoc;
            numerus_activorum++;
        }
    }
    /* sedes finales */
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (v[i].physicum >= ZEPHYRUM)
        {
            k->sedes[v[i].index] =
                _physicum_ad_registrum(v[i].physicum);
            si (v[i].physicum >= TEMPORARIA_NUMERUS)
            {
                k->servata_usa[v[i].physicum - TEMPORARIA_NUMERUS] =
                    VERUM;
            }
        }
        alioquin
        {
            k->sedes[v[i].index] = -(k->numerus_loculorum + II);
            k->numerus_loculorum++;
        }
    }
    k->numerus_servatorum = ZEPHYRUM;
    per (i = ZEPHYRUM; i < SERVATA_NUMERUS; i++)
    {
        si (k->servata_usa[i])
        {
            k->numerus_servatorum++;
        }
    }
    k->pons_basis = k->exitus_magnitudo + (s64)(k->numerus_loculorum
        + k->numerus_servatorum) * VIII;
    k->pons_basis   = (k->pons_basis + XV) & ~(s64)XV;
    k->arcae_basis  = k->pons_basis
        + (((s64)k->pons_verba * VIII + XV) & ~(s64)XV);
    k->tabulatum    = k->arcae_basis + k->arcae_magnitudo;
    si (k->tabulatum > (s64)TABULATUM_MAXIMUM)
    {
        _compilationem_recusare(k, "dorsum: tabulatum nimis magnum");
    }
}


/* ==================================================
 * III. Emissio
 * ================================================== */

interior vacuum
_inscriptionem_tabulati (
    Compilatio* k,
           s64  offset,
           s32  rd);

interior vacuum
_prologum_emittere (
    Compilatio* k)
{
                LimenCursor  cursor;
    constans MedullaFunctio* f      = k->functio;
                        s32  locus  = k->numerus_loculorum;
                        s32  i;
                        i32  p;
                        s32  r;

    /* stp x29,x30,[sp,#-16]!; mov x29,sp; sub sp,sp,#tabulatum */
    _a64(k, CODIFICATOR_A64_STP, VIII, R_FP, R_SP, R_LR, -(s64)XVI,
        CODIFICATOR_A64_ANTE);
    _a64(k, CODIFICATOR_A64_ADD_IMM, LXIV, R_FP, R_SP, 0, 0, 0);
    /* sub sp duobus immediatis XII bitorum (tabulatum < XVI MiB) */
    si ((k->tabulatum >> XII) != ZEPHYRUM)
    {
        _a64(k, CODIFICATOR_A64_SUB_IMM, LXIV, R_SP, R_SP, 0,
            k->tabulatum >> XII, XII);
    }
    si ((k->tabulatum & (s64)0xFFF) != ZEPHYRUM)
    {
        _a64(k, CODIFICATOR_A64_SUB_IMM, LXIV, R_SP, R_SP, 0,
            k->tabulatum & (s64)0xFFF, 0);
    }
    per (i = ZEPHYRUM; i < SERVATA_NUMERUS; i++)
    {
        si (k->servata_usa[i])
        {
            _a64(k, CODIFICATOR_A64_STR_IMM, VIII, SERVATA_PRIMUM + i,
                R_SP, 0, k->exitus_magnitudo + (s64)locus * VIII, 0);
            locus++;
        }
    }
    /* parametra: x0.. -> sedes (sedes numquam in x0-x8: nullus
     * ordo motuum servandus) */
    memset(&cursor, ZEPHYRUM, magnitudo(cursor));
    per (p = ZEPHYRUM; p < (i32)xar_numerus(f->parametra); p++)
    {
        constans MedullaParametrum* par = xar_obtinere(f->parametra, p);
                               s32  sedes = k->sedes[par->index];
                        LimenSedes  ls = _limen_collocare(&cursor,
                                       &par->limen);
                               s32 fons  = ls.index_x;

        si (   ls.genus == LIMEN_NULLUM
            || (ls.genus == LIMEN_REGISTRUM && ls.octeti > ZEPHYRUM))
        {
            /* aggregatum parvum in registris acceptum in locellum
             * suum conditur; %reditus parvus locellum vacuum habet.
             * Parametrum = inscriptio locelli. */
            s64 offset = k->arcae_basis + (s64)k->parametri_arca[p];

            si (ls.genus == LIMEN_REGISTRUM)
            {
                _inscriptionem_tabulati(k, offset, R_OPERIS_A);
                _a64(k, CODIFICATOR_A64_STR_IMM, VIII, ls.index_x,
                    R_OPERIS_A, 0, 0, 0);
                si (ls.octeti > VIII)
                {
                    _a64(k, CODIFICATOR_A64_STR_IMM, VIII,
                        ls.index_x + I, R_OPERIS_A, 0, VIII, 0);
                }
            }
            si (sedes == SEDES_NULLA)
            {
                perge;
            }
            fons = (sedes >= ZEPHYRUM) ? sedes : R_OPERIS_A;
            _inscriptionem_tabulati(k, offset, fons);
        }
        alioquin si (ls.genus == LIMEN_X_OCTO)
        {
            fons = VIII;
        }
        alioquin si (ls.genus == LIMEN_STIVA)
        {
            si (sedes == SEDES_NULLA)
            {
                perge;
            }
            fons = (sedes >= ZEPHYRUM) ? sedes : R_OPERIS_A;
            si (ls.octeti > ZEPHYRUM)
            {
                /* aggregatum parvum in stiva vocantis: parametrum =
                 * inscriptio eius ibi */
                _a64(k, CODIFICATOR_A64_ADD_IMM, LXIV, fons, R_FP, 0,
                    (s64)XVI + ls.offset, 0);
            }
            alioquin
            {
                /* [x29 + 16 + offset], magnitudine naturali; signata
                 * angusta signo extensa */
                _a64(k, _typus_signatus_angustus(ls.typus)
                    ? CODIFICATOR_A64_LDRS_IMM
                    : CODIFICATOR_A64_LDR_IMM,
                    _octeti_typi(ls.typus), fons, R_FP, 0,
                    (s64)XVI + ls.offset, 0);
            }
        }
        si (sedes >= ZEPHYRUM)
        {
            _registrum_movere(k, LXIV, sedes, fons);
        }
        alioquin si (sedes <= -II)
        {
            _a64(k, CODIFICATOR_A64_STR_IMM, VIII, fons, R_SP, 0,
                _offset_loculi(k, sedes), 0);
        }
    }
    /* registra lecta ante omnem definitionem: zephyrus (F13) */
    per (r = ZEPHYRUM; r < k->numerus_registrorum; r++)
    {
        i32 initium;
        i32 finis;
        b32 parametrum = FALSUM;

        si (   !medulla_fluxus_intervallum(k->fluxus, r, &initium,
                   &finis)
            || initium != ZEPHYRUM)
        {
            perge;
        }
        per (p = ZEPHYRUM; p < (i32)xar_numerus(f->parametra); p++)
        {
            si (((constans MedullaParametrum*)xar_obtinere(
                    f->parametra, p))->index == r)
            {
                parametrum = VERUM;
            }
        }
        si (parametrum)
        {
            perge;
        }
        si (k->sedes[r] >= ZEPHYRUM)
        {
            _a64(k, CODIFICATOR_A64_MOVZ, LXIV, k->sedes[r], 0, 0, 0,
                0);
        }
        alioquin si (k->sedes[r] <= -II)
        {
            _a64(k, CODIFICATOR_A64_STR_IMM, VIII, R_NULLUM, R_SP, 0,
                _offset_loculi(k, k->sedes[r]), 0);
        }
    }
}

interior vacuum
_saltum_localem_emittere (
    Compilatio* k,
           s32  forma,
           s32  latitudo,
           s32  locellus,
           s32  conditio,
           s32  nota);

/* arcae disponuntur: locellus per instructionem arca, XVI-ordinatus
 * (lex machinulae E14: arca in ansa eundem locellum reddit) */
interior vacuum
_arcas_tabulati_disponere (
    Compilatio* k)
{
    constans MedullaFunctio* f = k->functio;
                        i32  b;
                        i32  j;

    k->arcae_magnitudo  = ZEPHYRUM;
    k->reditus_arca     = -I;
    k->reditus_octeti   = ZEPHYRUM;
    k->arcae_offset = piscina_allocare(k->piscina, (memoriae_index)
        (k->numerus_instructionum + I) * magnitudo(s32));
    k->parametri_arca = piscina_allocare(k->piscina, (memoriae_index)
        (xar_numerus(f->parametra) + I) * magnitudo(s32));
    si (k->arcae_offset == NIHIL || k->parametri_arca == NIHIL)
    {
        _compilationem_recusare(k, "dorsum: memoria deest");
        redde;
    }
    /* locelli aggregatorum liminis PRIMI in arcis (offset parvus) */
    {
        LimenCursor cursor;

        memset(&cursor, ZEPHYRUM, magnitudo(cursor));
        per (b = ZEPHYRUM; b < (i32)xar_numerus(f->parametra); b++)
        {
            constans MedullaParametrum* p = xar_obtinere(f->parametra,
                b);
            LimenSedes s = _limen_collocare(&cursor, &p->limen);

            k->parametri_arca[b] = -I;
            si (   s.genus == LIMEN_NULLUM
                || (s.genus == LIMEN_REGISTRUM && s.octeti > ZEPHYRUM))
            {
                k->parametri_arca[b]  = (s32)k->arcae_magnitudo;
                k->arcae_magnitudo    += XVI;
            }
            si (s.genus == LIMEN_NULLUM)
            {
                k->reditus_arca    = (s64)k->parametri_arca[b];
                k->reditus_octeti  = s.octeti;
            }
        }
    }
    per (b = ZEPHYRUM; b < (i32)xar_numerus(f->blocci); b++)
    {
        constans MedullaBloccus* bloccus = xar_obtinere(f->blocci, b);

        per (j = ZEPHYRUM; j < (i32)xar_numerus(bloccus->instructiones);
             j++)
        {
            constans MedullaInstructio* ins = xar_obtinere(
                bloccus->instructiones, j);

            k->arcae_offset[k->basis_plana[b] + j] = -I;
            si (   ins->op == MEDULLA_OP_VOCARE
                && ins->extra_numerus > k->pons_verba)
            {
                k->pons_verba = ins->extra_numerus;
            }
            si (ins->op == MEDULLA_OP_VOCARE)
            {
                LimenCursor cursor;
                        s32 a;

                memset(&cursor, ZEPHYRUM, magnitudo(cursor));
                per (a = ZEPHYRUM; a < ins->extra_numerus; a++)
                {
                    constans MedullaTypusLiminis* limen =
                        _limen_argumenti(k, ins, a);

                    si (limen != NIHIL)
                    {
                        (vacuum)_limen_collocare(&cursor, limen);
                    }
                }
                cursor.stiva = (cursor.stiva + XV) & ~(s64)XV;
                si (cursor.stiva > k->exitus_magnitudo)
                {
                    k->exitus_magnitudo = cursor.stiva;
                }
            }
            si (   ins->op != MEDULLA_OP_ARCA
                || !medulla_fluxus_attingibilis(k->fluxus, (s32)b))
            {
                perge;
            }
            k->arcae_offset[k->basis_plana[b] + j] =
                (s32)k->arcae_magnitudo;
            k->arcae_magnitudo += (ins->a.datum.immediatum + XV)
                & ~(s64)XV;
            si (k->arcae_magnitudo > (s64)TABULATUM_MAXIMUM)
            {
                _compilationem_recusare(k,
                    "dorsum: tabulatum nimis magnum");
                redde;
            }
        }
    }
}

/* rd = sp + offset (duobus immediatis XII bitorum) */
interior vacuum
_inscriptionem_tabulati (
    Compilatio* k,
           s64  offset,
           s32  rd)
{
    _a64(k, CODIFICATOR_A64_ADD_IMM, LXIV, rd, R_SP, 0,
        offset & (s64)0xFFF, 0);
    si ((offset >> XII) != ZEPHYRUM)
    {
        _a64(k, CODIFICATOR_A64_ADD_IMM, LXIV, rd, rd, 0, offset >> XII,
            XII);
    }
}

interior s32
_octeti_typi (
    s32 typus)
{
    commutatio (typus)
    {
    casus MEDULLA_TYPUS_I8:
    casus MEDULLA_TYPUS_S8:
        redde I;
    casus MEDULLA_TYPUS_I16:
    casus MEDULLA_TYPUS_S16:
        redde II;
    casus MEDULLA_TYPUS_I32:
    casus MEDULLA_TYPUS_S32:
        redde IV;
    ordinarius:
        redde VIII;
    }
}

/* copia a (finis), b (fons), c (magnitudo): memmove in linea, per
 * octetos - superpositio in utramque partem recta. x0-x2 et x16
 * libera sunt; nulla vocatio (temporaria manent). */
interior vacuum
_copiam_emittere (
                    Compilatio* k,
    constans MedullaInstructio* ins)
{
    s32 nota_finis    = codificator_a64_notam_creare(k->c);
    s32 nota_retro    = codificator_a64_notam_creare(k->c);
    s32 nota_ante     = codificator_a64_notam_creare(k->c);
    s32 nota_retro_c  = codificator_a64_notam_creare(k->c);
    s32 r;

    r = _fons(k, &ins->a, LXIV, 0);
    _registrum_movere(k, LXIV, 0, r);
    r = _fons(k, &ins->b, LXIV, I);
    _registrum_movere(k, LXIV, I, r);
    r = _fons(k, &ins->c, LXIV, II);
    _registrum_movere(k, LXIV, II, r);
    _saltum_localem_emittere(k, CODIFICATOR_A64_CBZ, LXIV, II,
        CODIFICATOR_A64_AL, nota_finis);
    /* finis > fons: a cauda retro (fons non obruitur) */
    _a64(k, CODIFICATOR_A64_SUBS_REG, LXIV, R_NULLUM, 0, I, 0, 0);
    _saltum_localem_emittere(k, CODIFICATOR_A64_B_COND, LXIV, 0,
        CODIFICATOR_A64_HI, nota_retro);
    codificator_a64_notam_ponere(k->c, nota_ante);
    _a64(k, CODIFICATOR_A64_LDR_IMM, I, R_OPERIS_A, I, 0, 0, 0);
    _a64(k, CODIFICATOR_A64_STR_IMM, I, R_OPERIS_A, 0, 0, 0, 0);
    _a64(k, CODIFICATOR_A64_ADD_IMM, LXIV, I, I, 0, I, 0);
    _a64(k, CODIFICATOR_A64_ADD_IMM, LXIV, 0, 0, 0, I, 0);
    _a64(k, CODIFICATOR_A64_SUB_IMM, LXIV, II, II, 0, I, 0);
    _saltum_localem_emittere(k, CODIFICATOR_A64_CBNZ, LXIV, II,
        CODIFICATOR_A64_AL, nota_ante);
    _saltum_localem_emittere(k, CODIFICATOR_A64_B, LXIV, 0,
        CODIFICATOR_A64_AL, nota_finis);
    codificator_a64_notam_ponere(k->c, nota_retro);
    _a64(k, CODIFICATOR_A64_ADD_REG, LXIV, 0, 0, II, 0, 0);
    _a64(k, CODIFICATOR_A64_ADD_REG, LXIV, I, I, II, 0, 0);
    codificator_a64_notam_ponere(k->c, nota_retro_c);
    _a64(k, CODIFICATOR_A64_SUB_IMM, LXIV, 0, 0, 0, I, 0);
    _a64(k, CODIFICATOR_A64_SUB_IMM, LXIV, I, I, 0, I, 0);
    _a64(k, CODIFICATOR_A64_LDR_IMM, I, R_OPERIS_A, I, 0, 0, 0);
    _a64(k, CODIFICATOR_A64_STR_IMM, I, R_OPERIS_A, 0, 0, 0, 0);
    _a64(k, CODIFICATOR_A64_SUB_IMM, LXIV, II, II, 0, I, 0);
    _saltum_localem_emittere(k, CODIFICATOR_A64_CBNZ, LXIV, II,
        CODIFICATOR_A64_AL, nota_retro_c);
    codificator_a64_notam_ponere(k->c, nota_finis);
}

interior vacuum
_epilogum_emittere (
    Compilatio* k)
{
    s32 locus = k->numerus_loculorum;
    s32 i;

    per (i = ZEPHYRUM; i < SERVATA_NUMERUS; i++)
    {
        si (k->servata_usa[i])
        {
            _a64(k, CODIFICATOR_A64_LDR_IMM, VIII, SERVATA_PRIMUM + i,
                R_SP, 0, k->exitus_magnitudo + (s64)locus * VIII, 0);
            locus++;
        }
    }
    /* mov sp,x29; ldp x29,x30,[sp],#16; ret */
    _a64(k, CODIFICATOR_A64_ADD_IMM, LXIV, R_SP, R_FP, 0, 0, 0);
    _a64(k, CODIFICATOR_A64_LDP, VIII, R_FP, R_SP, R_LR, (s64)XVI,
        CODIFICATOR_A64_POST);
    _a64(k, CODIFICATOR_A64_RET, LXIV, 0, R_LR, 0, 0, 0);
}

/* operatio binaria rd = a OP b per formam registrorum */
interior vacuum
_binariam_emittere (
                    Compilatio* k,
    constans MedullaInstructio* ins,
                           s32  forma)
{
    s32 lat  = _latitudo(ins->typus);
    s32 ra   = _fons(k, &ins->a, lat, R_OPERIS_A);
    s32 rb   = _fons(k, &ins->b, lat, R_OPERIS_B);
    s32 rd   = _destinatio(k, ins->destinatio);

    si (forma == CODIFICATOR_A64_MADD)
    {
        CodificatorA64Instructio i;

        si (k->fractum)
        {
            redde;
        }
        memset(&i, ZEPHYRUM, magnitudo(i));
        i.forma     = CODIFICATOR_A64_MADD;
        i.latitudo  = lat;
        i.rd        = rd;
        i.rn        = ra;
        i.rm        = rb;
        i.ra        = R_NULLUM;
        si (!codificator_a64_emittere(k->c, &i))
        {
            _compilationem_recusare(k,
                "dorsum: instructio non codificabilis");
        }
    }
    alioquin
    {
        _a64(k, forma, lat, rd, ra, rb, 0, 0);
    }
    _destinationem_condere(k, ins->destinatio, rd);
}

/* conditio A64 comparationis medullae per signum typi */
interior s32
_conditio_comparationis (
    s32 op,
    s32 typus)
{
    b32 signata = typus == MEDULLA_TYPUS_S32
        || typus == MEDULLA_TYPUS_S64;

    commutatio (op)
    {
    casus MEDULLA_OP_AEQUALIS:
        redde CODIFICATOR_A64_EQ;
    casus MEDULLA_OP_INAEQUALIS:
        redde CODIFICATOR_A64_NE;
    casus MEDULLA_OP_MINOR:
        redde signata ? CODIFICATOR_A64_LT : CODIFICATOR_A64_LO;
    casus MEDULLA_OP_MINOR_AEQUALIS:
        redde signata ? CODIFICATOR_A64_LE : CODIFICATOR_A64_LS;
    casus MEDULLA_OP_MAIOR:
        redde signata ? CODIFICATOR_A64_GT : CODIFICATOR_A64_HI;
    ordinarius:   /* MAIOR_AEQUALIS */
        redde signata ? CODIFICATOR_A64_GE : CODIFICATOR_A64_HS;
    }
}

/* comparatio in ramum sequentem plicatur si fructus eius in eo ramo
 * SOLO legitur: intervallum registri = [definitio, usus rami] */
interior b32
_comparatio_plicanda (
                    Compilatio* k,
    constans MedullaInstructio* ins,
    constans MedullaInstructio* sequens)
{
    i32 initium;
    i32 finis;

    si (   sequens == NIHIL || sequens->op != MEDULLA_OP_RAMUS
        || sequens->a.genus != MEDULLA_OPERANDUM_REGISTRUM
        || sequens->a.datum.index != ins->destinatio
        || !medulla_fluxus_intervallum(k->fluxus, ins->destinatio,
               &initium, &finis))
    {
        redde FALSUM;
    }
    redde initium == k->index_dorsi * II + I
        && finis == (k->index_dorsi + I) * II;
}

/* saltus (b, b.cond, cbnz) ad notam blocci */
interior vacuum
_saltum_emittere (
    Compilatio* k,
           s32  forma,
           s32  latitudo,
           s32  locellus,
           s32  conditio,
           s32  bloccus)
{
    CodificatorA64Instructio i;

    si (k->fractum)
    {
        redde;
    }
    si (   bloccus < ZEPHYRUM
        || bloccus >= (s32)xar_numerus(k->functio->blocci))
    {
        _compilationem_recusare(k, "dorsum: saltus ad bloccum insanum");
        redde;
    }
    memset(&i, ZEPHYRUM, magnitudo(i));
    i.forma     = forma;
    i.latitudo  = latitudo;
    i.rd        = locellus;
    i.conditio  = conditio;
    si (!codificator_a64_saltum_emittere(k->c, &i, k->notae[bloccus]))
    {
        _compilationem_recusare(k,
            "dorsum: saltus non codificabilis");
    }
}

/* valorem in ra ad figuram typi in rd aptat: signata angusta signo in
 * XXXII bitos extensa, insignata angusta zephyro, XXXII per formam W
 * (pars superior zephyra), LXIV integra */
interior vacuum
_latitudinem_aptare (
    Compilatio* k,
           s32  typus,
           s32  rd,
           s32  ra)
{
    commutatio (typus)
    {
    casus MEDULLA_TYPUS_S8:
        _a64(k, CODIFICATOR_A64_SBFM, XXXII, rd, ra, 0, 0, VII);
        frange;
    casus MEDULLA_TYPUS_S16:
        _a64(k, CODIFICATOR_A64_SBFM, XXXII, rd, ra, 0, 0, XV);
        frange;
    casus MEDULLA_TYPUS_I8:
        _a64(k, CODIFICATOR_A64_UBFM, XXXII, rd, ra, 0, 0, VII);
        frange;
    casus MEDULLA_TYPUS_I16:
        _a64(k, CODIFICATOR_A64_UBFM, XXXII, rd, ra, 0, 0, XV);
        frange;
    casus MEDULLA_TYPUS_I32:
    casus MEDULLA_TYPUS_S32:
        _a64(k, CODIFICATOR_A64_ORR_REG, XXXII, rd, R_NULLUM, ra, 0, 0);
        frange;
    ordinarius:
        _a64(k, CODIFICATOR_A64_ORR_REG, LXIV, rd, R_NULLUM, ra, 0, 0);
        frange;
    }
}

/* stipes halitus: officina_effugium_salire(codex, pc, fp) - numquam
 * redit. pc = inscriptio stipitis ipsius (adr), intra codicem
 * instructionis medullae currentis: mappa pc eam invenit. x0-x2 et
 * x16 libera sunt (argumenta in prologo iam mota). */
interior vacuum
_stipitem_emittere (
    Compilatio* k,
           s32  genus,
           s32  index)
{
    vacuum (*salire)(s64, memoriae_index, memoriae_index)
        = officina_effugium_salire;
    memoriae_index inscriptio;
               s64 codex = (s64)(((i64)genus << XXXII)
                   | ((i64)index & (i64)0xFFFFFFFF));

    memcpy(&inscriptio, &salire, magnitudo(inscriptio));
    _constantem_ponere(k, codex, LXIV, 0);
    _a64(k, CODIFICATOR_A64_ADR, LXIV, I, 0, 0, 0, 0);
    _registrum_movere(k, LXIV, II, R_FP);
    _constantem_ponere(k, (s64)inscriptio, LXIV, R_OPERIS_A);
    _a64(k, CODIFICATOR_A64_BLR, LXIV, 0, R_OPERIS_A, 0, 0, 0);
}

/* saltus ad notam localem (non blocci) */
interior vacuum
_saltum_localem_emittere (
    Compilatio* k,
           s32  forma,
           s32  latitudo,
           s32  locellus,
           s32  conditio,
           s32  nota)
{
    CodificatorA64Instructio i;

    si (k->fractum)
    {
        redde;
    }
    memset(&i, ZEPHYRUM, magnitudo(i));
    i.forma     = forma;
    i.latitudo  = latitudo;
    i.rd        = locellus;
    i.conditio  = conditio;
    si (!codificator_a64_saltum_emittere(k->c, &i, nota))
    {
        _compilationem_recusare(k,
            "dorsum: saltus non codificabilis");
    }
}

/* divisio et residuum integra: probationes ANTE divisionem (sdiv
 * A64 numquam ruit, minimum / -1 tacite involvit) - divisor
 * zephyrus; signata: minimum TYPI / -1 (lex machinulae) */
interior vacuum
_divisionem_emittere (
                    Compilatio* k,
    constans MedullaInstructio* ins)
{
    s32 lat      = _latitudo(ins->typus);
    b32 signata  = ins->typus == MEDULLA_TYPUS_S32
        || ins->typus == MEDULLA_TYPUS_S64;
    s32 ra                 = _fons(k, &ins->a, lat, R_OPERIS_A);
    s32 rb                 = _fons(k, &ins->b, lat, R_OPERIS_B);
    s32 rd                 = _destinatio(k, ins->destinatio);
    s32 nota_non_zephyrus  = codificator_a64_notam_creare(k->c);
    CodificatorA64Instructio i;

    _saltum_localem_emittere(k, CODIFICATOR_A64_CBNZ, lat, rb,
        CODIFICATOR_A64_AL, nota_non_zephyrus);
    _stipitem_emittere(k, DORSUM_SALTUS_DIVISIO_ZEPHYRUM, ZEPHYRUM);
    codificator_a64_notam_ponere(k->c, nota_non_zephyrus);
    si (signata)
    {
        s32 nota_tuta = codificator_a64_notam_creare(k->c);

        /* x0 = -1; cmp rb, x0; b.ne tuta (latitudine typi: formae W
         * partem superiorem neglegunt) */
        _a64(k, CODIFICATOR_A64_MOVN, lat, 0, 0, 0, 0, 0);
        _a64(k, CODIFICATOR_A64_SUBS_REG, lat, R_NULLUM, rb, 0, 0, 0);
        _saltum_localem_emittere(k, CODIFICATOR_A64_B_COND, LXIV, 0,
            CODIFICATOR_A64_NE, nota_tuta);
        /* x0 = minimum typi; cmp ra, x0; b.ne tuta */
        _a64(k, CODIFICATOR_A64_MOVZ, lat, 0, 0, 0, (s64)0x8000,
            (lat == LXIV) ? XLVIII : XVI);
        _a64(k, CODIFICATOR_A64_SUBS_REG, lat, R_NULLUM, ra, 0, 0, 0);
        _saltum_localem_emittere(k, CODIFICATOR_A64_B_COND, LXIV, 0,
            CODIFICATOR_A64_NE, nota_tuta);
        _stipitem_emittere(k, DORSUM_SALTUS_DIVISIO_SUPERFLUXUS,
            ZEPHYRUM);
        codificator_a64_notam_ponere(k->c, nota_tuta);
    }
    si (ins->op == MEDULLA_OP_DIVIDERE)
    {
        _a64(k, signata ? CODIFICATOR_A64_SDIV : CODIFICATOR_A64_UDIV,
            lat, rd, ra, rb, 0, 0);
    }
    alioquin
    {
        /* quotiens in x0; residuum = a - quotiens * b (msub) */
        _a64(k, signata ? CODIFICATOR_A64_SDIV : CODIFICATOR_A64_UDIV,
            lat, 0, ra, rb, 0, 0);
        si (!k->fractum)
        {
            memset(&i, ZEPHYRUM, magnitudo(i));
            i.forma     = CODIFICATOR_A64_MSUB;
            i.latitudo  = lat;
            i.rd        = rd;
            i.rn        = 0;
            i.rm        = rb;
            i.ra        = ra;
            si (!codificator_a64_emittere(k->c, &i))
            {
                _compilationem_recusare(k,
                    "dorsum: instructio non codificabilis");
            }
        }
    }
    _destinationem_condere(k, ins->destinatio, rd);
}

/* vocatio aedificati cum statu per machinula_pons_nativus(index,
 * argumenta, numerus, pc, fp). Argumenta ut verba canonica LXIV in
 * aream pontis tabulati scribuntur, ut machinula ea aedificatis
 * praebet: signata signo extensa, insignata zephyro, immediatum
 * fluitans figura bitorum (f32 in dimidio inferiore), aggregatum
 * inscriptione sua. */
interior vacuum
_pontem_emittere (
                    Compilatio* k,
    constans MedullaInstructio* ins,
                           s32  aedificatum)
{
    s64 (*pons)(s64, constans i64*, s64, memoriae_index, memoriae_index)
        = machinula_pons_nativus;
    memoriae_index inscriptio;
               s32 j;

    memcpy(&inscriptio, &pons, magnitudo(inscriptio));
    per (j = ZEPHYRUM; j < ins->extra_numerus; j++)
    {
        constans MedullaTypusLiminis* limen = _limen_argumenti(k, ins,
            j);
        constans MedullaOperandum* operandum = xar_obtinere(
            k->functio->operanda, (i32)(ins->extra_index + j));
        s32 typus = (limen != NIHIL) ? limen->typus
            : MEDULLA_TYPUS_S64;

        si (operandum->genus == MEDULLA_OPERANDUM_IMMEDIATUM_F)
        {
            unio { f64 f; i64 v; } lata;
            unio { f32 f; i32 v; } angusta;

            lata.f     = operandum->datum.immediatum_f;
            angusta.f  = (f32)operandum->datum.immediatum_f;
            _constantem_ponere(k, (typus == MEDULLA_TYPUS_F32)
                ? (s64)(i64)angusta.v : (s64)lata.v, LXIV,
                R_OPERIS_A);
        }
        alioquin si (operandum->genus == MEDULLA_OPERANDUM_IMMEDIATUM)
        {
            _constantem_ponere(k, _canonicum_typi(
                operandum->datum.immediatum, typus), LXIV, R_OPERIS_A);
        }
        alioquin
        {
            s32 r = _fons(k, operandum, _latitudo(typus), R_OPERIS_A);

            commutatio (typus)
            {
            casus MEDULLA_TYPUS_S8:
                _a64(k, CODIFICATOR_A64_SBFM, LXIV, R_OPERIS_A, r, 0, 0,
                    VII);
                frange;
            casus MEDULLA_TYPUS_S16:
                _a64(k, CODIFICATOR_A64_SBFM, LXIV, R_OPERIS_A, r, 0, 0,
                    XV);
                frange;
            casus MEDULLA_TYPUS_S32:
                _a64(k, CODIFICATOR_A64_SBFM, LXIV, R_OPERIS_A, r, 0, 0,
                    XXXI);
                frange;
            casus MEDULLA_TYPUS_I8:
            casus MEDULLA_TYPUS_I16:
            casus MEDULLA_TYPUS_I32:
                _latitudinem_aptare(k, typus, R_OPERIS_A, r);
                frange;
            ordinarius:
                _registrum_movere(k, LXIV, R_OPERIS_A, r);
                frange;
            }
        }
        _a64(k, CODIFICATOR_A64_STR_IMM, VIII, R_OPERIS_A, R_SP, 0,
            k->pons_basis + (s64)j * VIII, 0);
    }
    /* x0 = aedificatum; x1 = verba; x2 = numerus; x3 = pc (hic);
     * x4 = fp */
    _constantem_ponere(k, (s64)aedificatum, LXIV, 0);
    _inscriptionem_tabulati(k, k->pons_basis, I);
    _constantem_ponere(k, (s64)ins->extra_numerus, LXIV, II);
    _a64(k, CODIFICATOR_A64_ADR, LXIV, III, 0, 0, 0, 0);
    _registrum_movere(k, LXIV, IV, R_FP);
    _constantem_ponere(k, (s64)inscriptio, LXIV, R_OPERIS_A);
    _a64(k, CODIFICATOR_A64_BLR, LXIV, 0, R_OPERIS_A, 0, 0, 0);
    si (ins->destinatio >= ZEPHYRUM)
    {
        s32 rd = _destinatio(k, ins->destinatio);

        /* fructus pontis verbum canonicum: figura typi vocationis */
        _latitudinem_aptare(k, ins->typus, rd, 0);
        _destinationem_condere(k, ins->destinatio, rd);
    }
}

/* rd = octeti (I - VIII) ab [basis + offset], EXACTE: nullus octetus
 * ultra aggregatum legitur. operis = registrum liberum (rd != basis).
 */
interior vacuum
_verbum_exactum_legere (
    Compilatio* k,
           s32  rd,
           s32  basis,
           s64  offset,
           s32  octeti,
           s32  operis)
{
    s32 situs   = ZEPHYRUM;
    b32 primum  = VERUM;
    s32 pars;

    per (pars = VIII; pars >= I; pars /= II)
    {
        si (octeti - situs < pars)
        {
            perge;
        }
        si (primum)
        {
            _a64(k, CODIFICATOR_A64_LDR_IMM, pars, rd, basis, 0,
                offset + situs, 0);
            primum = FALSUM;
        }
        alioquin
        {
            _a64(k, CODIFICATOR_A64_LDR_IMM, pars, operis, basis, 0,
                offset + situs, 0);
            _a64(k, CODIFICATOR_A64_ORR_REG, LXIV, rd, rd, operis, 0,
                situs * VIII);
        }
        situs += pars;
    }
}

/* octetos (I - VIII) infimos rs in [basis + offset] scribit, EXACTE
 * (vicini aggregati non tanguntur); rs destruitur */
interior vacuum
_verbum_exactum_scribere (
    Compilatio* k,
           s32  rs,
           s32  basis,
           s64  offset,
           s32  octeti)
{
    s32 situs = ZEPHYRUM;
    s32 pars;

    per (pars = VIII; pars >= I; pars /= II)
    {
        si (octeti - situs < pars)
        {
            perge;
        }
        _a64(k, CODIFICATOR_A64_STR_IMM, pars, rs, basis, 0,
            offset + situs, 0);
        situs += pars;
        si (situs < octeti)
        {
            /* lsr rs, rs, #(pars * 8) */
            _a64(k, CODIFICATOR_A64_UBFM, LXIV, rs, rs, 0,
                (s64)pars * VIII, LXIII);
        }
    }
}

/* argumenta vocationis nativae in sedes AAPCS64 ponit. Reddit
 * magnitudinem %reditus parvi (inscriptio finis in verbo primo areae
 * pontis servata - registrum eius vocationem non necessario transit),
 * aut zephyrum. */
interior s32
_argumenta_emittere (
                    Compilatio* k,
    constans MedullaInstructio* ins)
{
    LimenCursor cursor;
            s32 reditus_octeti = ZEPHYRUM;
            s32 transitus;
            s32 j;

    /* transitus 0: stiva exiens ([sp + offset]) - PRIMUM, dum x0-x7
     * adhuc libera; transitus 1: registra (sedes numquam in x0-x8:
     * nullus ordo motuum servandus) */
    per (transitus = ZEPHYRUM; transitus < II; transitus++)
    {
        memset(&cursor, ZEPHYRUM, magnitudo(cursor));
        per (j = ZEPHYRUM; j < ins->extra_numerus; j++)
        {
            constans MedullaTypusLiminis* limen = _limen_argumenti(k,
                ins, j);
            constans MedullaOperandum* operandum = xar_obtinere(
                k->functio->operanda, (i32)(ins->extra_index + j));
            LimenSedes s;
                   s32 r;
                   s32 v;

            si (limen == NIHIL)
            {
                _compilationem_recusare(k,
                    "dorsum: argumentum sine typo");
                redde ZEPHYRUM;
            }
            s = _limen_collocare(&cursor, limen);
            si ((s.genus == LIMEN_STIVA) != (transitus == ZEPHYRUM))
            {
                perge;
            }
            commutatio (s.genus)
            {
            casus LIMEN_STIVA:
                si (s.octeti > ZEPHYRUM)
                {
                    r = _fons(k, operandum, LXIV, R_OPERIS_A);
                    per (v = ZEPHYRUM; v * VIII < s.octeti; v++)
                    {
                        _verbum_exactum_legere(k, R_OPERIS_B, r,
                            (s64)v * VIII,
                            (s.octeti - v * VIII < VIII)
                                ? s.octeti - v * VIII : VIII, 0);
                        _a64(k, CODIFICATOR_A64_STR_IMM, VIII,
                            R_OPERIS_B, R_SP, 0,
                            s.offset + (s64)v * VIII, 0);
                    }
                    frange;
                }
                r = _fons(k, operandum, _latitudo(s.typus),
                    R_OPERIS_A);
                _a64(k, CODIFICATOR_A64_STR_IMM, _octeti_typi(s.typus),
                    r, R_SP, 0, s.offset, 0);
                frange;
            casus LIMEN_REGISTRUM:
                si (s.octeti > ZEPHYRUM)
                {
                    r = _fons(k, operandum, LXIV, R_OPERIS_A);
                    per (v = ZEPHYRUM; v * VIII < s.octeti; v++)
                    {
                        _verbum_exactum_legere(k, s.index_x + v, r,
                            (s64)v * VIII,
                            (s.octeti - v * VIII < VIII)
                                ? s.octeti - v * VIII : VIII,
                            R_OPERIS_B);
                    }
                    frange;
                }
                r = _fons(k, operandum, _latitudo(s.typus),
                    s.index_x);
                _registrum_movere(k, LXIV, s.index_x, r);
                frange;
            casus LIMEN_X_OCTO:
                r = _fons(k, operandum, LXIV, VIII);
                _registrum_movere(k, LXIV, VIII, r);
                frange;
            ordinarius:     /* LIMEN_NULLUM */
                r = _fons(k, operandum, LXIV, R_OPERIS_A);
                _a64(k, CODIFICATOR_A64_STR_IMM, VIII, r, R_SP, 0,
                    k->pons_basis, 0);
                reditus_octeti = s.octeti;
                frange;
            }
        }
    }
    redde reditus_octeti;
}

/* post vocationem: x0:x1 in locum %reditus parvi vocantis, octetos
 * EXACTE */
interior vacuum
_reditum_parvum_condere (
    Compilatio* k,
           s32  octeti)
{
    si (octeti <= ZEPHYRUM)
    {
        redde;
    }
    _a64(k, CODIFICATOR_A64_LDR_IMM, VIII, R_OPERIS_A, R_SP, 0,
        k->pons_basis, 0);
    _verbum_exactum_scribere(k, 0, R_OPERIS_A, 0,
        (octeti < VIII) ? octeti : VIII);
    si (octeti > VIII)
    {
        _verbum_exactum_scribere(k, I, R_OPERIS_A, VIII, octeti - VIII);
    }
}

/* sequens = instructio proxima in blocco (NIHIL ultima) */
interior vacuum
_instructionem_emittere (
                    Compilatio* k,
    constans MedullaInstructio* ins,
    constans MedullaInstructio* sequens)
{
    s32 lat = _latitudo(ins->typus);

    commutatio (ins->op)
    {
    casus MEDULLA_OP_MOVERE:
    {
        s32 rd = _destinatio(k, ins->destinatio);

        si (ins->a.genus == MEDULLA_OPERANDUM_IMMEDIATUM)
        {
            /* verbum canonicum plenum (ut machinula): tutum sub
             * omni latitudine */
            _constantem_ponere(k, _canonicum_typi(
                ins->a.datum.immediatum, ins->typus), LXIV, rd);
        }
        alioquin
        {
            s32 ra = _fons(k, &ins->a, lat, R_OPERIS_A);

            _latitudinem_aptare(k, ins->typus, rd, ra);
        }
        _destinationem_condere(k, ins->destinatio, rd);
        frange;
    }
    casus MEDULLA_OP_LEGERE:
    {
        /* onus typatum: signata signo in LXIV extensa (verbum
         * canonicum plenum), insignata zephyro */
        s32 ra      = _fons(k, &ins->a, LXIV, R_OPERIS_A);
        s32 rd      = _destinatio(k, ins->destinatio);
        s32 octeti  = _octeti_typi(ins->typus);

        si (_typus_signatus_angustus(ins->typus))
        {
            _a64(k, CODIFICATOR_A64_LDRS_IMM, octeti, rd, ra, 0, 0, 0);
        }
        alioquin
        {
            _a64(k, CODIFICATOR_A64_LDR_IMM, octeti, rd, ra, 0, 0, 0);
        }
        _destinationem_condere(k, ins->destinatio, rd);
        frange;
    }
    casus MEDULLA_OP_SCRIBERE:
    {
        s32 ra = _fons(k, &ins->a, LXIV, R_OPERIS_A);
        s32 rb = _fons(k, &ins->b, lat, R_OPERIS_B);

        _a64(k, CODIFICATOR_A64_STR_IMM, _octeti_typi(ins->typus), rb,
            ra, 0, 0, 0);
        frange;
    }
    casus MEDULLA_OP_ARCA:
    {
        s32 rd      = _destinatio(k, ins->destinatio);
        s32 offset  = (k->mappa_numerus > ZEPHYRUM)
            ? k->arcae_offset[k->mappa_index[k->mappa_numerus - I]]
            : -I;

        si (offset < ZEPHYRUM)
        {
            _compilationem_recusare(k, "dorsum: arca sine locello");
            frange;
        }
        _inscriptionem_tabulati(k, k->arcae_basis + (s64)offset, rd);
        _destinationem_condere(k, ins->destinatio, rd);
        frange;
    }
    casus MEDULLA_OP_LOCUS:
    {
        /* inscriptio sedis symboli (datum aut descriptor functionis)
         * ut constans - regio machinulae non movetur */
        s32 globale = conexio_symbolum_globale(k->d->conexio,
            k->modulus_index, ins->a.datum.index);
        s32 rd = _destinatio(k, ins->destinatio);

        si (globale < ZEPHYRUM)
        {
            _compilationem_recusare(k, "dorsum: locus sine symbolo");
            frange;
        }
        _constantem_ponere(k, (s64)(memoriae_index)
            conexio_symbolum_obtinere(k->d->conexio, globale)->sedes,
            LXIV, rd);
        _destinationem_condere(k, ins->destinatio, rd);
        frange;
    }
    casus MEDULLA_OP_COPIA:
        _copiam_emittere(k, ins);
        frange;
    casus MEDULLA_OP_AMPUTARE:
    {
        s32 ra = _fons(k, &ins->a, lat, R_OPERIS_A);
        s32 rd = _destinatio(k, ins->destinatio);

        _latitudinem_aptare(k, ins->typus_secundus, rd, ra);
        _destinationem_condere(k, ins->destinatio, rd);
        frange;
    }
    casus MEDULLA_OP_EXTENDERE:
    {
        /* verbum canonicum FONTIS in LXIV (signum a fonte) */
        s32 ra = _fons(k, &ins->a, lat, R_OPERIS_A);
        s32 rd = _destinatio(k, ins->destinatio);

        commutatio (ins->typus)
        {
        casus MEDULLA_TYPUS_S8:
            _a64(k, CODIFICATOR_A64_SBFM, LXIV, rd, ra, 0, 0, VII);
            frange;
        casus MEDULLA_TYPUS_S16:
            _a64(k, CODIFICATOR_A64_SBFM, LXIV, rd, ra, 0, 0, XV);
            frange;
        casus MEDULLA_TYPUS_S32:
            _a64(k, CODIFICATOR_A64_SBFM, LXIV, rd, ra, 0, 0, XXXI);
            frange;
        ordinarius:
            _latitudinem_aptare(k, ins->typus, rd, ra);
            frange;
        }
        _destinationem_condere(k, ins->destinatio, rd);
        frange;
    }
    casus MEDULLA_OP_VOCARE:
    {
                   s32 vocatus = _vocatum_resolvere(k, ins);
                   s32 reditus_octeti;
        memoriae_index inscriptio;

        si (vocatus == -I)
        {
            frange;
        }
        si (   vocatus <= -X
            && !_aedificatum_directum(k, ins,
                   AEDIFICATUM_VOCATI(vocatus)))
        {
            _pontem_emittere(k, ins, AEDIFICATUM_VOCATI(vocatus));
            frange;
        }
        reditus_octeti = _argumenta_emittere(k, ins);
        si (vocatus == VOCATUS_INDIRECTUS)
        {
            /* descriptor {signum, index}: NATIVUM -> index est
             * introitus; aliter halitus nominatus (limes C9 in
             * exsecutione) */
            s32 nota_nativa  = codificator_a64_notam_creare(k->c);
            s32 rf           = _fons(k, &ins->a, LXIV, R_OPERIS_A);

            _a64(k, CODIFICATOR_A64_LDR_IMM, VIII, R_OPERIS_B, rf, 0, 0,
                0);
            _a64(k, CODIFICATOR_A64_SUBS_IMM, LXIV, R_NULLUM,
                R_OPERIS_B, 0, (s64)CONEXIO_SIGNUM_NATIVUM, 0);
            _saltum_localem_emittere(k, CODIFICATOR_A64_B_COND, LXIV,
                0, CODIFICATOR_A64_EQ, nota_nativa);
            _stipitem_emittere(k, DORSUM_SALTUS_INDIRECTA, ZEPHYRUM);
            codificator_a64_notam_ponere(k->c, nota_nativa);
            rf = _fons(k, &ins->a, LXIV, R_OPERIS_A);
            _a64(k, CODIFICATOR_A64_LDR_IMM, VIII, R_OPERIS_A, rf, 0,
                VIII, 0);
            _a64(k, CODIFICATOR_A64_BLR, LXIV, 0, R_OPERIS_A, 0, 0, 0);
            _reditum_parvum_condere(k, reditus_octeti);
            si (ins->destinatio >= ZEPHYRUM)
            {
                s32 rd = _destinatio(k, ins->destinatio);

                _a64(k, CODIFICATOR_A64_ORR_REG, lat, rd, R_NULLUM, 0,
                    0, 0);
                _destinationem_condere(k, ins->destinatio, rd);
            }
            frange;
        }
        /* vocatus: se ipsam (introitus futurus = cursor areae) aut
         * iam locatam (clausura ante emissionem compilata) */
        si (vocatus < ZEPHYRUM)
        {
            /* aedificatum purum: functio libc ipsa */
            inscriptio = (memoriae_index)
                machinula_aedificatum_inscriptio(
                    AEDIFICATUM_VOCATI(vocatus));
        }
        alioquin si (vocatus == k->functio_index)
        {
            inscriptio = (memoriae_index)memoria_exsecutabilis_basis(
                k->d->area) + k->d->cursor;
        }
        alioquin
        {
            inscriptio = (memoriae_index)
                k->d->functiones[vocatus].introitus;
        }
        si (inscriptio == ZEPHYRUM)
        {
            _compilationem_recusare(k, "dorsum: vocatus sine introitu");
            frange;
        }
        _constantem_ponere(k, (s64)inscriptio, LXIV, R_OPERIS_A);
        _a64(k, CODIFICATOR_A64_BLR, LXIV, 0, R_OPERIS_A, 0, 0, 0);
        _reditum_parvum_condere(k, reditus_octeti);
        si (ins->destinatio >= ZEPHYRUM)
        {
            s32 rd = _destinatio(k, ins->destinatio);

            _a64(k, CODIFICATOR_A64_ORR_REG, lat, rd, R_NULLUM, 0, 0,
                0);
            _destinationem_condere(k, ins->destinatio, rd);
        }
        frange;
    }
    casus MEDULLA_OP_ADDERE:
    casus MEDULLA_OP_SUBTRAHERE:
    {
        b32 additio = ins->op == MEDULLA_OP_ADDERE;

        /* immediatum XII bitorum in b: forma immediata */
        si (   ins->b.genus            == MEDULLA_OPERANDUM_IMMEDIATUM
            && ins->b.datum.immediatum >= ZEPHYRUM
            && ins->b.datum.immediatum < (s64)4096)
        {
            s32 ra = _fons(k, &ins->a, lat, R_OPERIS_A);
            s32 rd = _destinatio(k, ins->destinatio);

            _a64(k, additio ? CODIFICATOR_A64_ADD_IMM
                : CODIFICATOR_A64_SUB_IMM, lat, rd, ra, 0,
                ins->b.datum.immediatum, 0);
            _destinationem_condere(k, ins->destinatio, rd);
        }
        alioquin
        {
            _binariam_emittere(k, ins, additio
                ? CODIFICATOR_A64_ADD_REG : CODIFICATOR_A64_SUB_REG);
        }
        frange;
    }
    casus MEDULLA_OP_MULTIPLICARE:
        _binariam_emittere(k, ins, CODIFICATOR_A64_MADD);
        frange;
    casus MEDULLA_OP_DIVIDERE:
    casus MEDULLA_OP_RESIDUUM:
        _divisionem_emittere(k, ins);
        frange;
    casus MEDULLA_OP_SISTERE:
        _stipitem_emittere(k, DORSUM_SALTUS_SISTERE,
            ins->a.datum.index);
        frange;
    casus MEDULLA_OP_ET:
        _binariam_emittere(k, ins, CODIFICATOR_A64_AND_REG);
        frange;
    casus MEDULLA_OP_VEL:
        _binariam_emittere(k, ins, CODIFICATOR_A64_ORR_REG);
        frange;
    casus MEDULLA_OP_AUT:
        _binariam_emittere(k, ins, CODIFICATOR_A64_EOR_REG);
        frange;
    casus MEDULLA_OP_SINISTRORSUM:
        _binariam_emittere(k, ins, CODIFICATOR_A64_LSLV);
        frange;
    casus MEDULLA_OP_DEXTRORSUM:
        _binariam_emittere(k, ins,
            (ins->typus == MEDULLA_TYPUS_S32
                || ins->typus == MEDULLA_TYPUS_S64)
            ? CODIFICATOR_A64_ASRV : CODIFICATOR_A64_LSRV);
        frange;
    casus MEDULLA_OP_NEGARE:
    casus MEDULLA_OP_COMPLEMENTUM:
    {
        s32 ra = _fons(k, &ins->a, lat, R_OPERIS_A);
        s32 rd = _destinatio(k, ins->destinatio);

        /* neg: sub rd, zr, ra; mvn: orn rd, zr, ra */
        _a64(k, (ins->op == MEDULLA_OP_NEGARE)
            ? CODIFICATOR_A64_SUB_REG : CODIFICATOR_A64_ORN_REG,
            lat, rd, R_NULLUM, ra, 0, 0);
        _destinationem_condere(k, ins->destinatio, rd);
        frange;
    }
    casus MEDULLA_OP_AEQUALIS:
    casus MEDULLA_OP_INAEQUALIS:
    casus MEDULLA_OP_MINOR:
    casus MEDULLA_OP_MINOR_AEQUALIS:
    casus MEDULLA_OP_MAIOR:
    casus MEDULLA_OP_MAIOR_AEQUALIS:
    {
        s32 conditio  = _conditio_comparationis(ins->op, ins->typus);
        s32 ra        = _fons(k, &ins->a, lat, R_OPERIS_A);

        /* cmp ra, b (subs zr, ra, b) */
        si (   ins->b.genus            == MEDULLA_OPERANDUM_IMMEDIATUM
            && ins->b.datum.immediatum >= ZEPHYRUM
            && ins->b.datum.immediatum < (s64)4096)
        {
            _a64(k, CODIFICATOR_A64_SUBS_IMM, lat, R_NULLUM, ra, 0,
                ins->b.datum.immediatum, 0);
        }
        alioquin
        {
            s32 rb = _fons(k, &ins->b, lat, R_OPERIS_B);

            _a64(k, CODIFICATOR_A64_SUBS_REG, lat, R_NULLUM, ra, rb,
                0, 0);
        }
        si (_comparatio_plicanda(k, ins, sequens))
        {
            /* fructus in ramo sequente solo legitur: vexilla manent,
             * registrum non scribitur */
            k->conditio_plicata = conditio;
        }
        alioquin
        {
            s32 rd = _destinatio(k, ins->destinatio);
            CodificatorA64Instructio i;

            /* cset rd, conditio = csinc rd, zr, zr, !conditio */
            memset(&i, ZEPHYRUM, magnitudo(i));
            i.forma     = CODIFICATOR_A64_CSINC;
            i.latitudo  = XXXII;
            i.rd        = rd;
            i.rn        = R_NULLUM;
            i.rm        = R_NULLUM;
            i.conditio  = conditio ^ I;
            si (!k->fractum && !codificator_a64_emittere(k->c, &i))
            {
                _compilationem_recusare(k,
                    "dorsum: instructio non codificabilis");
            }
            _destinationem_condere(k, ins->destinatio, rd);
        }
        frange;
    }
    casus MEDULLA_OP_SALIRE:
        si (ins->a.datum.index != k->bloccus_sequens)
        {
            _saltum_emittere(k, CODIFICATOR_A64_B, LXIV, 0,
                CODIFICATOR_A64_AL, ins->a.datum.index);
        }
        frange;
    casus MEDULLA_OP_RAMUS:
        /* a = conditio; b = @verum; c = @falsum */
        si (k->conditio_plicata >= ZEPHYRUM)
        {
            _saltum_emittere(k, CODIFICATOR_A64_B_COND, LXIV, 0,
                k->conditio_plicata, ins->b.datum.index);
            k->conditio_plicata = -I;
        }
        alioquin
        {
            /* verbum totum probatur (machinula: non-zephyrum LXIV) */
            s32 ra = _fons(k, &ins->a, LXIV, R_OPERIS_A);

            _saltum_emittere(k, CODIFICATOR_A64_CBNZ, LXIV, ra,
                CODIFICATOR_A64_AL, ins->b.datum.index);
        }
        si (ins->c.datum.index != k->bloccus_sequens)
        {
            _saltum_emittere(k, CODIFICATOR_A64_B, LXIV, 0,
                CODIFICATOR_A64_AL, ins->c.datum.index);
        }
        frange;
    casus MEDULLA_OP_REDDE:
    {
        s32 typus = k->functio->typus_reditus;

        si (ins->a.genus != MEDULLA_OPERANDUM_NIHIL)
        {
            s32 lat_reditus = _latitudo(typus);
            s32 ra = _fons(k, &ins->a, lat_reditus, R_OPERIS_A);

            _a64(k, CODIFICATOR_A64_ORR_REG, lat_reditus, 0, R_NULLUM,
                ra,
                0, 0);
            /* reditus angusti a vocato extenduntur */
            si (typus == MEDULLA_TYPUS_S8)
            {
                _a64(k, CODIFICATOR_A64_SBFM, XXXII, 0, 0, 0, 0, VII);
            }
            alioquin si (typus == MEDULLA_TYPUS_S16)
            {
                _a64(k, CODIFICATOR_A64_SBFM, XXXII, 0, 0, 0, 0, XV);
            }
            alioquin si (typus == MEDULLA_TYPUS_I8)
            {
                _a64(k, CODIFICATOR_A64_UBFM, XXXII, 0, 0, 0, 0, VII);
            }
            alioquin si (typus == MEDULLA_TYPUS_I16)
            {
                _a64(k, CODIFICATOR_A64_UBFM, XXXII, 0, 0, 0, 0, XV);
            }
        }
        si (k->reditus_octeti > ZEPHYRUM)
        {
            /* %reditus parvus: locellus -> x0:x1 */
            _inscriptionem_tabulati(k, k->arcae_basis + k->reditus_arca,
                R_OPERIS_A);
            _a64(k, CODIFICATOR_A64_LDR_IMM, VIII, 0, R_OPERIS_A, 0, 0,
                0);
            si (k->reditus_octeti > VIII)
            {
                _a64(k, CODIFICATOR_A64_LDR_IMM, VIII, I, R_OPERIS_A, 0,
                    VIII, 0);
            }
        }
        _epilogum_emittere(k);
        frange;
    }
    ordinarius:
        _operationem_recusare(k, ins->op);
        frange;
    }
}

interior vacuum
_corpus_emittere (
    Compilatio* k)
{
    constans MedullaFunctio* f = k->functio;
               constans s32* ordo;
                        s32  numerus_ordinis = ZEPHYRUM;
                        s32  o;
                        i32  j;

    ordo = medulla_fluxus_ordo(k->fluxus, &numerus_ordinis);
    per (o = ZEPHYRUM; o < numerus_ordinis && !k->fractum; o++)
    {
        constans MedullaBloccus* bloccus = xar_obtinere(f->blocci,
            (i32)ordo[o]);

        codificator_a64_notam_ponere(k->c, k->notae[ordo[o]]);
        k->bloccus_sequens = (o + I < numerus_ordinis)
            ? ordo[o + I] : -I;
        per (j = ZEPHYRUM; j < (i32)xar_numerus(bloccus->instructiones)
             && !k->fractum; j++)
        {
            k->mappa_offset[k->mappa_numerus] =
                codificator_a64_numerus(k->c) * IV;
            k->mappa_index[k->mappa_numerus] =
                k->basis_plana[ordo[o]] + j;
            k->mappa_numerus++;
            _instructionem_emittere(k, xar_obtinere(
                bloccus->instructiones, j),
                (j + I < (i32)xar_numerus(bloccus->instructiones))
                    ? xar_obtinere(bloccus->instructiones, j + I)
                    : NIHIL);
            k->index_dorsi++;
        }
    }
}


/* ==================================================
 * IV. Functio tota: probatio -> allocatio -> emissio -> area
 * ================================================== */

interior b32
_functionem_compilare (
    OfficinaDorsum* d,
               s32  functio_index,
            chorda* causa_out);

/* clausura: vocatae directae ante emissionem compilantur (introitus
 * earum in 'bl' vocantis intrant). Vocata recusata aut in opere
 * (recursio mutua) -> vocans recusatur nominans eam. */
interior vacuum
_clausuram_compilare (
    Compilatio* k)
{
    constans MedullaFunctio* f = k->functio;
                        i32  b;
                        i32  j;

    per (b = ZEPHYRUM; b < (i32)xar_numerus(f->blocci) && !k->fractum;
         b++)
    {
        constans MedullaBloccus* bloccus = xar_obtinere(f->blocci, b);

        si (!medulla_fluxus_attingibilis(k->fluxus, (s32)b))
        {
            perge;
        }
        per (j = ZEPHYRUM; j < (i32)xar_numerus(bloccus->instructiones)
             && !k->fractum; j++)
        {
            constans MedullaInstructio* ins = xar_obtinere(
                bloccus->instructiones, j);
                                   s32 vocatus;
                                chorda causa_vocatae;

            si (ins->op != MEDULLA_OP_VOCARE)
            {
                perge;
            }
            vocatus = _vocatum_resolvere(k, ins);
            si (   vocatus < ZEPHYRUM || vocatus == k->functio_index
                || k->d->functiones[vocatus].introitus != NIHIL)
            {
                perge;
            }
            causa_vocatae.datum    = NIHIL;
            causa_vocatae.mensura  = ZEPHYRUM;
            si (   k->d->functiones[vocatus].in_opere
                || !_functionem_compilare(k->d, vocatus,
                       &causa_vocatae))
            {
                constans ConexioFunctioNexa* nexa =
                    conexio_functionem_obtinere(k->d->conexio,
                        (s64)vocatus);
                character nuntius[CXXVIII];

                sprintf(nuntius, "vocat $%.*s interpretatum",
                    (nexa->functio->titulus.mensura < LXXX)
                        ? (int)nexa->functio->titulus.mensura : LXXX,
                    (constans character*)
                        nexa->functio->titulus.datum);
                _compilationem_recusare(k, nuntius);
            }
        }
    }
}

interior b32
_functionem_compilare_intus (
    OfficinaDorsum* d,
               s32  functio_index,
            chorda* causa_out)
{
    constans ConexioFunctioNexa* nexa = conexio_functionem_obtinere(
        d->conexio, (s64)functio_index);
                  DorsumFunctio* df = &d->functiones[functio_index];
                     Compilatio  k;
                            i32  numerus_bloccorum;
                            i32  numerus_instructionum = ZEPHYRUM;
                            i32  b;
                 memoriae_index  octeti;
                             i8* basis;

    memset(&k, ZEPHYRUM, magnitudo(k));
    k.d = d;
    k.piscina = d->piscina;
    k.functio = nexa->functio;
    k.causa_out = causa_out;
    k.conditio_plicata = -I;
    k.bloccus_sequens = -I;
    k.functio_index = functio_index;
    k.modulus_index = nexa->modulus_index;
    k.fluxus = medulla_fluxus_computare(d->piscina, k.functio);
    si (k.fluxus == NIHIL)
    {
        _compilationem_recusare(&k, "dorsum: functio sine bloccis");
        redde FALSUM;
    }
    numerus_bloccorum      = (i32)xar_numerus(k.functio->blocci);
    k.numerus_registrorum  = (s32)xar_numerus(k.functio->registra);
    k.sedes = piscina_allocare(d->piscina, (memoriae_index)
        (k.numerus_registrorum + I) * magnitudo(s32));
    k.notae = piscina_allocare(d->piscina, (memoriae_index)
        (numerus_bloccorum + I) * magnitudo(s32));
    k.basis_plana = piscina_allocare(d->piscina, (memoriae_index)
        (numerus_bloccorum + I) * magnitudo(i32));
    k.c = codificator_a64_creare(d->piscina);
    si (   k.sedes       == NIHIL || k.notae == NIHIL
        || k.basis_plana == NIHIL || k.c == NIHIL)
    {
        _compilationem_recusare(&k, "dorsum: memoria deest");
        redde FALSUM;
    }
    /* index planus: ambulatio canonica (pactum officina_medulla.h) */
    per (b = ZEPHYRUM; b < numerus_bloccorum; b++)
    {
        constans MedullaBloccus* bloccus = xar_obtinere(
            k.functio->blocci, b);

        k.basis_plana[b]  = numerus_instructionum;
        k.notae[b]        = codificator_a64_notam_creare(k.c);
        numerus_instructionum += (i32)xar_numerus(
            bloccus->instructiones);
    }
    k.mappa_offset = piscina_allocare(d->piscina, (memoriae_index)
        (numerus_instructionum + I) * magnitudo(i32));
    k.mappa_index = piscina_allocare(d->piscina, (memoriae_index)
        (numerus_instructionum + I) * magnitudo(i32));
    si (k.mappa_offset == NIHIL || k.mappa_index == NIHIL)
    {
        _compilationem_recusare(&k, "dorsum: memoria deest");
        redde FALSUM;
    }

    k.numerus_instructionum = numerus_instructionum;
    _functionem_probare(&k);
    si (!k.fractum)
    {
        _arcas_tabulati_disponere(&k);
    }
    si (!k.fractum)
    {
        _clausuram_compilare(&k);
    }
    si (!k.fractum)
    {
        _registra_allocare(&k);
    }
    si (!k.fractum)
    {
        _prologum_emittere(&k);
        _corpus_emittere(&k);
    }
    si (!k.fractum && !codificator_a64_finire(k.c, causa_out))
    {
        k.fractum = VERUM;
    }
    si (k.fractum)
    {
        redde FALSUM;
    }

    octeti = (memoriae_index)codificator_a64_numerus(k.c) * IV;
    si (d->cursor + octeti > memoria_exsecutabilis_mensura(d->area))
    {
        _compilationem_recusare(&k, "dorsum: area codicis plena");
        redde FALSUM;
    }
    basis = (i8*)memoria_exsecutabilis_basis(d->area) + d->cursor;
    memoria_exsecutabilis_aperire(d->area);
    memcpy(basis, codificator_a64_verba(k.c), octeti);
    si (!memoria_exsecutabilis_sigillare(d->area, d->cursor, octeti))
    {
        _compilationem_recusare(&k, "dorsum: codex sigillari nequit");
        redde FALSUM;
    }
    df->introitus = basis;
    df->initium = d->cursor;
    df->finis = d->cursor + octeti;
    df->mappa_offset = k.mappa_offset;
    df->mappa_index = k.mappa_index;
    df->mappa_numerus = k.mappa_numerus;
    d->cursor = (d->cursor + octeti + XV) & ~(memoriae_index)XV;
    redde VERUM;
}

interior b32
_functionem_compilare (
    OfficinaDorsum* d,
               s32  functio_index,
            chorda* causa_out)
{
    b32 bene;

    d->functiones[functio_index].in_opere = VERUM;
    bene = _functionem_compilare_intus(d, functio_index, causa_out);
    d->functiones[functio_index].in_opere = FALSUM;
    redde bene;
}


/* ==================================================
 * Superficies
 * ================================================== */

OfficinaDorsum*
officina_dorsum_creare (
                 Piscina* piscina,
        constans Conexio* conexio,
    MemoriaExsecutabilis* area)
{
    OfficinaDorsum* d;
               i32  numerus;
               i32  g;

    si (piscina == NIHIL || conexio == NIHIL || area == NIHIL)
    {
        redde NIHIL;
    }
    d = piscina_allocare(piscina, magnitudo(OfficinaDorsum));
    si (d == NIHIL)
    {
        redde NIHIL;
    }
    numerus = conexio_numerus_functionum(conexio);
    memset(d, ZEPHYRUM, magnitudo(OfficinaDorsum));
    d->piscina  = piscina;
    d->conexio  = conexio;
    d->area     = area;
    d->cursor   = (memoriae_index)OFFICINA_DORSUM_INITIUM;
    d->numerus  = numerus;
    d->functiones = piscina_allocare(piscina, (memoriae_index)
        (numerus + I) * magnitudo(DorsumFunctio));
    si (d->functiones == NIHIL)
    {
        redde NIHIL;
    }
    memset(d->functiones, ZEPHYRUM, (memoriae_index)(numerus + I)
        * magnitudo(DorsumFunctio));
    /* symbola functionum interpretatarum -> index conexionis */
    d->numerus_symbolorum = conexio_numerus_symbolorum(conexio);
    d->functio_symboli = piscina_allocare(piscina, (memoriae_index)
        (d->numerus_symbolorum + I) * magnitudo(s32));
    si (d->functio_symboli == NIHIL)
    {
        redde NIHIL;
    }
    per (g = ZEPHYRUM; g < d->numerus_symbolorum; g++)
    {
        constans ConexioSymbolum* symbolum = conexio_symbolum_obtinere(
            conexio, (s32)g);
        constans ConexioDescriptor* descriptor = symbolum->sedes;

        d->functio_symboli[g] = -I;
        si (   symbolum->genus    == CONEXIO_SYMBOLUM_FUNCTIO
            && descriptor         != NIHIL
            && descriptor->signum == CONEXIO_SIGNUM_INTERPRETATUM
            && descriptor->index  >= ZEPHYRUM
            && descriptor->index < (s64)numerus)
        {
            d->functio_symboli[g] = (s32)descriptor->index;
        }
    }
    redde d;
}

b32
officina_dorsum_compilare (
    OfficinaDorsum* d,
               s32  functio_index,
            chorda* causa_out)
{
    si (causa_out != NIHIL)
    {
        causa_out->datum    = NIHIL;
        causa_out->mensura  = ZEPHYRUM;
    }
    si (   d                  == NIHIL || functio_index < ZEPHYRUM
        || (i32)functio_index >= d->numerus)
    {
        redde FALSUM;
    }
    si (d->functiones[functio_index].introitus != NIHIL)
    {
        redde VERUM;
    }
    redde _functionem_compilare(d, functio_index, causa_out);
}

vacuum*
officina_dorsum_introitus (
    constans OfficinaDorsum* d,
                        s32  functio_index)
{
    si (   d                  == NIHIL || functio_index < ZEPHYRUM
        || (i32)functio_index >= d->numerus)
    {
        redde NIHIL;
    }
    redde d->functiones[functio_index].introitus;
}

b32
officina_dorsum_positio (
    constans OfficinaDorsum* d,
             memoriae_index  pc,
                        s32* functio_index_out,
                        i32* instructio_out)
{
    memoriae_index basis;
    memoriae_index offset;
               i32 f;

    si (d == NIHIL)
    {
        redde FALSUM;
    }
    basis = (memoriae_index)memoria_exsecutabilis_basis(d->area);
    si (pc < basis)
    {
        redde FALSUM;
    }
    offset = pc - basis;
    per (f = ZEPHYRUM; f < d->numerus; f++)
    {
        constans DorsumFunctio* df = &d->functiones[f];
                           i32  imum;
                           i32  summum;
                           i32  intra;

        si (   df->introitus == NIHIL || offset < df->initium
            || offset >= df->finis || df->mappa_numerus == ZEPHYRUM)
        {
            perge;
        }
        intra = (i32)(offset - df->initium);
        si (intra < df->mappa_offset[ZEPHYRUM])
        {
            redde FALSUM;   /* prologus: nulla instructio medullae */
        }
        /* maximum offset <= intra (mappa crescens) */
        imum    = ZEPHYRUM;
        summum  = df->mappa_numerus - I;
        dum (imum < summum)
        {
            i32 medium = imum + (summum - imum + I) / II;

            si (df->mappa_offset[medium] <= intra)
            {
                imum = medium;
            }
            alioquin
            {
                summum = medium - I;
            }
        }
        si (functio_index_out != NIHIL)
        {
            *functio_index_out = (s32)f;
        }
        si (instructio_out != NIHIL)
        {
            *instructio_out = df->mappa_index[imum];
        }
        redde VERUM;
    }
    redde FALSUM;
}
