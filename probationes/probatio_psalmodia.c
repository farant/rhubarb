/* probatio_psalmodia.c - aptator psalmorum (psalmodia-plan-2 T4-T6)
 *
 * T4: lectio textus punctati per psalmodia_aptare cum tono ficto
 * cuius clausulae locum unum habent (mediatio "f", soror monosyllaba
 * "g", terminatio "e", tenor "h"): syllaba ultima semiversus notam
 * clausulae capit, ceterae tenorem - ita lineae, signa, verba, voces
 * hebraicae et admonitiones videntur antequam regula ipsa (T5)
 * aptatur.
 *
 * T5: regula ipsa contra tabulam veram (musica/toni_psalmorum.stml):
 * exempla libri omnia (CCXLV aequalia, "initium solemne" omissum),
 * textus breves (TEXTUS DEFICIENS), toni rari.
 */

#include "latina.h"
#include "credo.h"
#include "psalmodia.h"
#include "psalmodia_tabula.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "gabc.h"
#include "piscina.h"
#include "sors.h"
#include <stdio.h>
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
    /* '$' (effugium gabc) et '~' (U+203F in gabc): omittuntur (T6) */
    VERSUS_MONITUS ("Dó$minus * re~git me.",
        "Dó(h)mi(h)nus(f) *(*) re(h)git(h) me.(e)",
        "t:1:3: signum gabc '$' omissum\n"
        "t:1:14: signum gabc '~' omissum\n");
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
 * Tabula vera: auxilia
 * ================================================== */

interior PsalmodiaTabula*
_tabula_vera (
    CredoContextus* ctx)
{
    redde psalmodia_tabulam_legere(filum_legere_totum(
        "musica/toni_psalmorum.stml", ctx->piscina), ctx->piscina);
}

interior b32
_chorda_literis (
                 chorda  c,
     constans character* literae)
{
    i32 n = (i32)strlen(literae);

    redde c.mensura == n && (n == ZEPHYRUM
        || memcmp(c.datum, literae, (size_t)n) == ZEPHYRUM);
}

/* tonus tabulae per modum (et positionem alteram) */
interior constans PsalmodiaTonus*
_tonus (
    constans PsalmodiaTabula* tabula,
          constans character* modus,
                         b32  altera)
{
    i32 k;

    si (tabula == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < tabula->numerus_tonorum; k++)
    {
        si (   _chorda_literis(tabula->toni[k].modus, modus)
            && tabula->toni[k].positio_altera == altera)
        {
            redde &tabula->toni[k];
        }
    }
    redde NIHIL;
}

/* terminatio communis differentiae (vacua = unica toni) */
interior constans PsalmodiaClausula*
_terminatio (
    constans PsalmodiaTonus* tonus,
                     chorda  differentia)
{
    constans PsalmodiaClausula* inventa   = NIHIL;
                           i32  communes  = ZEPHYRUM;
                           i32  k;

    per (k = ZEPHYRUM; k < tonus->numerus_terminationum; k++)
    {
        constans PsalmodiaClausula* c = &tonus->terminationes[k];

        si (c->monosyllaba || c->ad_libitum)
        {
            perge;
        }
        communes++;
        si (   differentia.mensura > ZEPHYRUM
            && chorda_aequalis(c->differentia, differentia))
        {
            redde c;
        }
        inventa = c;
    }
    redde differentia.mensura == ZEPHYRUM && communes == I ? inventa
        : NIHIL;
}

/* notae syllabarum versus spatiis iunctae ("h ixi hr h * ...") */
interior chorda
_notae (
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
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, ' ');
        }
        chorda_aedificator_appendere_chorda(a,
            psalmus->versus[index].syllabae[i].notae);
    }
    redde chorda_aedificator_finire(a);
}

/* pars una toni veri -> notae iunctae */
interior chorda
_partis_notae (
                  CredoContextus* ctx,
         constans PsalmodiaTonus* tonus,
      constans PsalmodiaClausula* clausula,
                  PsalmodiaFinis  finis,
                             b32  intonatio,
              constans character* textus)
{
    redde _notae(ctx, psalmodia_partem_aptare(tonus, clausula, finis,
        intonatio, chorda_ex_literis(textus, ctx->piscina),
        ctx->piscina), ZEPHYRUM);
}

/* psalmus toni veri sine Gloria */
interior PsalmodiaPsalmus*
_psalmus_verus (
                  CredoContextus* ctx,
         constans PsalmodiaTonus* tonus,
      constans PsalmodiaClausula* terminatio,
                             b32  antiqua,
                             b32  canticum,
              constans character* textus)
{
    PsalmodiaOptiones optiones;

    memset(&optiones, ZEPHYRUM, magnitudo(optiones));
    optiones.sine_gloria  = VERUM;
    optiones.antiqua      = antiqua;
    optiones.canticum     = canticum;
    redde psalmodia_aptare(tonus, terminatio, chorda_ex_literis(textus,
        ctx->piscina), &optiones, ctx->piscina);
}


/* ==================================================
 * Exempla libri (T5)
 * ================================================== */

/* exemplum -> psalmus noster (mappa plani T5) */
interior PsalmodiaPsalmus*
_exemplum_psalmus (
                CredoContextus* ctx,
       constans PsalmodiaTonus* tonus,
    constans PsalmodiaExemplum* ex)
{
     PsalmodiaPsalmus* ps = NIHIL;
    PsalmodiaOptiones  optiones;
                  b32  antiqua = _chorda_literis(ex->forma, "antiqua");

    commutatio (ex->pars)
    {
        casus PSALMODIA_PARS_MEDIATIO:
            ps = psalmodia_partem_aptare(tonus,
                psalmodia_mediationem_eligere(tonus, antiqua),
                PSALMODIA_FINIS_MEDIATIO, FALSUM, ex->textus,
                ctx->piscina);
            frange;
        casus PSALMODIA_PARS_SEMIVERSUS:
            ps = psalmodia_partem_aptare(tonus,
                psalmodia_mediationem_eligere(tonus, antiqua),
                PSALMODIA_FINIS_MEDIATIO, VERUM, ex->textus,
                ctx->piscina);
            frange;
        casus PSALMODIA_PARS_TERMINATIO:
            ps = psalmodia_partem_aptare(tonus, _terminatio(tonus,
                ex->differentia), PSALMODIA_FINIS_TERMINATIO, FALSUM,
                ex->textus, ctx->piscina);
            frange;
        casus PSALMODIA_PARS_FLEXA:
            ps = psalmodia_partem_aptare(tonus, NIHIL,
                PSALMODIA_FINIS_FLEXA, FALSUM, ex->textus,
                ctx->piscina);
            frange;
        casus PSALMODIA_PARS_INITIUM:
            ps = psalmodia_partem_aptare(tonus, NIHIL,
                PSALMODIA_FINIS_NULLUS, VERUM, ex->textus,
                ctx->piscina);
            frange;
        casus PSALMODIA_PARS_VERSUS:
            memset(&optiones, ZEPHYRUM, magnitudo(optiones));
            optiones.sine_gloria       = VERUM;
            optiones.antiqua           = antiqua;
            optiones.canticum          = ex->canticum;
            optiones.sine_intonatione  = !ex->primus;
            ps = psalmodia_aptare(tonus, _terminatio(tonus,
                ex->differentia), ex->textus, &optiones, ctx->piscina);
            frange;
    }
    redde ps;
}

/* exemplum -> notae nostrae iunctae */
interior chorda
_exemplum_aptare (
                CredoContextus* ctx,
       constans PsalmodiaTonus* tonus,
    constans PsalmodiaExemplum* ex)
{
    redde _notae(ctx, _exemplum_psalmus(ctx, tonus, ex), ZEPHYRUM);
}

interior vacuum
_exempla_libri_probare (
    CredoContextus* ctx)
{
     PsalmodiaTabula* t         = _tabula_vera(ctx);
                 i32  aequalia  = ZEPHYRUM;
                 i32  impares   = ZEPHYRUM;
                 i32  omissa    = ZEPHYRUM;
                 i32  k;
                 i32  e;
                 i32  j;

    CREDO_NECESSE (t != NIHIL && psalmodia_successus(t));
    per (k = ZEPHYRUM; k < t->numerus_tonorum; k++)
    {
        constans PsalmodiaTonus* tonus = &t->toni[k];

        per (e = ZEPHYRUM; e < tonus->numerus_exemplorum; e++)
        {
             constans PsalmodiaExemplum* ex = &tonus->exempla[e];
                      ChordaAedificator* a;
                                 chorda  nostrae;
                                 chorda  libri;

            si (_chorda_literis(ex->nota, "initium solemne"))
            {
                omissa++;
                perge;
            }
            a = chorda_aedificator_creare(ctx->piscina, CXXVIII);
            per (j = ZEPHYRUM; j < ex->numerus_notarum; j++)
            {
                si (j > ZEPHYRUM)
                {
                    chorda_aedificator_appendere_character(a, ' ');
                }
                chorda_aedificator_appendere_chorda(a, ex->notae[j]);
            }
            libri    = chorda_aedificator_finire(a);
            nostrae  = _exemplum_aptare(ctx, tonus, ex);
            si (chorda_aequalis(nostrae, libri))
            {
                aequalia++;
            }
            alioquin
            {
                impares++;
                imprimere("  tonus %.*s%s p. %.*s \"%.*s\"\n"
                    "    nostrae: %.*s\n    libri:   %.*s\n",
                    (integer)tonus->modus.mensura,
                    (character*)tonus->modus.datum,
                    tonus->positio_altera ? " altera" : "",
                    (integer)ex->pagina.mensura,
                    (character*)ex->pagina.datum,
                    (integer)ex->textus.mensura,
                    (character*)ex->textus.datum,
                    (integer)nostrae.mensura, (character*)nostrae.datum,
                    (integer)libri.mensura, (character*)libri.datum);
            }
        }
    }
    imprimere("  exempla libri: %u aequalia, %u impares, %u omissa\n",
        aequalia, impares, omissa);
    CREDO_AEQUALIS_I32 (aequalia, (i32)CCXLV);
    CREDO_AEQUALIS_I32 (impares, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (omissa, I);
}


/* ==================================================
 * Textus breves (TEXTUS DEFICIENS)
 * ================================================== */

interior vacuum
_breves_probare (
    CredoContextus* ctx)
{
               PsalmodiaTabula* t = _tabula_vera(ctx);
       constans PsalmodiaTonus* primus;
       constans PsalmodiaTonus* septimus;
       constans PsalmodiaTonus* octavus;
       constans PsalmodiaTonus* in_directum;
              PsalmodiaPsalmus* ps;
    constans PsalmodiaClausula* med;

    CREDO_NECESSE (t != NIHIL);
    primus       = _tonus(t, "1", FALSUM);
    septimus     = _tonus(t, "7", FALSUM);
    octavus      = _tonus(t, "8", FALSUM);
    in_directum  = _tonus(t, "in directum", FALSUM);
    CREDO_NECESSE (primus != NIHIL && septimus != NIHIL
        && octavus != NIHIL && in_directum != NIHIL);

    /* mediatio deficiens: notae reliquae cum tenore in syllabam
     * primam (liber: Qui fácit haec; hic etiam Qui solum) */
    med = psalmodia_mediationem_eligere(primus, FALSUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (_partis_notae(ctx, primus, med,
        PSALMODIA_FINIS_MEDIATIO, FALSUM, "Qui fácit haec"),
        "hixih g hr h");
    CREDO_CHORDA_AEQUALIS_LITERIS (_partis_notae(ctx, primus, med,
        PSALMODIA_FINIS_MEDIATIO, FALSUM, "fácit haec"),
        "hixihg hr h");
    CREDO_CHORDA_AEQUALIS_LITERIS (_partis_notae(ctx, primus, med,
        PSALMODIA_FINIS_MEDIATIO, FALSUM, "Qui"), "hixihgh");
    CREDO_CHORDA_AEQUALIS_LITERIS (_partis_notae(ctx, septimus,
        psalmodia_mediationem_eligere(septimus, FALSUM),
        PSALMODIA_FINIS_MEDIATIO, FALSUM, "Qui"), "ikjij");

    /* terminatio deficiens: notae primae omittuntur (AR p. 6*) */
    CREDO_CHORDA_AEQUALIS_LITERIS (_partis_notae(ctx, octavus,
        _terminatio(octavus, chorda_ex_literis("G", ctx->piscina)),
        PSALMODIA_FINIS_TERMINATIO, FALSUM, "Deus"), "h g");
    CREDO_CHORDA_AEQUALIS_LITERIS (_partis_notae(ctx, octavus,
        _terminatio(octavus, chorda_ex_literis("G", ctx->piscina)),
        PSALMODIA_FINIS_TERMINATIO, FALSUM, "te"), "g");

    /* semiversus primus nimis brevis pro intonatione et mediatione:
     * mediatio omittitur (I, Magníficat) */
    ps = _psalmus_verus(ctx, primus, _terminatio(primus,
        chorda_ex_literis("g", ctx->piscina)), FALSUM, VERUM,
        "Magníficat * ánima mea Dóminum.");
    CREDO_CHORDA_AEQUALIS_LITERIS (_notae(ctx, ps, ZEPHYRUM),
        "f gh h h * h h h g f gh gr g");

    /* flexa: monosyllaba et vox hebraica in tenore; proparoxytonon
     * duas syllabas deprimit */
    ps = _psalmus_verus(ctx, in_directum, _terminatio(in_directum,
        chorda_ex_literis("", ctx->piscina)), FALSUM, FALSUM,
        "Deus meus es tu, † et ego * ádjuva me.\n"
        "Lauda Sion, † et ego * ádjuva me.\n"
        "Laudáte Dóminum, † et ego * ádjuva me.\n");
    CREDO_NECESSE (ps != NIHIL && ps->numerus_versuum == III);
    CREDO_CHORDA_AEQUALIS_LITERIS (_notae(ctx, ps, ZEPHYRUM),
        "h h h h h h † f h h * h gr f g");
    CREDO_CHORDA_AEQUALIS_LITERIS (_notae(ctx, ps, I),
        "h h h h † f h h * h gr f g");
    CREDO_CHORDA_AEQUALIS_LITERIS (_notae(ctx, ps, II),
        "h h h h f f † f h h * h gr f g");

    /* intonatio flexam occupans: flexa omittitur */
    ps = _psalmus_verus(ctx, primus, _terminatio(primus,
        chorda_ex_literis("g", ctx->piscina)), FALSUM, FALSUM,
        "Dómine, † vivífica me * génui te.");
    CREDO_CHORDA_AEQUALIS_LITERIS (_notae(ctx, ps, ZEPHYRUM),
        "f gh h † h ixi h g h * g f gh g");

    /* semiversus vacuus et pars vacua: admonitio, nihil ruit */
    ps = _psalmus_verus(ctx, primus, _terminatio(primus,
        chorda_ex_literis("g", ctx->piscina)), FALSUM, FALSUM,
        "Dóminus regit *\n* regit me.\n");
    CREDO_NECESSE_NON_NIHIL (ps);
    CREDO_CHORDA_AEQUALIS_LITERIS (_monita(ctx, ps),
        "t:1:16: semiversus vacuus\n"
        "t:2:1: semiversus vacuus\n");
    CREDO_CHORDA_AEQUALIS_LITERIS (_notae(ctx, ps, I),
        "* gh gr g");
    ps = psalmodia_partem_aptare(primus, med, PSALMODIA_FINIS_MEDIATIO,
        FALSUM, chorda_ex_literis("", ctx->piscina), ctx->piscina);
    CREDO_NECESSE_NON_NIHIL (ps);
    CREDO_AEQUALIS_I32 (ps->numerus_versuum, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (_monita(ctx, ps),
        "t:1:1: pars vacua\n");

    /* signa in parte: admonitio, omittuntur */
    ps = psalmodia_partem_aptare(primus, med, PSALMODIA_FINIS_MEDIATIO,
        FALSUM, chorda_ex_literis("vivífica * me", ctx->piscina),
        ctx->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (_notae(ctx, ps, ZEPHYRUM),
        "h ixi h g h");
    CREDO_CHORDA_AEQUALIS_LITERIS (_monita(ctx, ps),
        "t:1:10: signum in parte omissum\n");

    /* tonus aut clausula absens */
    ps = psalmodia_partem_aptare(NIHIL, med, PSALMODIA_FINIS_MEDIATIO,
        FALSUM, chorda_ex_literis("me", ctx->piscina), ctx->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (_monita(ctx, ps),
        "t:0:0: tonus nullus\n");
    ps = psalmodia_partem_aptare(primus, NIHIL,
        PSALMODIA_FINIS_TERMINATIO, FALSUM, chorda_ex_literis("me",
        ctx->piscina), ctx->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (_monita(ctx, ps),
        "t:0:0: clausula nulla\n");
}


/* ==================================================
 * Toni rari
 * ================================================== */

interior vacuum
_toni_rari_probare (
    CredoContextus* ctx)
{
                   PsalmodiaTabula* t = _tabula_vera(ctx);
           constans PsalmodiaTonus* tonus;
        constans PsalmodiaClausula* term;
                  PsalmodiaPsalmus* ps;
                 ChordaAedificator* causa;

    CREDO_NECESSE (t != NIHIL);

    /* peregrinus: semiversus secundus versus primi intonatur (hixi),
     * tenor g; versus alter in g incipit */
    tonus = _tonus(t, "peregrinus", FALSUM);
    CREDO_NECESSE (tonus != NIHIL);
    ps = _psalmus_verus(ctx, tonus, _terminatio(tonus,
        chorda_ex_literis("", ctx->piscina)), FALSUM, FALSUM,
        "Laudáte púeri Dóminum: * laudáte nomen Dómini.\n"
        "Sit nomen Dómini benedíctum, * ex hoc nunc et usque in "
        "saéculum.\n");
    CREDO_CHORDA_AEQUALIS_LITERIS (_notae(ctx, ps, ZEPHYRUM),
        "hixi h h h ixi h g fr f * hixi g g g d f fr ed");
    CREDO_CHORDA_AEQUALIS_LITERIS (_notae(ctx, ps, I),
        "h h h h h h ixi h g f * g g g g g g d f fr ed");

    /* in directum: sine intonatione */
    tonus = _tonus(t, "in directum", FALSUM);
    CREDO_NECESSE (tonus != NIHIL);
    ps = _psalmus_verus(ctx, tonus, _terminatio(tonus,
        chorda_ex_literis("", ctx->piscina)), FALSUM, FALSUM,
        "Lauda ánima mea Dóminum: * laudábo Dóminum in vita mea.");
    CREDO_CHORDA_AEQUALIS_LITERIS (_notae(ctx, ps, ZEPHYRUM),
        "h h h h h g f h hr h * h h h h h h h h h h f");

    /* IV altera: clavis c3, tenor i, litterae suae */
    causa = chorda_aedificator_creare(ctx->piscina, LXIV);
    term = psalmodia_signum_invenire(t, chorda_ex_literis("4 A",
        ctx->piscina), &tonus, causa);
    CREDO_NECESSE (term != NIHIL && tonus != NIHIL);
    CREDO_VERUM (tonus->positio_altera);
    CREDO_CHORDA_AEQUALIS_LITERIS (tonus->clavis, "c3");
    ps = _psalmus_verus(ctx, tonus, term, FALSUM, FALSUM,
        "Dixit Dóminus Dómino meo: * Sede a dextris meis.");
    CREDO_CHORDA_AEQUALIS_LITERIS (_notae(ctx, ps, ZEPHYRUM),
        "i hi i i i i h i j i * i i h i j h f");

    /* VI antiqua: mediatio ut in I */
    tonus = _tonus(t, "6", FALSUM);
    CREDO_NECESSE (tonus != NIHIL);
    term = _terminatio(tonus, chorda_ex_literis("F", ctx->piscina));
    ps = _psalmus_verus(ctx, tonus, term, VERUM, FALSUM,
        "Credídi propter quod locútus sum: * ego autem humiliátus "
        "sum nimis.");
    CREDO_CHORDA_AEQUALIS_LITERIS (_notae(ctx, ps, ZEPHYRUM),
        "f gh h h h ixi h g hr h * h h h h h h h h f gh g f");
    ps = _psalmus_verus(ctx, tonus, term, FALSUM, FALSUM,
        "Credídi propter quod locútus sum: * ego autem humiliátus "
        "sum nimis.");
    CREDO_CHORDA_AEQUALIS_LITERIS (_notae(ctx, ps, ZEPHYRUM),
        "f gh h h h h h h g h * h h h h h h h h f gh g f");

    /* II A: clavis terminationis c3 */
    term = psalmodia_signum_invenire(t, chorda_ex_literis("2 A",
        ctx->piscina), &tonus, causa);
    CREDO_NECESSE (term != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (term->clavis, "c3");

    /* canticum: intonatio in omni versu */
    tonus = _tonus(t, "1", FALSUM);
    CREDO_NECESSE (tonus != NIHIL);
    term = _terminatio(tonus, chorda_ex_literis("g", ctx->piscina));
    ps = _psalmus_verus(ctx, tonus, term, FALSUM, VERUM,
        "Dixit Dóminus Dómino meo: * sede a dextris meis.\n"
        "Donec ponam inimícos tuos, * scabéllum pedum tuórum.\n");
    CREDO_NECESSE (ps != NIHIL && ps->numerus_versuum == II);
    CREDO_CHORDA_AEQUALIS_LITERIS (_notae(ctx, ps, ZEPHYRUM),
        "f gh h h h ixi hr h g h * h h h g f gh g");
    CREDO_CHORDA_AEQUALIS_LITERIS (_notae(ctx, ps, I),
        "f gh h h h h ixi h g h * h h h h g f gh g");
    ps = _psalmus_verus(ctx, tonus, term, FALSUM, FALSUM,
        "Dixit Dóminus Dómino meo: * sede a dextris meis.\n"
        "Donec ponam inimícos tuos, * scabéllum pedum tuórum.\n");
    CREDO_CHORDA_AEQUALIS_LITERIS (_notae(ctx, ps, I),
        "h h h h h h ixi h g h * h h h h g f gh g");
}


/* ==================================================
 * Scriptura gabc et reditus (T6)
 * ================================================== */

/* psalmus -> gabc */
interior chorda
_gabc (
                       Piscina* piscina,
     constans PsalmodiaPsalmus* psalmus,
    constans PsalmodiaOptiones* optiones)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina, MXXIV);

    (vacuum)psalmodia_gabc_scribere(psalmus, optiones, a);
    redde chorda_aedificator_finire(a);
}

/* syllaba lecta habet divisionem generis dati */
interior b32
_divisio_inest (
    constans GabcSyllaba* g,
             GabcDivisio  divisio)
{
    i32 k;

    per (k = ZEPHYRUM; k < g->numerus_elementorum; k++)
    {
        si (   g->elementa[k].genus   == GABC_EL_DIVISIO
            && g->elementa[k].divisio == divisio)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* exitus gabc per gabc_legere relegitur: vitium nullum; syllabae =
 * clavis + nostrae + divisio finalis per versum; textus, initium
 * verbi, stilus et divisiones ubi nostra sunt. Causa prima
 * imprimitur. */
interior b32
_reditus (
                      Piscina* piscina,
    constans PsalmodiaPsalmus* psalmus)
{
                chorda  gabc         = _gabc(piscina, psalmus, NIHIL);
            GabcCantus* cantus       = gabc_legere(gabc, piscina);
                   i32  exspectatae  = ZEPHYRUM;
                   i32  numerus      = ZEPHYRUM;
                   i32  k            = I;
                   i32  v;
                   i32  i;
                   i32  j;

    si (cantus == NIHIL)
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < cantus->numerus_vitiorum; j++)
    {
        si (cantus->vitia[j].gravitas == GABC_VITIUM)
        {
            imprimere("  reditus: vitium %.*s\n%.*s\n",
                (integer)cantus->vitia[j].causa.mensura,
                (character*)cantus->vitia[j].causa.datum,
                (integer)gabc.mensura, (character*)gabc.datum);
            redde FALSUM;
        }
    }
    si (psalmus->tonus == NIHIL)
    {
        redde cantus->numerus_syllabarum == ZEPHYRUM;
    }
    exspectatae = I;
    per (v = ZEPHYRUM; v < psalmus->numerus_versuum; v++)
    {
        exspectatae += psalmus->versus[v].numerus + I;
    }
    si (   cantus->numerus_syllabarum                     != exspectatae
        || cantus->syllabae[ZEPHYRUM].numerus_elementorum != I
        || cantus->syllabae[ZEPHYRUM].elementa[ZEPHYRUM].genus
           != GABC_EL_CLAVIS)
    {
        imprimere("  reditus: syllabae %u pro %u\n%.*s\n",
            cantus->numerus_syllabarum, exspectatae,
            (integer)gabc.mensura, (character*)gabc.datum);
        redde FALSUM;
    }
    per (v = ZEPHYRUM; v < psalmus->numerus_versuum; v++)
    {
        constans PsalmodiaVersus* versus = &psalmus->versus[v];

        si (!versus->gloria)
        {
            numerus++;
        }
        per (i = ZEPHYRUM; i < versus->numerus; i++)
        {
             constans PsalmodiaSyllaba* s = &versus->syllabae[i];
                  constans GabcSyllaba* g = &cantus->syllabae[k++];
                     ChordaAedificator* a;
                                chorda  lectus;
                                chorda  noster;
                                   i32  stilus     = ZEPHYRUM;
                                   i32  exspectatus;
                                   b32  bonum;

            a = chorda_aedificator_creare(piscina, LXIV);
            per (j = ZEPHYRUM; j < g->numerus_textuum; j++)
            {
                si (g->textus[j].genus == GABC_TEXTUS_CANTATUS)
                {
                    chorda_aedificator_appendere_chorda(a,
                        g->textus[j].textus);
                    stilus = g->textus[j].stilus;
                }
            }
            lectus  = chorda_aedificator_finire(a);
            a       = chorda_aedificator_creare(piscina, LXIV);
            si (i == ZEPHYRUM && !versus->gloria)
            {
                chorda_aedificator_appendere_i32(a, numerus);
                chorda_aedificator_appendere_literis(a, ". ");
            }
            chorda_aedificator_appendere_chorda(a, s->textus);
            noster       = chorda_aedificator_finire(a);
            exspectatus  = s->stilus == PSALMODIA_STILUS_ACCENTUS
                ? GABC_STILUS_CRASSUS : s->stilus
                == PSALMODIA_STILUS_PRAEPARATIO ? GABC_STILUS_ITALICUS
                : ZEPHYRUM;
            bonum = chorda_aequalis(lectus, noster)
                && g->verbum_incipit == (i == ZEPHYRUM ? VERUM
                : s->verbum_incipit)
                && (stilus & (GABC_STILUS_CRASSUS
                    | GABC_STILUS_ITALICUS))
                == exspectatus;
            si (bonum && _chorda_literis(s->textus, "*"))
            {
                bonum = _divisio_inest(g, GABC_DIVISIO_MAIOR);
            }
            alioquin si (bonum && _chorda_literis(s->textus, "†"))
            {
                bonum = _divisio_inest(g, GABC_DIVISIO_MINIMA);
            }
            si (!bonum)
            {
                imprimere("  reditus: versus %u syllaba %u \"%.*s\""
                    " lecta \"%.*s\"\n%.*s\n", v, i,
                    (integer)noster.mensura, (character*)noster.datum,
                    (integer)lectus.mensura, (character*)lectus.datum,
                    (integer)gabc.mensura, (character*)gabc.datum);
                redde FALSUM;
            }
        }
        si (   cantus->syllabae[k].numerus_textuum != ZEPHYRUM
            || !_divisio_inest(&cantus->syllabae[k],
                GABC_DIVISIO_FINALIS))
        {
            imprimere("  reditus: versus %u sine (::)\n", v);
            redde FALSUM;
        }
        k++;
    }
    redde VERUM;
}

#define PSALMUS_LAUDATE \
    "Laudáte Dóminum, omnes gentes: * laudáte eum, " \
    "omnes pópuli.\n" \
    "Quóniam confirmáta est super nos misericórdia " \
    "ejus: * et véritas Dómini manet in ætérnum.\n"

#define GABC_LAUDATE_VERSUS \
    "(c4) 1. Lau(g)dá(h)te(j) Dó(j)mi(j)num,(j) " \
    "o(j)mnes(j) <b>gen</b>(k)tes:(j) *(:) " \
    "lau(j)dá(j)te(j) e(j)um,(j) <i>o</i>(i)" \
    "<i>mnes</i>(j) <b>pó</b>(h)pu(gr)li.(g) (::)\n" \
    "2. Quó(j)ni(j)am(j) con(j)fir(j)má(j)ta(j) " \
    "est(j) su(j)per(j) nos(j) mi(j)se(j)ri(j)" \
    "cór(j)di(j)a(j) <b>e</b>(k)jus:(j) *(:) " \
    "et(j) vé(j)ri(j)tas(j) Dó(j)mi(j)ni(j) " \
    "ma(j)net(j) <i>in</i>(i) <i>æ</i>(j)" \
    "<b>tér</b>(h)num.(g) (::)\n"

#define GABC_GLORIA_VIII_G \
    "Gló(j)ri(j)a(j) Pa(j)tri,(j) et(j) " \
    "<b>Fí</b>(k)li(jr)o,(j) *(:) et(j) " \
    "Spi(j)rí(j)<i>tu</i>(i)<i>i</i>(j) " \
    "<b>San</b>(h)cto.(g) (::)\n" \
    "Sic(j)ut(j) e(j)rat(j) in(j) " \
    "prin(j)cí(j)pi(j)o,(j) et(j) nunc,(j) " \
    "et(j) <b>sem</b>(k)per,(j) *(:) et(j) " \
    "in(j) sǽ(j)cu(j)la(j) sæ(j)cu(j)" \
    "<i>ló</i>(i)<i>rum.</i>(j) <b>A</b>(h)men.(g) " \
    "(::)\n"

interior vacuum
_scriptura_probare (
    CredoContextus* ctx)
{
               PsalmodiaTabula* t = _tabula_vera(ctx);
       constans PsalmodiaTonus* tonus;
    constans PsalmodiaClausula* term;
             PsalmodiaOptiones  optiones;
             ChordaAedificator* causa = chorda_aedificator_creare(
                                 ctx->piscina, LXIV);
           PsalmodiaPsalmus* ps;
                     chorda  textus = chorda_ex_literis(PSALMUS_LAUDATE,
                                 ctx->piscina);

    CREDO_NECESSE (t != NIHIL);
    term = psalmodia_signum_invenire(t, chorda_ex_literis("8 G",
        ctx->piscina), &tonus, causa);
    CREDO_NECESSE (term != NIHIL && tonus != NIHIL);

    /* Ps 116, VIII G, cum Gloria (ordinariae) */
    ps = psalmodia_aptare(tonus, term, textus, NIHIL, ctx->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (_monita(ctx, ps), "");
    CREDO_CHORDA_AEQUALIS_LITERIS (_gabc(ctx->piscina, ps, NIHIL),
        "mode: 8;\n"
        "annotation: 8 G;\n"
        "%%\n"
        GABC_LAUDATE_VERSUS
        GABC_GLORIA_VIII_G);

    /* sine Gloria; titulus (';' et lineae novae in titulo spatia) */
    memset(&optiones, ZEPHYRUM, magnitudo(optiones));
    optiones.sine_gloria  = VERUM;
    optiones.titulus      = chorda_ex_literis("Ps. 116; Laudáte\n",
        ctx->piscina);
    ps = psalmodia_aptare(tonus, term, textus, &optiones, ctx->piscina);
    CREDO_AEQUALIS_I32 (ps->numerus_versuum, II);
    CREDO_CHORDA_AEQUALIS_LITERIS (_gabc(ctx->piscina, ps, &optiones),
        "name: Ps. 116  Laudáte ;\n"
        "mode: 8;\n"
        "annotation: 8 G;\n"
        "%%\n"
        GABC_LAUDATE_VERSUS);

    /* flexa: †(,); clavis terminationis (II A: c3) */
    term = psalmodia_signum_invenire(t, chorda_ex_literis("2 A",
        ctx->piscina), &tonus, causa);
    CREDO_NECESSE (term != NIHIL && tonus != NIHIL);
    ps = psalmodia_aptare(tonus, term, chorda_ex_literis(
        "Laudáte Dóminum, † omnes * gentes.", ctx->piscina),
        &optiones,
        ctx->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (_gabc(ctx->piscina, ps, NIHIL),
        "mode: 2;\n"
        "annotation: 2 A;\n"
        "%%\n"
        "(c3) 1. Lau(e)dá(f)te(h) Dó(h)mi(f)num,(f) †(,) "
        "<b>o</b>(i)mnes(h) *(:) <b>gen</b>(e)tes.(f) (::)\n");

    /* in directum: modus sine differentia */
    tonus = _tonus(t, "in directum", FALSUM);
    CREDO_NECESSE (tonus != NIHIL);
    ps = _psalmus_verus(ctx, tonus, _terminatio(tonus,
        chorda_ex_literis("", ctx->piscina)), FALSUM, FALSUM,
        "Deus, * in adjutórium.");
    CREDO_CHORDA_AEQUALIS_LITERIS (_gabc(ctx->piscina, ps, NIHIL),
        "mode: in directum;\n"
        "annotation: in directum;\n"
        "%%\n"
        "(c3) 1. <b>De</b>(h)us,(h) *(:) in(h) ad(h)ju(h)<b>tó</b>(h)"
        "ri(fr)um.(f) (::)\n");

    /* tonus nullus: caput solum */
    ps = psalmodia_aptare(NIHIL, term, textus, NIHIL, ctx->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (_gabc(ctx->piscina, ps, NIHIL),
        "%%\n");
    CREDO_FALSUM (psalmodia_gabc_scribere(NIHIL, NIHIL,
        chorda_aedificator_creare(ctx->piscina, VIII)));
}

/* exempla libri omnia et Ps 116 per gabc_legere relecta */
interior vacuum
_reditum_probare (
    CredoContextus* ctx)
{
               PsalmodiaTabula* t = _tabula_vera(ctx);
       constans PsalmodiaTonus* tonus;
    constans PsalmodiaClausula* term;
             ChordaAedificator* causa = chorda_aedificator_creare(
                                 ctx->piscina, LXIV);
                        i32 relecta = ZEPHYRUM;
                        i32 k;
                        i32 e;

    CREDO_NECESSE (t != NIHIL);
    per (k = ZEPHYRUM; k < t->numerus_tonorum; k++)
    {
        per (e = ZEPHYRUM; e < t->toni[k].numerus_exemplorum; e++)
        {
            si (_reditus(ctx->piscina, _exemplum_psalmus(ctx,
                &t->toni[k], &t->toni[k].exempla[e])))
            {
                relecta++;
            }
        }
    }
    CREDO_AEQUALIS_I32 (relecta, (i32)CCXLVI);

    term = psalmodia_signum_invenire(t, chorda_ex_literis("8 G",
        ctx->piscina), &tonus, causa);
    CREDO_NECESSE (term != NIHIL);
    CREDO_VERUM (_reditus(ctx->piscina, psalmodia_aptare(tonus, term,
        chorda_ex_literis(PSALMUS_LAUDATE, ctx->piscina), NIHIL,
        ctx->piscina)));
    /* signa sine vocali (« — ») syllabis adhaerentia relegi
     * possunt */
    CREDO_VERUM (_reditus(ctx->piscina, psalmodia_aptare(tonus, term,
        chorda_ex_literis("« Dóminus — regit * me. »\n",
        ctx->piscina),
        NIHIL, ctx->piscina)));
    /* tonus nullus */
    CREDO_VERUM (_reditus(ctx->piscina, psalmodia_aptare(NIHIL, term,
        chorda_ex_literis(PSALMUS_LAUDATE, ctx->piscina), NIHIL,
        ctx->piscina)));
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
                si (   strchr("()[]{}<>|%$~",
                    (integer)s->textus.datum[k])
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

/* textus omnibus tonis veris (terminatio communis prima; canticum;
 * partes omnium finium) -> VERUM si omnes psalmi validi */
interior b32
_vere_aptare (
                     Piscina* piscina,
    constans PsalmodiaTabula* tabula,
                      chorda  textus)
{
    PsalmodiaOptiones canticum;
                  b32 validi = VERUM;
                  i32 k;
                  i32 j;

    memset(&canticum, ZEPHYRUM, magnitudo(canticum));
    canticum.canticum = VERUM;
    per (k = ZEPHYRUM; tabula != NIHIL && k < tabula->numerus_tonorum;
        k++)
    {
             constans PsalmodiaTonus* tonus  = &tabula->toni[k];
          constans PsalmodiaClausula* term   = tonus->terminationes;
          constans PsalmodiaClausula* med   =
              psalmodia_mediationem_eligere(tonus, FALSUM);

        PsalmodiaPsalmus* ps = psalmodia_aptare(tonus, term, textus,
                              NIHIL, piscina);

        validi = validi && _validus(ps) && _reditus(piscina, ps)
            && _validus(psalmodia_aptare(tonus, term, textus, &canticum,
                piscina));
        per (j = ZEPHYRUM; j < (i32)IV; j++)
        {
            validi = validi && _validus(psalmodia_partem_aptare(tonus,
                j == PSALMODIA_FINIS_TERMINATIO ? term : med,
                (PsalmodiaFinis)j, (j % II) == ZEPHYRUM, textus,
                piscina));
        }
    }
    redde validi;
}

/* _vere_aptare in piscina propria (memoria post quemque textum
 * redditur) */
interior b32
_vere_aptare_seorsum (
    constans PsalmodiaTabula* tabula,
                      chorda  textus)
{
    Piscina* p = piscina_generare_dynamicum("psalmodia_vere",
                 (memoriae_index)(CXXVIII * MXXIV));
         b32 validi;

    si (p == NIHIL)
    {
        redde FALSUM;
    }
    validi = _vere_aptare(p, tabula, textus);
    piscina_destruere(p);
    redde validi;
}

/* in filio sub CREDO_NON_RUIT */
interior vacuum
_omnia_aptare (
                 constans i8* octeti,
                         i32  mensura,
    constans PsalmodiaTabula* tabula)
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
    (vacuum)_vere_aptare(p, tabula, textus);
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
                  i8  temere[MMMCMXCIX];
                  i8  copia[CCLVI];
                Sors  sors;
         TonusFictus  fictus = _tonum_fingere(ctx->piscina);
     PsalmodiaTabula* tabula = _tabula_vera(ctx);
              chorda  textus;
                 i32  n;
                 i32  k;
                 i32  r;

    /* textus bonus truncatus in omni octeto */
    n = (i32)strlen(bonus);
    CREDO_NECESSE (n < (i32)CCLVI);
    memcpy(copia, bonus, (size_t)n);
    per (k = ZEPHYRUM; k <= n; k++)
    {
        CREDO_NON_RUIT (_omnia_aptare(copia, k, tabula));
        textus.datum    = copia;
        textus.mensura  = k;
        CREDO_VERUM (_validus(psalmodia_aptare(fictus.tonus,
            fictus.terminatio, textus, NIHIL, ctx->piscina)));
        CREDO_VERUM (_vere_aptare_seorsum(tabula, textus));
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
        CREDO_NON_RUIT (_omnia_aptare(temere, n, tabula));
        textus.datum    = temere;
        textus.mensura  = n;
        CREDO_VERUM (_validus(psalmodia_aptare(fictus.tonus,
            fictus.terminatio, textus, NIHIL, ctx->piscina)));
        CREDO_VERUM (_vere_aptare_seorsum(tabula, textus));
    }
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "textus: lineae", _lineae_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "textus: signa", _signa_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "textus: verba et voces hebraicae", _verba_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "Gloria Patri; tonus absens", _gloria_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "exempla libri (tabula vera)", _exempla_libri_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "textus breves", _breves_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "toni rari", _toni_rari_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "scriptura gabc", _scriptura_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "reditus per gabc_legere", _reditum_probare,
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
