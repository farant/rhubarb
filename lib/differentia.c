/* differentia.c - corpus differentiae (doctrina in differentia.h)
 *
 * MYERS O(ND) CUM VESTIGIO FENESTRATO: gyrus d fenestram
 * [-d-1..d+1] ordinis V photographat ANTE se (status post gyrum
 * d-1) - retrogradatio easdem decisiones repetit quas cursus fecit.
 * Memoria vestigii ~ (D+2)^2 * IV bytes; praefixum/suffixum
 * communia ANTE Myers tonduntur, ergo D regionem mutatam solam
 * spectat, non plagulam totam.
 *
 * TECTUM PRAGMATICUM (D > MMXLVIII): scriptum triviale (DELETA
 * omnia + ADDITA omnia) pro medio - differentia tam magna homini
 * illegibilis esset et memoria vestigii quadratice cresceret.
 * Invarians partitionis etiam in casu triviali servatur.
 *
 * EMISSOR: gradus singulos in tractus fundit; in massa mutationis
 * deletiones ANTE additiones semper (forma canonica GNU) - via
 * Myersiana librare potest (-,+,-,+), emissor pendentia colligit
 * et ad IDEM proximum ordine canonico fundit.
 *
 * IDENTITAS LINEARUM: internatio super spatium '\n'-INCLUSIVUM
 * ("abc\n" et "abc" finalis sine '\n' = lineae DIVERSAE - byte
 * ultimus honestatur), sed lineae MONSTRATAE sine '\n' conduntur.
 */

#include "differentia.h"
#include "internamentum.h"
#include "chorda_aedificator.h"

#define DIFFERENTIA_TECTUM_D 2048


/* ==================================================
 * Emissor tractuum
 * ================================================== */

nomen structura {
    Xar* tractus;
    s32  pos_a;
    s32  pos_b;
    s32  deleta_pendentia;
    s32  addita_pendentia;
    b32  fractum;   /* memoria defecit */
} EmissorTractuum;

interior vacuum
_tractum_appendere (
     EmissorTractuum* e,
    DifferentiaGenus  genus,
                 s32  index_a,
                 s32  index_b,
                 s32  numerus);

interior vacuum
_tractum_appendere (
     EmissorTractuum* e,
    DifferentiaGenus  genus,
                 s32  index_a,
                 s32  index_b,
                 s32  numerus)
{
    DifferentiaTractus* t;

    si (e->fractum || numerus <= 0)
    {
        redde;
    }
    t = (DifferentiaTractus*)xar_addere(e->tractus);
    si (t == NIHIL)
    {
        e->fractum = VERUM;
        redde;
    }
    t->genus    = genus;
    t->index_a  = (i32)index_a;
    t->index_b  = (i32)index_b;
    t->numerus  = (i32)numerus;
}

interior vacuum
_pendentia_fundere (
    EmissorTractuum* e);

interior vacuum
_pendentia_fundere (
    EmissorTractuum* e)
{
    si (e->deleta_pendentia > 0)
    {
        _tractum_appendere(e, DIFFERENTIA_DELETA, e->pos_a,
            e->pos_b, e->deleta_pendentia);
        e->pos_a             = e->pos_a + e->deleta_pendentia;
        e->deleta_pendentia  = 0;
    }
    si (e->addita_pendentia > 0)
    {
        _tractum_appendere(e, DIFFERENTIA_ADDITA, e->pos_a,
            e->pos_b, e->addita_pendentia);
        e->pos_b             = e->pos_b + e->addita_pendentia;
        e->addita_pendentia  = 0;
    }
}

interior vacuum
_idem_emittere (
    EmissorTractuum* e,
                s32  numerus);

interior vacuum
_idem_emittere (
    EmissorTractuum* e,
                s32  numerus)
{
    si (numerus <= 0)
    {
        redde;
    }
    _pendentia_fundere(e);
    {
        i32 n = xar_numerus(e->tractus);

        si (n > 0)
        {
            DifferentiaTractus* ultimus = (DifferentiaTractus*)
                xar_obtinere(e->tractus, n - 1);

            si (ultimus->genus == DIFFERENTIA_IDEM)
            {
                ultimus->numerus  = ultimus->numerus + (i32)numerus;
                e->pos_a          = e->pos_a + numerus;
                e->pos_b          = e->pos_b + numerus;
                redde;
            }
        }
    }
    _tractum_appendere(e, DIFFERENTIA_IDEM, e->pos_a, e->pos_b,
        numerus);
    e->pos_a = e->pos_a + numerus;
    e->pos_b = e->pos_b + numerus;
}

interior vacuum
_gradum_emittere (
     EmissorTractuum* e,
    DifferentiaGenus  genus);

interior vacuum
_gradum_emittere (
     EmissorTractuum* e,
    DifferentiaGenus  genus)
{
    si (genus == DIFFERENTIA_DELETA)
    {
        e->deleta_pendentia = e->deleta_pendentia + 1;
    }
    alioquin si (genus == DIFFERENTIA_ADDITA)
    {
        e->addita_pendentia = e->addita_pendentia + 1;
    }
    alioquin
    {
        _idem_emittere(e, 1);
    }
}


/* ==================================================
 * Myers O(ND)
 * ================================================== */

interior s32*
_s32_ordinem_allocare (
    Piscina* piscina,
        s32  numerus);

interior s32*
_s32_ordinem_allocare (
    Piscina* piscina,
        s32  numerus)
{
    redde (s32*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(s32));
}

/* VERUM = scriptum emissum; FALSUM = tectum percussum aut memoria
 * defecit - vocator triviale emittat */
interior b32
_myers_currere (
    Piscina* piscina,
    constans vacuum* constans* a,
    s32 na,
    constans vacuum* constans* b,
    s32 nb,
    EmissorTractuum* emissor);

interior b32
_myers_currere (
    Piscina* piscina,
    constans vacuum* constans* a,
    s32 na,
    constans vacuum* constans* b,
    s32 nb,
    EmissorTractuum* emissor)
{
    s32  maximus    = na + nb;
    s32  d_finalis  = -1;
    s32* v;
    Xar* vestigia;   /* de s32* - fenestra per gyrum */
    Xar* gradus;     /* de s32 (genus) - ordine retrogrado */
    s32  d;
    s32  k;
    s32  x;
    s32  y;
    s32  i;

    v = _s32_ordinem_allocare(piscina, 2 * maximus + 3);
    si (v == NIHIL)
    {
        redde FALSUM;
    }
    per (i = 0; i < 2 * maximus + 3; i = i + 1)
    {
        v[i] = 0;
    }
    vestigia  = xar_creare(piscina, (i32)magnitudo(s32*));
    gradus    = xar_creare(piscina, (i32)magnitudo(s32));
    si (vestigia == NIHIL || gradus == NIHIL)
    {
        redde FALSUM;
    }

    per (d = 0; d <= maximus && d <= DIFFERENTIA_TECTUM_D;
        d = d + 1)
    {
        /* fenestra [-d-1..d+1] ANTE gyrum d (status post d-1);
         * gyrus d legit solum V[k+-1] cum |k| <= d, ergo lectio
         * intra [-(d-1)..d-1] manet - fenestra latior tuta est */
        s32*  nix = _s32_ordinem_allocare(piscina, 2 * d + 3);
        s32** cella;

        si (nix == NIHIL)
        {
            redde FALSUM;
        }
        per (k = -d - 1; k <= d + 1; k = k + 1)
        {
            nix[k + d + 1] = v[k + maximus + 1];
        }
        cella = (s32**)xar_addere(vestigia);
        si (cella == NIHIL)
        {
            redde FALSUM;
        }
        *cella = nix;

        per (k = -d; k <= d; k = k + 2)
        {
            si (   k == -d || (k != d
                && v[(k - 1) + maximus + 1]
                    < v[(k + 1) + maximus + 1]))
            {
                x = v[(k + 1) + maximus + 1];
            }
            alioquin
            {
                x = v[(k - 1) + maximus + 1] + 1;
            }
            y = x - k;
            dum (x < na && y < nb && a[x] == b[y])
            {
                x = x + 1;
                y = y + 1;
            }
            v[k + maximus + 1] = x;
            si (x >= na && y >= nb)
            {
                d_finalis = d;
                salta inventum;
            }
        }
    }
    redde FALSUM;   /* tectum percussum */

inventum:
    /* retrogradatio: decisiones easdem repetit, gradus retrorsum
     * colligit */
    x = na;
    y = nb;
    per (d = d_finalis; d >= 0; d = d - 1)
    {
        s32* nix  = *(s32**)xar_obtinere_s(vestigia, d);
        s32  kk   = x - y;
        s32  prior_k;
        s32  prior_x;
        s32  prior_y;

        si (   kk == -d || (kk != d
            && nix[(kk - 1) + d + 1] < nix[(kk + 1) + d + 1]))
        {
            prior_k = kk + 1;
        }
        alioquin
        {
            prior_k = kk - 1;
        }
        prior_x = nix[prior_k + d + 1];
        prior_y = prior_x - prior_k;

        dum (x > prior_x && y > prior_y)
        {
            s32* g = (s32*)xar_addere(gradus);

            si (g == NIHIL)
            {
                redde FALSUM;
            }
            *g  = (s32)DIFFERENTIA_IDEM;
            x   = x - 1;
            y   = y - 1;
        }
        si (d > 0)
        {
            s32* g = (s32*)xar_addere(gradus);

            si (g == NIHIL)
            {
                redde FALSUM;
            }
            /* descensus (x immotus) = elementum e B sumptum */
            *g = x == prior_x ? (s32)DIFFERENTIA_ADDITA
                : (s32)DIFFERENTIA_DELETA;
            x = prior_x;
            y = prior_y;
        }
    }

    /* replicatio recto ordine per emissorem */
    per (i = (s32)xar_numerus(gradus) - 1; i >= 0; i = i - 1)
    {
        s32 g = *(s32*)xar_obtinere_s(gradus, i);

        _gradum_emittere(emissor, (DifferentiaGenus)g);
    }
    redde VERUM;
}

Xar*
differentia_seriei (
    Piscina* piscina,
    constans vacuum* constans* a,
    i32 numerus_a,
    constans vacuum* constans* b,
    i32 numerus_b)
{
    EmissorTractuum emissor;
                s32 na         = (s32)numerus_a;
                s32 nb         = (s32)numerus_b;
                s32 praefixum  = 0;
                s32 suffixum   = 0;

    si (   (numerus_a > 0 && a == NIHIL)
        || (numerus_b > 0 && b == NIHIL))
    {
        redde NIHIL;
    }
    emissor.tractus = xar_creare(piscina,
        (i32)magnitudo(DifferentiaTractus));
    si (emissor.tractus == NIHIL)
    {
        redde NIHIL;
    }
    emissor.pos_a             = 0;
    emissor.pos_b             = 0;
    emissor.deleta_pendentia  = 0;
    emissor.addita_pendentia  = 0;
    emissor.fractum           = FALSUM;

    dum (   praefixum < na && praefixum < nb
         && a[praefixum] == b[praefixum])
    {
        praefixum = praefixum + 1;
    }
    dum (   suffixum < na - praefixum && suffixum < nb - praefixum
         && a[na - 1 - suffixum] == b[nb - 1 - suffixum])
    {
        suffixum = suffixum + 1;
    }

    _idem_emittere(&emissor, praefixum);
    {
        s32 media_a = na - praefixum - suffixum;
        s32 media_b = nb - praefixum - suffixum;

        si (media_a > 0 && media_b > 0)
        {
            si (!_myers_currere(piscina, a + praefixum, media_a,
                b + praefixum, media_b, &emissor))
            {
                /* tectum aut memoria: scriptum triviale */
                emissor.deleta_pendentia =
                    emissor.deleta_pendentia + media_a;
                emissor.addita_pendentia =
                    emissor.addita_pendentia + media_b;
            }
        }
        alioquin
        {
            emissor.deleta_pendentia =
                emissor.deleta_pendentia + media_a;
            emissor.addita_pendentia =
                emissor.addita_pendentia + media_b;
        }
    }
    _idem_emittere(&emissor, suffixum);
    _pendentia_fundere(&emissor);

    si (emissor.fractum)
    {
        redde NIHIL;
    }
    redde emissor.tractus;
}


/* ==================================================
 * Involucrum linearum
 * ================================================== */

/* lineae = visus in textum datum (non copiae); identitates =
 * monstratores internati super spatia '\n'-inclusiva */
interior b32
_lineas_findere (
                 chorda  textus,
                    Xar* lineae,
                    Xar* identitates,
    InternamentumChorda* intern,
                    b32* sine_fine);

interior b32
_lineas_findere (
                 chorda  textus,
                    Xar* lineae,
                    Xar* identitates,
    InternamentumChorda* intern,
                    b32* sine_fine)
{
    i32 initium = 0;
    i32 i;

    *sine_fine = FALSUM;
    per (i = 0; i < textus.mensura; i = i + 1)
    {
        si (textus.datum[i] == '\n')
        {
            chorda visum = chorda_ex_buffer(
                textus.datum + initium, i - initium);
            chorda identitas = chorda_ex_buffer(
                textus.datum + initium, i - initium + 1);
            chorda* internata = chorda_internare(intern,
                identitas);
            chorda*  cella_lineae = (chorda*)xar_addere(lineae);
            vacuum** cella_identitatis = (vacuum**)xar_addere(
                identitates);

            si (   internata         == NIHIL || cella_lineae == NIHIL
                || cella_identitatis == NIHIL)
            {
                redde FALSUM;
            }
            *cella_lineae       = visum;
            *cella_identitatis  = (vacuum*)internata;
            initium             = i + 1;
        }
    }
    si (initium < textus.mensura)
    {
        chorda visum = chorda_ex_buffer(textus.datum + initium,
            textus.mensura - initium);
        chorda*  internata     = chorda_internare(intern, visum);
        chorda*  cella_lineae  = (chorda*)xar_addere(lineae);
        vacuum** cella_identitatis = (vacuum**)xar_addere(
            identitates);

        si (   internata         == NIHIL || cella_lineae == NIHIL
            || cella_identitatis == NIHIL)
        {
            redde FALSUM;
        }
        *cella_lineae       = visum;
        *cella_identitatis  = (vacuum*)internata;
        *sine_fine          = VERUM;
    }
    redde VERUM;
}

interior vacuum**
_ordinem_identitatum (
         Piscina* piscina,
    constans Xar* identitates);

interior vacuum**
_ordinem_identitatum (
         Piscina* piscina,
    constans Xar* identitates)
{
       i32   numerus = xar_numerus(identitates);
    vacuum** ordo = (vacuum**)piscina_allocare(piscina,
        (memoriae_index)(numerus == 0 ? I : numerus)
            * magnitudo(vacuum*));
    i32 i;

    si (ordo == NIHIL)
    {
        redde NIHIL;
    }
    per (i = 0; i < numerus; i = i + 1)
    {
        ordo[i] = *(vacuum**)xar_obtinere(identitates, i);
    }
    redde ordo;
}

DifferentiaLinearum*
differentia_linearum (
    Piscina* piscina,
     chorda  a,
     chorda  b)
{
    DifferentiaLinearum*  d;
    InternamentumChorda*  intern;
                    Xar*  identitates_a;
                    Xar*  identitates_b;
                 vacuum** ordo_a;
                 vacuum** ordo_b;

    d = (DifferentiaLinearum*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(DifferentiaLinearum));
    intern = internamentum_creare(piscina);
    si (d == NIHIL || intern == NIHIL)
    {
        redde NIHIL;
    }
    d->lineae_a    = xar_creare(piscina, (i32)magnitudo(chorda));
    d->lineae_b    = xar_creare(piscina, (i32)magnitudo(chorda));
    identitates_a  = xar_creare(piscina, (i32)magnitudo(vacuum*));
    identitates_b  = xar_creare(piscina, (i32)magnitudo(vacuum*));
    si (   d->lineae_a   == NIHIL || d->lineae_b == NIHIL
        || identitates_a == NIHIL || identitates_b == NIHIL)
    {
        redde NIHIL;
    }
    si (   !_lineas_findere(a, d->lineae_a, identitates_a, intern,
            &d->a_sine_fine)
        || !_lineas_findere(b, d->lineae_b, identitates_b, intern,
            &d->b_sine_fine))
    {
        redde NIHIL;
    }
    ordo_a = _ordinem_identitatum(piscina, identitates_a);
    ordo_b = _ordinem_identitatum(piscina, identitates_b);
    si (ordo_a == NIHIL || ordo_b == NIHIL)
    {
        redde NIHIL;
    }
    d->tractus = differentia_seriei(piscina,
        (constans vacuum* constans*)ordo_a,
        xar_numerus(identitates_a),
        (constans vacuum* constans*)ordo_b,
        xar_numerus(identitates_b));
    si (d->tractus == NIHIL)
    {
        redde NIHIL;
    }
    redde d;
}


/* ==================================================
 * Emissio et summa
 * ================================================== */

DifferentiaSumma
differentia_summa (
    constans Xar* tractus)
{
    DifferentiaSumma s;
                 i32 i;

    s.additae = 0;
    s.deletae = 0;
    si (tractus == NIHIL)
    {
        redde s;
    }
    per (i = 0; i < xar_numerus(tractus); i = i + 1)
    {
        constans DifferentiaTractus* t =
            (constans DifferentiaTractus*)xar_obtinere(tractus, i);

        si (t->genus == DIFFERENTIA_ADDITA)
        {
            s.additae = s.additae + t->numerus;
        }
        alioquin si (t->genus == DIFFERENTIA_DELETA)
        {
            s.deletae = s.deletae + t->numerus;
        }
    }
    redde s;
}

interior chorda
_lineam_capere (
    constans Xar* lineae,
             s32  index);

interior chorda
_lineam_capere (
    constans Xar* lineae,
             s32  index)
{
    redde *(chorda*)xar_obtinere_s(lineae, index);
}

interior vacuum
_lineam_scribere (
    ChordaAedificator* aed,
            character  praefixum,
               chorda  linea,
                  b32  ultima,
                  b32  sine_fine);

interior vacuum
_lineam_scribere (
    ChordaAedificator* aed,
            character  praefixum,
               chorda  linea,
                  b32  ultima,
                  b32  sine_fine)
{
    chorda_aedificator_appendere_character(aed, praefixum);
    chorda_aedificator_appendere_chorda(aed, linea);
    chorda_aedificator_appendere_character(aed, '\n');
    si (ultima && sine_fine)
    {
        chorda_aedificator_appendere_literis(aed,
            "\\ sine linea nova in fine\n");
    }
}

/* numerus == 1: ",1" omittitur (mos GNU); numerus == 0: initium
 * 0-basatum ipsum (positio ANTE quam inseritur) */
interior vacuum
_latus_capitis_scribere (
    ChordaAedificator* aed,
                  s32  initium,
                  s32  numerus);

interior vacuum
_latus_capitis_scribere (
    ChordaAedificator* aed,
                  s32  initium,
                  s32  numerus)
{
    chorda_aedificator_appendere_s32(aed,
        numerus == 0 ? initium : initium + 1);
    si (numerus != 1)
    {
        chorda_aedificator_appendere_character(aed, ',');
        chorda_aedificator_appendere_s32(aed, numerus);
    }
}

chorda
differentia_unificata (
                         Piscina* piscina,
    constans DifferentiaLinearum* d,
              constans character* titulus_a,
              constans character* titulus_b,
                             i32  contextus)
{
    ChordaAedificator* aed;
                  i32  n;
                  i32  i;
                  s32  ctx = (s32)contextus;
                  s32  na_lineae;
                  s32  nb_lineae;

    si (d == NIHIL || d->tractus == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    {
        DifferentiaSumma s = differentia_summa(d->tractus);

        si (s.additae == 0 && s.deletae == 0)
        {
            redde chorda_ex_literis("", piscina);
        }
    }

    aed = chorda_aedificator_creare(piscina, (memoriae_index)256);
    si (aed == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    chorda_aedificator_appendere_literis(aed, "--- ");
    chorda_aedificator_appendere_literis(aed, titulus_a);
    chorda_aedificator_appendere_character(aed, '\n');
    chorda_aedificator_appendere_literis(aed, "+++ ");
    chorda_aedificator_appendere_literis(aed, titulus_b);
    chorda_aedificator_appendere_character(aed, '\n');

    n          = xar_numerus(d->tractus);
    na_lineae  = (s32)xar_numerus(d->lineae_a);
    nb_lineae  = (s32)xar_numerus(d->lineae_b);

    i = 0;
    dum (i < n)
    {
        constans DifferentiaTractus* primus =
            (constans DifferentiaTractus*)xar_obtinere(d->tractus,
                i);
        i32 finis;
        i32 j;
        s32 prae = 0;
        s32 post = 0;
        s32 initium_a;
        s32 initium_b;
        s32 summa_a;
        s32 summa_b;
        s32 l;

        si (primus->genus == DIFFERENTIA_IDEM)
        {
            i = i + 1;
            perge;
        }

        /* massa [i..finis]: mutationes quarum hiatus IDEM
         * <= 2*contextus coalescunt */
        finis  = i;
        j      = i + 1;
        dum (j < n)
        {
            constans DifferentiaTractus* u =
                (constans DifferentiaTractus*)xar_obtinere(
                    d->tractus, j);

            si (u->genus != DIFFERENTIA_IDEM)
            {
                finis  = j;
                j      = j + 1;
                perge;
            }
            si (j == n - 1 || (s32)u->numerus > 2 * ctx)
            {
                frange;
            }
            j = j + 1;
        }

        si (i > 0)
        {
            constans DifferentiaTractus* ante =
                (constans DifferentiaTractus*)xar_obtinere(
                    d->tractus, i - 1);

            prae = (s32)ante->numerus < ctx
                ? (s32)ante->numerus : ctx;
        }
        si (finis + 1 < n)
        {
            constans DifferentiaTractus* postea =
                (constans DifferentiaTractus*)xar_obtinere(
                    d->tractus, finis + 1);

            post = (s32)postea->numerus < ctx
                ? (s32)postea->numerus : ctx;
        }

        initium_a  = (s32)primus->index_a - prae;
        initium_b  = (s32)primus->index_b - prae;
        summa_a    = prae + post;
        summa_b    = prae + post;
        per (j = i; j <= finis; j = j + 1)
        {
            constans DifferentiaTractus* u =
                (constans DifferentiaTractus*)xar_obtinere(
                    d->tractus, j);

            si (u->genus == DIFFERENTIA_IDEM)
            {
                summa_a = summa_a + (s32)u->numerus;
                summa_b = summa_b + (s32)u->numerus;
            }
            alioquin si (u->genus == DIFFERENTIA_DELETA)
            {
                summa_a = summa_a + (s32)u->numerus;
            }
            alioquin
            {
                summa_b = summa_b + (s32)u->numerus;
            }
        }

        chorda_aedificator_appendere_literis(aed, "@@ -");
        _latus_capitis_scribere(aed, initium_a, summa_a);
        chorda_aedificator_appendere_literis(aed, " +");
        _latus_capitis_scribere(aed, initium_b, summa_b);
        chorda_aedificator_appendere_literis(aed, " @@\n");

        per (l = 0; l < prae; l = l + 1)
        {
            s32 index = initium_a + l;

            _lineam_scribere(aed, ' ',
                _lineam_capere(d->lineae_a, index),
                index == na_lineae - 1, d->a_sine_fine);
        }
        per (j = i; j <= finis; j = j + 1)
        {
            constans DifferentiaTractus* u =
                (constans DifferentiaTractus*)xar_obtinere(
                    d->tractus, j);

            si (u->genus == DIFFERENTIA_ADDITA)
            {
                per (l = 0; l < (s32)u->numerus; l = l + 1)
                {
                    s32 index = (s32)u->index_b + l;

                    _lineam_scribere(aed, '+',
                        _lineam_capere(d->lineae_b, index),
                        index == nb_lineae - 1, d->b_sine_fine);
                }
            }
            alioquin
            {
                character praefixum =
                    u->genus == DIFFERENTIA_IDEM ? ' ' : '-';

                per (l = 0; l < (s32)u->numerus; l = l + 1)
                {
                    s32 index = (s32)u->index_a + l;

                    _lineam_scribere(aed, praefixum,
                        _lineam_capere(d->lineae_a, index),
                        index == na_lineae - 1, d->a_sine_fine);
                }
            }
        }
        si (post > 0)
        {
            constans DifferentiaTractus* postea =
                (constans DifferentiaTractus*)xar_obtinere(
                    d->tractus, finis + 1);

            per (l = 0; l < post; l = l + 1)
            {
                s32 index = (s32)postea->index_a + l;

                _lineam_scribere(aed, ' ',
                    _lineam_capere(d->lineae_a, index),
                    index == na_lineae - 1, d->a_sine_fine);
            }
        }
        i = finis + 1;
    }
    redde chorda_aedificator_finire(aed);
}
