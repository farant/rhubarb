/* probatio_syllabae_latinae.c - divisio syllabarum et accentus
 * (psalmodia-plan-2 T2)
 *
 * Casus manu scripti: forma libri AR 1912 (regulae C capitis, ex
 * hyphenis OCR mensuratae; lib/syllabae_latinae.worklog.md). Specimen
 * GregoBase (LXXXII cantus, probationes/fixa/gabc/gregobase/):
 * quodque verbum NUMERUM syllabarum et syllabam accentus habet ut
 * fons, non formam (fontes stilos varios miscent). Discordiae notae
 * in DISCORDIAE, quaeque decreto in worklog.
 */

#include "latina.h"
#include "credo.h"
#include "syllabae_latinae.h"
#include "gabc.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "piscina.h"
#include "sors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_MANIFESTI  "probationes/fixa/gabc/gregobase.manifestum"
#define VIA_FIXORUM    "probationes/fixa/gabc/gregobase/"


/* ==================================================
 * Auxilia
 * ================================================== */

/* invariantia: syllabae ordine, intra verbum, accentus in ambitu */
interior b32
_valida (
                    chorda  verbum,
    constans VerbumLatinum* v)
{
    i32 i;
    i32 prior = ZEPHYRUM;

    si (v->numerus == ZEPHYRUM)
    {
        redde v->accentus == -I;
    }
    si (v->syllabae == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < v->numerus; i++)
    {
        si (   v->syllabae[i].initium < prior
            || v->syllabae[i].initium >= v->syllabae[i].finis
            || v->syllabae[i].finis > verbum.mensura)
        {
            redde FALSUM;
        }
        prior = v->syllabae[i].finis;
    }
    redde v->accentus >= -I && v->accentus < (s32)v->numerus;
}

/* "Dóminus" -> "Dó-mi-nus" (syllabae hyphenis iunctae) */
interior chorda
_forma (
         CredoContextus* ctx,
     constans character* literae)
{
                chorda verbum = chorda_ex_literis(literae,
                    ctx->piscina);
         VerbumLatinum  v;
     ChordaAedificator* a;
                   i32  i;

    si (!syllabae_latinae_dividere(verbum, ctx->piscina, &v))
    {
        redde chorda_ex_literis("(memoria)", ctx->piscina);
    }
    si (!_valida(verbum, &v))
    {
        redde chorda_ex_literis("(invalidum)", ctx->piscina);
    }
    a = chorda_aedificator_creare(ctx->piscina, CXXVIII);
    per (i = ZEPHYRUM; i < v.numerus; i++)
    {
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, '-');
        }
        chorda_aedificator_appendere_chorda(a, chorda_sectio(verbum,
            v.syllabae[i].initium, v.syllabae[i].finis));
    }
    redde chorda_aedificator_finire(a);
}

interior VerbumLatinum
_verbum (
         CredoContextus* ctx,
     constans character* literae)
{
    VerbumLatinum v;

    memset(&v, ZEPHYRUM, magnitudo(v));
    (vacuum)syllabae_latinae_dividere(chorda_ex_literis(literae,
        ctx->piscina), ctx->piscina, &v);
    redde v;
}


/* ==================================================
 * Divisio: casus manu
 * ================================================== */

nomen structura {
    constans character* verbum;
    constans character* forma;
} Casus;

hic_manens constans Casus CASUS[] = {
    /* planum T2 */
    { "Dóminus", "Dó-mi-nus" },
    { "exsultáte", "ex-sul-tá-te" },
    { "exaudívit", "ex-au-dí-vit" },
    { "justítia", "ju-stí-ti-a" },
    { "lingua", "lin-gua" },
    { "sanguis", "san-guis" },
    { "cui", "cu-i" },
    { "Deus", "De-us" },
    { "mihi", "mi-hi" },
    { "ejus", "e-jus" },
    { "Christus", "Chri-stus" },
    { "patris", "pa-tris" },
    { "ecclésia", "ec-clé-si-a" },
    { "sǽcula", "sǽ-cu-la" },
    { "Israël", "Is-ra-ël" },
    { "Is-ra-el", "Is-ra-el" },
    { "qui", "qui" },
    { "et", "et" },
    { "Dómino,", "Dó-mi-no," },
    /* regulae V */
    { "Israel", "Is-rael" },
    { "huic", "hu-ic" },
    { "eius", "e-ius" },
    { "allelúia", "al-le-lú-ia" },
    { "quia", "qui-a" },
    { "suávis", "su-á-vis" },
    { "Iesus", "Ie-sus" },
    { "iam", "iam" },
    { "iisdem", "i-is-dem" },
    { "heu", "heu" },
    { "Eucharístia", "Eu-cha-rí-sti-a" },
    { "euntes", "e-un-tes" },
    { "eum", "e-um" },
    { "coelum", "coe-lum" },
    { "poëta", "po-ë-ta" },
    { "quae", "quae" },
    { "aquae", "a-quae" },
    { "laudáte", "lau-dá-te" },
    { "fílii", "fí-li-i" },
    /* regulae C (AR 1912) */
    { "Sanctus", "San-ctus" },
    { "proptérea", "pro-pté-re-a" },
    { "surrexísti", "sur-re-xí-sti" },
    { "dixit", "di-xit" },
    { "omnes", "o-mnes" },
    { "magna", "ma-gna" },
    { "ignis", "i-gnis" },
    { "assumpsit", "as-sum-psit" },
    { "usque", "us-que" },
    { "exspectáre", "ex-spe-ctá-re" },
    { "discípulus", "di-scí-pu-lus" },
    { "terra", "ter-ra" },
    { "psalmus", "psal-mus" },
    { "spíritus", "spí-ri-tus" },
    { "Abraham", "A-bra-ham" },
    /* praefixa et verba integra */
    { "redémptor", "red-ém-ptor" },
    { "inimícus", "in-i-mí-cus" },
    { "iníquitas", "in-í-qui-tas" },
    { "exércitus", "ex-ér-ci-tus" },
    { "adoráre", "ad-o-rá-re" },
    { "perénnis", "per-én-nis" },
    { "abiit", "ab-i-it" },
    { "obumbrábit", "ob-um-brá-bit" },
    { "transiit", "trans-i-it" },
    { "súscipe", "sús-ci-pe" },
    { "sustíneo", "sus-tí-ne-o" },
    { "sicut", "sic-ut" },
    { "sicuti", "sic-u-ti" },
    { "eténim", "et-é-nim" },
    { "nobíscum", "no-bís-cum" },
    { "superexaltáte", "su-per-ex-al-tá-te" },
    /* non composita */
    { "Adam", "A-dam" },
    { "ádipe", "á-di-pe" },
    { "abyssus", "a-bys-sus" },
    { "redúxit", "re-dú-xit" },
    { "étiam", "é-ti-am" },
    /* maiusculae, U+0301, interpunctio */
    { "DÓMINUS", "DÓ-MI-NUS" },
    { "Do\xcc\x81minus", "Do\xcc\x81-mi-nus" },
    { "(Dómine)", "(Dó-mi-ne)" },
    { "«Ecce»:", "«Ec-ce»:" },
    { NIHIL, NIHIL }
};

interior vacuum
_divisio_probare (
    CredoContextus* ctx)
{
    VerbumLatinum v;
              i32 i;

    per (i = ZEPHYRUM; CASUS[i].verbum != NIHIL; i++)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS (_forma(ctx, CASUS[i].verbum),
            CASUS[i].forma);
    }

    /* '-' cogens in nulla syllaba */
    v = _verbum(ctx, "Is-ra-el");
    CREDO_NECESSE (v.numerus == III);
    CREDO_AEQUALIS_I32 (v.syllabae[ZEPHYRUM].finis, (i32)II);
    CREDO_AEQUALIS_I32 (v.syllabae[I].initium, (i32)III);
    CREDO_AEQUALIS_I32 (v.syllabae[II].initium, (i32)VI);
    CREDO_AEQUALIS_I32 (v.syllabae[II].finis, (i32)VIII);

    /* syllabae verbum tegunt: interpunctio intra spatia */
    v = _verbum(ctx, "Dómino,");
    CREDO_NECESSE (v.numerus == III);
    CREDO_AEQUALIS_I32 (v.syllabae[ZEPHYRUM].initium, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (v.syllabae[II].finis, (i32)VIII);
}


/* ==================================================
 * Accentus
 * ================================================== */

interior vacuum
_accentus_probare (
    CredoContextus* ctx)
{
    VerbumLatinum v;

    v = _verbum(ctx, "Dóminus");
    CREDO_AEQUALIS_S32 (v.accentus, ZEPHYRUM);
    CREDO_VERUM (v.notatus);
    v = _verbum(ctx, "Deus");
    CREDO_AEQUALIS_S32 (v.accentus, ZEPHYRUM);
    CREDO_FALSUM (v.notatus);
    v = _verbum(ctx, "et");
    CREDO_AEQUALIS_S32 (v.accentus, -I);
    CREDO_FALSUM (v.notatus);
    v = _verbum(ctx, "Dominus");
    CREDO_AEQUALIS_S32 (v.accentus, -I);
    CREDO_FALSUM (v.notatus);
    v = _verbum(ctx, "Ómnes");
    CREDO_AEQUALIS_S32 (v.accentus, ZEPHYRUM);
    CREDO_VERUM (v.notatus);
    v = _verbum(ctx, "sǽcula");
    CREDO_AEQUALIS_S32 (v.accentus, ZEPHYRUM);
    CREDO_VERUM (v.notatus);
    v = _verbum(ctx, "Dominúsque");
    CREDO_AEQUALIS_S32 (v.accentus, (s32)II);
    CREDO_VERUM (v.notatus);
    v = _verbum(ctx, "Do\xcc\x81minus");
    CREDO_AEQUALIS_S32 (v.accentus, ZEPHYRUM);
    CREDO_VERUM (v.notatus);
    /* acuti plures -> ultimus; diaeresis accentus non est */
    v = _verbum(ctx, "Dóminúsque");
    CREDO_AEQUALIS_S32 (v.accentus, (s32)II);
    v = _verbum(ctx, "Israël");
    CREDO_AEQUALIS_I32 (v.numerus, (i32)III);
    CREDO_AEQUALIS_S32 (v.accentus, -I);
    CREDO_FALSUM (v.notatus);
    /* accentus in diphthongo (acutus in littera secunda) */
    v = _verbum(ctx, "coélicus");
    CREDO_AEQUALIS_I32 (v.numerus, (i32)III);
    CREDO_AEQUALIS_S32 (v.accentus, ZEPHYRUM);
}


/* ==================================================
 * Vocalis
 * ================================================== */

interior vacuum
_vocalis_probare (
    CredoContextus* ctx)
{
    chorda d = chorda_ex_literis("Dóminus æ ǽ o\xcc\x81\xcc\x88 Y",
                   ctx->piscina);
       i32 l = ZEPHYRUM;

    CREDO_FALSUM (syllabae_latinae_vocalis(d, ZEPHYRUM, &l));
    CREDO_VERUM (syllabae_latinae_vocalis(d, I, &l));
    CREDO_AEQUALIS_I32 (l, (i32)II);
    CREDO_FALSUM (syllabae_latinae_vocalis(d, II, &l));
    CREDO_VERUM (syllabae_latinae_vocalis(d, VI, &l));
    CREDO_AEQUALIS_I32 (l, I);
    CREDO_VERUM (syllabae_latinae_vocalis(d, IX, &l));
    CREDO_AEQUALIS_I32 (l, (i32)II);
    CREDO_VERUM (syllabae_latinae_vocalis(d, XII, &l));
    CREDO_AEQUALIS_I32 (l, (i32)II);
    CREDO_VERUM (syllabae_latinae_vocalis(d, XV, &l));
    CREDO_AEQUALIS_I32 (l, (i32)V);
    CREDO_VERUM (syllabae_latinae_vocalis(d, XXI, NIHIL));
    CREDO_FALSUM (syllabae_latinae_vocalis(d, XX, &l));
    CREDO_FALSUM (syllabae_latinae_vocalis(d, d.mensura, &l));
    CREDO_FALSUM (syllabae_latinae_vocalis(d, (i32)M, &l));
}


/* ==================================================
 * Specimen GregoBase: numerus et accentus
 * ================================================== */

/* discordiae notae (verbum ut in fonte, fons, nos); quaeque cum
 * decreto in worklog */
nomen structura {
    constans character* verbum;
                   i32  fons;
                   i32  nos;
} Discordia;

hic_manens constans Discordia DISCORDIAE[] = {
    { "Israel", III, II },   /* hebraicum: aptator (VOCES_HEBRAICAE) */
    { "cui", I, II },        /* metrum hymni (00537) */
    { "Patre", I, II },      /* elisio hymni, Patr<i>e</i> (03324) */
    { NIHIL, ZEPHYRUM, ZEPHYRUM }
};

/* acutus in octetis (praecompositus aut U+0301) */
interior b32
_acutum_habet (
    chorda s)
{
    i32 i;

    per (i = ZEPHYRUM; i + I < s.mensura; i++)
    {
        insignatus character p = (insignatus character)s.datum[i];
        insignatus character q = (insignatus character)s.datum[i + I];

        si (   (p == 0xC3 && (q == 0xA1 || q == 0xA9 || q == 0xAD
                    || q == 0xB3 || q == 0xBA || q == 0xBD || q == 0x81
                    || q == 0x89 || q == 0x8D || q == 0x93 || q == 0x9A
                    || q == 0x9D))
            || (p == 0xC7 && (q == 0xBD || q == 0xBC))
            || (p == 0xCC && q == 0x81))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_nota (
    chorda verbum,
       i32 fons,
       i32 nos)
{
    i32 i;

    per (i = ZEPHYRUM; DISCORDIAE[i].verbum != NIHIL; i++)
    {
        si (   chorda_aequalis_literis(verbum, DISCORDIAE[i].verbum)
            && DISCORDIAE[i].fons == fons && DISCORDIAE[i].nos == nos)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* textus cantatus syllabae iunctus; FALSUM si nullus */
interior b32
_textus_cantatus (
    constans GabcSyllaba* sy)
{
    i32 k;

    per (k = ZEPHYRUM; k < sy->numerus_textuum; k++)
    {
        si (   sy->textus[k].genus == GABC_TEXTUS_CANTATUS
            && sy->textus[k].textus.mensura > ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* verbum fontis ex syllabis indices[0..n); FALSUM si probari non
 * potest: syllaba sine vocali (Ps., V/., crux), sine notis (titulus
 * repetendus "Glória() laus()"), spatium aut signum intra, elisio */
interior b32
_verbum_fontis (
      constans GabcCantus* g,
             constans i32* indices,
                      i32  n,
                  Piscina* p,
                   chorda* verbum,
                      s32* accentus)
{
     ChordaAedificator* a = chorda_aedificator_creare(p, LXIV);
                   i32  s;
                   i32  k;
                   i32  j;

    *accentus = -I;
    per (s = ZEPHYRUM; s < n; s++)
    {
        constans GabcSyllaba* sy       = &g->syllabae[indices[s]];
                         b32  vocalis  = FALSUM;
                      chorda  t;

        si (sy->numerus_elementorum == ZEPHYRUM)
        {
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < sy->numerus_textuum; k++)
        {
            si (sy->textus[k].genus != GABC_TEXTUS_CANTATUS)
            {
                perge;
            }
            si (sy->textus[k].stilus & GABC_STILUS_ELISIO)
            {
                redde FALSUM;
            }
            t = sy->textus[k].textus;
            per (j = ZEPHYRUM; j < t.mensura; j++)
            {
                insignatus character c =
                    (insignatus character)t.datum[j];

                si (c == ' ' || c == '*' || c == '+' || c == '-')
                {
                    redde FALSUM;
                }
                si (syllabae_latinae_vocalis(t, j, NIHIL))
                {
                    vocalis = VERUM;
                }
            }
            si (_acutum_habet(t))
            {
                *accentus = (s32)s;
            }
            chorda_aedificator_appendere_chorda(a, t);
        }
        si (!vocalis)
        {
            redde FALSUM;
        }
    }
    *verbum = chorda_aedificator_finire(a);
    redde VERUM;
}

nomen structura {
    i32 verba;
    i32 numeri_mali;
    i32 accentus_mali;
    i32 notae;
} Census;

interior vacuum
_verbum_iudicare (
    constans GabcCantus* g,
             constans i32* indices,
                    i32  n,
                Piscina* p,
     constans character* via,
                 Census* census)
{
           chorda verbum;
              s32 acc_fons;
    VerbumLatinum v;

    si (   n == ZEPHYRUM
        || !_verbum_fontis(g, indices, n, p, &verbum, &acc_fons))
    {
        redde;
    }
    CREDO_NECESSE (syllabae_latinae_dividere(verbum, p, &v));
    census->verba++;
    si (v.numerus != n)
    {
        si (_nota(verbum, n, v.numerus))
        {
            census->notae++;
            redde;
        }
        census->numeri_mali++;
        imprimere("  numerus: %.*s fons %d nos %d (%s)\n",
            (integer)verbum.mensura, (constans character*)verbum.datum,
            (integer)n, (integer)v.numerus, via);
    }
    alioquin si (acc_fons >= ZEPHYRUM && v.accentus != acc_fons)
    {
        census->accentus_mali++;
        imprimere("  accentus: %.*s fons %d nos %d (%s)\n",
            (integer)verbum.mensura, (constans character*)verbum.datum,
            (integer)acc_fons, (integer)v.accentus, via);
    }
}

#define SYLLABAE_VERBI_MAXIMAE  LXIV

interior vacuum
_exemplar_probare (
    CredoContextus* ctx)
{
    chorda m = filum_legere_totum(VIA_MANIFESTI, ctx->piscina);
    Census census;
       i32 i       = ZEPHYRUM;
       i32 cantus  = ZEPHYRUM;

    memset(&census, ZEPHYRUM, magnitudo(census));
    CREDO_NECESSE (m.mensura > ZEPHYRUM);
    dum (i < m.mensura)
    {
        i32 initium = i;
        i32 k;

        dum (i < m.mensura && m.datum[i] != '\n')
        {
            i++;
        }
        si (   i - initium > IV
            && memcmp(m.datum + initium, "DATA", IV) != ZEPHYRUM)
        {
             character  via[CXXVIII];
                   i32  indices[SYLLABAE_VERBI_MAXIMAE];
                   i32  n = ZEPHYRUM;
               Piscina* p;
            GabcCantus* g;
                   i32  s;

            per (k = initium; k < i && m.datum[k] != '\t'; k++)
            {
            }
            CREDO_NECESSE (k - initium < LXIV);
            strcpy(via, VIA_FIXORUM);
            strncat(via, (constans character*)(m.datum + initium),
                (size_t)(k - initium));
            p = piscina_generare_dynamicum("syllabae_exemplar",
                (memoriae_index)(CXXVIII * MXXIV));
            CREDO_NECESSE (p != NIHIL);
            g = gabc_legere(filum_legere_totum(via, p), p);
            CREDO_NECESSE (g != NIHIL);
            cantus++;
            per (s = ZEPHYRUM; s < g->numerus_syllabarum; s++)
            {
                si (!_textus_cantatus(&g->syllabae[s]))
                {
                    perge;
                }
                si (   g->syllabae[s].verbum_incipit
                    || n == SYLLABAE_VERBI_MAXIMAE)
                {
                    _verbum_iudicare(g, indices, n, p, via,
                        &census);
                    n = ZEPHYRUM;
                }
                indices[n++] = s;
            }
            _verbum_iudicare(g, indices, n, p, via, &census);
            piscina_destruere(p);
        }
        i++;
    }
    imprimere("  specimen: cantus %d, verba %d, discordiae notae %d\n",
        (integer)cantus, (integer)census.verba, (integer)census.notae);
    CREDO_AEQUALIS_I32 (cantus, LXXXII);
    CREDO_MAIOR_I32 (census.verba, (i32)M);
    CREDO_AEQUALIS_I32 (census.numeri_mali, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census.accentus_mali, ZEPHYRUM);
}


/* ==================================================
 * Totalitas
 * ================================================== */

/* in filio sub CREDO_NON_RUIT */
interior vacuum
_omnia_dividere (
    constans i8* octeti,
            i32  mensura)
{
          Piscina* p = piscina_generare_dynamicum("syllabae_totalitas",
                           (memoriae_index)(LXIV * MXXIV));
           chorda verbum;
    VerbumLatinum v;
              i32 i;

    verbum.datum = (i8*)malloc((size_t)(mensura > ZEPHYRUM
        ? mensura : I));
    si (p == NIHIL || verbum.datum == NIHIL)
    {
        redde;
    }
    si (mensura > ZEPHYRUM)
    {
        memcpy(verbum.datum, octeti, (size_t)mensura);
    }
    verbum.mensura = mensura;
    (vacuum)syllabae_latinae_dividere(verbum, p, &v);
    per (i = ZEPHYRUM; i <= mensura; i++)
    {
        (vacuum)syllabae_latinae_vocalis(verbum, i, NIHIL);
    }
    free(verbum.datum);
    piscina_destruere(p);
}

interior vacuum
_totalitas_probare (
    CredoContextus* ctx)
{
    constans character* fracta[] = {
        "", "--", "-", "1.", "Ps", "\xff\xfe", "a\xcc", "\xcc\x81",
        "-a-", "a--e", "é-", "\xc3", "o\xcc\x81\xcc\x81\xcc\x81", NIHIL
    };
               i8  temere[MMMCMXCIX];
             Sors  sors;
          Piscina* p;
           chorda  verbum;
    VerbumLatinum  v;
              i32  k;
              i32  r;

    per (k = ZEPHYRUM; fracta[k] != NIHIL; k++)
    {
        CREDO_NON_RUIT (_omnia_dividere((constans i8*)fracta[k],
            (i32)strlen(fracta[k])));
        verbum = chorda_ex_literis(fracta[k], ctx->piscina);
        CREDO_VERUM (syllabae_latinae_dividere(verbum, ctx->piscina,
            &v));
        CREDO_VERUM (_valida(verbum, &v));
    }
    CREDO_AEQUALIS_I32 (_verbum(ctx, "").numerus, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_verbum(ctx, "--").numerus, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_verbum(ctx, "1.").numerus, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_verbum(ctx, "Ps").numerus, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_verbum(ctx, "Ps").accentus, -I);
    CREDO_AEQUALIS_I32 (_verbum(ctx, "\xff" "a\xfe" "e").numerus,
        (i32)II);

    /* octeti temere: in filio, deinde invariantia hic */
    sors_seminare(&sors, (i64)MMXXVI, (i64)XI);
    p = piscina_generare_dynamicum("syllabae_temere",
        (memoriae_index)(CXXVIII * MXXIV));
    CREDO_NECESSE (p != NIHIL);
    per (r = ZEPHYRUM; r < (i32)L; r++)
    {
        i32 n = (i32)sors_intra(&sors, (i32)MMMCMXCIX);

        per (k = ZEPHYRUM; k < n; k++)
        {
            /* dimidium litterae (a e i u s t q g -), cetera temere */
            si (sors_intra(&sors, II) == ZEPHYRUM)
            {
                temere[k] = (i8)"aeiustqg-\xc3\xa1\xcc\x81"
                    [sors_intra(&sors, XII)];
            }
            alioquin
            {
                temere[k] = (i8)sors_intra(&sors, CCLVI);
            }
        }
        CREDO_NON_RUIT (_omnia_dividere(temere, n));
        piscina_vacare(p);
        verbum.datum    = temere;
        verbum.mensura  = n;
        CREDO_VERUM (syllabae_latinae_dividere(verbum, p, &v));
        CREDO_VERUM (_valida(verbum, &v));
    }
    piscina_destruere(p);
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "divisio: casus manu (AR 1912)", _divisio_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "accentus", _accentus_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vocalis", _vocalis_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "specimen GregoBase: numerus et accentus", _exemplar_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "totalitas", _totalitas_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("syllabae_latinae", SECTIONES, NIHIL,
        NIHIL);
}
