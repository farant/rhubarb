/* probatio_codificator_a64.c - probationes codificatoris A64
 *
 * Aurum: probationes/fixa/codificator_a64/aurum.tsv (verba ex
 * clang, tools/codificator_a64_oraculum.sh). Portae clang numquam
 * vocant. bitmask et FMOV exhauriuntur cum decodificatoribus
 * propriis huius plagulae (non ex codificatore sumptis).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "xar.h"
#include "codificator_a64.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_AURI "probationes/fixa/codificator_a64/aurum.tsv"

/* CBZ: imm19 verborum, -2^18 .. 2^18 - 1 */
#define CBZ_VERBA_MAXIMA 0x3FFFF

hic_manens constans character* NOMINA_FORMARUM[] = {
    "ADD_IMM", "SUB_IMM", "SUBS_IMM",
    "ADD_REG", "SUB_REG", "ADDS_REG", "SUBS_REG",
    "AND_REG", "ORR_REG", "EOR_REG", "ORN_REG",
    "AND_IMM", "ORR_IMM", "EOR_IMM",
    "LSLV", "LSRV", "ASRV",
    "UBFM", "SBFM",
    "MADD", "MSUB", "SDIV", "UDIV",
    "MOVZ", "MOVK", "MOVN",
    "CSINC", "CSEL",
    "LDR_IMM", "STR_IMM", "LDRS_IMM", "LDR_REG", "STR_REG",
    "LDP", "STP",
    "LDR_FP_IMM", "STR_FP_IMM", "LDP_FP", "STP_FP",
    "ADR", "ADRP",
    "B", "BL", "B_COND", "CBZ", "CBNZ", "BR", "BLR", "RET",
    "FADD", "FSUB", "FMUL", "FDIV", "FNEG",
    "FCMP", "FCMP_ZERO", "FMOV_REG", "FMOV_IMM", "FMOV_GEN",
    "FCVT", "SCVTF", "UCVTF", "FCVTZS", "FCVTZU",
    "BRK"
};

hic_manens constans character* NOMINA_CONDITIONUM[] = {
    "EQ", "NE", "HS", "LO", "MI", "PL", "VS", "VC",
    "HI", "LS", "GE", "LT", "GT", "LE", "AL"
};

nomen structura {
    CodificatorA64Instructio instructio;
                         b32 refutanda;
                         i32 verbum;
                      chorda textus;
                         i32 versus;
} LineaAuri;


    /* ==================================================
     * Lectio auri
     * ================================================== */

interior s32
_formam_invenire (
    chorda s)
{
    s32 i;

    per (i = ZEPHYRUM; i < (s32)CODIFICATOR_A64_NUMERUS; i++)
    {
        si (chorda_aequalis_literis(s, NOMINA_FORMARUM[i]))
        {
            redde i;
        }
    }
    redde -I;
}

interior s32
_conditionem_invenire (
    chorda s)
{
    s32 i;

    si (chorda_aequalis_literis(s, "-"))
    {
        redde ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i <= (s32)CODIFICATOR_A64_AL; i++)
    {
        si (chorda_aequalis_literis(s, NOMINA_CONDITIONUM[i]))
        {
            redde i;
        }
    }
    redde -I;
}

/* decimalis cum signo, aut 0x hexadecimalis (exemplar crudum) */
interior b32
_numerum_legere (
    chorda  s,
       s64* fructus)
{
    i32 i;
    b32 negativus;
    b32 hexadecimalis;
    i64 valor;
    i32 cifrae;

    i              = ZEPHYRUM;
    negativus      = FALSUM;
    hexadecimalis  = FALSUM;
    valor          = ZEPHYRUM;
    cifrae         = ZEPHYRUM;
    si (s.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (s.datum[0] == '-')
    {
        negativus  = VERUM;
        i          = I;
    }
    si (   i + I < s.mensura && s.datum[i] == '0'
        && s.datum[i + I] == 'x')
    {
        hexadecimalis  = VERUM;
        i              += II;
    }
    per (; i < s.mensura; i++)
    {
         i8 ch = s.datum[i];
        i64 cifra;

        si (ch >= '0' && ch <= '9')
        {
            cifra = (i64)(ch - '0');
        }
        alioquin si (hexadecimalis && ch >= 'a' && ch <= 'f')
        {
            cifra = (i64)(ch - 'a') + (i64)X;
        }
        alioquin
        {
            redde FALSUM;
        }
        valor = valor * (hexadecimalis ? (i64)XVI : (i64)X) + cifra;
        cifrae++;
    }
    si (cifrae == ZEPHYRUM)
    {
        redde FALSUM;
    }
    *fructus = negativus ? -(s64)valor : (s64)valor;
    redde VERUM;
}

/* NIHIL + nuntius si linea mala */
interior Xar*
_aurum_legere (
    Piscina* piscina)
{
    FilumLector* lector;
            Xar* lineae;
         chorda  linea;

    lector = filum_lector_aperire(VIA_AURI, piscina);
    si (lector == NIHIL)
    {
        imprimere("\n  aurum non apertum: %s\n", VIA_AURI);
        redde NIHIL;
    }
    lineae = xar_creare(piscina, (i32)magnitudo(LineaAuri));
    dum (filum_lector_lineam_proximam(lector, &linea))
    {
        chorda_fissio_fructus  campi;
                    LineaAuri* l;
                          s64  n[VIII];
                          s32  forma;
                          s32  conditio;
                          s32  k;
                          s64  verbum;

        si (linea.mensura == ZEPHYRUM || linea.datum[0] == '#')
        {
            perge;
        }
        campi = chorda_fissio(linea, '\t', piscina);
        forma = (campi.numerus == XI)
            ? _formam_invenire(campi.elementa[0]) : -I;
        conditio = (campi.numerus == XI)
            ? _conditionem_invenire(campi.elementa[VIII]) : -I;
        si (forma < ZEPHYRUM || conditio < ZEPHYRUM)
        {
            imprimere("\n  aurum versus %u malus\n",
                filum_lector_numerus_versus(lector));
            filum_lector_claudere(lector);
            redde NIHIL;
        }
        per (k = I; k <= VII; k++)
        {
            si (!_numerum_legere(campi.elementa[k], &n[k]))
            {
                imprimere("\n  aurum versus %u: campus %d\n",
                    filum_lector_numerus_versus(lector), (integer)k);
                filum_lector_claudere(lector);
                redde NIHIL;
            }
        }
        l = (LineaAuri*)xar_addere(lineae);
        memset(l, ZEPHYRUM, magnitudo(LineaAuri));
        l->instructio.forma       = forma;
        l->instructio.latitudo    = (s32)n[I];
        l->instructio.rd          = (s32)n[II];
        l->instructio.rn          = (s32)n[III];
        l->instructio.rm          = (s32)n[IV];
        l->instructio.ra          = (s32)n[V];
        l->instructio.immediatum  = n[VI];
        l->instructio.translatio  = (s32)n[VII];
        l->instructio.conditio    = conditio;
        l->textus                 = campi.elementa[X];
        l->versus                 = filum_lector_numerus_versus(lector);
        si (chorda_aequalis_literis(campi.elementa[IX], "FALSUM"))
        {
            l->refutanda = VERUM;
        }
        alioquin si (_numerum_legere(campi.elementa[IX], &verbum))
        {
            l->verbum = (i32)verbum;
        }
        alioquin
        {
            imprimere("\n  aurum versus %u: verbum\n", l->versus);
            filum_lector_claudere(lector);
            redde NIHIL;
        }
    }
    filum_lector_claudere(lector);
    redde lineae;
}

interior b32
_verbum_in_auro (
    constans Xar* lineae,
             i32  verbum)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(lineae); i++)
    {
        constans LineaAuri* l;

        l = (constans LineaAuri*)xar_obtinere(lineae, i);
        si (!l->refutanda && l->verbum == verbum)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior CodificatorA64Instructio
_instructio (
    s32 forma,
    s32 latitudo,
    s32 rd,
    s32 rn,
    s64 immediatum,
    s32 conditio)
{
    CodificatorA64Instructio instructio;

    memset(&instructio, ZEPHYRUM, magnitudo(instructio));
    instructio.forma       = forma;
    instructio.latitudo    = latitudo;
    instructio.rd          = rd;
    instructio.rn          = rn;
    instructio.immediatum  = immediatum;
    instructio.conditio    = conditio;
    redde instructio;
}


    /* ==================================================
     * PROBARE: aurum (omnis linea, omnis forma)
     * ================================================== */

interior vacuum
_sectio_aurum (
    CredoContextus* c)
{
    Xar* lineae;
    i32  i;
    i32  fracturae;
    i32  codificatae;
    i32  refutatae;
    s32  visae[CODIFICATOR_A64_NUMERUS];
    s32  forma;
    i32  sine_linea;

    lineae = _aurum_legere(c->piscina);
    CREDO_NECESSE_NON_NIHIL (lineae);
    fracturae    = ZEPHYRUM;
    codificatae  = ZEPHYRUM;
    refutatae    = ZEPHYRUM;
    sine_linea   = ZEPHYRUM;
    memset(visae, ZEPHYRUM, magnitudo(visae));
    per (i = ZEPHYRUM; i < xar_numerus(lineae); i++)
    {
        constans LineaAuri* l;
                       i32  verbum;
                       b32  bonum;

        l       = (constans LineaAuri*)xar_obtinere(lineae, i);
        verbum  = ZEPHYRUM;
        bonum   = codificator_a64_codificare(&l->instructio, &verbum);
        si (l->refutanda && !bonum)
        {
            refutatae++;
        }
        alioquin si (!l->refutanda && bonum && verbum == l->verbum)
        {
            codificatae++;
            visae[l->instructio.forma]++;
        }
        alioquin
        {
            fracturae++;
            si (fracturae <= XX)
            {
                imprimere("\n  versus %u '%.*s': exspectatum %s"
                    " %08x, datum %s %08x", l->versus,
                    (integer)l->textus.mensura,
                    (constans character*)l->textus.datum,
                    l->refutanda ? "FALSUM" : "verbum", l->verbum,
                    bonum ? "verbum" : "FALSUM", verbum);
            }
        }
    }
    per (forma = ZEPHYRUM; forma < (s32)CODIFICATOR_A64_NUMERUS;
         forma++)
    {
        si (visae[forma] == ZEPHYRUM)
        {
            sine_linea++;
            imprimere("\n  forma %s sine linea aurea",
                NOMINA_FORMARUM[forma]);
        }
    }
    CREDO_AEQUALIS_I32 (fracturae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (sine_linea, ZEPHYRUM);
    CREDO_VERUM (refutatae > ZEPHYRUM);
    CREDO_VERUM (codificatae > (i32)(5334 + CCLVI));
}


    /* ==================================================
     * PROBARE: bitmask omnes (5334 LXIV, 1302 XXXII)
     * ================================================== */

interior i64
_larva (
    i32 amplitudo)
{
    redde (amplitudo >= LXIV) ? ~(i64)ZEPHYRUM
        : (((i64)I << amplitudo) - (i64)I);
}

/* DecodeBitMasks (forma canonica: immr < amplitudo elementi) */
interior b32
_bitmask_decodere (
    i32  n,
    i32  immr,
    i32  imms,
    i64* exemplar,
    i32* amplitudo_out)
{
    i32 clavis;
    s32 longitudo;
    s32 k;
    i32 amplitudo;
    i32 gradus;
    i32 unae;
    i64 elementum;

    clavis     = (n << VI) | (~imms & 0x3F);
    longitudo  = -I;
    per (k = VI; k >= ZEPHYRUM; k--)
    {
        si ((clavis & ((i32)I << k)) != ZEPHYRUM)
        {
            longitudo = k;
            frange;
        }
    }
    si (longitudo < I)
    {
        redde FALSUM;
    }
    amplitudo  = (i32)I << longitudo;
    gradus     = amplitudo - I;
    unae       = (imms & gradus) + I;
    si (unae == amplitudo || immr >= amplitudo)
    {
        redde FALSUM;
    }
    elementum = _larva(unae);
    si (immr > ZEPHYRUM)
    {
        elementum = ((elementum >> immr)
            | (elementum << (amplitudo - immr))) & _larva(amplitudo);
    }
    per (k = (s32)amplitudo; k < LXIV; k *= II)
    {
        elementum |= elementum << k;
    }
    *exemplar       = elementum;
    *amplitudo_out  = amplitudo;
    redde VERUM;
}

interior vacuum
_sectio_bitmask (
    CredoContextus* c)
{
    i32 n;
    i32 immr;
    i32 imms;
    i32 validae;
    i32 validae_xxxii;
    i32 fracturae;
    i32 triplex;
    i32 t;

    (vacuum)c;
    validae        = ZEPHYRUM;
    validae_xxxii  = ZEPHYRUM;
    fracturae      = ZEPHYRUM;
    /* omnes triplices (N, immr, imms): 2 * 64 * 64 */
    per (t = ZEPHYRUM; t < 0x2000; t++)
    {
        i64 exemplar;
        i32 amplitudo;
        i32 exspectatum;

        n     = t >> XII;
        immr  = (t >> VI) & 0x3F;
        imms  = t & 0x3F;
        si (!_bitmask_decodere(n, immr, imms, &exemplar, &amplitudo))
        {
            perge;
        }
        validae++;
        exspectatum  = (n << XII) | (immr << VI) | imms;
        triplex      = ZEPHYRUM;
        si (   !codificator_a64_bitmask(exemplar, LXIV, &triplex)
            || triplex != exspectatum)
        {
            fracturae++;
            si (fracturae <= X)
            {
                imprimere("\n  bitmask %016lx: exspectatum %04x,"
                    " datum %04x", (longus insignatus)exemplar,
                    exspectatum, triplex);
            }
        }
        si (amplitudo <= XXXII)
        {
            validae_xxxii++;
            triplex = ZEPHYRUM;
            si (   !codificator_a64_bitmask(exemplar & 0xFFFFFFFFUL,
                    XXXII, &triplex) || triplex != exspectatum)
            {
                fracturae++;
            }
        }
    }
    CREDO_AEQUALIS_I32 (validae, 5334);
    CREDO_AEQUALIS_I32 (validae_xxxii, 1302);
    CREDO_AEQUALIS_I32 (fracturae, ZEPHYRUM);

    CREDO_FALSUM (codificator_a64_bitmask(ZEPHYRUM, LXIV, &triplex));
    CREDO_FALSUM (codificator_a64_bitmask(~(i64)ZEPHYRUM, LXIV,
        &triplex));
    CREDO_FALSUM (codificator_a64_bitmask(0x5, LXIV, &triplex));
    CREDO_FALSUM (codificator_a64_bitmask(0x1234, LXIV, &triplex));
    CREDO_FALSUM (codificator_a64_bitmask(0xFFFFFFFFUL, XXXII,
        &triplex));
    CREDO_FALSUM (codificator_a64_bitmask(0x100000000UL, XXXII,
        &triplex));
    CREDO_FALSUM (codificator_a64_bitmask(0xFF, XVI, &triplex));
}


    /* ==================================================
     * PROBARE: fmov omnes (CCLVI)
     * ================================================== */

/* VFPExpandImm */
interior f64
_fmov_expandere (
    i32 imm8)
{
    i32 b;
    i32 cd;
    s32 exponens;
    f64 valor;

    b         = (imm8 >> VI) & I;
    cd        = (imm8 >> IV) & III;
    exponens  = (b != ZEPHYRUM) ? (s32)cd - III : (s32)cd + I;
    valor     = (f64)(XVI + (imm8 & 0xF)) / 16.0;
    dum (exponens > ZEPHYRUM)
    {
        valor *= 2.0;
        exponens--;
    }
    dum (exponens < ZEPHYRUM)
    {
        valor /= 2.0;
        exponens++;
    }
    redde ((imm8 & 0x80) != ZEPHYRUM) ? -valor : valor;
}

interior vacuum
_sectio_fmov (
    CredoContextus* c)
{
    i32 i;
    i32 imm8;
    i32 fracturae;

    (vacuum)c;
    fracturae = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= CCLV; i++)
    {
        imm8 = CCLVI;
        si (!codificator_a64_fmov_immediatum(_fmov_expandere(i), &imm8)
            || imm8 != i)
        {
            fracturae++;
            si (fracturae <= X)
            {
                imprimere("\n  fmov %.17g: exspectatum %u, datum %u",
                    _fmov_expandere(i), i, imm8);
            }
        }
    }
    CREDO_AEQUALIS_I32 (fracturae, ZEPHYRUM);
    CREDO_FALSUM (codificator_a64_fmov_immediatum(0.0, &imm8));
    CREDO_FALSUM (codificator_a64_fmov_immediatum(-0.0, &imm8));
    CREDO_FALSUM (codificator_a64_fmov_immediatum(0.1, &imm8));
    CREDO_FALSUM (codificator_a64_fmov_immediatum(32.0, &imm8));
    CREDO_FALSUM (codificator_a64_fmov_immediatum(0.0625, &imm8));
    CREDO_FALSUM (codificator_a64_fmov_immediatum(1.03125, &imm8));
}


    /* ==================================================
     * PROBARE: constans (MOVZ/MOVN/MOVK)
     * ================================================== */

nomen structura {
    i64 valor;
    s32 latitudo;
    s32 numerus;
} CasusConstantis;

/* exsequitur verba move-wide manu; FALSUM si verbum alienum */
interior b32
_constantem_simulare (
    constans i32* verba,
             s32  numerus,
             s32  latitudo,
             s32  rd,
             i64* fructus)
{
    s32 i;
    i64 r;

    r = ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        i32 w;
        i32 opc;
        i32 translatio;
        i64 imm16;

        w           = verba[i];
        opc         = (w >> XXIX) & III;
        translatio  = ((w >> XXI) & III) * XVI;
        imm16       = (i64)((w >> V) & 0xFFFF);
        si (   (w & 0x1F800000)          != 0x12800000
            || (s32)(w & 0x1F)           != rd
            || ((w >> XXXI) != ZEPHYRUM) != (latitudo == LXIV))
        {
            redde FALSUM;
        }
        si (opc == ZEPHYRUM && i == ZEPHYRUM)
        {
            r = ~(imm16 << translatio);
        }
        alioquin si (opc == II && i == ZEPHYRUM)
        {
            r = imm16 << translatio;
        }
        alioquin si (opc == III && i > ZEPHYRUM)
        {
            r = (r & ~((i64)0xFFFF << translatio))
                | (imm16 << translatio);
        }
        alioquin
        {
            redde FALSUM;
        }
    }
    *fructus = (latitudo == XXXII) ? (r & 0xFFFFFFFFUL) : r;
    redde VERUM;
}

interior vacuum
_sectio_constans (
    CredoContextus* c)
{
    hic_manens constans CasusConstantis CASUS[] = {
        { 0x0UL,                LXIV,  I   },
        { 0xFFFFUL,             LXIV,  I   },
        { 0x10000UL,            LXIV,  I   },
        { ~0x0UL,               LXIV,  I   },
        { 0x300000000000UL,     LXIV,  I   },
        { 0x123456789ABCDEF0UL, LXIV,  IV  },
        { 0xFFFF1234UL,         LXIV,  II  },
        { 0xFFFFFFFFFFFF1234UL, LXIV,  I   },
        { 0xFFFFFFFFUL,         XXXII, I   },
        { 0x12345678UL,         XXXII, II  }
    };
    Xar* lineae;
    i32  i;
    i32  verba[IV];
    s32  numerus;
    i32  numerus_casuum;

    lineae = _aurum_legere(c->piscina);
    CREDO_NECESSE_NON_NIHIL (lineae);
    numerus_casuum = (i32)(magnitudo(CASUS) / magnitudo(CASUS[0]));
    per (i = ZEPHYRUM; i < numerus_casuum; i++)
    {
        i64 fructus;
        s32 k;

        fructus = ZEPHYRUM;
        numerus = codificator_a64_constans(CASUS[i].valor,
            CASUS[i].latitudo, III, verba);
        CREDO_AEQUALIS_S32 (numerus, CASUS[i].numerus);
        si (numerus < I || numerus > IV)
        {
            perge;
        }
        CREDO_VERUM (_constantem_simulare(verba, numerus,
            CASUS[i].latitudo, III, &fructus));
        CREDO_AEQUALIS_I64 (fructus, CASUS[i].valor);
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            si (!_verbum_in_auro(lineae, verba[k]))
            {
                imprimere("\n  constans %016lx: verbum %08x non in"
                    " auro", (longus insignatus)CASUS[i].valor,
                    verba[k]);
                CREDO_CULPA ("verbum constantis non in auro");
            }
        }
    }
    CREDO_AEQUALIS_S32 (codificator_a64_constans(0x100000000UL,
        XXXII, III, verba), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (codificator_a64_constans(I, XVI, III, verba),
        ZEPHYRUM);
    CREDO_AEQUALIS_S32 (codificator_a64_constans(I, LXIV, XXXII,
        verba), ZEPHYRUM);
}


    /* ==================================================
     * PROBARE: notae (saltus et fines)
     * ================================================== */

interior vacuum
_impletiones (
    CodificatorA64* codificator,
               i32  numerus)
{
    CodificatorA64Instructio impletio;
                         i32 i;

    impletio = _instructio(CODIFICATOR_A64_ADD_IMM, LXIV,
        ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        codificator_a64_emittere(codificator, &impletio);
    }
}

interior vacuum
_sectio_notae (
    CredoContextus* c)
{
              CodificatorA64* codificator;
    CodificatorA64Instructio  saltus;
                         s32  nota_a;
                         s32  nota_b;
                      chorda  causa;

    /* b.ne ante, b retro */
    codificator = codificator_a64_creare(c->piscina);
    CREDO_NECESSE_NON_NIHIL (codificator);
    nota_a = codificator_a64_notam_creare(codificator);
    nota_b = codificator_a64_notam_creare(codificator);
    codificator_a64_notam_ponere(codificator, nota_b);
    saltus = _instructio(CODIFICATOR_A64_B_COND, LXIV, ZEPHYRUM,
        ZEPHYRUM, ZEPHYRUM, CODIFICATOR_A64_NE);
    CREDO_VERUM (codificator_a64_saltum_emittere(codificator, &saltus,
        nota_a));
    _impletiones(codificator, II);
    codificator_a64_notam_ponere(codificator, nota_a);
    saltus = _instructio(CODIFICATOR_A64_B, LXIV, ZEPHYRUM, ZEPHYRUM,
        ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM (codificator_a64_saltum_emittere(codificator, &saltus,
        nota_b));
    causa.mensura = ZEPHYRUM;
    CREDO_VERUM (codificator_a64_finire(codificator, &causa));
    CREDO_AEQUALIS_I32 (codificator_a64_numerus(codificator), IV);
    CREDO_AEQUALIS_I32 (codificator_a64_verba(codificator)[0],
        0x54000061);                                   /* b.ne #12 */
    CREDO_AEQUALIS_I32 (codificator_a64_verba(codificator)[III],
        0x17FFFFFD);                                   /* b #-12 */

    /* cbz ad finem ante: CBZ_VERBA_MAXIMA verba */
    codificator = codificator_a64_creare(c->piscina);
    CREDO_NECESSE_NON_NIHIL (codificator);
    nota_a = codificator_a64_notam_creare(codificator);
    saltus = _instructio(CODIFICATOR_A64_CBZ, LXIV, ZEPHYRUM,
        ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
    CREDO_VERUM (codificator_a64_saltum_emittere(codificator, &saltus,
        nota_a));
    _impletiones(codificator, CBZ_VERBA_MAXIMA - I);
    codificator_a64_notam_ponere(codificator, nota_a);
    CREDO_VERUM (codificator_a64_finire(codificator, &causa));
    CREDO_AEQUALIS_I32 (codificator_a64_verba(codificator)[0],
        0xB47FFFE0);                           /* cbz x0, #1048572 */

    /* verbum unum ultra: FALSUM cum causa */
    codificator = codificator_a64_creare(c->piscina);
    CREDO_NECESSE_NON_NIHIL (codificator);
    nota_a = codificator_a64_notam_creare(codificator);
    CREDO_VERUM (codificator_a64_saltum_emittere(codificator, &saltus,
        nota_a));
    _impletiones(codificator, CBZ_VERBA_MAXIMA);
    codificator_a64_notam_ponere(codificator, nota_a);
    causa.mensura = ZEPHYRUM;
    CREDO_FALSUM (codificator_a64_finire(codificator, &causa));
    CREDO_VERUM (causa.mensura > ZEPHYRUM);

    /* cbz ad finem retro */
    codificator = codificator_a64_creare(c->piscina);
    CREDO_NECESSE_NON_NIHIL (codificator);
    nota_a = codificator_a64_notam_creare(codificator);
    codificator_a64_notam_ponere(codificator, nota_a);
    _impletiones(codificator, CBZ_VERBA_MAXIMA + I);
    CREDO_VERUM (codificator_a64_saltum_emittere(codificator, &saltus,
        nota_a));
    CREDO_VERUM (codificator_a64_finire(codificator, &causa));
    CREDO_AEQUALIS_I32 (codificator_a64_verba(codificator)
        [CBZ_VERBA_MAXIMA + I], 0xB4800000); /* cbz x0, #-1048576 */

    /* nota numquam posita; instructio invalida */
    codificator = codificator_a64_creare(c->piscina);
    CREDO_NECESSE_NON_NIHIL (codificator);
    nota_a = codificator_a64_notam_creare(codificator);
    CREDO_VERUM (codificator_a64_saltum_emittere(codificator, &saltus,
        nota_a));
    causa.mensura = ZEPHYRUM;
    CREDO_FALSUM (codificator_a64_finire(codificator, &causa));
    CREDO_VERUM (causa.mensura > ZEPHYRUM);
    saltus = _instructio(CODIFICATOR_A64_ADD_IMM, LXIV, ZEPHYRUM,
        ZEPHYRUM, 4096, ZEPHYRUM);
    codificator = codificator_a64_creare(c->piscina);
    CREDO_FALSUM (codificator_a64_emittere(codificator, &saltus));
    CREDO_AEQUALIS_I32 (codificator_a64_numerus(codificator),
        ZEPHYRUM);
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "aurum",         _sectio_aurum,    NIHIL, NIHIL, NIHIL, NIHIL },
    { "bitmask omnes", _sectio_bitmask,  NIHIL, NIHIL, NIHIL, NIHIL },
    { "fmov omnes",    _sectio_fmov,     NIHIL, NIHIL, NIHIL, NIHIL },
    { "constans",      _sectio_constans, NIHIL, NIHIL, NIHIL, NIHIL },
    { "notae",         _sectio_notae,    NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("codificator_a64", SECTIONES);
}
