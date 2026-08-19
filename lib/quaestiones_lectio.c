/* GENERATUM a canon_coquere e silva/quaestiones.canon - NOLI MANU MUTARE.
 * Vide caput pro legibus folii. */
#include "quaestiones_lectio.h"

QsQuaestiones*
qs_quaestiones_legere_nodum (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    QsQuaestiones* res;

    (vacuum)intern;
    res = (QsQuaestiones*)piscina_allocare(piscina,
        magnitudo(QsQuaestiones));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus     = nodus;
    res->quaestio  = xar_creare(piscina, (i32)magnitudo(QsQuaestio*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (   !l || l->genus != STML_NODUS_ELEMENTUM
                || !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "quaestio"))
            {
                QsQuaestio*  filius;
                QsQuaestio** locus;

                filius = qs_quaestio_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus   = (QsQuaestio**)xar_addere(res->quaestio);
                    *locus  = filius;
                }
            }
        }
    }

    redde res;
}

QsQuaestiones*
qs_quaestiones_legere (
                 chorda  fons,
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda* causa)
{
    StmlResultus r;

    si (causa)
    {
        causa->datum    = NIHIL;
        causa->mensura  = ZEPHYRUM;
    }

    r = stml_legere(fons, piscina, intern);
    si (!r.successus || !r.elementum_radix)
    {
        si (causa)
        {
            *causa = chorda_ex_literis(
                "stml parsari nequit", piscina);
        }
        redde NIHIL;
    }
    si (   !r.elementum_radix->titulus
        || !chorda_aequalis_literis(*r.elementum_radix->titulus,
                                 "quaestiones"))
    {
        si (causa)
        {
            *causa = chorda_ex_literis(
                "radix non est <quaestiones>", piscina);
        }
        redde NIHIL;
    }

    redde qs_quaestiones_legere_nodum(r.elementum_radix, piscina,
        intern);
}

QsQuaestio*
qs_quaestio_legere_nodum (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    QsQuaestio* res;

    (vacuum)intern;
    res = (QsQuaestio*)piscina_allocare(piscina,
        magnitudo(QsQuaestio));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->titulus = stml_attributum_capere(nodus, "titulus");

    res->gradus = QS_QUAESTIO_GRADUS_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "gradus");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "oculi"))
            {
                res->gradus = QS_QUAESTIO_GRADUS_OCULI;
            }
            alioquin si (chorda_aequalis_literis(*v, "invarians"))
            {
                res->gradus = QS_QUAESTIO_GRADUS_INVARIANS;
            }
        }
    }
    res->selector = NIHIL;
    res->causa = NIHIL;
    res->parametrum = xar_creare(piscina, (i32)magnitudo(QsParametrum*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (   !l || l->genus != STML_NODUS_ELEMENTUM
                || !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "selector"))
            {
                si (!res->selector)
                {
                    res->selector =
                        qs_selector_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                         "causa"))
            {
                si (!res->causa)
                {
                    res->causa =
                        qs_causa_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                         "parametrum"))
            {
                QsParametrum*  filius;
                QsParametrum** locus;

                filius = qs_parametrum_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (QsParametrum**)xar_addere(res->parametrum);
                    *locus = filius;
                }
            }
        }
    }

    redde res;
}

QsSelector*
qs_selector_legere_nodum (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    QsSelector* res;

    (vacuum)intern;
    res = (QsSelector*)piscina_allocare(piscina,
        magnitudo(QsSelector));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;
    res->textus = chorda_praecidere(
        stml_textus_normalizatus(nodus, piscina));

    redde res;
}

QsCausa*
qs_causa_legere_nodum (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    QsCausa* res;

    (vacuum)intern;
    res = (QsCausa*)piscina_allocare(piscina,
        magnitudo(QsCausa));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;
    res->textus = chorda_praecidere(
        stml_textus_normalizatus(nodus, piscina));

    redde res;
}

QsParametrum*
qs_parametrum_legere_nodum (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    QsParametrum* res;

    (vacuum)intern;
    res = (QsParametrum*)piscina_allocare(piscina,
        magnitudo(QsParametrum));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->titulus = stml_attributum_capere(nodus, "titulus");

    redde res;
}
