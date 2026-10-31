/* musicxml_ordo.c - ordo exsecutionis (caput include/musicxml.h)
 *
 * Ex lineis mensurae (repeat, ending) et directionibus saltus (sound
 * dacapo/dalsegno/tocoda/fine, segno/coda; verba sola cum monito)
 * visitationes (mensura, transitus). Informatio per mensuram ex
 * OMNIBUS partibus colligitur (unio). Mos communis:
 *   - repeat backward sine times: transitus tot quot numerus
 *     conclusionis maximus sequentis (minimum II); sine forward: ab
 *     repetitione priori aut ab initio (mos communis, sine monito);
 *     conclusio (ending start) sine fine: monitum REPETITIO_APERTA;
 *   - conclusio (ending) sonat solum transitu suo;
 *   - D.C./D.S. semel quisque, in fine mensurae; post saltum
 *     repetitiones non iterantur, fine sistit, to coda ad codam salit
 *     (semel);
 *   - transitus = maximum (visitationes mensurae, transitus
 *     repetitionis vigentis): versus n transitu n.
 * Limes visitationum (LXIV per mensuram + MXXIV) ordinem sine fine
 * secat (times ingens) cum monito. Spec par. III, plan P6.
 */

#include "musicxml.h"
#include "xar.h"
#include <string.h>

nomen structura {
     b32 prorsum;          /* repeat forward */
     b32 retrorsum;        /* repeat backward */
     i32 vices;            /* times scriptum (0 = nullum) */
     i32 conclusio_maxima; /* numerus maximus conclusionis (0 = non in
                             * conclusione) */
     i32* conclusiones;     /* numeri conclusionis huius mensurae */
     i32  numerus_conclusionum;
     b32  signum;
  chorda  signum_id;
     b32  cauda;
  chorda  cauda_id;
     b32  a_capite;
     b32  a_signo;
  chorda  a_signo_id;
     b32  ad_caudam;
  chorda  ad_caudam_id;
     b32  finis;
     b32  verbis;            /* saltus quidam verbis solis */
     i32  linea;             /* repetitionis (monita) */
     i32  columna;
     i32  linea_verborum;
     i32  columna_verborum;
     s32  regio_initium;     /* conclusionis huius: mensura prima et
                              * ultima; -I = non in conclusione */
     s32 regio_finis;
     s32 gubernator;        /* backward cuius transitus conclusionem
                              * eligit; -I = nullus */
} Informatio;

nomen structura {
    Piscina* piscina;
        Xar* vitia;
} Ordinator;

interior vacuum
_monitum (
              Ordinator* o,
    MusicxmlVitiumGenus  genus,
                    i32  linea,
                    i32  columna,
     constans character* causa)
{
    MusicxmlVitium* v = (MusicxmlVitium*)xar_addere(o->vitia);

    si (v == NIHIL)
    {
        redde;
    }
    memset(v, ZEPHYRUM, magnitudo(MusicxmlVitium));
    v->gravitas  = MUSICXML_MONITUM;
    v->genus     = genus;
    v->linea     = linea;
    v->columna   = columna;
    v->causa     = chorda_ex_literis(causa, o->piscina);
}

/* verba incipiunt a praefixo (litterae minusculae comparantur) */
interior b32
_incipit (
                 chorda  t,
     constans character* praefixum)
{
    i32 n = (i32)strlen(praefixum);
    i32 i;

    si (t.mensura < n)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        character c = (character)t.datum[i];

        si (c >= 'A' && c <= 'Z')
        {
            c = (character)(c - 'A' + 'a');
        }
        si (c != praefixum[i])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_continet_verba (
                 chorda  t,
     constans character* acus)
{
    i32 i;

    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        si (_incipit(chorda_sectio(t, i, t.mensura), acus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_directiones_colligere (
    constans MusicxmlMensura* m,
                  Informatio* x)
{
    i32 i;

    /* saltus per sound primum (verba eiusdem generis tunc praetereuntur) */
    per (i = ZEPHYRUM; i < m->numerus_directionum; i++)
    {
        constans MusicxmlDirectio* d = &m->directiones[i];

        commutatio (d->genus)
        {
            casus MUSICXML_DIRECTIO_SIGNUM:
                x->signum = VERUM;
                si (d->textus.mensura > ZEPHYRUM)
                {
                    x->signum_id = d->textus;
                }
                frange;
            casus MUSICXML_DIRECTIO_CAUDA:
                x->cauda = VERUM;
                si (d->textus.mensura > ZEPHYRUM)
                {
                    x->cauda_id = d->textus;
                }
                frange;
            casus MUSICXML_DIRECTIO_A_CAPITE:
                x->a_capite = VERUM;
                frange;
            casus MUSICXML_DIRECTIO_A_SIGNO:
                x->a_signo     = VERUM;
                x->a_signo_id  = d->textus;
                frange;
            casus MUSICXML_DIRECTIO_AD_CAUDAM:
                x->ad_caudam     = VERUM;
                x->ad_caudam_id  = d->textus;
                frange;
            casus MUSICXML_DIRECTIO_FINIS:
                x->finis = VERUM;
                frange;
            ordinarius:
                frange;
        }
    }
    per (i = ZEPHYRUM; i < m->numerus_directionum; i++)
    {
         constans MusicxmlDirectio* d         = &m->directiones[i];
                               b32  inventum  = FALSUM;

        si (d->genus != MUSICXML_DIRECTIO_VERBA)
        {
            perge;
        }
        si (   (_incipit(d->textus, "d.c.") || _incipit(d->textus,
                    "da capo"))
            && !x->a_capite)
        {
            x->a_capite  = VERUM;
            inventum     = VERUM;
        }
        alioquin si (   (_incipit(d->textus, "d.s.")
                         || _incipit(d->textus, "dal segno"))
                     && !x->a_signo)
        {
            x->a_signo  = VERUM;
            inventum    = VERUM;
        }
        alioquin si (   _continet_verba(d->textus, "to coda")
                     && !x->ad_caudam)
        {
            x->ad_caudam  = VERUM;
            inventum      = VERUM;
        }
        alioquin si (   _incipit(d->textus, "fine")
                     && d->textus.mensura == IV && !x->finis)
        {
            x->finis = VERUM;
            inventum = VERUM;
        }
        si (inventum)
        {
            x->verbis            = VERUM;
            x->linea_verborum    = d->linea;
            x->columna_verborum  = d->columna;
        }
    }
}

/* regio conclusionis [a, f]: pars prima eam ponit (unio) */
interior vacuum
_regionem_ponere (
    Informatio* x,
           i32  a,
           i32  f)
{
    i32 j;

    per (j = a; j <= f; j++)
    {
        si (x[j].regio_initium < ZEPHYRUM)
        {
            x[j].regio_initium  = (s32)a;
            x[j].regio_finis    = (s32)f;
        }
    }
}

/* informatio per mensuram, unio partium; conclusiones per regionem
 * (ab ending start usque ad stop/discontinue inclusive) */
interior Informatio*
_informationem_colligere (
    constans MusicxmlOpus* opus,
                  Piscina* piscina,
                Ordinator* o)
{
    Informatio* x;
           i32  n = opus->numerus_mensurarum;
           i32  p;
           i32  k;
           i32  b;

    x = (Informatio*)piscina_allocare(piscina,
        (memoriae_index)((n + I) * (i32)magnitudo(Informatio)));
    si (x == NIHIL)
    {
        redde NIHIL;
    }
    memset(x, ZEPHYRUM, (size_t)(n + I) * magnitudo(Informatio));
    per (k = ZEPHYRUM; k <= n; k++)
    {
        x[k].regio_initium  = -I;
        x[k].regio_finis    = -I;
        x[k].gubernator     = -I;
    }
    per (p = ZEPHYRUM; p < opus->numerus_partium; p++)
    {
         constans MusicxmlLineaMensurae* regio             = NIHIL;
                                    s32  initium_regionis  = -I;

        per (k = ZEPHYRUM; k < opus->partes[p].numerus_mensurarum
            && k < n;
             k++)
        {
             constans MusicxmlMensura* m = &opus->partes[p].mensurae[k];
                                  b32  claudit = FALSUM;

            per (b = ZEPHYRUM; b < m->numerus_linearum; b++)
            {
                constans MusicxmlLineaMensurae* l = &m->lineae[b];

                si (l->repetitio == MUSICXML_REPETITIO_PRORSUM)
                {
                    x[k].prorsum = VERUM;
                }
                si (l->repetitio == MUSICXML_REPETITIO_RETRORSUM)
                {
                    x[k].retrorsum = VERUM;
                    si (l->vices > x[k].vices)
                    {
                        x[k].vices = l->vices;
                    }
                    si (x[k].linea == ZEPHYRUM)
                    {
                        x[k].linea    = l->linea;
                        x[k].columna  = l->columna;
                    }
                }
                si (l->conclusio == MUSICXML_CONCLUSIO_INITIUM)
                {
                    regio             = l;
                    initium_regionis  = (s32)k;
                }
                si (   l->conclusio == MUSICXML_CONCLUSIO_FINIS
                    || l->conclusio == MUSICXML_CONCLUSIO_INTERRUPTA)
                {
                    claudit = VERUM;
                }
            }
            si (regio != NIHIL && x[k].numerus_conclusionum == ZEPHYRUM)
            {
                i32 j;

                x[k].conclusiones = regio->conclusiones;
                x[k].numerus_conclusionum =
                    regio->numerus_conclusionum;
                per (j = ZEPHYRUM; j < regio->numerus_conclusionum; j++)
                {
                    si ((i32)regio->conclusiones[j]
                        > x[k].conclusio_maxima)
                    {
                        x[k].conclusio_maxima = regio->conclusiones[j];
                    }
                }
            }
            si (claudit && initium_regionis >= ZEPHYRUM)
            {
                _regionem_ponere(x, (i32)initium_regionis, k);
                initium_regionis = -I;
            }
            si (claudit)
            {
                regio = NIHIL;
            }
            _directiones_colligere(m, &x[k]);
        }
        si (initium_regionis >= ZEPHYRUM && n > ZEPHYRUM)
        {
            _regionem_ponere(x, (i32)initium_regionis, n - I);
        }
        si (regio != NIHIL)
        {
            _monitum(o, MUSICXML_REPETITIO_APERTA, regio->linea,
                regio->columna,
                "conclusio (ending start) sine stop aut discontinue");
        }
    }
    /* gubernator: backward in fine regionis; aliter (conclusio ultima
     * sine backward) gubernator regionis praecedentis contiguae */
    per (k = ZEPHYRUM; k < n; k++)
    {
        s32 f = x[k].regio_finis;
        s32 a = x[k].regio_initium;

        si (a < ZEPHYRUM)
        {
            perge;
        }
        si (x[f].retrorsum)
        {
            x[k].gubernator = f;
        }
        alioquin si (a > ZEPHYRUM && x[a - I].regio_initium >= ZEPHYRUM)
        {
            x[k].gubernator = x[a - I].gubernator;
        }
    }
    redde x;
}

interior b32
_in_conclusione (
    constans Informatio* x,
                    i32  transitus)
{
    i32 j;

    per (j = ZEPHYRUM; j < x->numerus_conclusionum; j++)
    {
        si (x->conclusiones[j] == transitus)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* initium repetitionis: forward eiusdem mensurae; intra conclusionem
 * paria interiora (forward ... backward) praetereuntur (repetitio
 * nidificata, suita 45i) et quaestio ante conclusionem pergit; deinde
 * forward proximum, aut post backward priorem; nihil = ab initio
 * (aperta) */
interior i32
_initium_repetitionis (
    constans Informatio* x,
                    i32  k,
                    b32* aperta)
{
    s32 j = (s32)k - I;

    *aperta = FALSUM;
    si (x[k].prorsum)
    {
        redde k;
    }
    si (x[k].regio_initium >= ZEPHYRUM)
    {
        i32 profunditas = ZEPHYRUM;

        per (; j >= x[k].regio_initium; j--)
        {
            si (x[j].retrorsum)
            {
                profunditas++;
            }
            si (x[j].prorsum)
            {
                si (profunditas == ZEPHYRUM)
                {
                    redde (i32)j;
                }
                profunditas--;
            }
        }
        j = x[k].regio_initium - I;
    }
    per (; j >= ZEPHYRUM; j--)
    {
        si (x[j].retrorsum)
        {
            redde (i32)j + I;
        }
        si (x[j].prorsum)
        {
            redde (i32)j;
        }
    }
    *aperta = VERUM;
    redde ZEPHYRUM;
}

/* vices repetitionis: times scriptum, aliter numerus maximus
 * conclusionum a k sequentium (minimum II) */
interior i32
_vices (
    constans Informatio* x,
                    i32  k,
                    i32  n)
{
    i32 maxima = II;
    i32 j;

    si (x[k].vices > ZEPHYRUM)
    {
        redde x[k].vices;
    }
    /* ex conclusionibus solum si hic backward eas gubernat (finis
     * regionis aut regio statim sequitur) - repetitio interior
     * conclusionis II manet */
    si (   x[k].regio_finis != (s32)k
        && !(k + I < n && x[k + I].regio_initium == (s32)(k + I)))
    {
        redde maxima;
    }
    per (j = k; j < n; j++)
    {
        si (j > k && x[j].numerus_conclusionum == ZEPHYRUM)
        {
            frange;
        }
        si (x[j].conclusio_maxima > maxima)
        {
            maxima = x[j].conclusio_maxima;
        }
    }
    redde maxima;
}

/* mensura saltus: id congruens, aliter prima cum signo; -I si nulla */
interior s32
_metam_invenire (
    constans Informatio* x,
                    i32  n,
                    b32  cauda,
                 chorda  id)
{
    i32 k;
    s32 prima = -I;

    per (k = ZEPHYRUM; k < n; k++)
    {
           b32 est   = cauda ? x[k].cauda : x[k].signum;
        chorda suum  = cauda ? x[k].cauda_id : x[k].signum_id;

        si (!est)
        {
            perge;
        }
        si (id.mensura > ZEPHYRUM && chorda_aequalis(id, suum))
        {
            redde (s32)k;
        }
        si (prima < ZEPHYRUM)
        {
            prima = (s32)k;
        }
    }
    redde prima;
}

MusicxmlOrdo*
musicxml_ordo (
    constans MusicxmlOpus* opus,
                  Piscina* piscina)
{
    MusicxmlOrdo* ordo;
      Informatio* x;
       Ordinator  o;
             Xar* visitationes;
             i32  n;
             i32  k         = ZEPHYRUM;
             i32  r         = I;        /* transitus repetitionis */
             b32  activa    = FALSUM;   /* intra repetitionem */
             b32  perfecta  = FALSUM;   /* backward ultimo transitu */
             b32  reditus   = FALSUM;   /* advenit per saltum retro */
             b32  saltus    = FALSUM;   /* D.C./D.S. factus */
             s64  initium   = ZEPHYRUM;
             i32  limes;
             i32* iteratio;
             i32* visitata;
             i32* adventus;    /* backward attactus (gubernator) */
             i32  transitus_conclusionis;
             b32* capite;
             b32* signo;
             b32* caudam;

    si (opus == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    ordo = (MusicxmlOrdo*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(MusicxmlOrdo));
    si (ordo == NIHIL)
    {
        redde NIHIL;
    }
    memset(ordo, ZEPHYRUM, magnitudo(MusicxmlOrdo));
    n = opus->numerus_mensurarum;
    si (n == ZEPHYRUM)
    {
        redde ordo;
    }
    o.piscina  = piscina;
    o.vitia    = xar_creare(piscina, (i32)magnitudo(MusicxmlVitium));
    visitationes = xar_creare(piscina,
        (i32)magnitudo(MusicxmlVisitatio));
    x = _informationem_colligere(opus, piscina, &o);
    iteratio      = (i32*)piscina_allocare(piscina,
                        (memoriae_index)(n * (i32)magnitudo(i32)));
    visitata      = (i32*)piscina_allocare(piscina,
                        (memoriae_index)(n * (i32)magnitudo(i32)));
    adventus      = (i32*)piscina_allocare(piscina,
                        (memoriae_index)(n * (i32)magnitudo(i32)));
    capite        = (b32*)piscina_allocare(piscina,
                        (memoriae_index)(n * (i32)magnitudo(b32)));
    signo         = (b32*)piscina_allocare(piscina,
                        (memoriae_index)(n * (i32)magnitudo(b32)));
    caudam        = (b32*)piscina_allocare(piscina,
                        (memoriae_index)(n * (i32)magnitudo(b32)));
    si (   o.vitia  == NIHIL || visitationes == NIHIL || x == NIHIL
        || iteratio == NIHIL || visitata == NIHIL || capite == NIHIL
        || signo    == NIHIL || caudam == NIHIL || adventus == NIHIL)
    {
        redde NIHIL;
    }
    memset(iteratio, ZEPHYRUM, (size_t)n * magnitudo(i32));
    memset(visitata, ZEPHYRUM, (size_t)n * magnitudo(i32));
    memset(adventus, ZEPHYRUM, (size_t)n * magnitudo(i32));
    memset(capite, ZEPHYRUM, (size_t)n * magnitudo(b32));
    memset(signo, ZEPHYRUM, (size_t)n * magnitudo(b32));
    memset(caudam, ZEPHYRUM, (size_t)n * magnitudo(b32));
    limes = n * LXIV + MXXIV;

    dum (k < n)
    {
          constans Informatio* y = &x[k];
            MusicxmlVisitatio* v;
                          s64  longitudo;

        si (xar_numerus(visitationes) >= limes)
        {
            _monitum(&o, MUSICXML_SALTUS_CIRCULUS, y->linea, y->columna,
                "ordo exsecutionis nimis longus (secatur): repetitio aut "
                "saltus sine fine");
            frange;
        }
        si (y->prorsum && !reditus)
        {
            r         = I;
            activa    = VERUM;
            perfecta  = FALSUM;
        }
        /* conclusio: transitus repetitionis gubernantis (adventus eius
         * + I), non r - repetitio interior r mutat (45i) */
        transitus_conclusionis = y->gubernator >= ZEPHYRUM
            ? adventus[y->gubernator] + I : (activa ? r : I);
        si (y->numerus_conclusionum > ZEPHYRUM)
        {
            si (!_in_conclusione(y, transitus_conclusionis))
            {
                k++;
                reditus = FALSUM;
                perge;
            }
        }
        alioquin si (   !reditus && activa
                     && (perfecta
                         || (k > ZEPHYRUM
                             && x[k - I].numerus_conclusionum
                                 > ZEPHYRUM)))
        {
            activa    = FALSUM;
            perfecta  = FALSUM;
            r         = I;
        }

        /* visitatio */
        visitata[k]++;
        v = (MusicxmlVisitatio*)xar_addere(visitationes);
        si (v == NIHIL)
        {
            redde NIHIL;
        }
        v->mensura    = k;
        v->transitus  = visitata[k];
        si (y->numerus_conclusionum > ZEPHYRUM)
        {
            si (transitus_conclusionis > v->transitus)
            {
                v->transitus = transitus_conclusionis;
            }
        }
        alioquin si (activa && r > v->transitus)
        {
            v->transitus = r;
        }
        v->initium = initium;
        longitudo  = (k + I < n ? opus->initia[k + I] : opus->longitudo)
                     - opus->initia[k];
        initium += longitudo;
        reditus = FALSUM;

        /* finis mensurae: fine, to coda, repetitio, D.C., D.S. */
        si (saltus && y->finis)
        {
            si (y->verbis)
            {
                _monitum(&o, MUSICXML_SALTUS_SINE_SONO,
                    y->linea_verborum,
                    y->columna_verborum,
                    "saltus verbis solis intellectus (sine sound)");
            }
            frange;
        }
        si (saltus && y->ad_caudam && !caudam[k])
        {
            s32 meta = _metam_invenire(x, n, VERUM, y->ad_caudam_id);

            caudam[k] = VERUM;
            si (meta >= ZEPHYRUM)
            {
                k = (i32)meta;
                perge;
            }
        }
        si (y->retrorsum && !saltus)
        {
            i32 vices = _vices(x, k, n);

            adventus[k]++;
            si (iteratio[k] + I < vices)
            {
                b32 aperta;

                iteratio[k]++;
                r       = iteratio[k] + I;
                activa  = VERUM;
                k       = _initium_repetitionis(x, k, &aperta);
                /* sine forward: ab initio - mos communis, non vitium */
                (vacuum)aperta;
                reditus = VERUM;
                perge;
            }
            iteratio[k]  = ZEPHYRUM;
            perfecta     = VERUM;
        }
        si (y->a_capite && !capite[k])
        {
            capite[k] = VERUM;
            si (y->verbis)
            {
                _monitum(&o, MUSICXML_SALTUS_SINE_SONO,
                    y->linea_verborum,
                    y->columna_verborum,
                    "saltus verbis solis intellectus (sine sound)");
            }
            saltus  = VERUM;
            activa  = FALSUM;
            r       = I;
            k       = ZEPHYRUM;
            perge;
        }
        si (y->a_signo && !signo[k])
        {
            s32 meta = _metam_invenire(x, n, FALSUM, y->a_signo_id);

            signo[k] = VERUM;
            si (meta >= ZEPHYRUM)
            {
                si (y->verbis)
                {
                    _monitum(&o, MUSICXML_SALTUS_SINE_SONO,
                        y->linea_verborum, y->columna_verborum,
                        "saltus verbis solis intellectus (sine sound)");
                }
                saltus  = VERUM;
                activa  = FALSUM;
                r       = I;
                k       = (i32)meta;
                perge;
            }
        }
        k++;
    }
    ordo->visitationes = (MusicxmlVisitatio*)piscina_allocare(piscina,
        (memoriae_index)((xar_numerus(visitationes) + I)
            * (i32)magnitudo(MusicxmlVisitatio)));
    si (ordo->visitationes == NIHIL)
    {
        redde NIHIL;
    }
    ordo->numerus = xar_copiare_ad_tabulam(visitationes,
        ordo->visitationes, ZEPHYRUM, xar_numerus(visitationes));
    ordo->longitudo = initium;
    si (xar_numerus(o.vitia) > ZEPHYRUM)
    {
        ordo->vitia = (MusicxmlVitium*)piscina_allocare(piscina,
            (memoriae_index)(xar_numerus(o.vitia)
                * (i32)magnitudo(MusicxmlVitium)));
        si (ordo->vitia == NIHIL)
        {
            redde NIHIL;
        }
        ordo->numerus_vitiorum = xar_copiare_ad_tabulam(o.vitia,
            ordo->vitia, ZEPHYRUM, xar_numerus(o.vitia));
    }
    redde ordo;
}
