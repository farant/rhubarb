/* musicxml_carmen.c - conversor: exemplar MusicXML -> textus carminis
 * (caput include/musicxml_carmen.h; spec par. IV, plan P10)
 *
 * ITER: (1) notae ordine exsecutionis (musicxml_ordo) per lineas
 * carminis - linea = (pars, vox, gradus acordi) (M7) - ligaturis
 * iunctis; (2) tempus in pulsus (CMLX per quartam), initium et finis
 * separatim rotundata ne error cumuletur; mensura cuius terminus extra
 * reticulum est aut cuius intervallum MML dici non potest ad reticulum
 * LX pulsuum (sexagesima quarta) adducitur (approximatio); (3) mensurae
 * carminis = visitationes ordinis (mensura vacua praeteritur): metrum
 * quaeque mensura suum (M14), clavis, tempus, dynamica, fermatae; (4)
 * textus MML per lineam, longitudines per tabulam MML (programmatio
 * dynamica, pars maior prior), '&' trans lineas mensurae; (5) verba
 * per notam (purgata; '_' melisma).
 * Strictus: approximatio prima conversionem sistit, exitus intactus.
 */

#include "musicxml_carmen.h"
#include "xar.h"
#include <string.h>

#define PULSUS_QUARTAE   ((s64)CMLX)
#define PULSUS_INTEGRAE  ((s64)(IV * CMLX))
#define RETICULUM        ((s64)LX)


/* ==================================================
 * Conversor et relatio
 * ================================================== */

nomen structura {
                       Piscina* piscina;
         constans MusicxmlOpus* opus;
         constans MusicxmlOrdo* ordo;
    constans MusicxmlCarmenForma* forma;
                           Xar* approximationes;
                           Xar* structurae;
                           i32  signa[MUSICXML_CARMEN_SIGNORUM_GENERA];
                           b32  fractus;   /* strictus et approximatio */
                           b32  memoria_defecit;
} Conversor;

interior vacuum
_approximatio (
                 Conversor* cv,
 MusicxmlApproximatioGenus  genus,
                    chorda  pars,
                    chorda  mensura,
                       i32  transitus,
                       i32  linea,
                       i32  columna,
        constans character* descriptio)
{
    MusicxmlApproximatio* a;

    si (cv->fractus)
    {
        redde;
    }
    a = (MusicxmlApproximatio*)xar_addere(cv->approximationes);
    si (a == NIHIL)
    {
        cv->memoria_defecit = VERUM;
        redde;
    }
    a->genus       = genus;
    a->pars        = pars;
    a->mensura     = mensura;
    a->transitus   = transitus;
    a->linea       = linea;
    a->columna     = columna;
    a->descriptio  = chorda_ex_literis(descriptio, cv->piscina);
    si (cv->forma->strictus)
    {
        cv->fractus = VERUM;
    }
}

interior vacuum
_structura (
                 Conversor* cv,
        constans character* textus,
                    chorda  adiectum)
{
               chorda* s;
    ChordaAedificator* a;

    a = chorda_aedificator_creare(cv->piscina, CXXVIII);
    s = (chorda*)xar_addere(cv->structurae);
    si (a == NIHIL || s == NIHIL)
    {
        cv->memoria_defecit = VERUM;
        redde;
    }
    chorda_aedificator_appendere_literis(a, textus);
    chorda_aedificator_appendere_chorda(a, adiectum);
    *s = chorda_aedificator_finire(a);
}

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior vacuum
_s64_scribere (
    ChordaAedificator* a,
                  s64  n)
{
    character cifrae[XXIV];
          i32 k = ZEPHYRUM;

    si (n < ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, '-');
        n = -n;
    }
    fac
    {
        cifrae[k++]  = (character)('0' + (integer)(n % X));
        n            /= X;
    }
    dum (n > ZEPHYRUM && k < XXIII);
    dum (k > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, cifrae[--k]);
    }
}


/* ==================================================
 * Tabula longitudinum MML
 * ================================================== */

nomen structura {
          s64 pulsus;
    character textus[VIII];
} Longitudo;

hic_manens Longitudo longitudines[LXIV];
hic_manens i32       numerus_longitudinum = ZEPHYRUM;

/* divisores integrae MML (cantus.h) cum punctis 0..II; ordine pulsuum
 * descendente */
interior vacuum
_longitudines_parare (vacuum)
{
    constans i32 divisores[] = { I, II, III, IV, V, VI, VIII, X, XII,
        XV,
        XVI, XX, XXIV, XXX, XXXII, XL, XLVIII, LX, LXIV };
    i32 d;
    i32 p;
    i32 i;
    i32 j;

    si (numerus_longitudinum > ZEPHYRUM)
    {
        redde;
    }
    per (d = ZEPHYRUM; d < (i32)(magnitudo(divisores)
                             / magnitudo(divisores[ZEPHYRUM])); d++)
    {
        s64 basis = PULSUS_INTEGRAE / (s64)divisores[d];

        per (p = ZEPHYRUM; p <= II; p++)
        {
            s64 v = p == ZEPHYRUM ? basis
                  : p == I ? basis * III / II : basis * VII / IV;
            Longitudo* l;
                  i32  k = ZEPHYRUM;
                  i32  n = divisores[d];
            character  tmp[IV];
                  i32  t = ZEPHYRUM;

            si (   (p == I && (basis * III) % II != ZEPHYRUM)
                || (p == II && (basis * VII) % IV != ZEPHYRUM))
            {
                perge;
            }
            l          = &longitudines[numerus_longitudinum++];
            l->pulsus  = v;
            fac
            {
                tmp[t++]  = (character)('0' + (integer)(n % X));
                n         /= X;
            }
            dum (n > ZEPHYRUM);
            dum (t > ZEPHYRUM)
            {
                l->textus[k++] = tmp[--t];
            }
            per (t = ZEPHYRUM; t < p; t++)
            {
                l->textus[k++] = '.';
            }
            l->textus[k] = '\0';
        }
    }
    /* ordo descendens (insertio) */
    per (i = I; i < numerus_longitudinum; i++)
    {
        Longitudo x = longitudines[i];

        per (j = i; j > ZEPHYRUM
            && longitudines[j - I].pulsus < x.pulsus;
             j--)
        {
            longitudines[j] = longitudines[j - I];
        }
        longitudines[j] = x;
    }
}

/* d pulsus -> indices longitudinum (partes minimae, maior prior) in
 * partes[]; numerus redditur, 0 si dici non potest (d <= 0 aut nulla
 * summa) */
interior i32
_longitudinem_dividere (
    Piscina* piscina,
        s64  d,
        i32* partes,
        i32  maximum)
{
     i32* minimae;
     s32* ultima;
     s64  q;
     i32  k;
     i32  n = ZEPHYRUM;

    si (d <= ZEPHYRUM || d % IV != ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    /* avare primum (fere semper sufficit) */
    {
        s64 r = d;

        per (k = ZEPHYRUM; k < numerus_longitudinum
            && r > ZEPHYRUM; k++)
        {
            dum (r >= longitudines[k].pulsus && n < maximum)
            {
                partes[n++]  = k;
                r            -= longitudines[k].pulsus;
            }
        }
        si (r == ZEPHYRUM)
        {
            redde n;
        }
    }
    /* programmatio dynamica in quartis pulsuum (omnes longitudines
     * multiplices IV) */
    q = d / IV;
    minimae = (i32*)piscina_allocare(piscina,
        (memoriae_index)((q + I) * (s64)magnitudo(i32)));
    ultima = (s32*)piscina_allocare(piscina,
        (memoriae_index)((q + I) * (s64)magnitudo(s32)));
    si (minimae == NIHIL || ultima == NIHIL)
    {
        redde ZEPHYRUM;
    }
    {
        s64 x;

        minimae[ZEPHYRUM] = ZEPHYRUM;
        per (x = I; x <= q; x++)
        {
            minimae[x]  = (i32)M * M;
            ultima[x]   = -I;
            per (k = ZEPHYRUM; k < numerus_longitudinum; k++)
            {
                s64 l = longitudines[k].pulsus / IV;

                si (   l <= x && minimae[x - l] + I < minimae[x]
                    && minimae[x - l] < (i32)M * M)
                {
                    minimae[x]  = minimae[x - l] + I;
                    ultima[x]   = (s32)k;
                }
            }
        }
        si (ultima[q] < ZEPHYRUM && q > ZEPHYRUM)
        {
            redde ZEPHYRUM;
        }
        n = ZEPHYRUM;
        x = q;
        dum (x > ZEPHYRUM && n < maximum)
        {
            partes[n++]  = (i32)ultima[x];
            x            -= longitudines[ultima[x]].pulsus / IV;
        }
    }
    /* maior prior */
    per (k = I; k < n; k++)
    {
        i32 v = partes[k];
        i32 j = k;

        dum (   j > ZEPHYRUM
             && longitudines[partes[j - I]].pulsus
                < longitudines[v].pulsus)
        {
            partes[j] = partes[j - I];
            j--;
        }
        partes[j] = v;
    }
    redde n;
}


/* ==================================================
 * Eventus et lineae
 * ================================================== */

nomen structura {
                    s64  initium;   /* pulsus absoluti */
                    s64  finis;
                    s32  nota;      /* MIDI sonans */
              character  littera;   /* MML 'a'..'g' */
                    s32  signum;    /* -I, 0, I */
                    s32  octava;    /* MML */
                    b32  aperta;    /* ligatura pendens */
                 chorda  syllaba;   /* purgata; vacua = melisma */
                    b32  divisio;   /* post syllabam '-' */
                    i32  visitatio;
                    b32  inexactus; /* terminus extra reticulum CMLX */
                    b32  notatus;   /* approximatio reticuli iam scripta */
    constans MusicxmlNota* fons;
} Eventus;

nomen structura {
       i32  pars;          /* index partis MusicXML */
    chorda  vox;
       i32  gradus;        /* gradus acordi (0 = nota prima) */
       Xar* eventus;       /* Eventus, ordine initii */
    chorda  titulus;
    chorda  instrumentum;
} Linea;

/* gradus diatonicus litterae; -I si non littera */
interior s32
_gradus (
    character littera)
{
    commutatio (littera)
    {
        casus 'C': casus 'c': redde ZEPHYRUM;
        casus 'D': casus 'd': redde II;
        casus 'E': casus 'e': redde IV;
        casus 'F': casus 'f': redde V;
        casus 'G': casus 'g': redde VII;
        casus 'A': casus 'a': redde IX;
        casus 'B': casus 'b': redde XI;
        ordinarius: redde -I;
    }
}

/* orthographia MML ex nota scripta sonante; alteratio duplex aut fracta
 * ex MIDI (sursum: diesis, deorsum: bemollis) */
interior vacuum
_orthographiam_ponere (
                Conversor* cv,
    constans MusicxmlNota* n,
                  Eventus* e,
                      b32* scriptura_mutata)
{
    constans character* acuti   = "ccddeffggaab";
          constans s32  acuti_s[XII]  = { 0, 1, 0, 1, 0, 0, 1, 0,
              1, 0,
        1,
                                          0 };
    constans character* graves  = "cddeefggaabb";
          constans s32  graves_s[XII] = { 0, -1, 0, -1, 0, 0, -1,
              0, -1,
                                          0, -1, 0 };
    f64 alt    = n->alteratio_sonans;
    s32 integ  = (s32)(alt < 0.0 ? alt - 0.5 : alt + 0.5);
    s32 m      = n->nota;

    (vacuum)cv;
    e->nota = m;
    si (   (f64)integ == alt && integ >= -I && integ <= I
        && n->littera_sonans)
    {
        e->littera  = (character)(n->littera_sonans - 'A' + 'a');
        e->signum   = integ;
    }
    alioquin
    {
        s32 r = ((m % XII) + XII) % XII;

        si (alt >= 0.0)
        {
            e->littera  = acuti[r];
            e->signum   = acuti_s[r];
        }
        alioquin
        {
            e->littera  = graves[r];
            e->signum   = graves_s[r];
        }
        si ((f64)integ == alt)
        {
            *scriptura_mutata = VERUM;
        }
    }
    e->octava = (m - _gradus(e->littera) - e->signum) / XII - I;
}

interior b32
_eadem_vox (
    chorda a,
    chorda b)
{
    redde chorda_aequalis(a, b);
}

/* linea (pars, vox, gradus); creatur si abest */
interior Linea*
_lineam_invenire (
    Conversor* cv,
          Xar* lineae,
          i32  pars,
       chorda  vox,
          i32  gradus)
{
    Linea* l;
      i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(lineae); i++)
    {
        l = (Linea*)xar_obtinere(lineae, i);
        si (l->pars == pars && l->gradus == gradus && _eadem_vox(l->vox,
                vox))
        {
            redde l;
        }
    }
    l = (Linea*)xar_addere(lineae);
    si (l == NIHIL)
    {
        cv->memoria_defecit = VERUM;
        redde NIHIL;
    }
    memset(l, ZEPHYRUM, magnitudo(Linea));
    l->pars     = pars;
    l->vox      = vox;
    l->gradus   = gradus;
    l->eventus  = xar_creare(cv->piscina, (i32)magnitudo(Eventus));
    si (l->eventus == NIHIL)
    {
        cv->memoria_defecit = VERUM;
        redde NIHIL;
    }
    redde l;
}

/* pulsus ex divisionibus operis (rotundatio ad proximum) */
interior s64
_pulsus (
    constans MusicxmlOpus* opus,
                      s64  d)
{
    s64 num  = d * PULSUS_QUARTAE;
    s64 q    = opus->divisiones;

    redde num >= ZEPHYRUM ? (num + q / II) / q : -((-num + q / II) / q);
}

interior b32
_pulsus_exactus (
    constans MusicxmlOpus* opus,
                      s64  d)
{
    redde (d * PULSUS_QUARTAE) % opus->divisiones == ZEPHYRUM;
}

/* syllaba purgata: characteres quos carmen dividit et '<' tolluntur */
interior chorda
_syllabam_purgare (
     Piscina* piscina,
      chorda  t,
         b32* purgata)
{
     ChordaAedificator* a = chorda_aedificator_creare(piscina, XXXII);
                   i32  i;

    si (a == NIHIL)
    {
        redde _vacua();
    }
    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        character c = (character)t.datum[i];

        si (strchr(" \t\n\r-_,.;:!?<", c) != NIHIL)
        {
            *purgata = VERUM;
            perge;
        }
        chorda_aedificator_appendere_character(a, c);
    }
    redde chorda_aedificator_finire(a);
}

/* syllaba versus electi (elisiones concatenatae); FALSUM si nulla */
interior b32
_syllabam_eligere (
                 Conversor* cv,
     constans MusicxmlNota* n,
                       i32  versus,
                    chorda* syllaba,
                       b32* divisio,
                       b32* alius,
                       b32* purgata)
{
     ChordaAedificator* a = NIHIL;
                   i32  i;
                   b32  inventa = FALSUM;

    per (i = ZEPHYRUM; i < n->numerus_syllabarum; i++)
    {
        constans MusicxmlSyllaba* y = &n->syllabae[i];

        si (y->versus != versus)
        {
            *alius = VERUM;
            perge;
        }
        si (a == NIHIL)
        {
            a = chorda_aedificator_creare(cv->piscina, XXXII);
            si (a == NIHIL)
            {
                redde FALSUM;
            }
        }
        chorda_aedificator_appendere_chorda(a,
            _syllabam_purgare(cv->piscina, y->textus, purgata));
        *divisio = y->locus == MUSICXML_SYLLABA_INITIUM
                   || y->locus == MUSICXML_SYLLABA_MEDIA;
        inventa = VERUM;
    }
    si (inventa)
    {
        *syllaba = chorda_aedificator_finire(a);
    }
    redde inventa;
}

/* (1) notae ordine exsecutionis in lineas */
interior b32
_lineas_colligere (
    Conversor* cv,
          Xar* lineae,
          i32* syllabae_purgatae,
          b32* scriptura_mutata)
{
     constans MusicxmlOpus* opus = cv->opus;
                       i32  p;
                       i32  v;
                       i32  i;

    per (p = ZEPHYRUM; p < opus->numerus_partium; p++)
    {
         constans MusicxmlPars* pars          = &opus->partes[p];
                           b32  percussio     = FALSUM;
                           b32  versus_alius  = FALSUM;

        per (v = ZEPHYRUM; v < cv->ordo->numerus; v++)
        {
            constans MusicxmlVisitatio* vis =
                &cv->ordo->visitationes[v];
            constans MusicxmlMensura* m;
                               i32 gradus = ZEPHYRUM;
                               i32 versus = cv->forma->versus > ZEPHYRUM
                                   ? cv->forma->versus : vis->transitus;

            si (vis->mensura >= pars->numerus_mensurarum)
            {
                perge;
            }
            m = &pars->mensurae[vis->mensura];
            per (i = ZEPHYRUM; i < m->numerus_notarum; i++)
            {
                 constans MusicxmlNota* n = &m->notae[i];
                                 Linea* l;
                               Eventus  e;
                                   s64  a;
                                   s64  b;
                                   i32  k;
                                   b32  purgata = FALSUM;

                si (n->acordum)
                {
                    gradus++;
                }
                alioquin
                {
                    gradus = ZEPHYRUM;
                }
                si (n->genus == MUSICXML_NOTA_PAUSA)
                {
                    perge;
                }
                si (n->gratia)
                {
                    _approximatio(cv, MUSICXML_CARMEN_GRATIA_OMISSA,
                        pars->id,
                        m->numerus, vis->transitus, n->linea,
                        n->columna,
                        "gratia omissa");
                    perge;
                }
                si (n->parva)
                {
                    _approximatio(cv, MUSICXML_CARMEN_PARVA_OMISSA,
                        pars->id,
                        m->numerus, vis->transitus, n->linea,
                        n->columna,
                        "nota parva (cue) omissa");
                    perge;
                }
                si (n->genus == MUSICXML_NOTA_PERCUSSA)
                {
                    si (!percussio)
                    {
                        _approximatio(cv,
                            MUSICXML_CARMEN_PERCUSSIO_OMISSA,
                            pars->id, m->numerus, vis->transitus,
                            n->linea,
                            n->columna, "percussio (unpitched) omissa");
                        percussio = VERUM;
                    }
                    perge;
                }
                si (cv->fractus)
                {
                    redde VERUM;
                }
                l = _lineam_invenire(cv, lineae, p, n->vox, gradus);
                si (l == NIHIL)
                {
                    redde FALSUM;
                }
                a = vis->initium + n->positio;
                b = a + n->longitudo;
                /* ligatura: eventum apertum eiusdem notae continuat */
                si (   n->ligatura_finis
                    && xar_numerus(l->eventus) > ZEPHYRUM)
                {
                    Eventus* q = (Eventus*)xar_obtinere(l->eventus,
                        xar_numerus(l->eventus) - I);

                    si (   q->aperta && q->nota == n->nota
                        && q->finis == _pulsus(opus, a))
                    {
                        q->finis   = _pulsus(opus, b);
                        q->aperta  = n->ligatura_initium;
                        q->inexactus  = q->inexactus
                                        || !_pulsus_exactus(opus, b);
                        perge;
                    }
                }
                memset(&e, ZEPHYRUM, magnitudo(e));
                e.initium  = _pulsus(opus, a);
                e.finis    = _pulsus(opus, b);
                e.aperta   = n->ligatura_initium;
                e.inexactus  = !_pulsus_exactus(opus, a)
                               || !_pulsus_exactus(opus, b);
                e.visitatio  = v;
                e.fons       = n;
                si (n->alteratio_sonans
                    != (f64)(s32)n->alteratio_sonans)
                {
                    _approximatio(cv, MUSICXML_CARMEN_ALTERATIO_FRACTA,
                        pars->id, m->numerus, vis->transitus, n->linea,
                        n->columna, "alteratio non integra: semitonium "
                        "proximum");
                }
                _orthographiam_ponere(cv, n, &e, scriptura_mutata);
                /* ambitus MML o0..o8 */
                si (e.octava < ZEPHYRUM || e.octava > VIII)
                {
                    _approximatio(cv, MUSICXML_CARMEN_EXTRA_AMBITUM,
                        pars->id,
                        m->numerus, vis->transitus, n->linea,
                        n->columna,
                        "nota extra o0..o8: octava mota");
                    dum (e.octava < ZEPHYRUM)
                    {
                        e.octava++;
                        e.nota += XII;
                    }
                    dum (e.octava > VIII)
                    {
                        e.octava--;
                        e.nota -= XII;
                    }
                }
                si (gradus == ZEPHYRUM)
                {
                    (vacuum)_syllabam_eligere(cv, n, versus, &e.syllaba,
                        &e.divisio, &versus_alius, &purgata);
                    si (purgata)
                    {
                        (*syllabae_purgatae)++;
                    }
                }
                /* notae se tegentes in linea: prior truncatur */
                k = xar_numerus(l->eventus);
                si (k > ZEPHYRUM)
                {
                    Eventus* q = (Eventus*)xar_obtinere(l->eventus, k
                        - I);

                    si (q->finis > e.initium)
                    {
                        _approximatio(cv,
                            MUSICXML_CARMEN_NOTAE_SE_TEGUNT,
                            pars->id, m->numerus, vis->transitus,
                            n->linea,
                            n->columna, "notae vocis se tegunt: prior "
                            "truncata");
                        q->finis = e.initium;
                    }
                }
                si (e.finis > e.initium)
                {
                    Eventus* x = (Eventus*)xar_addere(l->eventus);

                    si (x == NIHIL)
                    {
                        redde FALSUM;
                    }
                    *x = e;
                }
            }
        }
        si (versus_alius && cv->forma->versus > ZEPHYRUM)
        {
            _approximatio(cv, MUSICXML_CARMEN_VERSUS_OMISSUS, pars->id,
                _vacua(), I, ZEPHYRUM, ZEPHYRUM,
                "versus praeter electum omissi");
        }
    }
    redde VERUM;
}


/* ==================================================
 * Tituli et instrumenta
 * ================================================== */

/* titulus [a-z0-9_]+: litterae ASCII minusculae et cifrae, cetera '_',
 * sine '_' duplici, ducente aut trahente */
interior chorda
_plicare (
     Piscina* piscina,
      chorda  t)
{
     ChordaAedificator* a = chorda_aedificator_creare(piscina, XXXII);
                   i32  i;
                   b32  linea = FALSUM;
                chorda  r;

    si (a == NIHIL)
    {
        redde _vacua();
    }
    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        character c = (character)t.datum[i];

        si (c >= 'A' && c <= 'Z')
        {
            c = (character)(c - 'A' + 'a');
        }
        si ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
        {
            si (linea && chorda_aedificator_longitudo(a) > ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(a, '_');
            }
            chorda_aedificator_appendere_character(a, c);
            linea = FALSUM;
        }
        alioquin
        {
            linea = VERUM;
        }
    }
    r = chorda_aedificator_finire(a);
    redde r;
}

/* titulus unicus inter lineas iam nominatas: basis, basis_2, ... */
interior chorda
_titulum_unicum (
    Conversor* cv,
          Xar* lineae,
          i32  usque,
       chorda  basis)
{
    i32 n = I;

    per (;;)
    {
        ChordaAedificator* a = chorda_aedificator_creare(cv->piscina,
            XXXII);
                    chorda t;
                       i32 i;
                       b32 iteratus = FALSUM;

        si (a == NIHIL)
        {
            redde basis;
        }
        chorda_aedificator_appendere_chorda(a, basis);
        si (n > I)
        {
            chorda_aedificator_appendere_character(a, '_');
            chorda_aedificator_appendere_i32(a, n);
        }
        t = chorda_aedificator_finire(a);
        per (i = ZEPHYRUM; i < usque; i++)
        {
            si (chorda_aequalis(((Linea*)xar_obtinere(lineae,
                i))->titulus,
                    t))
            {
                iteratus = VERUM;
                frange;
            }
        }
        si (!iteratus)
        {
            redde t;
        }
        n++;
    }
}

/* verbum t in titulo (vocabulis minusculis separatis) */
interior b32
_verbum_adest (
                 chorda  titulus,
     constans character* verbum)
{
    i32 i = ZEPHYRUM;
    i32 n = (i32)strlen(verbum);

    dum (i < titulus.mensura)
    {
        i32 j = i;
        i32 k;
        b32 par;

        dum (   j < titulus.mensura
             && ((titulus.datum[j] >= 'a' && titulus.datum[j] <= 'z')
                 || (titulus.datum[j] >= 'A'
                     && titulus.datum[j] <= 'Z')))
        {
            j++;
        }
        par = (j - i) == n;
        per (k = ZEPHYRUM; par && k < n; k++)
        {
            character c = (character)titulus.datum[i + k];

            si (c >= 'A' && c <= 'Z')
            {
                c = (character)(c - 'A' + 'a');
            }
            par = c == verbum[k];
        }
        si (par)
        {
            redde VERUM;
        }
        i = j + I;
    }
    redde FALSUM;
}

interior b32
_verbum_ex (
                 chorda  titulus,
    constans character* const* verba)
{
    i32 i;

    per (i = ZEPHYRUM; verba[i] != NIHIL; i++)
    {
        si (_verbum_adest(titulus, verba[i]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior constans character*
_vox_ex_ambitu (
    constans Linea* l)
{
    i32 i;
    s64 summa  = ZEPHYRUM;
    i32 n      = xar_numerus(l->eventus);

    per (i = ZEPHYRUM; i < n; i++)
    {
        summa += ((Eventus*)xar_obtinere(l->eventus, i))->nota;
    }
    si (n == ZEPHYRUM)
    {
        redde "superius";
    }
    summa /= n;
    redde summa >= LXVII ? "superius" : summa >= LX ? "altus"
         : summa >= LIII ? "tenor" : "bassus";
}

/* tabula instrumentorum (musicxml_carmen.h INSTRUMENTA); regula in
 * *regula */
interior constans character*
_instrumentum_eligere (
     constans MusicxmlPars*  pars,
            constans Linea*  l,
        constans character** regula)
{
    constans character* superius[] = { "soprano", "sopran", "superius",
        "cantus", "discantus", "treble", NIHIL };
    constans character* altus[] = { "alto", "alt", "altus", "contralto",
        "contratenor", "countertenor", "mezzo", NIHIL };
    constans character* tenor[] = { "tenor", NIHIL };
    constans character* bassus[] = { "bass", "bassus", "basso",
        "baritone",
        "bariton", NIHIL };
    constans character* vox[] = { "voice", "voce", "stimme",
        "singstimme", "vox", NIHIL };
    constans character* chordae[] = { "violin", "violino", "violine",
        "viola", "cello", "violoncello", "kontrabass", "contrabass",
            NIHIL };
    constans character* cithara[] = { "harp", "arpa", "harfe", NIHIL };
    constans character* malleoli[] = { "piano", "pianoforte", "klavier",
        NIHIL };
    constans character* plectra[] = { "harpsichord", "cembalo", NIHIL };
    constans character* organum[] = { "organ", "organo", "orgel",
        NIHIL };
    constans character* fistula[] = { "flute", "flauto", NIHIL };
    constans character* calami[] = { "clarinet", "clarinetto",
        "klarinette", NIHIL };
    constans character* cornu[]         = { "horn", "corno", NIHIL };
    constans character* pulsae[]        = { "pizzicato", NIHIL };
    constans character* campanae[]      = { "bells", "glocken", NIHIL };
    constans character* tintinnabula[]  = { "glockenspiel", NIHIL };
    constans character* ligna[]         = { "marimba", NIHIL };
    chorda t                            = pars->titulus;
    i32 programma = pars->numerus_instrumentorum > ZEPHYRUM
        ? pars->instrumenta[ZEPHYRUM].programma : ZEPHYRUM;

    *regula = "vox";
    /* abbreviatio vocis sola ("S.", "T. 1") */
    si (   t.mensura >= I && t.mensura <= V
        && (t.mensura == I || t.datum[I] == '.' || t.datum[I] == ' '))
    {
        character c = (character)t.datum[ZEPHYRUM];

        si (c == 'S' || c == 's') redde "superius";
        si (c == 'A' || c == 'a') redde "altus";
        si (c == 'T' || c == 't') redde "tenor";
        si (c == 'B' || c == 'b') redde "bassus";
    }
    si (_verbum_ex(t, superius)) redde "superius";
    si (_verbum_ex(t, altus))    redde "altus";
    si (_verbum_ex(t, tenor))    redde "tenor";
    si (_verbum_ex(t, bassus))   redde "bassus";
    si (_verbum_ex(t, vox))      redde _vox_ex_ambitu(l);
    *regula = "nomen instrumenti";
    si (_verbum_ex(t, pulsae))  redde "pizzicato";
    si (_verbum_ex(t, chordae))    redde "chordae";
    si (_verbum_ex(t, cithara))      redde "harpa";
    si (_verbum_ex(t, plectra))    redde "cembalo";
    si (_verbum_ex(t, malleoli))      redde "chorda";
    si (_verbum_ex(t, organum))    redde "organum";
    si (_verbum_ex(t, fistula))    redde "fistula";
    si (_verbum_ex(t, calami)) redde "clarinetum";
    si (_verbum_ex(t, cornu))      redde "cornu";
    si (_verbum_ex(t, tintinnabula))      redde "glockenspiel";
    si (_verbum_ex(t, campanae))   redde "campanae_tubulares";
    si (_verbum_ex(t, ligna))    redde "marimba";
    *regula = "programma MIDI";
    si (programma >= I && programma <= VI)       redde "chorda";
    si (programma >= VII && programma <= VIII)   redde "cembalo";
    si (programma == XIII)                       redde "marimba";
    si (programma
        == XV)                         redde "campanae_tubulares";
    si (programma >= IX && programma <= XVI)     redde "glockenspiel";
    si (programma >= XVII && programma <= XXIV)  redde "organum";
    si (programma >= XXV && programma <= XL)     redde "chorda";
    si (programma >= XLI && programma <= XLIV)   redde "chordae";
    si (programma >= XLV && programma <= XLVI)   redde "pizzicato";
    si (programma == XLVII)                      redde "harpa";
    si (programma >= XLIX && programma <= LII)   redde "chordae";
    si (   programma >= LIII
        && programma <= LV)    redde _vox_ex_ambitu(l);
    si (programma >= LVII && programma <= LXIV)  redde "cornu";
    si (programma >= LXV && programma <= LXXII)  redde "clarinetum";
    si (programma >= LXXIII && programma <= LXXX) redde "fistula";
    *regula = "ordinarium";
    redde "aether";
}


/* ==================================================
 * Mensurae carminis
 * ================================================== */

nomen structura {
       s64 initium;      /* pulsus */
       s64 finis;
       i32 visitatio;
       i32 mensura;      /* index MusicXML */
       i32 numerus;      /* carminis (I-basatus); 0 = praetermissa */
       s32 metri_n;
       s32 metri_d;
       b32 liberum;
       b32 clavis_tradita;
       s32 quintae;
    chorda modus;
       b32 ad_reticulum; /* termini ad reticulum LX adducti */
} Mensura;

/* metrum, clavis vigentes per mensuram (pars prima, ordine scripto) */
interior vacuum
_attributa_vigentia (
                 Conversor* cv,
                   Mensura* mensurae,
                       i32  numerus)
{
     constans MusicxmlOpus* opus  = cv->opus;
     constans MusicxmlPars* p0    = &opus->partes[ZEPHYRUM];
                       i32  k;
                       i32  v;
                       s32  n          = IV;
                       s32  d          = IV;
                       b32  liberum    = FALSUM;
                       b32  additivum  = FALSUM;
                       b32  tradita    = VERUM;
                       s32  quintae    = ZEPHYRUM;
                    chorda  modus      = _vacua();
                       s32* metri_n;
                       s32* metri_d;
                       b32* libera;
                       b32* additiva;
                       b32* traditae;
                       s32* quintae_m;
                    chorda* modi;
                       b32* non_tradita;

    metri_n = (s32*)piscina_allocare(cv->piscina,
        (memoriae_index)((p0->numerus_mensurarum + I)
            * (i32)(VI * magnitudo(s32) + magnitudo(chorda)
                    + II * magnitudo(b32))));
    si (metri_n == NIHIL)
    {
        cv->memoria_defecit = VERUM;
        redde;
    }
    metri_d      = metri_n + p0->numerus_mensurarum + I;
    quintae_m    = metri_d + p0->numerus_mensurarum + I;
    libera       = (b32*)(quintae_m + p0->numerus_mensurarum + I);
    additiva     = libera + p0->numerus_mensurarum + I;
    traditae     = additiva + p0->numerus_mensurarum + I;
    non_tradita  = traditae + p0->numerus_mensurarum + I;
    modi       = (chorda*)piscina_allocare(cv->piscina,
        (memoriae_index)((p0->numerus_mensurarum + I)
            * (i32)magnitudo(chorda)));
    si (modi == NIHIL)
    {
        cv->memoria_defecit = VERUM;
        redde;
    }
    per (k = ZEPHYRUM; k < p0->numerus_mensurarum; k++)
    {
          constans MusicxmlMensura* m = &p0->mensurae[k];
                               i32  a;

        additivum       = FALSUM;
        non_tradita[k]  = FALSUM;
        per (a = ZEPHYRUM; a < m->numerus_attributorum; a++)
        {
            constans MusicxmlAttributa* t = &m->attributa[a];

            si (t->metrum_adest)
            {
                liberum = t->metrum.liberum;
                si (!liberum && t->metrum.numerus_partium > ZEPHYRUM)
                {
                    i32 q;

                    n = ZEPHYRUM;
                    d = (s32)t->metrum.partes[ZEPHYRUM].unitas;
                    per (q = ZEPHYRUM; q
                        < t->metrum.numerus_partium; q++)
                    {
                        n += (s32)t->metrum.partes[q].summa;
                        si (   t->metrum.partes[q].pulsus.mensura
                            > ZEPHYRUM
                            && memchr(t->metrum.partes[q].pulsus.datum,
                            '+',
                                   (size_t)t->metrum.partes[q].pulsus
                                       .mensura) != NIHIL)
                        {
                            additivum = VERUM;
                        }
                    }
                    si (t->metrum.numerus_partium > I)
                    {
                        additivum = VERUM;
                    }
                }
            }
            si (t->clavis_adest)
            {
                si (t->clavis.tradita)
                {
                    tradita  = VERUM;
                    quintae  = t->clavis.quintae;
                    modus    = t->clavis.modus;
                }
                alioquin
                {
                    non_tradita[k] = VERUM;
                }
            }
        }
        metri_n[k]    = n;
        metri_d[k]    = d;
        libera[k]     = liberum;
        additiva[k]   = additivum;
        traditae[k]   = tradita;
        quintae_m[k]  = quintae;
        modi[k]       = modus;
    }
    per (v = ZEPHYRUM; v < numerus; v++)
    {
        i32 q = mensurae[v].mensura;

        si (q >= p0->numerus_mensurarum)
        {
            q = p0->numerus_mensurarum - I;
        }
        mensurae[v].metri_n         = metri_n[q];
        mensurae[v].metri_d         = metri_d[q];
        mensurae[v].liberum         = libera[q];
        mensurae[v].clavis_tradita  = traditae[q];
        mensurae[v].quintae         = quintae_m[q];
        mensurae[v].modus           = modi[q];
        si (additiva[q])
        {
            _approximatio(cv, MUSICXML_CARMEN_METRUM_ADDITIVUM, p0->id,
                p0->mensurae[q].numerus, cv->ordo->visitationes[v]
                    .transitus, p0->mensurae[q].linea,
                p0->mensurae[q].columna, "metrum additivum: summa");
            additiva[q] = FALSUM;   /* semel per mensuram scriptam */
        }
        si (non_tradita[q])
        {
            _approximatio(cv, MUSICXML_CARMEN_CLAVIS_NON_TRADITA,
                p0->id,
                p0->mensurae[q].numerus, cv->ordo->visitationes[v]
                    .transitus, p0->mensurae[q].linea,
                p0->mensurae[q].columna,
                "armatura non tradita omissa (clavis prior servatur)");
            non_tradita[q] = FALSUM;
        }
    }
}

/* clavis carminis: "littera [#|b] maior|minor|modus"; FALSUM si modus
 * carmini ignotus (locrius) */
interior b32
_clavem_scribere (
    ChordaAedificator* a,
                  s32  quintae,
               chorda  modus)
{
    constans character* nomina[] = { "fb", "cb", "gb", "db", "ab", "eb",
        "bb", "f", "c", "g", "d", "a", "e", "b", "f#", "c#", "g#", "d#",
        "a#", "e#", "b#" };
                   s32  delta  = ZEPHYRUM;
    constans character* m      = "maior";
                   s32  t;

    si (   chorda_aequalis_literis(modus, "minor")
        || chorda_aequalis_literis(modus, "aeolian"))
    {
        delta = III;
        m = chorda_aequalis_literis(modus,
            "minor") ? "minor" : "aeolius";
    }
    alioquin si (chorda_aequalis_literis(modus, "dorian"))
    {
        delta  = II;
        m      = "dorius";
    }
    alioquin si (chorda_aequalis_literis(modus, "phrygian"))
    {
        delta  = IV;
        m      = "phrygius";
    }
    alioquin si (chorda_aequalis_literis(modus, "lydian"))
    {
        delta  = -I;
        m      = "lydius";
    }
    alioquin si (chorda_aequalis_literis(modus, "mixolydian"))
    {
        delta  = I;
        m      = "mixolydius";
    }
    alioquin si (chorda_aequalis_literis(modus, "ionian"))
    {
        m = "ionius";
    }
    alioquin si (chorda_aequalis_literis(modus, "locrian"))
    {
        redde FALSUM;
    }
    t = quintae + delta + VIII;
    si (t < ZEPHYRUM || t > XX)
    {
        redde FALSUM;
    }
    chorda_aedificator_appendere_literis(a, nomina[t]);
    chorda_aedificator_appendere_character(a, ' ');
    chorda_aedificator_appendere_literis(a, m);
    redde VERUM;
}

/* positio carminis: mensura="M" ad initium mensurae, aliter pulsus="N" */
interior vacuum
_positionem_scribere (
    ChordaAedificator* a,
     constans Mensura* mensurae,
                  i32  numerus,
                  s64  pulsus)
{
    i32 v;

    per (v = ZEPHYRUM; v < numerus; v++)
    {
        si (   mensurae[v].numerus > ZEPHYRUM
            && mensurae[v].initium == pulsus)
        {
            chorda_aedificator_appendere_literis(a, "mensura=\"");
            chorda_aedificator_appendere_i32(a,
                (i32)mensurae[v].numerus);
            chorda_aedificator_appendere_character(a, '"');
            redde;
        }
    }
    chorda_aedificator_appendere_literis(a, "pulsus=\"");
    _s64_scribere(a, pulsus);
    chorda_aedificator_appendere_character(a, '"');
}

/* numerus mensurae carminis cuius initium >= pulsus (cuneus usque);
 * initium eius in *initium (numerus != index visitationis: visitationes
 * vacuae numerum non habent) */
interior i32
_mensura_ab (
    constans Mensura* mensurae,
                 i32  numerus,
                 s64  pulsus,
                 s64* initium,
                 b32* motus)
{
    i32 v;

    *motus    = FALSUM;
    *initium  = ZEPHYRUM;
    per (v = ZEPHYRUM; v < numerus; v++)
    {
        si (   mensurae[v].numerus == ZEPHYRUM
            || mensurae[v].initium < pulsus)
        {
            perge;
        }
        *motus    = mensurae[v].initium > pulsus ? VERUM : FALSUM;
        *initium  = mensurae[v].initium;
        redde mensurae[v].numerus;
    }
    redde ZEPHYRUM;
}


/* ==================================================
 * Textus MML
 * ================================================== */

/* longitudo d ut longitudines MML; nota (littera >= 0) cum '&' inter
 * partes, pausa 'r' repetita. FALSUM si dici non potest */
interior b32
_longitudinem_scribere (
         Conversor* cv,
 ChordaAedificator* a,
               s64  d,
         character  littera,
               s32  signum,
               b32* primum)
{
    i32 partes[CXXVIII];
    i32 n = _longitudinem_dividere(cv->piscina, d, partes, CXXVIII);
    i32 i;

    si (n == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (littera == 'r')
        {
            si (!*primum)
            {
                chorda_aedificator_appendere_character(a, ' ');
            }
            chorda_aedificator_appendere_character(a, 'r');
        }
        alioquin
        {
            si (i > ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(a, '&');
            }
            alioquin si (!*primum)
            {
                chorda_aedificator_appendere_character(a, ' ');
            }
            chorda_aedificator_appendere_character(a, littera);
            si (signum > ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(a, '+');
            }
            alioquin si (signum < ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(a, '-');
            }
        }
        chorda_aedificator_appendere_literis(a,
            longitudines[partes[i]].textus);
        *primum = FALSUM;
    }
    redde VERUM;
}

/* textus lineae: per mensuram, pausae in lacunis, notae trans lineas
 * mensurae '&' */
interior b32
_textum_lineae (
            Conversor* cv,
       constans Linea* l,
     constans Mensura* mensurae,
                  i32  numerus,
    ChordaAedificator* a)
{
    i32 v;
    i32 e              = ZEPHYRUM;
    s32 octava         = -I;
    b32 primum         = VERUM;
    b32 prima_mensura  = VERUM;
    i32 n              = xar_numerus(l->eventus);

    per (v = ZEPHYRUM; v < numerus; v++)
    {
        s64 cursor;
        i32 k;

        si (mensurae[v].numerus == ZEPHYRUM)
        {
            perge;
        }
        si (!prima_mensura)
        {
            chorda_aedificator_appendere_literis(a, " |");
        }
        prima_mensura  = FALSUM;
        cursor         = mensurae[v].initium;
        /* eventus prior qui in hanc mensuram pergit iam per '&' */
        per (k = e; k < n; k++)
        {
            constans Eventus* x = (constans Eventus*)xar_obtinere(
                l->eventus, k);
                        s64 a0;
                        s64 a1;

            si (x->finis <= mensurae[v].initium)
            {
                e = k + I;
                perge;
            }
            si (x->initium >= mensurae[v].finis)
            {
                frange;
            }
            a0 = x->initium
                > mensurae[v].initium ? x->initium : mensurae[v].initium;
            a1 = x->finis
                < mensurae[v].finis ? x->finis : mensurae[v].finis;
            si (a0 > cursor)
            {
                si (!_longitudinem_scribere(cv, a, a0 - cursor, 'r',
                        ZEPHYRUM, &primum))
                {
                    redde FALSUM;
                }
            }
            si (x->octava != octava)
            {
                si (!primum)
                {
                    chorda_aedificator_appendere_character(a, ' ');
                }
                chorda_aedificator_appendere_literis(a, "o");
                chorda_aedificator_appendere_i32(a, (i32)x->octava);
                octava = x->octava;
                primum = FALSUM;
            }
            si (!_longitudinem_scribere(cv, a, a1 - a0, x->littera,
                    x->signum, &primum))
            {
                redde FALSUM;
            }
            si (x->finis > mensurae[v].finis)
            {
                /* ligatura trans lineam mensurae */
                chorda_aedificator_appendere_character(a, '&');
            }
            cursor = a1;
        }
        si (cursor < mensurae[v].finis)
        {
            si (!_longitudinem_scribere(cv, a, mensurae[v].finis
                - cursor,
                'r',
                    ZEPHYRUM, &primum))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

/* verba lineae: syllaba per notam, '_' sine syllaba; FALSUM si nulla */
interior b32
_verba_lineae (
       constans Linea* l,
    ChordaAedificator* a)
{
    i32 i;
    b32 ullae  = FALSUM;
    i32 n      = xar_numerus(l->eventus);

    per (i = ZEPHYRUM; i < n; i++)
    {
        si (((Eventus*)xar_obtinere(l->eventus, i))->syllaba.mensura
            > ZEPHYRUM)
        {
            ullae = VERUM;
            frange;
        }
    }
    si (!ullae)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans Eventus* x =
            (constans Eventus*)xar_obtinere(l->eventus, i);

        si (x->syllaba.mensura > ZEPHYRUM)
        {
            b32 sequens = i + I < n
                && ((Eventus*)xar_obtinere(l->eventus, i + I))->syllaba
                       .mensura > ZEPHYRUM;

            chorda_aedificator_appendere_chorda(a, x->syllaba);
            /* '-' solum ante syllabam (ante '_' spatium: "sae-cu _ li") */
            chorda_aedificator_appendere_character(a,
                (x->divisio && sequens) ? '-' : ' ');
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(a, "_ ");
        }
    }
    /* spatium trahens tollitur */
    dum (chorda_aedificator_longitudo(a) > ZEPHYRUM)
    {
        chorda s = chorda_aedificator_spectare(a);

        si (s.datum[s.mensura - I] != ' ')
        {
            frange;
        }
        chorda_aedificator_truncare(a, s.mensura - I);
    }
    redde VERUM;
}


/* ==================================================
 * Reticulum: mensura lineae ad LX pulsus adducitur si terminus
 * inexactus aut intervallum MML dici non potest
 * ================================================== */

interior b32
_intervallum_dici_potest (
    Conversor* cv,
          s64  d)
{
    i32 partes[CXXVIII];

    redde d == ZEPHYRUM
        || _longitudinem_dividere(cv->piscina, d, partes, CXXVIII)
           > ZEPHYRUM;
}

interior s64
_ad_reticulum (
    s64 x,
    s64 basis,
    s64 finis)
{
    s64 r = basis + ((x - basis + RETICULUM / II)
        / RETICULUM) * RETICULUM;

    redde r > finis ? finis : r;
}

interior vacuum
_reticulum_lineae (
            Conversor* cv,
                Linea* l,
     constans Mensura* mensurae,
                  i32  numerus)
{
    i32 v;
    i32 n           = xar_numerus(l->eventus);
    i32 k;
    s64 finis_operis = ZEPHYRUM;

    /* finis mensurae ultimae in reticulo adductus: eventus ultra eum
     * secantur (ligatura '&' in nihilum aliter) */
    per (v = ZEPHYRUM; v < numerus; v++)
    {
        si (   mensurae[v].numerus > ZEPHYRUM
            && mensurae[v].finis > finis_operis)
        {
            finis_operis = mensurae[v].finis;
        }
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        Eventus* x = (Eventus*)xar_obtinere(l->eventus, k);

        si (x->finis > finis_operis)
        {
            x->finis = x->initium < finis_operis ? finis_operis
                                                 : x->initium;
        }
    }
    per (v = ZEPHYRUM; v < numerus; v++)
    {
        b32 adducere = FALSUM;
        s64 cursor;

        si (mensurae[v].numerus == ZEPHYRUM)
        {
            perge;
        }
        cursor = mensurae[v].initium;
        per (k = ZEPHYRUM; k < n && !adducere; k++)
        {
             Eventus* x = (Eventus*)xar_obtinere(l->eventus, k);
                 s64  a0;
                 s64  a1;

            si (   x->finis   <= mensurae[v].initium
                || x->initium >= mensurae[v].finis)
            {
                perge;
            }
            a0 = x->initium
                > mensurae[v].initium ? x->initium : mensurae[v].initium;
            a1 = x->finis
                < mensurae[v].finis ? x->finis : mensurae[v].finis;
            si (   x->inexactus
                || !_intervallum_dici_potest(cv, a0 - cursor)
                || !_intervallum_dici_potest(cv, a1 - a0))
            {
                adducere = VERUM;
            }
            cursor = a1;
        }
        si (   !adducere
            && !_intervallum_dici_potest(cv, mensurae[v].finis
                - cursor))
        {
            adducere = VERUM;
        }
        si (!adducere)
        {
            perge;
        }
        per (k = ZEPHYRUM; k < n; k++)
        {
             Eventus* x   = (Eventus*)xar_obtinere(l->eventus, k);
                 s64  i0  = x->initium;
                 s64  f0  = x->finis;

            si (   x->initium >= mensurae[v].initium
                && x->initium < mensurae[v].finis)
            {
                x->initium = _ad_reticulum(x->initium,
                    mensurae[v].initium,
                    mensurae[v].finis);
            }
            si (   x->finis > mensurae[v].initium
                && x->finis <= mensurae[v].finis)
            {
                x->finis = _ad_reticulum(x->finis, mensurae[v].initium,
                    mensurae[v].finis);
            }
            si ((i0 != x->initium || f0 != x->finis) && !x->notatus)
            {
                     constans MusicxmlNota* fn = x->fons;
                constans MusicxmlVisitatio* vis =
                    &cv->ordo->visitationes[x->visitatio];

                x->notatus = VERUM;
                _approximatio(cv, MUSICXML_CARMEN_EXTRA_RETICULUM,
                    cv->opus->partes[l->pars].id,
                    cv->opus->partes[l->pars].mensurae[vis->mensura].numerus,
                    vis->transitus, fn != NIHIL ? fn->linea : ZEPHYRUM,
                    fn != NIHIL ? fn->columna : ZEPHYRUM,
                    "longitudo extra reticulum CMLX: ad sexagesimam "
                    "quartam adducta");
            }
        }
    }
    /* eventus vacui tolluntur */
    {
        i32 w = ZEPHYRUM;

        per (k = ZEPHYRUM; k < n; k++)
        {
            Eventus* x = (Eventus*)xar_obtinere(l->eventus, k);

            si (x->finis > x->initium)
            {
                si (w != k)
                {
                    *(Eventus*)xar_obtinere(l->eventus, w) = *x;
                }
                w++;
            }
        }
        (vacuum)xar_truncare(l->eventus, w);
    }
}


/* ==================================================
 * Elementa carminis
 * ================================================== */

hic_manens constans character* const GRADUS_DYNAMICAE[] = {
    "pp", "p", "mp", "mf", "f", "ff", NIHIL
};

interior s32
_gradus_dynamicae (
    chorda t)
{
    s32 i;

    per (i = ZEPHYRUM; GRADUS_DYNAMICAE[i] != NIHIL; i++)
    {
        si (chorda_aequalis_literis(t, GRADUS_DYNAMICAE[i]))
        {
            redde i;
        }
    }
    redde -I;
}

/* dynamica collecta partis: gradus (usque 0) aut cuneus (usque =
 * numerus mensurae finis). Gradus = index GRADUS_DYNAMICAE; cuneo ante
 * resolutionem +I/-I (crescendo/diminuendo), post eam gradus finis */
nomen structura {
       s64 pulsus;
       s64 usque_pulsus;   /* initium mensurae usque */
       i32 usque;
       s32 gradus;
    chorda mensura;        /* numerus scriptus (approximationes) */
       i32 transitus;
       i32 linea;
       i32 columna;
} Intensio;

/* positione; eadem positione gradus ante cuneum (carmen) */
interior s32
_intensiones_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans Intensio* x = (constans Intensio*)a;
    constans Intensio* y = (constans Intensio*)b;

    si (x->pulsus != y->pulsus)
    {
        redde x->pulsus < y->pulsus ? -I : I;
    }
    si ((x->usque == ZEPHYRUM) != (y->usque == ZEPHYRUM))
    {
        redde x->usque == ZEPHYRUM ? -I : I;
    }
    redde ZEPHYRUM;
}

interior s32
_pulsus_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    s64 x = *(constans s64*)a;
    s64 y = *(constans s64*)b;

    redde x < y ? -I : (x > y ? I : ZEPHYRUM);
}

/* dynamica partis p, ordine positionis. Carmen mutationem intra clivum
 * vetat: gradus intra cuneum fit gradus finis cunei, cuneus intra
 * cuneum omittitur (ambo CLIVUS_MOTUS); gradus iteratus eadem positione
 * - ultimus vincit. Deinde scribitur per lineam partis */
interior vacuum
_intensiones_scribere (
            Conversor* cv,
                  Xar* lineae,
                  i32  p,
                  Xar* intensiones,
     constans Mensura* mensurae,
                  i32  numerus,
    ChordaAedificator* a)
{
    constans MusicxmlPars* pars    = &cv->opus->partes[p];
                      s32  vigens  = III;   /* mf */
                      s64  finis   = -I;    /* cunei servati ultimi */
                      i32  cuneus  = ZEPHYRUM;
                      i32  w       = ZEPHYRUM;
                      i32  i;
                      i32  j;

    xar_ordinare(intensiones, _intensiones_comparare);
    per (i = ZEPHYRUM; i < xar_numerus(intensiones); i++)
    {
        Intensio* x      = (Intensio*)xar_obtinere(intensiones, i);
        Intensio* prior  = w > ZEPHYRUM
            ? (Intensio*)xar_obtinere(intensiones, w - I) : NIHIL;

        si (x->pulsus < finis)
        {
            Intensio* c = (Intensio*)xar_obtinere(intensiones, cuneus);

            si (x->usque == ZEPHYRUM)
            {
                c->gradus  = x->gradus;
                vigens     = x->gradus;
                _approximatio(cv, MUSICXML_CARMEN_CLIVUS_MOTUS,
                    pars->id,
                    x->mensura, x->transitus, x->linea, x->columna,
                    "dynamica intra cuneum: gradus finis cunei facta");
            }
            alioquin
            {
                _approximatio(cv, MUSICXML_CARMEN_CLIVUS_MOTUS,
                    pars->id,
                    x->mensura, x->transitus, x->linea, x->columna,
                    "cuneus intra cuneum priorem omissus");
            }
            perge;
        }
        si (   prior        != NIHIL && x->usque == ZEPHYRUM
            && prior->usque == ZEPHYRUM && prior->pulsus == x->pulsus)
        {
            prior->gradus  = x->gradus;
            vigens         = x->gradus;
            perge;
        }
        si (x->usque == ZEPHYRUM)
        {
            vigens = x->gradus;
        }
        alioquin
        {
            vigens = x->gradus > ZEPHYRUM
                ? (vigens < V ? vigens + I : V)
                : (vigens > ZEPHYRUM ? vigens - I : ZEPHYRUM);
            x->gradus  = vigens;
            finis      = x->usque_pulsus;
            cuneus     = w;
        }
        si (w != i)
        {
            *(Intensio*)xar_obtinere(intensiones, w) = *x;
        }
        w++;
    }
    per (i = ZEPHYRUM; i < w; i++)
    {
        constans Intensio* x = (constans Intensio*)xar_obtinere(
            intensiones, i);

        per (j = ZEPHYRUM; j < xar_numerus(lineae); j++)
        {
            constans Linea* l = (constans Linea*)xar_obtinere(lineae,
                j);

            si (l->pars != p)
            {
                perge;
            }
            chorda_aedificator_appendere_literis(a,
                "    <dynamica pars=\"");
            chorda_aedificator_appendere_chorda(a, l->titulus);
            chorda_aedificator_appendere_literis(a, "\" ");
            _positionem_scribere(a, mensurae, numerus, x->pulsus);
            chorda_aedificator_appendere_literis(a, " valor=\"");
            chorda_aedificator_appendere_literis(a,
                GRADUS_DYNAMICAE[x->gradus]);
            si (x->usque > ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(a, "\" usque=\"");
                chorda_aedificator_appendere_i32(a, x->usque);
            }
            chorda_aedificator_appendere_literis(a, "\"/>\n");
        }
    }
}

interior vacuum
_intensionem_addere (
                    Conversor* cv,
                          Xar* intensiones,
                          s64  pulsus,
                          i32  usque,
                          s64  usque_pulsus,
                          s32  gradus,
                       chorda  mensura,
                          i32  transitus,
    constans MusicxmlDirectio* d)
{
    Intensio* x = (Intensio*)xar_addere(intensiones);

    si (x == NIHIL)
    {
        cv->memoria_defecit = VERUM;
        redde;
    }
    x->pulsus        = pulsus;
    x->usque         = usque;
    x->usque_pulsus  = usque_pulsus;
    x->gradus        = gradus;
    x->mensura       = mensura;
    x->transitus     = transitus;
    x->linea         = d->linea;
    x->columna       = d->columna;
}

/* fermatae ordine positionis. Carmen fermatam tenet usque ad initium
 * notae sonantis proximae (pars quaelibet): fermata intra priorem
 * eandem moram designat et omittitur */
interior vacuum
_coronas_scribere (
                  Xar* coronae,
                  Xar* lineae,
     constans Mensura* mensurae,
                  i32  numerus,
    ChordaAedificator* a)
{
    s64 finis_prior   = -I;
    s64 finis_operis  = ZEPHYRUM;
    i32 i;
    i32 j;
    i32 k;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (   mensurae[i].numerus > ZEPHYRUM
            && mensurae[i].finis > finis_operis)
        {
            finis_operis = mensurae[i].finis;
        }
    }
    xar_ordinare(coronae, _pulsus_comparare);
    per (i = ZEPHYRUM; i < xar_numerus(coronae); i++)
    {
        s64 x = *(s64*)xar_obtinere(coronae, i);

        si (x < finis_prior || x >= finis_operis)
        {
            perge;
        }
        chorda_aedificator_appendere_literis(a, "    <fermata ");
        _positionem_scribere(a, mensurae, numerus, x);
        chorda_aedificator_appendere_literis(a, " factor=\"2\"/>\n");
        finis_prior = finis_operis;
        per (j = ZEPHYRUM; j < xar_numerus(lineae); j++)
        {
            constans Linea* l = (constans Linea*)xar_obtinere(lineae,
                j);

            per (k = ZEPHYRUM; k < xar_numerus(l->eventus); k++)
            {
                s64 o = ((constans Eventus*)xar_obtinere(l->eventus,
                    k))->initium;

                si (o > x && o < finis_prior)
                {
                    finis_prior = o;
                }
            }
        }
    }
}

/* tempus (pars quaelibet; positio prima vincit), dynamica et cunei per
 * partem, fermatae - ordine exsecutionis */
interior vacuum
_elementa_scribere (
            Conversor* cv,
                  Xar* lineae,
     constans Mensura* mensurae,
                  i32  numerus,
    ChordaAedificator* a,
                  s64* tempus_initiale,
                  b32* tempus_adest)
{
    constans MusicxmlOpus* opus = cv->opus;
                      Xar* tempora;   /* s64 pulsus, s64 valor (paria) */
                      Xar* coronae;   /* s64 */
                      Xar* intensiones;   /* Intensio, per partem */
                      i32  p;
                      i32  v;
                      i32  i;
                      i32  j;

    tempora      = xar_creare(cv->piscina, (i32)(II * magnitudo(s64)));
    coronae      = xar_creare(cv->piscina, (i32)magnitudo(s64));
    intensiones  = xar_creare(cv->piscina, (i32)magnitudo(Intensio));
    si (tempora == NIHIL || coronae == NIHIL || intensiones == NIHIL)
    {
        cv->memoria_defecit = VERUM;
        redde;
    }
    *tempus_adest = FALSUM;
    per (p = ZEPHYRUM; p < opus->numerus_partium; p++)
    {
        constans MusicxmlPars* pars = &opus->partes[p];

        (vacuum)xar_truncare(intensiones, ZEPHYRUM);
        per (v = ZEPHYRUM; v < cv->ordo->numerus; v++)
        {
            constans MusicxmlVisitatio* vis =
                &cv->ordo->visitationes[v];
              constans MusicxmlMensura* m;

            si (vis->mensura >= pars->numerus_mensurarum)
            {
                perge;
            }
            m = &pars->mensurae[vis->mensura];
            /* fermatae notarum */
            per (i = ZEPHYRUM; i < m->numerus_notarum; i++)
            {
                constans MusicxmlNota* n = &m->notae[i];

                per (j = ZEPHYRUM; j < n->numerus_signorum; j++)
                {
                    si (n->signa[j].genus == MUSICXML_SIGNUM_CORONA)
                    {
                        s64 x = _pulsus(opus, vis->initium
                            + n->positio);
                        i32 q;
                        b32 iterata = FALSUM;

                        per (q = ZEPHYRUM; q
                            < xar_numerus(coronae); q++)
                        {
                            si (*(s64*)xar_obtinere(coronae, q) == x)
                            {
                                iterata = VERUM;
                            }
                        }
                        si (!iterata)
                        {
                            s64* c = (s64*)xar_addere(coronae);

                            si (c != NIHIL)
                            {
                                *c = x;
                            }
                        }
                        frange;
                    }
                }
            }
            per (i = ZEPHYRUM; i < m->numerus_directionum; i++)
            {
                 constans MusicxmlDirectio* d = &m->directiones[i];
                                       s64  x = _pulsus(opus,
                                           vis->initium + d->positio);

                si (   d->genus == MUSICXML_DIRECTIO_TEMPUS
                    && d->valor_adest)
                {
                    f64 t = d->valor;
                    s64 r = (s64)(t + 0.5);
                    i32 q;
                    b32 iterata = FALSUM;

                    per (q = ZEPHYRUM; q < xar_numerus(tempora); q++)
                    {
                        si (((s64*)xar_obtinere(tempora, q))[ZEPHYRUM]
                            == x)
                        {
                            iterata = VERUM;
                        }
                    }
                    si (iterata)
                    {
                        perge;
                    }
                    si ((f64)r != t)
                    {
                        _approximatio(cv,
                            MUSICXML_CARMEN_TEMPUS_ROTUNDATUM, pars->id,
                            m->numerus, vis->transitus, d->linea,
                            d->columna,
                            "tempus non integrum rotundatum");
                    }
                    si (r < XX || r > CD)
                    {
                        _approximatio(cv,
                            MUSICXML_CARMEN_TEMPUS_EXTRA_FINES,
                            pars->id,
                            m->numerus, vis->transitus, d->linea,
                            d->columna,
                            "tempus extra XX..CD coercitum");
                        r = r < XX ? XX : CD;
                    }
                    {
                        s64* tp = (s64*)xar_addere(tempora);

                        si (tp != NIHIL)
                        {
                            tp[ZEPHYRUM]  = x;
                            tp[I]         = r;
                        }
                    }
                }
                alioquin si (   d->genus == MUSICXML_DIRECTIO_DYNAMICA
                             && d->textus.mensura > ZEPHYRUM)
                {
                    s32 g = _gradus_dynamicae(d->textus);

                    si (g < ZEPHYRUM)
                    {
                        _approximatio(cv,
                            MUSICXML_CARMEN_DYNAMICA_IGNOTA,
                            pars->id, m->numerus, vis->transitus,
                            d->linea,
                            d->columna,
                            "dynamica carmini ignota omissa");
                        perge;
                    }
                    _intensionem_addere(cv, intensiones, x, ZEPHYRUM,
                        ZEPHYRUM, g, m->numerus, vis->transitus, d);
                }
                alioquin si (   d->genus == MUSICXML_DIRECTIO_CUNEUS
                             && (d->cuneus == MUSICXML_CUNEUS_CRESCENDO
                                 || d->cuneus
                                    == MUSICXML_CUNEUS_DIMINUENDO))
                {
                    /* finis: cuneus FINIS eiusdem numeri, ordine
                     * exsecutionis */
                    s64 finis = -I;
                    i32 w;
                    i32 usque;
                    s64 usque_pulsus;
                    b32 motus;

                    per (w = v; w < cv->ordo->numerus
                        && finis < ZEPHYRUM;
                         w++)
                    {
                        constans MusicxmlVisitatio* vis_altera =
                            &cv->ordo->visitationes[w];
                        constans MusicxmlMensura* m_altera;
                                             i32  q;

                        si (vis_altera->mensura
                            >= pars->numerus_mensurarum)
                        {
                            perge;
                        }
                        m_altera = &pars->mensurae[vis_altera->mensura];
                        per (q = w == v ? i + I : ZEPHYRUM;
                             q < m_altera->numerus_directionum; q++)
                        {
                            constans MusicxmlDirectio* f =
                                &m_altera->directiones[q];

                            si (   f->genus == MUSICXML_DIRECTIO_CUNEUS
                                && f->cuneus == MUSICXML_CUNEUS_FINIS
                                && f->numerus == d->numerus)
                            {
                                finis = _pulsus(opus,
                                    vis_altera->initium + f->positio);
                                frange;
                            }
                        }
                    }
                    si (finis < ZEPHYRUM)
                    {
                        perge;
                    }
                    usque = _mensura_ab(mensurae, numerus, finis,
                        &usque_pulsus,
                        &motus);
                    si (usque == ZEPHYRUM || usque_pulsus <= x)
                    {
                        perge;   /* sine mensura sequenti: omissus */
                    }
                    si (motus)
                    {
                        _approximatio(cv, MUSICXML_CARMEN_CLIVUS_MOTUS,
                            pars->id, m->numerus, vis->transitus,
                            d->linea,
                            d->columna,
                            "cuneus intra mensuram finitus: ad "
                            "initium mensurae proximae");
                    }
                    _intensionem_addere(cv, intensiones, x, usque,
                        usque_pulsus,
                        d->cuneus == MUSICXML_CUNEUS_CRESCENDO ? I : -I,
                        m->numerus, vis->transitus, d);
                }
            }
        }
        _intensiones_scribere(cv, lineae, p, intensiones, mensurae,
            numerus,
            a);
    }
    /* tempora ordine pulsus (insertio); primum ad 0 in caput */
    per (i = I; i < xar_numerus(tempora); i++)
    {
        s64 x0 = ((s64*)xar_obtinere(tempora, i))[ZEPHYRUM];
        s64 x1 = ((s64*)xar_obtinere(tempora, i))[I];

        j = i;
        dum (   j > ZEPHYRUM
             && ((s64*)xar_obtinere(tempora, j - I))[ZEPHYRUM] > x0)
        {
            ((s64*)xar_obtinere(tempora, j))[ZEPHYRUM] =
                ((s64*)xar_obtinere(tempora, j - I))[ZEPHYRUM];
            ((s64*)xar_obtinere(tempora, j))[I] =
                ((s64*)xar_obtinere(tempora, j - I))[I];
            j--;
        }
        ((s64*)xar_obtinere(tempora, j))[ZEPHYRUM]  = x0;
        ((s64*)xar_obtinere(tempora, j))[I]         = x1;
    }
    {
        s64 prior = -I;

        per (i = ZEPHYRUM; i < xar_numerus(tempora); i++)
        {
            s64* t = (s64*)xar_obtinere(tempora, i);

            si (t[ZEPHYRUM] == ZEPHYRUM && !*tempus_adest)
            {
                *tempus_initiale  = t[I];
                *tempus_adest     = VERUM;
                prior             = t[I];
                perge;
            }
            si (t[I] == prior)
            {
                perge;
            }
            chorda_aedificator_appendere_literis(a, "    <tempus ");
            _positionem_scribere(a, mensurae, numerus, t[ZEPHYRUM]);
            chorda_aedificator_appendere_literis(a, " valor=\"");
            _s64_scribere(a, t[I]);
            chorda_aedificator_appendere_literis(a, "\"/>\n");
            prior = t[I];
        }
    }
    _coronas_scribere(coronae, lineae, mensurae, numerus, a);
}

/* signa non conversa: per elementum scriptum (non per visitationem) */
interior vacuum
_signa_numerare (
    Conversor* cv)
{
     constans MusicxmlOpus* opus = cv->opus;
                       i32  p;
                       i32  k;
                       i32  i;
                       i32  j;

    per (p = ZEPHYRUM; p < opus->numerus_partium; p++)
    {
        per (k = ZEPHYRUM; k < opus->partes[p].numerus_mensurarum; k++)
        {
            constans MusicxmlMensura* m = &opus->partes[p].mensurae[k];

            per (i = ZEPHYRUM; i < m->numerus_notarum; i++)
            {
                per (j = ZEPHYRUM; j
                    < m->notae[i].numerus_signorum; j++)
                {
                    commutatio (m->notae[i].signa[j].genus)
                    {
                        casus MUSICXML_SIGNUM_CORONA:
                            frange;
                        casus MUSICXML_SIGNUM_ARTICULATIO:
                            cv->signa[MUSICXML_CARMEN_ARTICULATIO]++;
                            frange;
                        casus MUSICXML_SIGNUM_ORNAMENTUM:
                            cv->signa[MUSICXML_CARMEN_ORNAMENTUM]++;
                            frange;
                        casus MUSICXML_SIGNUM_ARCUS:
                            cv->signa[MUSICXML_CARMEN_ARCUS]++;
                            frange;
                        casus MUSICXML_SIGNUM_TECHNICUM:
                            cv->signa[MUSICXML_CARMEN_TECHNICUM]++;
                            frange;
                        casus MUSICXML_SIGNUM_ACORDUM_FRACTUM:
                            cv->signa[MUSICXML_CARMEN_ACORDUM_FRACTUM]++;
                            frange;
                        casus MUSICXML_SIGNUM_DYNAMICA:
                            cv->signa[MUSICXML_CARMEN_DYNAMICA_NOTAE]++;
                            frange;
                        ordinarius:
                            cv->signa[MUSICXML_CARMEN_SIGNUM_ALIUD]++;
                            frange;
                    }
                }
            }
            per (i = ZEPHYRUM; i < m->numerus_directionum; i++)
            {
                commutatio (m->directiones[i].genus)
                {
                    casus MUSICXML_DIRECTIO_VERBA:
                        cv->signa[MUSICXML_CARMEN_VERBA_DIRECTIONIS]++;
                        frange;
                    casus MUSICXML_DIRECTIO_EXERCITIUM:
                        cv->signa[MUSICXML_CARMEN_EXERCITIUM]++;
                        frange;
                    casus MUSICXML_DIRECTIO_PEDALE:
                        cv->signa[MUSICXML_CARMEN_PEDALE]++;
                        frange;
                    casus MUSICXML_DIRECTIO_OCTAVA:
                        cv->signa[MUSICXML_CARMEN_OCTAVA_SIGNATA]++;
                        frange;
                    casus MUSICXML_DIRECTIO_ALIA:
                        cv->signa[MUSICXML_CARMEN_DIRECTIO_ALIA]++;
                        frange;
                    ordinarius:
                        frange;
                }
            }
        }
    }
}

/* textus commentarii: '--' et '>' vitantur */
interior vacuum
_commentarium_appendere (
    ChordaAedificator* a,
               chorda  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        character c = (character)t.datum[i];

        si (c == '-' && i + I < t.mensura && t.datum[i + I] == '-')
        {
            perge;
        }
        si (c == '>' || c == '\n' || c == '\r')
        {
            c = ' ';
        }
        chorda_aedificator_appendere_character(a, c);
    }
}


/* ==================================================
 * API
 * ================================================== */

vacuum
musicxml_carmen_formam_ordinariam (
    MusicxmlCarmenForma* forma)
{
    si (forma == NIHIL)
    {
        redde;
    }
    memset(forma, ZEPHYRUM, magnitudo(MusicxmlCarmenForma));
    forma->versus = I;
}

interior b32
_metrum_scribere (
    ChordaAedificator* a,
                  s32  n,
                  s32  d)
{
    chorda_aedificator_appendere_i32(a, (i32)n);
    chorda_aedificator_appendere_character(a, '/');
    chorda_aedificator_appendere_i32(a, (i32)d);
    redde VERUM;
}

/* longitudo proxima quam metrum N/D (N, D <= LXIV) dicit: reticulum
 * PULSUS_INTEGRAE/D, D maximum cui N <= LXIV sufficit; minima
 * RETICULUM. Ultra LXIV integras: intacta (metrum impossibile) */
interior s64
_longitudo_metrica (
    s64 l)
{
    s64 q;

    si (l < RETICULUM)
    {
        redde RETICULUM;
    }
    per (q = LXIV; q >= I; q /= II)
    {
        s64 g = PULSUS_INTEGRAE / q;

        si (l <= LXIV * g)
        {
            l = ((l + g / II) / g) * g;
            redde l < g ? g : l;
        }
    }
    redde l;
}

/* metrum mensurae ex longitudine (M14): N/D scriptum si congruit,
 * aliter D minima potentia II; FALSUM si carmen dicere non potest */
interior b32
_metrum_mensurae (
    constans Mensura* m,
                 s32* n,
                 s32* d)
{
    s64 l = m->finis - m->initium;
    s32 q;

    si (   m->metri_d >= I && m->metri_d <= LXIV
        && (m->metri_d & (m->metri_d - I)) == ZEPHYRUM
        && m->metri_n >= I && m->metri_n <= LXIV
        && PULSUS_INTEGRAE * m->metri_n / m->metri_d == l
        && (PULSUS_INTEGRAE * m->metri_n) % m->metri_d == ZEPHYRUM)
    {
        *n = m->metri_n;
        *d = m->metri_d;
        redde VERUM;
    }
    per (q = I; q <= LXIV; q *= II)
    {
        si ((l * q) % PULSUS_INTEGRAE == ZEPHYRUM)
        {
            s64 x = l * q / PULSUS_INTEGRAE;

            si (x >= I && x <= LXIV)
            {
                *n = (s32)x;
                *d = q;
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

b32
musicxml_carmen_scribere (
           constans MusicxmlOpus* opus,
           constans MusicxmlOrdo* ordo,
    constans MusicxmlCarmenForma* forma,
                         Piscina* piscina,
               ChordaAedificator* exitus,
           MusicxmlCarmenRelatio* relatio)
{
              Conversor  cv;
                    Xar* lineae;
                Mensura* mensurae;
      ChordaAedificator* a;
      ChordaAedificator* elementa;
                    i32  numerus_mensurarum;
                    i32  i;
                    i32  v;
                    i32  carminis           = ZEPHYRUM;
                    i32  syllabae_purgatae  = ZEPHYRUM;
                    b32  scriptura_mutata   = FALSUM;
                    b32  renumerata         = FALSUM;
                    s64  tempus_initiale    = ZEPHYRUM;
                    b32  tempus_adest       = FALSUM;
                    b32  liberum;
                    s64  cursor             = ZEPHYRUM;
    MusicxmlCarmenForma  forma_ordinaria;
                 chorda  titulus;

    si (relatio != NIHIL)
    {
        memset(relatio, ZEPHYRUM, magnitudo(MusicxmlCarmenRelatio));
    }
    si (   opus == NIHIL || piscina == NIHIL || exitus == NIHIL
        || relatio == NIHIL || !musicxml_successus(opus)
        || opus->numerus_partium == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (forma == NIHIL)
    {
        musicxml_carmen_formam_ordinariam(&forma_ordinaria);
        forma = &forma_ordinaria;
    }
    _longitudines_parare();
    memset(&cv, ZEPHYRUM, magnitudo(cv));
    cv.piscina  = piscina;
    cv.opus     = opus;
    cv.ordo     = ordo != NIHIL ? ordo : musicxml_ordo(opus, piscina);
    cv.forma    = forma;
    cv.approximationes = xar_creare(piscina,
        (i32)magnitudo(MusicxmlApproximatio));
    cv.structurae  = xar_creare(piscina, (i32)magnitudo(chorda));
    lineae         = xar_creare(piscina, (i32)magnitudo(Linea));
    a              = chorda_aedificator_creare(piscina, MMMCMXCIX);
    elementa       = chorda_aedificator_creare(piscina, MXXIV);
    si (   cv.ordo       == NIHIL || cv.approximationes == NIHIL
        || cv.structurae == NIHIL || lineae == NIHIL || a == NIHIL
        || elementa      == NIHIL)
    {
        redde FALSUM;
    }

    /* (1) lineae */
    si (!_lineas_colligere(&cv, lineae, &syllabae_purgatae,
        &scriptura_mutata))
    {
        redde FALSUM;
    }

    /* (2) mensurae carminis = visitationes (vacuae praetermissae) */
    numerus_mensurarum = cv.ordo->numerus;
    mensurae = (Mensura*)piscina_allocare(piscina,
        (memoriae_index)((numerus_mensurarum + I)
            * (i32)magnitudo(Mensura)));
    si (mensurae == NIHIL)
    {
        redde FALSUM;
    }
    memset(mensurae, ZEPHYRUM, (size_t)(numerus_mensurarum + I)
        * magnitudo(Mensura));
    per (v = ZEPHYRUM; v < numerus_mensurarum; v++)
    {
        constans MusicxmlVisitatio* vis = &cv.ordo->visitationes[v];
                               s64  finis = v + I < numerus_mensurarum
                                   ? cv.ordo->visitationes[v
                                       + I].initium : cv.ordo->longitudo;

        s64 i0 = _pulsus(opus, vis->initium);
        s64 f0 = _pulsus(opus, finis);

        /* carmen positiones ex metris computat: longitudo mensurae
         * metro dicibilis (_longitudo_metrica), finis versus reticulum
         * LX absolutum (deviatio in mensura proxima redit) */
        si (v == ZEPHYRUM)
        {
            cursor = _ad_reticulum(i0, ZEPHYRUM, i0 + RETICULUM);
        }
        mensurae[v].visitatio  = v;
        mensurae[v].mensura    = vis->mensura;
        mensurae[v].initium    = cursor;
        mensurae[v].finis      = f0 > i0
            ? cursor + _longitudo_metrica(_ad_reticulum(f0, ZEPHYRUM,
                  f0 + RETICULUM) - cursor)
            : cursor;
        cursor = mensurae[v].finis;
        si (mensurae[v].finis > mensurae[v].initium)
        {
            mensurae[v].numerus = ++carminis;
        }
        si (   f0 > i0
            && (   !_pulsus_exactus(opus, finis - vis->initium)
                || mensurae[v].finis - mensurae[v].initium != f0 - i0))
        {
            _approximatio(&cv, MUSICXML_CARMEN_EXTRA_RETICULUM,
                opus->partes[ZEPHYRUM].id,
                vis->mensura < opus->partes[ZEPHYRUM].numerus_mensurarum
                    ? opus->partes[ZEPHYRUM].mensurae[vis->mensura].numerus
                    : _vacua(),
                vis->transitus, ZEPHYRUM, ZEPHYRUM,
                "longitudo mensurae extra reticulum: ad sexagesimam "
                "quartam adducta");
        }
    }
    _attributa_vigentia(&cv, mensurae, numerus_mensurarum);
    liberum = numerus_mensurarum > ZEPHYRUM
        && mensurae[ZEPHYRUM].liberum;

    si (cv.fractus || cv.memoria_defecit)
    {
        salta relatio_scribenda;   /* strictus: approximatio sola */
    }

    /* reticulum; lineae sine nota (gratiae omissae, eventus vacui)
     * tolluntur - carmen frustum sine notis reicit */
    {
        i32 w = ZEPHYRUM;

        per (i = ZEPHYRUM; i < xar_numerus(lineae); i++)
        {
            Linea* l = (Linea*)xar_obtinere(lineae, i);

            _reticulum_lineae(&cv, l, mensurae, numerus_mensurarum);
            si (xar_numerus(l->eventus) == ZEPHYRUM)
            {
                perge;
            }
            si (w != i)
            {
                *(Linea*)xar_obtinere(lineae, w) = *l;
            }
            w++;
        }
        (vacuum)xar_truncare(lineae, w);
    }
    si (xar_numerus(lineae) == ZEPHYRUM)
    {
        _structura(&cv, "nulla nota sonans: carmen non scribitur",
            _vacua());
        cv.fractus = VERUM;
        salta relatio_scribenda;
    }

    /* tituli et instrumenta linearum */
    per (i = ZEPHYRUM; i < xar_numerus(lineae); i++)
    {
        Linea* l                     = (Linea*)xar_obtinere(lineae, i);
        constans MusicxmlPars* pars  = &opus->partes[l->pars];
        constans character* regula;
        chorda basis = _plicare(piscina, pars->titulus.mensura
            > ZEPHYRUM
                                             ? pars->titulus : pars->id);

        si (basis.mensura == ZEPHYRUM)
        {
            ChordaAedificator* b = chorda_aedificator_creare(piscina,
                XVI);

            chorda_aedificator_appendere_literis(b, "pars_");
            chorda_aedificator_appendere_i32(b, (i32)l->pars + I);
            basis = chorda_aedificator_finire(b);
        }
        l->titulus  = _titulum_unicum(&cv, lineae, i, basis);
        regula      = "forma";
        l->instrumentum = forma->instrumentum.mensura > ZEPHYRUM
            ? forma->instrumentum
            : chorda_ex_literis(_instrumentum_eligere(pars, l, &regula),
                  piscina);
        {
            ChordaAedificator* b = chorda_aedificator_creare(piscina,
                LXIV);

            chorda_aedificator_appendere_chorda(b, l->titulus);
            chorda_aedificator_appendere_literis(b, ": ");
            chorda_aedificator_appendere_chorda(b, l->instrumentum);
            chorda_aedificator_appendere_literis(b, " (");
            chorda_aedificator_appendere_literis(b, regula);
            chorda_aedificator_appendere_character(b, ')');
            _structura(&cv, "instrumentum ",
                chorda_aedificator_finire(b));
        }
    }
    _signa_numerare(&cv);
    si (cv.fractus || cv.memoria_defecit)
    {
        salta relatio_scribenda;
    }

    /* (3) elementa (tempus, dynamica, fermatae) */
    _elementa_scribere(&cv, lineae, mensurae, numerus_mensurarum,
        elementa,
        &tempus_initiale, &tempus_adest);
    si (cv.fractus || cv.memoria_defecit)
    {
        salta relatio_scribenda;
    }

    /* commentarium */
    chorda_aedificator_appendere_literis(a, "  <!-- musicxml:");
    si (opus->titulus_operis.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, ' ');
        _commentarium_appendere(a, opus->titulus_operis);
    }
    si (opus->titulus_motus.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, "; ");
        _commentarium_appendere(a, opus->titulus_motus);
    }
    si (opus->compositor.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, "; compositor ");
        _commentarium_appendere(a, opus->compositor);
    }
    si (opus->poeta.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, "; poeta ");
        _commentarium_appendere(a, opus->poeta);
    }
    si (opus->ius.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, "; ius ");
        _commentarium_appendere(a, opus->ius);
    }
    per (v = ZEPHYRUM; v < numerus_mensurarum; v++)
    {
        chorda num = opus->partes[ZEPHYRUM].numerus_mensurarum
            > mensurae[v].mensura
            ? opus->partes[ZEPHYRUM].mensurae[mensurae[v].mensura].numerus
            : _vacua();
        ChordaAedificator* b = chorda_aedificator_creare(piscina, XVI);

        chorda_aedificator_appendere_i32(b, (i32)mensurae[v].numerus);
        si (   mensurae[v].numerus > ZEPHYRUM
            && !chorda_aequalis(chorda_aedificator_finire(b), num))
        {
            renumerata = VERUM;
        }
    }
    si (renumerata)
    {
        chorda_aedificator_appendere_literis(a,
            "\n       mensurae carminis -> scriptae:");
        per (v = ZEPHYRUM; v < numerus_mensurarum; v++)
        {
            si (mensurae[v].numerus == ZEPHYRUM)
            {
                perge;
            }
            chorda_aedificator_appendere_character(a, ' ');
            chorda_aedificator_appendere_i32(a,
                (i32)mensurae[v].numerus);
            chorda_aedificator_appendere_character(a, '=');
            _commentarium_appendere(a,
                opus->partes[ZEPHYRUM].mensurae[mensurae[v].mensura].numerus);
        }
    }
    chorda_aedificator_appendere_literis(a, " -->\n");

    /* caput carminis */
    titulus = _plicare(piscina, forma->titulus.mensura > ZEPHYRUM
                  ? forma->titulus
                  : opus->titulus_operis.mensura > ZEPHYRUM
                        ? opus->titulus_operis : opus->titulus_motus);
    si (titulus.mensura == ZEPHYRUM)
    {
        titulus = chorda_ex_literis("musicxml", piscina);
    }
    chorda_aedificator_appendere_literis(a, "  <carmen titulus=\"");
    chorda_aedificator_appendere_chorda(a, titulus);
    chorda_aedificator_appendere_character(a, '"');
    si (tempus_adest)
    {
        chorda_aedificator_appendere_literis(a, " tempus=\"");
        _s64_scribere(a, tempus_initiale);
        chorda_aedificator_appendere_character(a, '"');
    }
    {
                      s32  n                   = IV;
                      s32  d                   = IV;
                      s32  prior_n             = ZEPHYRUM;
                      s32  prior_d             = ZEPHYRUM;
                      b32  caput               = VERUM;
                      b32  clavis_prior_adest  = FALSUM;
                      s32  quintae_prior       = ZEPHYRUM;
                   chorda  modus_prior         = _vacua();
        ChordaAedificator* mutationes =
            chorda_aedificator_creare(piscina,
            CCLVI);

        per (v = ZEPHYRUM; v < numerus_mensurarum; v++)
        {
            ChordaAedificator* k = chorda_aedificator_creare(piscina,
                XXXII);
                          b32 clavis = FALSUM;

            si (mensurae[v].numerus == ZEPHYRUM)
            {
                perge;
            }
            si (!liberum && !_metrum_mensurae(&mensurae[v], &n, &d))
            {
                _approximatio(&cv, MUSICXML_CARMEN_METRUM_IMPOSSIBILE,
                    opus->partes[ZEPHYRUM].id, opus->partes[ZEPHYRUM]
                        .mensurae[mensurae[v].mensura].numerus,
                    cv.ordo->visitationes[v].transitus, ZEPHYRUM,
                    ZEPHYRUM,
                    "metrum mensurae carmini alienum");
                n = prior_n > ZEPHYRUM ? prior_n : IV;
                d = prior_d > ZEPHYRUM ? prior_d : IV;
            }
            si (   mensurae[v].clavis_tradita
                && (!clavis_prior_adest
                || mensurae[v].quintae != quintae_prior
                    || !chorda_aequalis(mensurae[v].modus,
                    modus_prior)))
            {
                si (_clavem_scribere(k, mensurae[v].quintae,
                    mensurae[v].modus))
                {
                    clavis = VERUM;
                }
                alioquin
                {
                    _approximatio(&cv,
                        MUSICXML_CARMEN_CLAVIS_NON_TRADITA,
                        opus->partes[ZEPHYRUM].id,
                        opus->partes[ZEPHYRUM]
                            .mensurae[mensurae[v].mensura].numerus,
                        cv.ordo->visitationes[v].transitus, ZEPHYRUM,
                        ZEPHYRUM, "modus carmini ignotus (locrius)");
                }
                clavis_prior_adest  = VERUM;
                quintae_prior       = mensurae[v].quintae;
                modus_prior         = mensurae[v].modus;
            }
            si (caput)
            {
                chorda_aedificator_appendere_literis(a, " metrum=\"");
                si (liberum)
                {
                    chorda_aedificator_appendere_literis(a, "liberum");
                }
                alioquin
                {
                    _metrum_scribere(a, n, d);
                }
                chorda_aedificator_appendere_character(a, '"');
                si (clavis)
                {
                    chorda_aedificator_appendere_literis(a,
                        " clavis=\"");
                    chorda_aedificator_appendere_chorda(a,
                        chorda_aedificator_finire(k));
                    chorda_aedificator_appendere_character(a, '"');
                }
                caput = FALSUM;
            }
            alioquin
            {
                si (!liberum && (n != prior_n || d != prior_d))
                {
                    chorda_aedificator_appendere_literis(mutationes,
                        "    <metrum mensura=\"");
                    chorda_aedificator_appendere_i32(mutationes,
                        (i32)mensurae[v].numerus);
                    chorda_aedificator_appendere_literis(mutationes,
                        "\" valor=\"");
                    _metrum_scribere(mutationes, n, d);
                    chorda_aedificator_appendere_literis(mutationes,
                        "\"/>\n");
                }
                si (clavis)
                {
                    chorda_aedificator_appendere_literis(mutationes,
                        "    <clavis mensura=\"");
                    chorda_aedificator_appendere_i32(mutationes,
                        (i32)mensurae[v].numerus);
                    chorda_aedificator_appendere_literis(mutationes,
                        "\" valor=\"");
                    chorda_aedificator_appendere_chorda(mutationes,
                        chorda_aedificator_finire(k));
                    chorda_aedificator_appendere_literis(mutationes,
                        "\"/>\n");
                }
            }
            prior_n = n;
            prior_d = d;
        }
        chorda_aedificator_appendere_literis(a, ">\n");
        chorda_aedificator_appendere_chorda(a,
            chorda_aedificator_finire(mutationes));
    }
    si (cv.fractus || cv.memoria_defecit)
    {
        salta relatio_scribenda;
    }
    chorda_aedificator_appendere_chorda(a,
        chorda_aedificator_finire(elementa));

    /* (4) partes */
    per (i = ZEPHYRUM; i < xar_numerus(lineae); i++)
    {
           constans Linea* l = (constans Linea*)xar_obtinere(lineae, i);
        ChordaAedificator* verba = chorda_aedificator_creare(piscina,
            CCLVI);

        chorda_aedificator_appendere_literis(a, "    <pars titulus=\"");
        chorda_aedificator_appendere_chorda(a, l->titulus);
        chorda_aedificator_appendere_literis(a, "\" instrumentum=\"");
        chorda_aedificator_appendere_chorda(a, l->instrumentum);
        chorda_aedificator_appendere_literis(a,
            "\">\n      <frustum mensura=\"1\"><textus!>");
        si (!_textum_lineae(&cv, l, mensurae, numerus_mensurarum, a))
        {
            _approximatio(&cv, MUSICXML_CARMEN_EXTRA_RETICULUM,
                opus->partes[l->pars].id, _vacua(), I, ZEPHYRUM,
                ZEPHYRUM,
                "longitudo MML dici non potest");
        }
        chorda_aedificator_appendere_literis(a, "</textus>");
        si (verba != NIHIL && _verba_lineae(l, verba))
        {
            chorda_aedificator_appendere_literis(a,
                "\n        <verba!>");
            chorda_aedificator_appendere_chorda(a,
                chorda_aedificator_finire(verba));
            chorda_aedificator_appendere_literis(a, "</verba>");
        }
        chorda_aedificator_appendere_literis(a,
            "</frustum>\n    </pars>\n");
    }
    chorda_aedificator_appendere_literis(a, "  </carmen>\n");

    /* notae structurae */
    si (cv.ordo->numerus != opus->numerus_mensurarum)
    {
        _structura(&cv, "repetitiones evolutae (visitationes ordinis)",
            _vacua());
    }
    si (xar_numerus(lineae) > opus->numerus_partium)
    {
        _structura(&cv, "voces et acorda in partes scissa (M7)",
            _vacua());
    }
    si (scriptura_mutata)
    {
        _structura(&cv, "alterationes duplices enharmonice scriptae",
            _vacua());
    }
    si (syllabae_purgatae > ZEPHYRUM)
    {
        _structura(&cv, "syllabae purgatae (characteres carminis "
            "divisores sublati)", _vacua());
    }
    si (renumerata)
    {
        _structura(&cv, "mensurae carminis renumeratae (commentarium "
            "numeros scriptos dat)", _vacua());
    }

relatio_scribenda:
    relatio->approximationes = (MusicxmlApproximatio*)piscina_allocare(
        piscina, (memoriae_index)((xar_numerus(cv.approximationes) + I)
            * (i32)magnitudo(MusicxmlApproximatio)));
    si (relatio->approximationes != NIHIL)
    {
        relatio->numerus_approximationum = xar_copiare_ad_tabulam(
            cv.approximationes, relatio->approximationes, ZEPHYRUM,
            xar_numerus(cv.approximationes));
    }
    relatio->structurae = (chorda*)piscina_allocare(piscina,
        (memoriae_index)((xar_numerus(cv.structurae) + I)
            * (i32)magnitudo(chorda)));
    si (relatio->structurae != NIHIL)
    {
        relatio->numerus_structurarum = xar_copiare_ad_tabulam(
            cv.structurae, relatio->structurae, ZEPHYRUM,
            xar_numerus(cv.structurae));
    }
    memcpy(relatio->signa, cv.signa, magnitudo(cv.signa));
    si (cv.fractus || cv.memoria_defecit)
    {
        redde FALSUM;
    }
    chorda_aedificator_appendere_chorda(exitus,
        chorda_aedificator_finire(a));
    redde VERUM;
}

constans character*
musicxml_approximationis_titulus (
    MusicxmlApproximatioGenus genus)
{
    commutatio (genus)
    {
        casus MUSICXML_CARMEN_ALTERATIO_FRACTA:   redde "alteratio fracta";
        casus MUSICXML_CARMEN_EXTRA_RETICULUM:    redde "extra reticulum";
        casus MUSICXML_CARMEN_EXTRA_AMBITUM:      redde "extra ambitum";
        casus MUSICXML_CARMEN_NOTAE_SE_TEGUNT:    redde "notae se tegunt";
        casus MUSICXML_CARMEN_GRATIA_OMISSA:      redde "gratia omissa";
        casus MUSICXML_CARMEN_PARVA_OMISSA:       redde "nota parva omissa";
        casus MUSICXML_CARMEN_VERSUS_OMISSUS:     redde "versus omissus";
        casus MUSICXML_CARMEN_TEMPUS_ROTUNDATUM:  redde "tempus rotundatum";
        casus MUSICXML_CARMEN_TEMPUS_EXTRA_FINES: redde "tempus coercitum";
        casus MUSICXML_CARMEN_DYNAMICA_IGNOTA:    redde "dynamica ignota";
        casus MUSICXML_CARMEN_CLIVUS_MOTUS:       redde "clivus motus";
        casus MUSICXML_CARMEN_CLAVIS_NON_TRADITA: redde "clavis non tradita";
        casus MUSICXML_CARMEN_METRUM_ADDITIVUM:   redde "metrum additivum";
        casus MUSICXML_CARMEN_METRUM_IMPOSSIBILE: redde "metrum alienum";
        casus MUSICXML_CARMEN_PERCUSSIO_OMISSA:   redde "percussio omissa";
        ordinarius:                               redde "?";
    }
}

constans character*
musicxml_signi_carminis_titulus (
    MusicxmlSignumCarminisGenus genus)
{
    commutatio (genus)
    {
        casus MUSICXML_CARMEN_ARCUS:             redde "arcus";
        casus MUSICXML_CARMEN_ARTICULATIO:       redde "articulationes";
        casus MUSICXML_CARMEN_ORNAMENTUM:        redde "ornamenta";
        casus MUSICXML_CARMEN_TECHNICUM:         redde "technica";
        casus MUSICXML_CARMEN_ACORDUM_FRACTUM:   redde "acorda fracta";
        casus MUSICXML_CARMEN_DYNAMICA_NOTAE:    redde "dynamica notarum";
        casus MUSICXML_CARMEN_VERBA_DIRECTIONIS: redde "verba directionum";
        casus MUSICXML_CARMEN_EXERCITIUM:        redde "exercitia";
        casus MUSICXML_CARMEN_PEDALE:            redde "pedale";
        casus MUSICXML_CARMEN_OCTAVA_SIGNATA:    redde "octavae signatae";
        casus MUSICXML_CARMEN_DIRECTIO_ALIA:     redde "directiones aliae";
        casus MUSICXML_CARMEN_SIGNUM_ALIUD:      redde "signa alia";
        ordinarius:                              redde "?";
    }
}

b32
musicxml_relationem_scribere (
    constans MusicxmlCarmenRelatio* relatio,
                constans character* via,
                 ChordaAedificator* exitus)
{
    i32 i;

    si (relatio == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    via = via != NIHIL ? via : "";
    per (i = ZEPHYRUM; i < relatio->numerus_approximationum; i++)
    {
        constans MusicxmlApproximatio* x = &relatio->approximationes[i];

        chorda_aedificator_appendere_literis(exitus, via);
        chorda_aedificator_appendere_literis(exitus, ": ");
        chorda_aedificator_appendere_chorda(exitus, x->pars);
        chorda_aedificator_appendere_literis(exitus, " m.");
        chorda_aedificator_appendere_chorda(exitus, x->mensura);
        chorda_aedificator_appendere_literis(exitus, " t.");
        chorda_aedificator_appendere_i32(exitus, x->transitus);
        chorda_aedificator_appendere_character(exitus, ' ');
        chorda_aedificator_appendere_i32(exitus, x->linea);
        chorda_aedificator_appendere_character(exitus, ':');
        chorda_aedificator_appendere_i32(exitus, x->columna);
        chorda_aedificator_appendere_literis(exitus, ": ");
        chorda_aedificator_appendere_literis(exitus,
            musicxml_approximationis_titulus(x->genus));
        chorda_aedificator_appendere_literis(exitus, " - ");
        chorda_aedificator_appendere_chorda(exitus, x->descriptio);
        chorda_aedificator_appendere_character(exitus, '\n');
    }
    per (i = ZEPHYRUM; i < (i32)MUSICXML_CARMEN_SIGNORUM_GENERA; i++)
    {
        si (relatio->signa[i] > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(exitus, via);
            chorda_aedificator_appendere_literis(exitus,
                ": signa non conversa: ");
            chorda_aedificator_appendere_literis(exitus,
                musicxml_signi_carminis_titulus(
                    (MusicxmlSignumCarminisGenus)i));
            chorda_aedificator_appendere_character(exitus, ' ');
            chorda_aedificator_appendere_i32(exitus, relatio->signa[i]);
            chorda_aedificator_appendere_character(exitus, '\n');
        }
    }
    per (i = ZEPHYRUM; i < relatio->numerus_structurarum; i++)
    {
        chorda_aedificator_appendere_literis(exitus, via);
        chorda_aedificator_appendere_literis(exitus,
            ": nota structurae: ");
        chorda_aedificator_appendere_chorda(exitus,
            relatio->structurae[i]);
        chorda_aedificator_appendere_character(exitus, '\n');
    }
    redde VERUM;
}
