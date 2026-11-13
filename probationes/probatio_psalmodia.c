/* probatio_psalmodia.c - aptator psalmorum (psalmodia-plan-2 T4-T6)
 *
 * T4: lectio textus punctati per psalmodia_aptare cum tono ficto
 * cuius clausulae locum unum habent (mediatio "f", soror monosyllaba
 * "g", terminatio "e", tenor "h"): syllaba ultima semiversus notam
 * clausulae capit, ceterae tenorem - ita lineae, signa, verba, voces
 * hebraicae et admonitiones videntur antequam regula ipsa (T5)
 * aptatur.
 */

#include "latina.h"
#include "credo.h"
#include "psalmodia.h"
#include "psalmodia_tabula.h"
#include "chorda_aedificator.h"
#include "piscina.h"
#include "sors.h"
#include <stdlib.h>
#include <string.h>


/* ==================================================
 * Tonus fictus
 * ================================================== */

nomen structura {
       PsalmodiaTonus* tonus;
    PsalmodiaClausula* terminatio;
} TonusFictus;

interior PsalmodiaLocus*
_locum_fingere (
               Piscina* piscina,
    constans character* notae)
{
    PsalmodiaLocus* locus;

    locus = (PsalmodiaLocus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(PsalmodiaLocus));
    memset(locus, ZEPHYRUM, magnitudo(PsalmodiaLocus));
    locus->genus = PSALMODIA_LOCUS_NOTA;
    locus->notae = chorda_ex_literis(notae, piscina);
    redde locus;
}

/* tonus VIII fictus: mediatio "f", soror monosyllaba "g",
 * terminatio "e"; tenor "h", sine intonatione, sine flexa */
interior TonusFictus
_tonum_fingere (
    Piscina* piscina)
{
          TonusFictus  fictus;
    PsalmodiaClausula* mediationes;

    fictus.tonus = (PsalmodiaTonus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(PsalmodiaTonus));
    mediationes = (PsalmodiaClausula*)piscina_allocare(piscina,
        (memoriae_index)(II * magnitudo(PsalmodiaClausula)));
    fictus.terminatio = (PsalmodiaClausula*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(PsalmodiaClausula));
    memset(fictus.tonus, ZEPHYRUM, magnitudo(PsalmodiaTonus));
    memset(mediationes, ZEPHYRUM, II * magnitudo(PsalmodiaClausula));
    memset(fictus.terminatio, ZEPHYRUM, magnitudo(PsalmodiaClausula));

    mediationes[ZEPHYRUM].loci             = _locum_fingere(piscina,
        "f");
    mediationes[ZEPHYRUM].numerus_locorum  = I;
    mediationes[I].loci                    = _locum_fingere(piscina,
        "g");
    mediationes[I].numerus_locorum  = I;
    mediationes[I].monosyllaba      = VERUM;
    fictus.terminatio->loci                = _locum_fingere(piscina,
        "e");
    fictus.terminatio->numerus_locorum     = I;

    fictus.tonus->modus                  = chorda_ex_literis("8",
        piscina);
    fictus.tonus->modus_numerus          = (s32)VIII;
    fictus.tonus->clavis                 = chorda_ex_literis("c4",
        piscina);
    fictus.tonus->tenor                  = chorda_ex_literis("h",
        piscina);
    fictus.tonus->mediationes            = mediationes;
    fictus.tonus->numerus_mediationum    = II;
    fictus.tonus->terminationes          = fictus.terminatio;
    fictus.tonus->numerus_terminationum  = I;
    redde fictus;
}


/* ==================================================
 * Auxilia
 * ================================================== */

/* textus -> psalmus sine Gloria, tono ficto */
interior PsalmodiaPsalmus*
_psalmus (
        CredoContextus* ctx,
    constans character* textus)
{
          TonusFictus fictus = _tonum_fingere(ctx->piscina);
    PsalmodiaOptiones optiones;

    memset(&optiones, ZEPHYRUM, magnitudo(optiones));
    optiones.sine_gloria = VERUM;
    redde psalmodia_aptare(fictus.tonus, fictus.terminatio,
        chorda_ex_literis(textus, ctx->piscina), &optiones,
        ctx->piscina);
}

/* versus -> "Dó(h)mi(h)nus(f) *(*) ..." (spatium ante verbum) */
interior chorda
_reddere (
               CredoContextus* ctx,
    constans PsalmodiaPsalmus* psalmus,
                          i32  index)
{
    ChordaAedificator* a;
                  i32  i;

    si (psalmus == NIHIL || index >= psalmus->numerus_versuum)
    {
        redde chorda_ex_literis("<versus nullus>", ctx->piscina);
    }
    a = chorda_aedificator_creare(ctx->piscina, CXXVIII);
    per (i = ZEPHYRUM; i < psalmus->versus[index].numerus; i++)
    {
        constans PsalmodiaSyllaba* s = &psalmus->versus[index]
            .syllabae[i];

        si (i > ZEPHYRUM && s->verbum_incipit)
        {
            chorda_aedificator_appendere_character(a, ' ');
        }
        chorda_aedificator_appendere_chorda(a, s->textus);
        chorda_aedificator_appendere_character(a, '(');
        chorda_aedificator_appendere_chorda(a, s->notae);
        chorda_aedificator_appendere_character(a, ')');
    }
    redde chorda_aedificator_finire(a);
}

/* admonitiones -> "t:1:9: causa\n..." */
interior chorda
_monita (
               CredoContextus* ctx,
    constans PsalmodiaPsalmus* psalmus)
{
    ChordaAedificator* a = chorda_aedificator_creare(ctx->piscina,
        CXXVIII);

    psalmodia_admonitiones_scribere(psalmus, "t", a);
    redde chorda_aedificator_finire(a);
}

/* textus unius versus sine admonitione -> versus redditus */
#define VERSUS_PURUS(textus, expectatum)                               \
    fac {                                                              \
        PsalmodiaPsalmus* ps_ = _psalmus(ctx, textus);                 \
        CREDO_NECESSE_NON_NIHIL (ps_);                                 \
        CREDO_AEQUALIS_I32 (ps_->numerus_versuum, I);                  \
        CREDO_CHORDA_AEQUALIS_LITERIS (_reddere(ctx, ps_, ZEPHYRUM),   \
            expectatum);                                               \
        CREDO_CHORDA_AEQUALIS_LITERIS (_monita(ctx, ps_), "");         \
    } dum (ZEPHYRUM)

/* textus unius versus -> versus redditus et admonitiones */
#define VERSUS_MONITUS(textus, expectatum, monita)                     \
    fac {                                                              \
        PsalmodiaPsalmus* ps_ = _psalmus(ctx, textus);                 \
        CREDO_NECESSE_NON_NIHIL (ps_);                                 \
        CREDO_AEQUALIS_I32 (ps_->numerus_versuum, I);                  \
        CREDO_CHORDA_AEQUALIS_LITERIS (_reddere(ctx, ps_, ZEPHYRUM),   \
            expectatum);                                               \
        CREDO_CHORDA_AEQUALIS_LITERIS (_monita(ctx, ps_), monita);     \
    } dum (ZEPHYRUM)


/* ==================================================
 * Lineae
 * ================================================== */

#define VERSUS_PRIMUS                                                  \
    "Dó(h)mi(h)nus(h) re(h)git(h) me,(g) *(*) "                       \
    "et(h) ni(h)hil(h) mi(h)hi(h) dé(h)e(h)rit.(e)"
#define VERSUS_ALTER                                                   \
    "De(h)us(h) me(h)us(h) es(h) tu,(h) †(†) "                     \
    "e(h)go(h) hó(h)di(h)e(f) *(*) gé(h)nu(h)i(h) te.(e)"

interior vacuum
_lineae_probare (
    CredoContextus* ctx)
{
    PsalmodiaPsalmus* purus;
    PsalmodiaPsalmus* sordidus;

    purus = _psalmus(ctx,
        "Dóminus regit me, * et nihil mihi déerit.\n"
        "Deus meus es tu, † ego hódie * génui te.\n");
    CREDO_NECESSE_NON_NIHIL (purus);
    CREDO_AEQUALIS_I32 (purus->numerus_versuum, II);
    CREDO_NECESSE (purus->numerus_versuum == II);
    CREDO_CHORDA_AEQUALIS_LITERIS (_reddere(ctx, purus, ZEPHYRUM),
        VERSUS_PRIMUS);
    CREDO_CHORDA_AEQUALIS_LITERIS (_reddere(ctx, purus, I),
        VERSUS_ALTER);
    CREDO_CHORDA_AEQUALIS_LITERIS (_monita(ctx, purus), "");
    CREDO_AEQUALIS_I32 (purus->versus[ZEPHYRUM].linea, I);
    CREDO_AEQUALIS_I32 (purus->versus[I].linea, II);

    /* BOM, commentaria, lineae vacuae, CRLF, CR, numeri initio,
     * '+' pro '†': idem psalmus */
    sordidus = _psalmus(ctx,
        "\xEF\xBB\xBF% nota\r\n"
        "\r\n"
        "   \t \r\n"
        "1. Dóminus regit me, * et nihil mihi déerit.\r\n"
        "  % altera\r"
        "12 Deus meus es tu, + ego hódie * génui te.\r");
    CREDO_NECESSE_NON_NIHIL (sordidus);
    CREDO_AEQUALIS_I32 (sordidus->numerus_versuum, II);
    CREDO_NECESSE (sordidus->numerus_versuum == II
        && sordidus->versus[I].numerus > VI);
    CREDO_CHORDA_AEQUALIS_LITERIS (_reddere(ctx, sordidus, ZEPHYRUM),
        VERSUS_PRIMUS);
    CREDO_CHORDA_AEQUALIS_LITERIS (_reddere(ctx, sordidus, I),
        VERSUS_ALTER);
    CREDO_CHORDA_AEQUALIS_LITERIS (_monita(ctx, sordidus), "");
    CREDO_AEQUALIS_I32 (sordidus->versus[ZEPHYRUM].linea, IV);
    CREDO_AEQUALIS_I32 (sordidus->versus[I].linea, VI);
    /* columnae textus originalis: "1. Dó" -> IV */
    CREDO_AEQUALIS_I32 (sordidus->versus[ZEPHYRUM].syllabae[ZEPHYRUM]
        .columna, IV);
    CREDO_AEQUALIS_I32 (sordidus->versus[ZEPHYRUM].syllabae[ZEPHYRUM]
        .linea, IV);
    /* signum '+' textum "†" accipit, locum suum servat */
    CREDO_CHORDA_AEQUALIS_LITERIS (sordidus->versus[I].syllabae[VI]
        .textus, "†");
    CREDO_AEQUALIS_I32 (sordidus->versus[I].syllabae[VI].columna,
        XXI);

    /* BOM ante versum: columna I post BOM */
    purus = _psalmus(ctx, "\xEF\xBB\xBF" "Dóminus * regit me.");
    CREDO_NECESSE_NON_NIHIL (purus);
    CREDO_NECESSE (purus->numerus_versuum == I);
    CREDO_AEQUALIS_I32 (purus->versus[ZEPHYRUM].syllabae[ZEPHYRUM]
        .columna, I);

    /* numerus sine spatio post punctum */
    VERSUS_PURUS ("1.Dóminus * regit me.",
        "Dó(h)mi(h)nus(f) *(*) re(h)git(h) me.(e)");

    /* textus vacuus, commentaria sola: psalmus sine versibus */
    purus = _psalmus(ctx, "");
    CREDO_NECESSE_NON_NIHIL (purus);
    CREDO_AEQUALIS_I32 (purus->numerus_versuum, ZEPHYRUM);
    purus = _psalmus(ctx, "% sola\n\n\r\n");
    CREDO_NECESSE_NON_NIHIL (purus);
    CREDO_AEQUALIS_I32 (purus->numerus_versuum, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (_monita(ctx, purus), "");

    /* linea sine syllaba: admonitio, versus nullus */
    purus = _psalmus(ctx, "3.\n— …\nDóminus * regit me.\n");
    CREDO_NECESSE_NON_NIHIL (purus);
    CREDO_AEQUALIS_I32 (purus->numerus_versuum, I);
    CREDO_NECESSE (purus->numerus_versuum == I);
    CREDO_AEQUALIS_I32 (purus->versus[ZEPHYRUM].linea, III);
    CREDO_CHORDA_AEQUALIS_LITERIS (_monita(ctx, purus),
        "t:1:1: versus sine syllabis\n"
        "t:2:1: versus sine syllabis\n");
}


/* ==================================================
 * Signa: '*', '†', '+'
 * ================================================== */

interior vacuum
_signa_probare (
    CredoContextus* ctx)
{
    /* signa verbis adhaerentia */
    VERSUS_PURUS ("Deus meus es tu,+ego hódie* génui te.",
        "De(h)us(h) me(h)us(h) es(h) tu,(h) †(†) "
        "e(h)go(h) hó(h)di(h)e(f) *(*) gé(h)nu(h)i(h) te.(e)");

    /* sine '*': admonitio fine lineae; totus = semiversus secundus */
    VERSUS_MONITUS ("Laudáte Dóminum de cælis.  ",
        "Lau(h)dá(h)te(h) Dó(h)mi(h)num(h) de(h) cæ(h)lis.(e)",
        "t:1:26: sine asterisco\n");

    /* '*' iteratus: admonitio ad alterum, alter omittitur */
    VERSUS_MONITUS ("Dóminus * regit * me.",
        "Dó(h)mi(h)nus(f) *(*) re(h)git(h) me.(e)",
        "t:1:17: asteriscus iteratus\n");

    /* '†' post '*': admonitio, omittitur */
    VERSUS_MONITUS ("Dóminus * regit † me.",
        "Dó(h)mi(h)nus(f) *(*) re(h)git(h) me.(e)",
        "t:1:17: flexa post asteriscum\n");

    /* '†' iterata */
    VERSUS_MONITUS ("Deus † meus + es * tu.",
        "De(h)us(h) †(†) me(h)us(h) es(g) *(*) tu.(e)",
        "t:1:13: flexa iterata\n");

    /* partes vacuae */
    VERSUS_MONITUS ("* regit me.",
        "*(*) re(h)git(h) me.(e)",
        "t:1:1: semiversus vacuus\n");
    VERSUS_MONITUS ("Dóminus regit *",
        "Dó(h)mi(h)nus(h) re(h)git(f) *(*)",
        "t:1:16: semiversus vacuus\n");
    VERSUS_MONITUS ("† Dóminus * regit me.",
        "†(†) Dó(h)mi(h)nus(f) *(*) re(h)git(h) me.(e)",
        "t:1:1: pars ante flexam vacua\n");
}


/* ==================================================
 * Verba: interpunctio, accentus, voces hebraicae
 * ================================================== */

interior vacuum
_verba_probare (
    CredoContextus* ctx)
{
    PsalmodiaPsalmus* ps;

    /* verbum trium syllabarum sine acuto: admonitio verbum
     * nominans, columna characterum (non octetorum) */
    ps = _psalmus(ctx, "Dóminus, Dominus * regit me.");
    CREDO_NECESSE_NON_NIHIL (ps);
    CREDO_CHORDA_AEQUALIS_LITERIS (_reddere(ctx, ps, ZEPHYRUM),
        "Dó(h)mi(h)nus,(h) Do(h)mi(h)nus(f) *(*) re(h)git(h) me.(e)");
    CREDO_CHORDA_AEQUALIS_LITERIS (_monita(ctx, ps),
        "t:1:10: verbum \"Dominus\" sine accentu\n");
    CREDO_NECESSE (ps->numerus_versuum == I
        && ps->versus[ZEPHYRUM].numerus > IV);
    CREDO_AEQUALIS_I32 (ps->versus[ZEPHYRUM].syllabae[III].columna, X);
    CREDO_AEQUALIS_I32 (ps->versus[ZEPHYRUM].syllabae[IV].columna, XII);
    CREDO_VERUM (ps->versus[ZEPHYRUM].syllabae[III].verbum_incipit);
    CREDO_FALSUM (ps->versus[ZEPHYRUM].syllabae[IV].verbum_incipit);

    /* voces hebraicae: syllabae indicis, sine admonitione; in fine
     * semiversus abruptae (soror monosyllaba "g") */
    VERSUS_PURUS ("Speret Israel * in Dómino.",
        "Spe(h)ret(h) Is(h)ra(h)el(g) *(*) in(h) Dó(h)mi(h)no.(e)");
    VERSUS_PURUS ("Speret israel * in Dómino.",
        "Spe(h)ret(h) is(h)ra(h)el(g) *(*) in(h) Dó(h)mi(h)no.(e)");
    VERSUS_PURUS ("Speret Israël, * in Dómino.",
        "Spe(h)ret(h) Is(h)ra(h)ël,(g) *(*) in(h) Dó(h)mi(h)no.(e)");
    VERSUS_PURUS ("Speret «Ísrael» * in Dómino.",
        "Spe(h)ret(h) «Ís(h)ra(h)el»(g) *(*) "
        "in(h) Dó(h)mi(h)no.(e)");
    VERSUS_PURUS ("Dóminus ex Sion * benedícat tibi.",
        "Dó(h)mi(h)nus(h) ex(h) Si(h)on(g) *(*) "
        "be(h)ne(h)dí(h)cat(h) ti(h)bi.(e)");
    VERSUS_PURUS ("Lauda Jerúsalem * Dóminum.",
        "Lau(h)da(h) Je(h)rú(h)sa(h)lem(g) *(*) Dó(h)mi(h)num.(e)");
    /* in medio non abrupta */
    VERSUS_PURUS ("Sion lauda * Deum.",
        "Si(h)on(h) lau(h)da(f) *(*) De(h)um.(e)");
    /* '-' in textu divisionem indicis vincit */
    VERSUS_PURUS ("Speret Is-rael * in Dómino.",
        "Spe(h)ret(h) Is(h)rael(g) *(*) in(h) Dó(h)mi(h)no.(e)");

    /* signa non verba: adhaerent syllabae vicinae */
    VERSUS_PURUS ("« Dóminus — regit * me. »",
        "« Dó(h)mi(h)nus —(h) re(h)git(f) *(*) me. »(e)");

    /* signa gabc et octeti mali: admonitio, omittuntur */
    VERSUS_MONITUS ("Dó(mi)nus * regit me.",
        "Dó(h)mi(h)nus(f) *(*) re(h)git(h) me.(e)",
        "t:1:3: signum gabc '(' omissum\n"
        "t:1:6: signum gabc ')' omissum\n");
    VERSUS_MONITUS ("Dómi\xffnus * re<b>git me.",
        "Dó(h)mi(h)nus(f) *(*) reb(h)git(h) me.(e)",
        "t:1:5: octetus 0xFF omissus\n"
        "t:1:14: signum gabc '<' omissum\n"
        "t:1:16: signum gabc '>' omissum\n");
}


/* ==================================================
 * Gloria Patri; tonus absens
 * ================================================== */

interior vacuum
_gloria_probare (
    CredoContextus* ctx)
{
          TonusFictus  fictus = _tonum_fingere(ctx->piscina);
     PsalmodiaPsalmus* ps;
               chorda  textus =
                   chorda_ex_literis("Dóminus * regit me.",
                           ctx->piscina);

    ps = psalmodia_aptare(fictus.tonus, fictus.terminatio, textus,
        NIHIL, ctx->piscina);
    CREDO_NECESSE_NON_NIHIL (ps);
    CREDO_AEQUALIS_I32 (ps->numerus_versuum, III);
    CREDO_NECESSE (ps->numerus_versuum == III);
    CREDO_FALSUM (ps->versus[ZEPHYRUM].gloria);
    CREDO_VERUM (ps->versus[I].gloria);
    CREDO_VERUM (ps->versus[II].gloria);
    CREDO_AEQUALIS_I32 (ps->versus[I].linea, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (_reddere(ctx, ps, I),
        "Gló(h)ri(h)a(h) Pa(h)tri,(h) et(h) Fí(h)li(h)o,(f) *(*) "
        "et(h) Spi(h)rí(h)tu(h)i(h) San(h)cto.(e)");
    CREDO_CHORDA_AEQUALIS_LITERIS (_reddere(ctx, ps, II),
        "Sic(h)ut(h) e(h)rat(h) in(h) prin(h)cí(h)pi(h)o,(h) et(h) "
        "nunc,(h) et(h) sem(h)per,(f) *(*) et(h) in(h) "
        "sǽ(h)cu(h)la(h) sæ(h)cu(h)ló(h)rum.(h) A(h)men.(e)");
    CREDO_CHORDA_AEQUALIS_LITERIS (_monita(ctx, ps), "");
    CREDO_AEQUALIS_PTR (ps->tonus, fictus.tonus);
    CREDO_AEQUALIS_PTR (ps->terminatio, fictus.terminatio);

    /* tonus aut terminatio absens: admonitio, versus nulli */
    ps = psalmodia_aptare(NIHIL, fictus.terminatio, textus, NIHIL,
        ctx->piscina);
    CREDO_NECESSE_NON_NIHIL (ps);
    CREDO_AEQUALIS_I32 (ps->numerus_versuum, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (_monita(ctx, ps),
        "t:0:0: tonus nullus\n");
    ps = psalmodia_aptare(fictus.tonus, NIHIL, textus, NIHIL,
        ctx->piscina);
    CREDO_NECESSE_NON_NIHIL (ps);
    CREDO_AEQUALIS_I32 (ps->numerus_versuum, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (_monita(ctx, ps),
        "t:0:0: terminatio nulla\n");
}


/* ==================================================
 * Totalitas
 * ================================================== */

/* invariantia psalmi cuiusvis */
interior b32
_validus (
    constans PsalmodiaPsalmus* ps)
{
    i32 v;
    i32 i;
    i32 k;

    si (ps == NIHIL)
    {
        redde FALSUM;
    }
    per (v = ZEPHYRUM; v < ps->numerus_versuum; v++)
    {
        si (ps->versus[v].numerus == ZEPHYRUM)
        {
            redde FALSUM;
        }
        per (i = ZEPHYRUM; i < ps->versus[v].numerus; i++)
        {
            constans PsalmodiaSyllaba* s = &ps->versus[v].syllabae[i];

            si (   s->textus.mensura == ZEPHYRUM
                || s->notae.mensura  == ZEPHYRUM)
            {
                redde FALSUM;
            }
            per (k = ZEPHYRUM; k < s->textus.mensura; k++)
            {
                si (   strchr("()[]{}<>|%", (integer)s->textus.datum[k])
                    != NIHIL && s->textus.datum[k] != '\0')
                {
                    redde FALSUM;
                }
            }
        }
    }
    per (k = ZEPHYRUM; k < ps->numerus_admonitionum; k++)
    {
        si (ps->admonitiones[k].causa.mensura == ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* in filio sub CREDO_NON_RUIT */
interior vacuum
_omnia_aptare (
    constans i8* octeti,
            i32  mensura)
{
              Piscina* p = piscina_generare_dynamicum(
                           "psalmodia_totalitas",
                           (memoriae_index)(CXXVIII * MXXIV));
          TonusFictus  fictus;
               chorda  textus;
     PsalmodiaPsalmus* ps;
    ChordaAedificator* a;

    si (p == NIHIL)
    {
        redde;
    }
    fictus        = _tonum_fingere(p);
    textus.datum  = (i8*)malloc((size_t)(mensura > ZEPHYRUM
        ? mensura : I));
    si (textus.datum == NIHIL)
    {
        redde;
    }
    si (mensura > ZEPHYRUM)
    {
        memcpy(textus.datum, octeti, (size_t)mensura);
    }
    textus.mensura = mensura;
    ps = psalmodia_aptare(fictus.tonus, fictus.terminatio, textus,
        NIHIL, p);
    a = chorda_aedificator_creare(p, CCLVI);
    (vacuum)psalmodia_admonitiones_scribere(ps, "t", a);
    free(textus.datum);
    piscina_destruere(p);
}

interior vacuum
_totalitas_probare (
    CredoContextus* ctx)
{
    constans character* bonus =
        "\xEF\xBB\xBF"
        "1. Dóminus regit me, * et nihil mihi déerit.\r\n"
        "Deus † meus Israël, * génui «te».\n"
        "% nota\n"
        "Lauda Jerúsalem Dóminum: + lauda Deum tuum, * Sion.\n";
                  i8 temere[MMMCMXCIX];
                  i8 copia[CCLVI];
                Sors sors;
         TonusFictus fictus = _tonum_fingere(ctx->piscina);
              chorda textus;
                 i32 n;
                 i32 k;
                 i32 r;

    /* textus bonus truncatus in omni octeto */
    n = (i32)strlen(bonus);
    CREDO_NECESSE (n < (i32)CCLVI);
    memcpy(copia, bonus, (size_t)n);
    per (k = ZEPHYRUM; k <= n; k++)
    {
        CREDO_NON_RUIT (_omnia_aptare(copia, k));
        textus.datum    = copia;
        textus.mensura  = k;
        CREDO_VERUM (_validus(psalmodia_aptare(fictus.tonus,
            fictus.terminatio, textus, NIHIL, ctx->piscina)));
    }

    /* octeti temere: dimidium signa textus, cetera temere */
    sors_seminare(&sors, (i64)MMXXVI, (i64)XII);
    per (r = ZEPHYRUM; r < (i32)L; r++)
    {
        n = (i32)sors_intra(&sors, (i32)MMMCMXCIX);
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (sors_intra(&sors, II) == ZEPHYRUM)
            {
                temere[k] = (i8)"aeius *+ \n.-1(%\xc3\xa1\xe2\x80\xa0"
                    [sors_intra(&sors, XIX)];
            }
            alioquin
            {
                temere[k] = (i8)sors_intra(&sors, CCLVI);
            }
        }
        CREDO_NON_RUIT (_omnia_aptare(temere, n));
        textus.datum    = temere;
        textus.mensura  = n;
        CREDO_VERUM (_validus(psalmodia_aptare(fictus.tonus,
            fictus.terminatio, textus, NIHIL, ctx->piscina)));
    }
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "textus: lineae", _lineae_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "textus: signa", _signa_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "textus: verba et voces hebraicae", _verba_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "Gloria Patri; tonus absens", _gloria_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "totalitas", _totalitas_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("psalmodia", SECTIONES, NIHIL,
        NIHIL);
}
