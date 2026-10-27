/* fabrica_sanare.c - sanare: iudicare, agere, iterum iudicare -
 * ante/post agere, membrum agere, auditus, scripturae notatae, undae
 * sanandi, fabrica_sanare. Pars bibliothecae fabrica (fabrica-6 H2;
 * vide fabrica.c). */

#include "fabrica.h"
#include "fabrica_interna.h"
#include "numerus_romanus.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Sanare (plan 1b T3): iudicare, agere, iterum iudicare
 * ================================================== */

/* status interni actionis per cursum */
nomen enumeratio {
    SANANDI_NONDUM = ZEPHYRUM,
    SANANDI_RECENS,
    SANANDI_SANATUM,
    SANANDI_PRAEPARATUM,
    SANANDI_FRACTUM,
    SANANDI_OMISSUM,
    SANANDI_AGENDUM,
    SANANDI_FORTASSE,
    SANANDI_AUDITUM     /* iudicium RECENS tamen agendum (auditus) */
} StatusSanandi;

/* VERUM si omnes exitus strategiae non iudicatae (ignota): actio
 * praecondicione SOLA realizatur */
interior b32
_actio_ignota (
    constans FabricaActio* actio)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(actio->exitus); i++)
    {
        si (((FabricaExitus*)xar_obtinere(actio->exitus,
                i))->strategia->iudicatur)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* exitus iudicati actionis NUNC (plenus): VERUM si omnes RECENS;
 * aliter causa = primus non RECENS "via: causa" */
interior b32
_exitus_recentes (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                    chorda* causa_out)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(actio->exitus); i++)
    {
        constans FabricaExitus* exitus;
               FabricaIudicium  iudicium;

        exitus = (FabricaExitus*)xar_obtinere(actio->exitus, i);
        si (!exitus->strategia->iudicatur)
        {
            perge;
        }
        iudicium = fabrica_iudicare(sutura, actio, exitus, VERUM,
            piscina);
        si (iudicium.status != FABRICA_RECENS)
        {
            *causa_out = fabricae_iungere(piscina, "",
                fabricae_iungere(piscina, "", exitus->via, ": "),
                chorda_ut_cstr(iudicium.causa, piscina));
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* memoriae per cursum descriptiones octetorum sunt quae actum modo
 * mutavit (Review Focus 2): post quodque actum vacantur; regenerationes
 * solum si 'regenerationes_quoque' (membrum intra locum suum: eas
 * servat - _membrum_agere) */
interior vacuum
_memorias_vacare (
    constans FabricaSutura* sutura,
                       b32  regenerationes_quoque)
{
    si (sutura->sigilla != NIHIL)
    {
        tabula_dispersa_vacare(sutura->sigilla);
    }
    si (regenerationes_quoque && sutura->regenerationes != NIHIL)
    {
        tabula_dispersa_vacare(sutura->regenerationes);
    }
    si (sutura->digesta != NIHIL)
    {
        tabula_dispersa_vacare(sutura->digesta);
    }
}

/* agere per suturam; VERUM si codex 0. causa: "exitus N: cauda" aut
 * cauda (non incepit) */
/* VERDICTUM EX ACTIS (plan 5 T3): signum (praefixum litterale) in actis
 * cursoris sine sequentiis ANSI quaeritur (prima occurrentia, ut
 * re.search porta() silvae); compendium = signum + spatia + verbum
 * sequens (usque ad spatium) - octeti deterministici etiam si cursor
 * tempora appendit. FRACTUM: acta illegibilia, signum absens, 'FRACT'
 * (aut 'Fracti:'/'Failed:' cum numero non nullo) in compendio.
 * Verdictum '<porta>: <compendium>' (porta = titulus sine 'porta_')
 * atomice. */
interior b32
_verdictum_ex_actis (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
        constans character* acta_via,
                   Piscina* piscina,
                    chorda* causa_out)
{
    constans FabricaExitus* exitus = (constans FabricaExitus*)
        xar_obtinere(actio->exitus, ZEPHYRUM);
                    chorda  acta;
                    chorda  compendium;
                    chorda  porta;
                    chorda  verdictum;
                 character* area;
                 character* signum;
        constans character* inventum;
        constans character* finis;
                       i32  i;
                       i32  j = ZEPHYRUM;

    si (!sutura->legere(sutura->datum, acta_via, piscina, &acta))
    {
        *causa_out = fabricae_iungere(piscina, "acta illegibilia: ",
            chorda_ex_literis(acta_via, piscina), "");
        redde FALSUM;
    }
    area = (character*)piscina_allocare(piscina,
        (memoriae_index)acta.mensura + I);
    si (area == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < acta.mensura; i++)
    {
        character c = (character)acta.datum[i];

        si (   c                 == '\033' && i + I < acta.mensura
            && acta.datum[i + I] == '[')
        {
            /* ESC [ ... littera: sequentia ANSI omittitur */
            i += II;
            dum (   i < acta.mensura
                 && !(   (acta.datum[i] >= 'A' && acta.datum[i] <= 'Z')
                      || (acta.datum[i] >= 'a'
                          && acta.datum[i] <= 'z')))
            {
                i++;
            }
            perge;
        }
        area[j++] = c == '\0' ? ' ' : c;
    }
    area[j]   = '\0';
    signum    = chorda_ut_cstr(actio->signum, piscina);
    inventum  = strstr(area, signum);
    si (inventum == NIHIL)
    {
        *causa_out = fabricae_iungere(piscina, "signum absens: ",
            actio->signum,
            "");
        redde FALSUM;
    }
    /* signum, spatia sequentia, verbum unum (signum spatio finali non
     * eget: attributum STML spatium finale formatione amitteret) */
    finis = inventum + strlen(signum);
    dum (*finis == ' ' || *finis == '\t')
    {
        finis++;
    }
    dum (   *finis != '\0' && *finis != ' ' && *finis != '\t'
         && *finis != '\n' && *finis != '\r')
    {
        finis++;
    }
    compendium = chorda_ex_literis("", piscina);
    compendium = chorda_concatenare(compendium, chorda_sectio(
        chorda_ex_literis(inventum, piscina), ZEPHYRUM,
        (i32)(finis - inventum)), piscina);
    {
        constans character* c = chorda_ut_cstr(compendium, piscina);
        constans character* f = strstr(c, "Fracti:");
        constans character* g = strstr(c, "Failed:");

        si (   strstr(c, "FRACT") != NIHIL
            || (f != NIHIL && strpbrk(f, "123456789") != NIHIL)
            || (g != NIHIL && strpbrk(g, "123456789") != NIHIL))
        {
            *causa_out = fabricae_iungere(piscina,
                "compendium fractum: ",
                compendium, "");
            redde FALSUM;
        }
    }
    porta = actio->titulus;
    si (   porta.mensura > VI
        && memcmp(porta.datum, "porta_", VI) == ZEPHYRUM)
    {
        porta = chorda_sectio(porta, VI, porta.mensura);
    }
    verdictum = fabricae_iungere(piscina, "", porta, ": ");
    verdictum = chorda_concatenare(verdictum, compendium, piscina);
    verdictum = fabricae_iungere(piscina, "", verdictum, "\n");
    si (!sutura->verdictum_ponere(sutura->datum,
            chorda_ut_cstr(exitus->via, piscina), &verdictum))
    {
        *causa_out = fabricae_iungere(piscina,
            "verdictum scribi nequit: ",
            exitus->via, "");
        redde FALSUM;
    }
    redde VERUM;
}

/* "scripsit extra vestigium: a, b, c +N" (III nominatae, ut
 * iudicia_coniungere) */
interior chorda
_extra_nominare (
    constans Xar* extra,
         Piscina* piscina)
{
    chorda nuntius;
       i32 k;

    nuntius = chorda_ex_literis("scripsit extra vestigium: ", piscina);
    per (k = ZEPHYRUM; k < xar_numerus(extra) && k < III; k++)
    {
        si (k > 0)
        {
            nuntius = fabricae_iungere(piscina, "", nuntius, ", ");
        }
        nuntius = chorda_concatenare(nuntius,
            *(constans chorda*)xar_obtinere(extra, k), piscina);
    }
    si (xar_numerus(extra) > III)
    {
        nuntius = fabricae_iungere(piscina, "", nuntius, " +");
        nuntius = chorda_concatenare(nuntius, numerus_romanus_exprimere(
            (i64)(xar_numerus(extra) - III), NIHIL, piscina), piscina);
    }
    redde nuntius;
}

/* lectiones E libri (nomina; valor neglectus): Xar de FabricaLectio,
 * vacua si liber absens */
interior Xar*
_lectiones_ambitus_libri (
    constans FabricaSutura* sutura,
        constans character* liber_via,
                   Piscina* piscina)
{
    chorda  liber;
       Xar* lectiones;
       i32  i;
       i32  initium;

    lectiones = xar_creare(piscina, (i32)magnitudo(FabricaLectio));
    si (   lectiones == NIHIL
        || !sutura->legere(sutura->datum, liber_via, piscina, &liber))
    {
        redde lectiones;
    }
    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; i < liber.mensura; i++)
    {
               chorda  linea;
               chorda  titulus;
                  s32  t;
        FabricaLectio* lectio;

        si (liber.datum[i] != '\n')
        {
            perge;
        }
        linea    = chorda_sectio(liber, initium, i);
        initium  = i + I;
        si (   linea.mensura < III || linea.datum[0] != 'E'
            || linea.datum[I] != '\t')
        {
            perge;
        }
        titulus  = chorda_sectio(linea, II, linea.mensura);
        t        = chorda_invenire_index(titulus,
            chorda_ex_literis("\t",
            piscina));
        si (t >= ZEPHYRUM)
        {
            titulus = chorda_sectio(titulus, ZEPHYRUM, (i32)t);
        }
        lectio = (FabricaLectio*)xar_addere(lectiones);
        si (lectio != NIHIL)
        {
            lectio->genus  = LECTIO_AMBITUS;
            lectio->via    = titulus;
            memset(&lectio->sigillum, ZEPHYRUM, magnitudo(Sigillum));
        }
    }
    redde lectiones;
}

/* MUTATA PHOTOGRAPHIARUM: viae quae inter 'ante' et 'post' (ordinatae
 * per viam) apparuerunt, evanuerunt aut tempus/mensuram mutaverunt.
 * Xar de chorda (ordine viae); NIHIL si memoria deficit */
interior Xar*
_mutata_photographiarum (
    constans Xar* ante,
    constans Xar* post,
         Piscina* piscina)
{
    Xar* mutata;
    i32  i;
    i32  j;

    mutata = fabricae_xar_chordarum(piscina);
    si (mutata == NIHIL)
    {
        redde NIHIL;
    }
    i = ZEPHYRUM;
    j = ZEPHYRUM;
    dum (i < xar_numerus(ante) || j < xar_numerus(post))
    {
        constans FabricaVestigium* x = i < xar_numerus(ante)
            ? (constans FabricaVestigium*)xar_obtinere(ante, i) : NIHIL;
        constans FabricaVestigium* y = j < xar_numerus(post)
            ? (constans FabricaVestigium*)xar_obtinere(post, j) : NIHIL;
                              s32 ordo;

        ordo = x == NIHIL ? I : y == NIHIL ? -I
            : chorda_comparare(x->via, y->via);
        si (ordo < ZEPHYRUM)
        {
            fabricae_chordam_addere(mutata, x->via);
            i++;
        }
        alioquin si (ordo > ZEPHYRUM)
        {
            fabricae_chordam_addere(mutata, y->via);
            j++;
        }
        alioquin
        {
            si (   x->tempus_ns != y->tempus_ns
                || x->mensura   != y->mensura)
            {
                fabricae_chordam_addere(mutata, x->via);
            }
            i++;
            j++;
        }
    }
    redde mutata;
}

/* DEBITA NOTARE (fabrica-7 T3): post cursum membri cum debitis -
 * <area>debita.txt: "cursus\ttransiit|fractus", deinde viae debitorum
 * quas membrum USURPAVIT - photographiae ante/post mutatas ostendunt
 * aut liber lectionum nominat (fabrica_debitum_iudicare eam legit).
 * Sine photographia nihil scribitur (IGNOTUM). */
interior vacuum
_debita_notare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                    chorda  area,
              constans Xar* ante,
              constans Xar* post,
                       b32  transiit,
                   Piscina* piscina)
{
    ChordaAedificator* a;
                  Xar* mutata;
               chorda  contentum;
                  i32  i;
                  i32  d;

    si (   actio->debita              == NIHIL
        || xar_numerus(actio->debita) == ZEPHYRUM)
    {
        redde;
    }
    mutata  = _mutata_photographiarum(ante, post, piscina);
    a       = chorda_aedificator_creare(piscina, CCLVI);
    si (mutata == NIHIL || a == NIHIL)
    {
        redde;
    }
    /* LIBER LECTIONUM quoque (fabrica-7 T4): scriptura TRANSIENS
     * (creata et deleta intra cursum - sqlite, sera, scripta ficta)
     * photographias fallit; via in libro nominata (S scripsit, L/X/A
     * legit aut quaesivit) = usus debiti. Lineae E (ambitus) omissae */
    {
        chorda liber;
           i32 initium;
           i32 k;

        si (   sutura->legere != NIHIL
            && sutura->legere(sutura->datum, chorda_ut_cstr(
                   fabrica_liber_via(actio->titulus, piscina), piscina),
                   piscina, &liber))
        {
            initium = ZEPHYRUM;
            per (k = ZEPHYRUM; k <= liber.mensura; k++)
            {
                chorda versus;
                chorda via;

                si (k < liber.mensura && liber.datum[k] != '\n')
                {
                    perge;
                }
                versus   = chorda_sectio(liber, initium, k);
                initium  = k + I;
                si (   versus.mensura < III || versus.datum[I] != '\t'
                    || versus.datum[0] == 'E'
                    || versus.datum[0] == 'P')
                {
                    perge;   /* E ambitus, P exsecutio: non debita */
                }
                via = chorda_sectio(versus, II, versus.mensura);
                si (   sutura->radix.mensura > 0
                    && via.mensura > sutura->radix.mensura
                    && chorda_incipit(via, sutura->radix)
                    && via.datum[sutura->radix.mensura] == '/')
                {
                    via = chorda_sectio(via, sutura->radix.mensura + I,
                        via.mensura);
                }
                fabricae_chordam_addere(mutata, via);
            }
        }
    }
    (vacuum)chorda_aedificator_appendere_literis(a, transiit
        ? "cursus\ttransiit\n" : "cursus\tfractus\n");
    per (d = ZEPHYRUM; d < xar_numerus(actio->debita); d++)
    {
        constans FabricaDebitum* debitum = (constans FabricaDebitum*)
            xar_obtinere(actio->debita, d);
                            Xar* locus;
                            b32  scriptum = FALSUM;

        locus = xar_creare(piscina, (i32)magnitudo(FabricaLocus));
        si (locus == NIHIL)
        {
            redde;
        }
        fabricae_locum_debiti_addere(locus, debitum, piscina);
        per (i = ZEPHYRUM; i < xar_numerus(mutata) && !scriptum; i++)
        {
            scriptum = fabricae_in_locis(locus, *(chorda*)xar_obtinere(
                mutata, i), piscina);
        }
        si (scriptum)
        {
            (vacuum)chorda_aedificator_appendere_chorda(a,
                debitum->via);
            (vacuum)chorda_aedificator_appendere_character(a, '\n');
        }
    }
    contentum = chorda_aedificator_finire(a);
    (vacuum)sutura->verdictum_ponere(sutura->datum, chorda_ut_cstr(
        fabricae_iungere(piscina, "", area, "debita.txt"), piscina),
        &contentum);
}

/* via in loco actionis: in area sua (membra; area vacua = nulla), aut
 * liber lectionum aut acta sua (fabrica ea pro actione scribit) */
interior b32
_via_membri (
    constans FabricaActio* actio,
                   chorda  area,
                   chorda  via,
                  Piscina* piscina)
{
    redde (area.mensura > ZEPHYRUM && chorda_incipit(via, area))
        || chorda_aequalis(via, fabrica_liber_via(actio->titulus,
               piscina))
        || chorda_aequalis(via, fabrica_acta_via(actio->titulus,
               piscina));
}

/* MUTATA INTRA LOCUM (2026-10-23; fabrica.h): locus = _via_membri */
b32
fabrica_mutata_intra_locum (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                    chorda  area,
              constans Xar* ante,
              constans Xar* post,
                   Piscina* piscina)
{
    chorda liber;
       i32 i;
       i32 j;
       i32 initium;
       i32 k;

    si (ante == NIHIL || post == NIHIL || sutura->legere == NIHIL)
    {
        redde FALSUM;
    }
    i = ZEPHYRUM;
    j = ZEPHYRUM;
    dum (i < xar_numerus(ante) || j < xar_numerus(post))
    {
        constans FabricaVestigium* x = i < xar_numerus(ante)
            ? (constans FabricaVestigium*)xar_obtinere(ante, i) : NIHIL;
        constans FabricaVestigium* y = j < xar_numerus(post)
            ? (constans FabricaVestigium*)xar_obtinere(post, j) : NIHIL;
                              s32 ordo;
                           chorda mutatum;

        ordo = x == NIHIL ? I : y == NIHIL ? -I
            : chorda_comparare(x->via, y->via);
        mutatum.datum    = NIHIL;
        mutatum.mensura  = ZEPHYRUM;
        si (ordo < ZEPHYRUM)
        {
            mutatum = x->via;
            i++;
        }
        alioquin si (ordo > ZEPHYRUM)
        {
            mutatum = y->via;
            j++;
        }
        alioquin
        {
            si (   x->tempus_ns != y->tempus_ns
                || x->mensura   != y->mensura)
            {
                mutatum = x->via;
            }
            i++;
            j++;
        }
        si (   mutatum.mensura > ZEPHYRUM
            && !_via_membri(actio, area, mutatum, piscina))
        {
            redde FALSUM;
        }
    }
    si (!sutura->legere(sutura->datum, chorda_ut_cstr(
            fabrica_liber_via(actio->titulus, piscina), piscina),
            piscina, &liber))
    {
        redde VERUM;
    }
    initium = ZEPHYRUM;
    per (k = ZEPHYRUM; k <= liber.mensura; k++)
    {
        chorda versus;
        chorda via;

        si (k < liber.mensura && liber.datum[k] != '\n')
        {
            perge;
        }
        versus   = chorda_sectio(liber, initium, k);
        initium  = k + I;
        si (   versus.mensura < III || versus.datum[0] != 'S'
            || versus.datum[I] != '\t')
        {
            perge;
        }
        via = chorda_sectio(versus, II, versus.mensura);
        si (   sutura->radix.mensura > 0
            && via.mensura > sutura->radix.mensura
            && chorda_incipit(via, sutura->radix)
            && via.datum[sutura->radix.mensura] == '/')
        {
            via = chorda_sectio(via, sutura->radix.mensura + I,
                via.mensura);
        }
        si (!_via_membri(actio, area, via, piscina))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_viam_continet (
    constans Xar* viae,
          chorda  via);

interior Xar*
_scripturas_notatas (
    constans FabricaSutura* sutura,
        constans character* liber_via,
                   Piscina* piscina);

/* VESTIGIA CONFERRE (fabrica-8 mensura; sutura->vestigia_conferre):
 * mutata photographiarum EXTRA locum membri (area, liber, acta) contra
 * scripturas libri (S) extra eundem. <area>vestigia_collata.txt: linea
 * 'cursus' semper, deinde 'photographia\t<via>' = mutatum quod liber
 * non nominat (scriptura librum fallens - lacuna vestigii fidelis) et
 * 'liber\t<via>' = notatum quod photographia non vidit (transiens aut
 * status idem). Iudicium membri non mutatur: mensura sola. */
interior vacuum
_vestigia_conferre (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                    chorda  area,
              constans Xar* ante,
              constans Xar* post,
                   Piscina* piscina)
{
    ChordaAedificator* a;
                  Xar* mutata;
                  Xar* notatae;
               chorda  contentum;
                  i32  k;

    mutata   = _mutata_photographiarum(ante, post, piscina);
    notatae  = _scripturas_notatas(sutura, chorda_ut_cstr(
        fabrica_liber_via(actio->titulus, piscina), piscina), piscina);
    a        = chorda_aedificator_creare(piscina, CCLVI);
    si (mutata == NIHIL || notatae == NIHIL || a == NIHIL)
    {
        redde;
    }
    (vacuum)chorda_aedificator_appendere_literis(a, "cursus\n");
    per (k = ZEPHYRUM; k < xar_numerus(mutata); k++)
    {
        chorda via = *(chorda*)xar_obtinere(mutata, k);

        si (   !_via_membri(actio, area, via, piscina)
            && !_viam_continet(notatae, via))
        {
            (vacuum)chorda_aedificator_appendere_literis(a,
                "photographia\t");
            (vacuum)chorda_aedificator_appendere_chorda(a, via);
            (vacuum)chorda_aedificator_appendere_character(a, '\n');
        }
    }
    per (k = ZEPHYRUM; k < xar_numerus(notatae); k++)
    {
        chorda via = *(chorda*)xar_obtinere(notatae, k);

        si (   !_via_membri(actio, area, via, piscina)
            && !_viam_continet(mutata, via))
        {
            (vacuum)chorda_aedificator_appendere_literis(a, "liber\t");
            (vacuum)chorda_aedificator_appendere_chorda(a, via);
            (vacuum)chorda_aedificator_appendere_character(a, '\n');
        }
    }
    contentum = chorda_aedificator_finire(a);
    (vacuum)sutura->verdictum_ponere(sutura->datum, chorda_ut_cstr(
        fabricae_iungere(piscina, "", area, "vestigia_collata.txt"),
        piscina), &contentum);
}

/* MEMBRUM AGERE (T5): verdictum vetus deletum, area parata et vacua,
 * ambitus basis, genus gradus agit; deinde scriptura extra vestigium,
 * ambitus non declaratus -> FALSUM nominatum; transitus -> verdictum
 * '<id>: transiit' a fabrica scriptum (post photographiam) */
/* SCRIPTURAE TRANSIENTES (quaestio …4GY9MJ4): photographia ante/post
 * plagulam intra cursum creatam et deletam non videt - liber lectionum
 * (lineae S) eam videt. Cursus seriales libro recenti (membra gradus,
 * actiones iudicii) ita iudicantur ut undae iam iudicabantur. Restat
 * scriptura sine libro (system, sqlite, filii sine lectionibus). */
interior chorda
_scripturam_alienam (
     constans FabricaActio* actio,
              constans Xar* notatae,
                   Piscina* piscina);

/* horologium suturae (ms); ZEPHYRUM sine eo (sutura ficta) */
interior s64
_momentum_ms (
    constans FabricaSutura* sutura)
{
    redde sutura->momentum_ms != NIHIL
        ? sutura->momentum_ms(sutura->datum) : (s64)ZEPHYRUM;
}

interior b32
_membrum_agere (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                       i32* duratio_out,
                    chorda* causa_out)
{
    constans FabricaExitus* exitus;
              FabricaActum  actum;
                    chorda  area;
                    chorda  verdictum;
                       Xar* ambitus;
                       Xar* ante;
                       Xar* post;
                       Xar* alieni;
                       b32  incepit;
                 character  numerus[XXXII];
                       s64  photographia_ms;
                       s64  secuta_ms;
                       s64  t0;
                       Xar* ante_secuta;
                       Xar* post_secuta;

    *duratio_out = ZEPHYRUM;
    si (   actio->gradus == NIHIL || actio->gradus->agere == NIHIL
        || sutura->area_parare == NIHIL
        || sutura->in_area_currere == NIHIL
        || sutura->verdictum_ponere == NIHIL)
    {
        *causa_out = chorda_ex_literis("gradus agi nequit (sutura sine "
            "area_parare, in_area_currere aut verdictum_ponere)",
            piscina);
        redde FALSUM;
    }
    exitus = (constans FabricaExitus*)xar_obtinere(actio->exitus,
        ZEPHYRUM);
    (vacuum)sutura->verdictum_ponere(sutura->datum,
        chorda_ut_cstr(exitus->via, piscina), NIHIL);
    area = fabricae_area_membri(actio, piscina);
    si (!sutura->area_parare(sutura->datum, chorda_ut_cstr(area,
            piscina)))
    {
        *causa_out = fabricae_iungere(piscina, "area parari nequit: ",
            area,
            "");
        redde FALSUM;
    }
    ambitus = fabrica_ambitum_basis(sutura, actio, chorda_ut_cstr(area,
        piscina), piscina);
    si (ambitus == NIHIL)
    {
        *causa_out = chorda_ex_literis("memoria deficit (ambitus)",
            piscina);
        redde FALSUM;
    }
    ante             = NIHIL;
    post             = NIHIL;
    ante_secuta      = NIHIL;
    post_secuta      = NIHIL;
    photographia_ms  = ZEPHYRUM;
    secuta_ms        = ZEPHYRUM;
    si (sutura->vestigium_capere != NIHIL)
    {
        t0 = _momentum_ms(sutura);
        (vacuum)sutura->vestigium_capere(sutura->datum, piscina, &ante);
        photographia_ms += _momentum_ms(sutura) - t0;
    }
    /* T2: testis secutorum (T3 eum in umbra confert) */
    si (sutura->secuta_capere != NIHIL)
    {
        t0 = _momentum_ms(sutura);
        (vacuum)sutura->secuta_capere(sutura->datum, piscina,
            &ante_secuta);
        secuta_ms += _momentum_ms(sutura) - t0;
    }
    actum.codex       = -I;
    actum.duratio_ms  = ZEPHYRUM;
    actum.cauda       = chorda_ex_literis("", piscina);
    incepit = actio->gradus->agere(sutura, actio, actio->membrum,
        chorda_ut_cstr(area, piscina), ambitus, piscina, &actum);
    *duratio_out = actum.duratio_ms;
    si (!incepit)
    {
        _memorias_vacare(sutura, VERUM);
        *causa_out = fabricae_iungere(piscina, "non actum: ",
            actum.cauda, "");
        redde FALSUM;
    }
    /* photographia post STATIM (fabrica-7 T3): debita notantur etiam
     * cursu fracto (fractura debitum mortuum non probat) */
    t0 = _momentum_ms(sutura);
    si (   ante != NIHIL
        && !sutura->vestigium_capere(sutura->datum, piscina, &post))
    {
        post = NIHIL;
    }
    photographia_ms += _momentum_ms(sutura) - t0;
    si (ante_secuta != NIHIL)
    {
        t0 = _momentum_ms(sutura);
        si (!sutura->secuta_capere(sutura->datum, piscina,
                &post_secuta))
        {
            post_secuta = NIHIL;
        }
        secuta_ms += _momentum_ms(sutura) - t0;
    }
    si (   (ante != NIHIL || ante_secuta != NIHIL)
        && sutura->momentum_ms          != NIHIL
        && sutura->photographiam_notare != NIHIL)
    {
        sutura->photographiam_notare(sutura->datum, actio->titulus,
            ante != NIHIL ? (s32)photographia_ms : -I,
            ante_secuta != NIHIL ? (s32)secuta_ms : -I);
    }
    si (sutura->vestigia_conferre && post != NIHIL)
    {
        _vestigia_conferre(sutura, actio, area, ante, post, piscina);
    }
    /* MEMORIAE (2026-10-23): sigilla et digesta octetos describunt -
     * semper vacantur (area membri mutata est). Regenerationes
     * (generatores semel per cursum) solum si membrum extra locum suum
     * aliquid mutavit: ingressus generatorum in arbore sunt, numquam in
     * area membri aut libris fabricae. Olim post QUODQUE membrum -
     * lectores_cocti, tabulae_silvae et ceteri bis per cursum radicis
     * regenerabantur (~V s cursu sine mutatione). */
    _memorias_vacare(sutura, !fabrica_mutata_intra_locum(sutura, actio,
        area, ante, post, piscina));
    si (post != NIHIL)
    {
        _debita_notare(sutura, actio, area, ante, post, actum.codex
            == 0,
            piscina);
    }
    si (actum.codex != 0)
    {
        sprintf(numerus, "exitus %d: ", (integer)actum.codex);
        *causa_out = fabricae_iungere(piscina, numerus, actum.cauda,
            "");
        redde FALSUM;
    }
    si (post != NIHIL)
    {
        Xar* extra;

        extra = fabrica_vestigia_comparare(actio, ante, post, piscina);
        si (extra != NIHIL && xar_numerus(extra) > 0)
        {
            *causa_out = _extra_nominare(extra, piscina);
            redde FALSUM;
        }
    }
    /* transientes per librum (lineae S): photographia eas non videt */
    {
        chorda aliena = _scripturam_alienam(actio, _scripturas_notatas(
            sutura, chorda_ut_cstr(fabrica_liber_via(actio->titulus,
            piscina), piscina), piscina), piscina);

        si (aliena.mensura > ZEPHYRUM)
        {
            *causa_out = fabricae_iungere(piscina,
                "scripsit extra vestigium (S, liber lectionum): ",
                aliena, "");
            redde FALSUM;
        }
    }
    alieni = fabrica_ambitum_non_declaratum(actio,
        _lectiones_ambitus_libri(sutura, chorda_ut_cstr(
            fabrica_liber_via(actio->titulus, piscina), piscina),
            piscina), piscina);
    si (alieni != NIHIL && xar_numerus(alieni) > 0)
    {
        chorda nuntius;
           i32 k;

        nuntius = chorda_ex_literis("ambitus non declaratus: ",
            piscina);
        per (k = ZEPHYRUM; k < xar_numerus(alieni); k++)
        {
            si (k > 0)
            {
                nuntius = fabricae_iungere(piscina, "", nuntius, ", ");
            }
            nuntius = chorda_concatenare(nuntius,
                *(chorda*)xar_obtinere(alieni, k), piscina);
        }
        *causa_out = fabricae_iungere(piscina, "", nuntius,
            " (declara <ambitus> in actione)");
        redde FALSUM;
    }
    /* nota verdicti (cauda transitus, a genere gradus posita) */
    verdictum = actum.cauda.mensura > ZEPHYRUM
        ? chorda_concatenare(fabricae_iungere(piscina, "",
        actio->titulus,
              ": transiit ("), fabricae_iungere(piscina, "",
              actum.cauda,
              ")\n"),
              piscina)
        : fabricae_iungere(piscina, "", actio->titulus, ": transiit\n");
    si (!sutura->verdictum_ponere(sutura->datum,
            chorda_ut_cstr(exitus->via, piscina), &verdictum))
    {
        *causa_out = fabricae_iungere(piscina,
            "verdictum scribi nequit: ",
            exitus->via, "");
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_actionem_agere (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                       i32* duratio_out,
                    chorda* causa_out)
{
    FabricaActum  actum;
             b32  incepit;
       character  numerus[32];
             Xar* ante;
             Xar* post;

    /* membrum gradus (fabrica-6 T5): per genus gradus, in area */
    si (actio->membrum != NIHIL)
    {
        redde _membrum_agere(sutura, actio, piscina, duratio_out,
            causa_out);
    }
    actum.codex       = -I;
    actum.duratio_ms  = ZEPHYRUM;
    actum.cauda       = chorda_ex_literis("", piscina);
    ante              = NIHIL;
    post              = NIHIL;
    si (sutura->agere == NIHIL)
    {
        *causa_out = chorda_ex_literis("sutura sine agere", piscina);
        redde FALSUM;
    }
    /* SIGNUM (plan 5 T3): verdictum vetus ANTE cursum deletum - porta
     * fracta nihil relinquit */
    si (   actio->genus == FABRICA_ACTIO_IUDICIUM
        && actio->signum.mensura > ZEPHYRUM)
    {
        si (sutura->verdictum_ponere == NIHIL)
        {
            *causa_out =
                chorda_ex_literis("signum sine verdictum_ponere",
                piscina);
            redde FALSUM;
        }
        (vacuum)sutura->verdictum_ponere(sutura->datum,
            chorda_ut_cstr(((constans FabricaExitus*)xar_obtinere(
                actio->exitus, ZEPHYRUM))->via, piscina), NIHIL);
    }
    /* photographia ante et post (T4): scriptura extra vestigium */
    si (sutura->vestigium_capere != NIHIL)
    {
        (vacuum)sutura->vestigium_capere(sutura->datum, piscina, &ante);
    }
    incepit = sutura->agere(sutura->datum, actio,
        chorda_ut_cstr(fabricae_iungere(piscina, "build/fabrica/acta/",
            actio->titulus, ".log"), piscina), piscina, &actum);
    *duratio_out = actum.duratio_ms;
    si (   ante != NIHIL
        && !sutura->vestigium_capere(sutura->datum, piscina, &post))
    {
        post = NIHIL;
    }
    /* MEMORIAE: regenerationes servantur si actio nihil extra acta et
     * librum suum mutavit (e.g. praecondicio residentis iam viventis:
     * tabulariumd, speculum_hospes - olim memoriam ante membra
     * vacabant, generatores iterum regenerabantur) */
    _memorias_vacare(sutura, !incepit
        || !fabrica_mutata_intra_locum(sutura, actio,
        chorda_ex_literis("", piscina), ante, post, piscina));
    si (!incepit)
    {
        *causa_out = fabricae_iungere(piscina, "non actum: ",
            actum.cauda, "");
        redde FALSUM;
    }
    si (actum.codex != 0)
    {
        sprintf(numerus, "exitus %d: ", (integer)actum.codex);
        *causa_out = fabricae_iungere(piscina, numerus, actum.cauda,
            "");
        redde FALSUM;
    }
    si (post != NIHIL)
    {
        Xar* extra;

        extra = fabrica_vestigia_comparare(actio, ante, post, piscina);
        si (extra != NIHIL && xar_numerus(extra) > 0)
        {
            *causa_out = fabricae_iungere(piscina, "",
                _extra_nominare(extra,
                piscina), " - aut manu mutata dum currebat");
            redde FALSUM;
        }
    }
    /* transientes per librum (lineae S): solum actio iudicii, cuius
     * liber hoc cursu recens est (generatoris liber iudicis manet) */
    si (actio->genus == FABRICA_ACTIO_IUDICIUM)
    {
        chorda aliena = _scripturam_alienam(actio, _scripturas_notatas(
            sutura, chorda_ut_cstr(fabrica_liber_via(actio->titulus,
            piscina), piscina), piscina), piscina);

        si (aliena.mensura > ZEPHYRUM)
        {
            *causa_out = fabricae_iungere(piscina,
                "scripsit extra vestigium (S, liber lectionum): ",
                aliena, "");
            redde FALSUM;
        }
    }
    si (   actio->genus == FABRICA_ACTIO_IUDICIUM
        && actio->signum.mensura > ZEPHYRUM)
    {
        redde _verdictum_ex_actis(sutura, actio, chorda_ut_cstr(
            fabricae_iungere(piscina, "build/fabrica/acta/",
            actio->titulus,
                ".log"), piscina), piscina, causa_out);
    }
    redde VERUM;
}

interior s32
_index_actionis (
     constans Xar* ordo,
           chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(ordo); i++)
    {
        si (chorda_aequalis((*(FabricaActio**)xar_obtinere(ordo,
                i))->titulus, titulus))
        {
            redde (s32)i;
        }
    }
    redde -I;
}

/* sanationem addere: eventus solus decernit - actum (sanatum,
 * fractum, praeparatum) in cursu scribitur; siccum (agendum,
 * fortasse) tempus ex cursu ultimo aestimat; omissum neutrum (1b T7) */
interior vacuum
_sanationem_stalum_notare (
     constans FabricaSutura* sutura,
                    Piscina* piscina,
                        Xar* sanationes,
                     chorda  stalum,
      constans FabricaActio* actio,
             FabricaEventus  eventus,
                     chorda  causa,
                        i32  duratio_ms)
{
    FabricaSanatio* sanatio;

    sanatio = (FabricaSanatio*)xar_addere(sanationes);
    si (sanatio == NIHIL)
    {
        redde;
    }
    /* causa et stalum ultra actum vivunt (piscina cursus) */
    causa   = fabricae_in_cursum(sutura, causa, piscina);
    stalum  = fabricae_in_cursum(sutura, stalum, piscina);

    sanatio->actio         = actio;
    sanatio->eventus       = eventus;
    sanatio->causa         = causa;
    sanatio->stalum        = stalum;
    sanatio->duratio_ms    = duratio_ms;
    sanatio->tempus_notum  = VERUM;
    commutatio (eventus)
    {
        casus FABRICA_SANATUM:
        casus FABRICA_FRACTUM:
        casus FABRICA_PRAEPARATUM:
            si (sutura->cursum_inscribere != NIHIL)
            {
                sutura->cursum_inscribere(sutura->datum, sanatio);
            }
            frange;
        casus FABRICA_AGENDUM:
        casus FABRICA_FORTASSE:
            sanatio->duratio_ms    = ZEPHYRUM;
            sanatio->tempus_notum  = sutura->cursum_legere != NIHIL
                && sutura->cursum_legere(sutura->datum,
                       chorda_ut_cstr(actio->titulus, piscina),
                       &sanatio->duratio_ms);
            frange;
        ordinarius:
            sanatio->tempus_notum = FALSUM;
            frange;
    }
}

/* sanatio sine causa stali (non acta, aut ante actum) */
interior vacuum
_sanationem_notare (
     constans FabricaSutura* sutura,
                    Piscina* piscina,
                        Xar* sanationes,
      constans FabricaActio* actio,
             FabricaEventus  eventus,
                     chorda  causa,
                        i32  duratio_ms)
{
    chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    _sanationem_stalum_notare(sutura, piscina, sanationes, vacua, actio,
        eventus, causa, duratio_ms);
}

/* dependentia fracta aut omissa (ingressus aut praecondicio): VERUM
 * et titulus eius */
interior b32
_dependentia_fracta (
     constans Xar* ordo,
              i32  i,
    StatusSanandi* status,
           chorda* titulus_out)
{
    constans FabricaActio* actio;
                      i32  j;

    actio = *(FabricaActio**)xar_obtinere(ordo, i);
    per (j = ZEPHYRUM; j < i; j++)
    {
        si (   (   status[j] == SANANDI_FRACTUM
                || status[j] == SANANDI_OMISSUM)
            && fabricae_pendet(actio,
            *(FabricaActio**)xar_obtinere(ordo, j)))
        {
            *titulus_out = (*(FabricaActio**)xar_obtinere(ordo,
                j))->titulus;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ANTE AGERE: dependentia fracta, recentia, siccum, praecondiciones
 * ignotae. VERUM = actio agenda (status SANANDI_NONDUM manet); FALSUM =
 * iam tractata (OMISSUM, RECENS, FORTASSE, AGENDUM notata). */
/* AUDITUS TRANSITUS (spec 3 par. XIII): actio iudicium RECENS hic
 * tamen currenda? auditus I = omnes; N = unus ex N (octetus primus
 * clavis). Electa: vestigium vetus in sutura->audita servatur. */
interior b32
_auditum_iudicii (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina)
{
    constans FabricaExitus* exitus;
                  Sigillum  ingressus;
                  Sigillum  clavis;
                  Sigillum  artificium;
                    chorda  verdictum;
                    chorda  causa;
                       Xar* vetus;

    si (   sutura->auditus == ZEPHYRUM
        || actio->genus    != FABRICA_ACTIO_IUDICIUM)
    {
        redde FALSUM;
    }
    causa = chorda_ex_literis("", piscina);
    si (!fabricae_actionem_sigillare_semel(sutura, actio, piscina,
        &ingressus,
            &causa))
    {
        redde FALSUM;
    }
    clavis = fabricae_clavem_memoriae(sutura, actio, &ingressus,
        piscina);
    si (   sutura->auditus > I
        && (clavis.octeti[0] % sutura->auditus) != 0)
    {
        redde FALSUM;
    }
    exitus = (constans FabricaExitus*)xar_obtinere(actio->exitus,
        ZEPHYRUM);
    si (   sutura->audita != NIHIL && sutura->lectiones_legere != NIHIL
        && sutura->legere(sutura->datum, chorda_ut_cstr(exitus->via,
               piscina), piscina, &verdictum))
    {
        artificium = sigillum_computare(verdictum.datum,
            (memoriae_index)verdictum.mensura);
        si (sutura->lectiones_legere(sutura->datum,
                chorda_ut_cstr(actio->titulus, piscina),
                chorda_ut_cstr(exitus->via, piscina), &clavis,
                &artificium,
                fabricae_piscina_cursus(sutura, piscina), &vetus))
        {
            (vacuum)tabula_dispersa_inserere(sutura->audita,
                actio->titulus,
                vetus);
        }
    }
    redde VERUM;
}

/* lectiones vestigii novi quas vetus non habet: III nominatae + numerus */
interior chorda
_lectiones_novas_nominare (
     constans Xar* nova,
     constans Xar* vetus,
          Piscina* piscina)
{
     TabulaDispersa* notae;
             chorda  nuntius;
                i32  i;
                i32  numerus = ZEPHYRUM;

    notae    = tabula_dispersa_creare_chorda(piscina, 1024);
    nuntius  = chorda_ex_literis("", piscina);
    per (i = ZEPHYRUM; vetus != NIHIL && i < xar_numerus(vetus); i++)
    {
        (vacuum)tabula_dispersa_inserere(notae,
            ((constans FabricaLectio*)xar_obtinere(vetus, i))->via,
            NIHIL);
    }
    per (i = ZEPHYRUM; i < xar_numerus(nova); i++)
    {
        chorda via = ((constans FabricaLectio*)xar_obtinere(nova,
            i))->via;

        si (tabula_dispersa_continet(notae, via))
        {
            perge;
        }
        si (numerus < III)
        {
            nuntius = fabricae_iungere(piscina, "", nuntius,
                numerus > ZEPHYRUM ? ", " : "");
            nuntius = chorda_concatenare(nuntius, via, piscina);
        }
        numerus++;
    }
    si (numerus == ZEPHYRUM)
    {
        redde chorda_ex_literis("nulla (missum extra librum?)",
            piscina);
    }
    si (numerus > III)
    {
        nuntius = fabricae_iungere(piscina, "", nuntius, " +");
        nuntius = chorda_concatenare(nuntius, numerus_romanus_exprimere(
            (i64)(numerus - III), NIHIL, piscina), piscina);
    }
    redde nuntius;
}

interior b32
_ante_agere (
    constans FabricaSutura* sutura,
              constans Xar* ordo,
                       i32  i,
             StatusSanandi* status,
                    chorda* stala,
                       Xar* sanationes,
                       b32  siccum,
                   Piscina* piscina)
{
    constans FabricaActio* actio;
                   chorda  causa;
                   chorda  fracta;
                      b32  recens;
                      b32  auditum;
                      i32  duratio;
                      i32  j;

    actio = *(FabricaActio**)xar_obtinere(ordo, i);

    /* dependentia fracta aut omissa (ingressus aut praecondicio):
     * iudicium unum cum via undarum (_dependentia_fracta) */
    fracta = chorda_ex_literis("", piscina);
    si (_dependentia_fracta(ordo, i, status, &fracta))
    {
        status[i] = SANANDI_OMISSUM;
        _sanationem_notare(sutura, piscina, sanationes, actio,
            FABRICA_OMISSUM,
            fabricae_iungere(piscina, "dependentia fracta: ", fracta,
            ""),
            ZEPHYRUM);
        redde FALSUM;
    }

    causa   = chorda_ex_literis("", piscina);
    recens  = _exitus_recentes(sutura, actio, piscina, &causa);
    auditum = recens && !siccum
        && _auditum_iudicii(sutura, actio, piscina);
    /* CUR ACTA (plan-5 T1): causa iudicii in cursum post actum */
    stala[i] = auditum
        ? chorda_ex_literis("auditus: RECENS, tamen currit", piscina)
        : causa;
    si (auditum)
    {
        status[i] = SANANDI_AUDITUM;   /* RECENS, tamen currit */
    }
    si (recens && !auditum)
    {
        /* siccum: recens nunc, sed post actionem agendam fortasse
         * non */
        si (siccum)
        {
            per (j = ZEPHYRUM; j < i; j++)
            {
                si (   (   status[j] == SANANDI_AGENDUM
                        || status[j] == SANANDI_FORTASSE)
                    && fabricae_pendet(actio,
                           *(FabricaActio**)xar_obtinere(ordo, j)))
                {
                    status[i] = SANANDI_FORTASSE;
                    _sanationem_notare(sutura, piscina, sanationes,
                        actio,
                        FABRICA_FORTASSE, fabricae_iungere(piscina,
                            "post ", (*(FabricaActio**)xar_obtinere(
                            ordo, j))->titulus, ""), ZEPHYRUM);
                    frange;
                }
            }
        }
        si (status[i] == SANANDI_NONDUM)
        {
            status[i] = SANANDI_RECENS;
        }
        redde FALSUM;
    }
    si (siccum)
    {
        status[i] = SANANDI_AGENDUM;
        _sanationem_notare(sutura, piscina, sanationes, actio,
            FABRICA_AGENDUM,
            causa,
            ZEPHYRUM);
        redde FALSUM;
    }

    /* praecondiciones ignotae: semel per cursum */
    per (j = ZEPHYRUM;
         actio->praecondiciones != NIHIL
         && j < xar_numerus(actio->praecondiciones);
         j++)
    {
        s32 k;

        k = _index_actionis(ordo,
            *(chorda*)xar_obtinere(actio->praecondiciones, j));
        si (   k < 0
            || status[k] != SANANDI_NONDUM
            || !_actio_ignota(*(FabricaActio**)xar_obtinere(ordo,
                   (i32)k)))
        {
            perge;
        }
        duratio = ZEPHYRUM;
        si (_actionem_agere(sutura, *(FabricaActio**)xar_obtinere(
                ordo, (i32)k), piscina, &duratio, &causa))
        {
            status[k] = SANANDI_PRAEPARATUM;
            _sanationem_notare(sutura, piscina, sanationes,
                *(FabricaActio**)xar_obtinere(ordo, (i32)k),
                FABRICA_PRAEPARATUM, chorda_ex_literis("", piscina),
                duratio);
        }
        alioquin
        {
            status[k] = SANANDI_FRACTUM;
            _sanationem_notare(sutura, piscina, sanationes,
                *(FabricaActio**)xar_obtinere(ordo, (i32)k),
                FABRICA_FRACTUM, causa, duratio);
        }
    }
    /* praecondicio modo fracta: omittitur */
    per (j = ZEPHYRUM;
         actio->praecondiciones != NIHIL
         && j < xar_numerus(actio->praecondiciones);
         j++)
    {
        s32 k;

        k = _index_actionis(ordo,
            *(chorda*)xar_obtinere(actio->praecondiciones, j));
        si (   k >= 0
            && (   status[k] == SANANDI_FRACTUM
                || status[k] == SANANDI_OMISSUM))
        {
            status[i] = SANANDI_OMISSUM;
            _sanationem_notare(sutura, piscina, sanationes, actio,
                FABRICA_OMISSUM,
                fabricae_iungere(piscina, "praecondicio fracta: ",
                    *(chorda*)xar_obtinere(actio->praecondiciones, j),
                    ""), ZEPHYRUM);
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* VESTIGIUM VACUUM (fabrica-6 T6c): transitus qui nihil legit (membrum
 * nexu solo iudicatum, probatio sine lectionibus) vestigium VACUUM
 * habet - sed scriptura vacua in memoria DELETIO est (spec 3 T5b), ergo
 * transitus numquam servaretur et semper iterum curreret. Lectio una
 * signans additur: X '.' (radix exstat) - status eius stabilis. */
interior b32
_vestigium_vacuum_signare (
    constans FabricaSutura* sutura,
                       Xar* vestigium,
                   Piscina* piscina)
{
    FabricaLectio* signum;

    signum = (FabricaLectio*)xar_addere(vestigium);
    si (signum == NIHIL)
    {
        redde FALSUM;
    }
    signum->genus  = LECTIO_EXSTAT;
    signum->via    = chorda_ex_literis(".", piscina);
    redde fabricae_lectionem_sigillare(sutura, LECTIO_EXSTAT,
        signum->via,
        piscina, &signum->sigillum);
}

/* POST AGERE: actum FALSUM -> FRACTUM (causa); aliter post-condicio
 * (Review Focus 3: exitus 0 non sufficit) -> SANATUM aut FRACTUM */
interior vacuum
_post_agere (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                       i32  i,
             StatusSanandi* status,
                    chorda* stala,
                       Xar* sanationes,
                       b32  actum,
                    chorda  causa,
                       i32  duratio,
                   Piscina* piscina)
{
    si (   !actum && actio->genus == FABRICA_ACTIO_IUDICIUM
        && status[i] == SANANDI_AUDITUM)
    {
        /* AUDITUM DISCORS (spec 3 par. XIII): transitus servatus RECENS
         * dicebat, porta nunc fracta - ingressus quem vestigium non
         * videt; lectiones cursus fracti quas vetus non habet nominantur */
        chorda  ratio;
        chorda  novae;
           Xar* nova;
          void* vetus;

        ratio = chorda_ex_literis("", piscina);
        si (fabricae_lectiones_transitus_colligere(sutura, actio,
                chorda_ut_cstr(fabrica_liber_via(actio->titulus,
                piscina),
                    piscina), piscina, &nova, &ratio))
        {
            vetus = NIHIL;
            si (sutura->audita != NIHIL)
            {
                (vacuum)tabula_dispersa_invenire(sutura->audita,
                    actio->titulus, &vetus);
            }
            novae = _lectiones_novas_nominare(nova,
                (constans Xar*)vetus,
                piscina);
        }
        alioquin
        {
            novae = fabricae_iungere(piscina, "liber non collectus: ",
                ratio,
                "");
        }
        status[i] = SANANDI_FRACTUM;
        _sanationem_stalum_notare(sutura, piscina, sanationes,
            stala[i], actio,
            FABRICA_AUDITUM_DISCORS, fabricae_iungere(piscina,
            "AUDITUM DISCORS:"
                " transitus servatus RECENS dicebat, porta nunc fracta (",
                causa, chorda_ut_cstr(fabricae_iungere(piscina,
                    "); lectiones novae: ", novae, ""), piscina)),
            duratio);
        redde;
    }
    si (!actum)
    {
        status[i] = SANANDI_FRACTUM;
        _sanationem_stalum_notare(sutura, piscina, sanationes,
            stala[i], actio,
            FABRICA_FRACTUM, causa, duratio);
        redde;
    }
    si (actio->genus == FABRICA_ACTIO_IUDICIUM)
    {
        /* TRANSITUS (spec 3 par. XII): porta in loco transiit - vestigium
         * libri sui sub clave (ingressus hodierni, verdictum) servatur;
         * non reutilis = transitus tamen (SANATUM), causa nominata */
        constans FabricaExitus* exitus;
                        chorda  verdictum;
                        chorda  ratio;
                      Sigillum  ingressus;
                      Sigillum  clavis;
                      Sigillum  artificium;
                           Xar* vestigium;
                           Xar* particulae;
                           b32  servatum = FALSUM;

        exitus = (constans FabricaExitus*)xar_obtinere(actio->exitus,
            ZEPHYRUM);
        si (!sutura->legere(sutura->datum,
                chorda_ut_cstr(exitus->via, piscina), piscina,
                &verdictum))
        {
            status[i] = SANANDI_FRACTUM;
            _sanationem_stalum_notare(sutura, piscina, sanationes,
                stala[i], actio,
                FABRICA_FRACTUM, fabricae_iungere(piscina,
                    "exitus 0 sed verdictum non scriptum: ",
                    exitus->via,
                    ""), duratio);
            redde;
        }
        artificium = sigillum_computare(verdictum.datum,
            (memoriae_index)verdictum.mensura);
        ratio = chorda_ex_literis("", piscina);
        si (!fabricae_actionem_sigillare_semel(sutura, actio, piscina,
            &ingressus,
                &ratio))
        {
            ratio = fabricae_iungere(piscina,
                "transitus non servatus (ingressus): ",
                ratio, "");
        }
        alioquin si (!fabricae_lectiones_transitus_colligere(sutura,
                     actio,
                     chorda_ut_cstr(fabrica_liber_via(actio->titulus,
                     piscina),
                     piscina), piscina, &vestigium, &ratio))
        {
            ratio = fabricae_iungere(piscina,
                "transitus non servatus: ", ratio,
                "");
        }
        alioquin si (sutura->lectiones_scribere == NIHIL)
        {
            ratio = chorda_ex_literis(
                "transitus non servatus: sine memoria", piscina);
        }
        alioquin si (   xar_numerus(vestigium) == ZEPHYRUM
                     && !_vestigium_vacuum_signare(sutura, vestigium,
                            piscina))
        {
            ratio = chorda_ex_literis(
                "transitus non servatus: signum vestigii vacui",
                piscina);
        }
        alioquin
        {
            clavis = fabricae_clavem_memoriae(sutura, actio,
                &ingressus, piscina);
            sutura->lectiones_scribere(sutura->datum,
                chorda_ut_cstr(actio->titulus, piscina),
                chorda_ut_cstr(exitus->via, piscina), &clavis,
                &artificium,
                vestigium);
            /* particulae (plan-5 T1): ut iudex postea nominet quis
             * ingressus transitum mutavit */
            si (   sutura->particulas_scribere != NIHIL
                && fabricae_particulas_transitus(sutura, actio,
                &artificium,
                       piscina, &particulae))
            {
                sutura->particulas_scribere(sutura->datum,
                    chorda_ut_cstr(actio->titulus, piscina),
                    chorda_ut_cstr(exitus->via, piscina), particulae);
            }
            /* consensus: iudicium idem nunc RECENS dicere debet */
            si (!_exitus_recentes(sutura, actio, piscina, &ratio))
            {
                status[i] = SANANDI_FRACTUM;
                _sanationem_stalum_notare(sutura, piscina, sanationes,
                    stala[i], actio,
                    FABRICA_FRACTUM, fabricae_iungere(piscina,
                        "vestigium scriptum sed iudicium non RECENS: ",
                        ratio, ""), duratio);
                redde;
            }
            servatum = VERUM;
            ratio = chorda_ex_literis(status[i] == SANANDI_AUDITUM
                ? "auditus: transitus iterum congruit" : "", piscina);
        }
        /* transitus non servatus: vestigium VETUS deletur (scriptura
         * vacua) - cursus hic legit quod vetus non explicat, ergo
         * transitus vetus testis non est */
        si (   !servatum
            && sutura->lectiones_scribere != NIHIL)
        {
            Xar* vacuum_vestigium;

            vacuum_vestigium = xar_creare(piscina,
                (i32)magnitudo(FabricaLectio));
            memset(&clavis, ZEPHYRUM, magnitudo(clavis));
            sutura->lectiones_scribere(sutura->datum,
                chorda_ut_cstr(actio->titulus, piscina),
                chorda_ut_cstr(exitus->via, piscina), &clavis,
                &artificium,
                vacuum_vestigium);
        }
        status[i] = SANANDI_SANATUM;
        _sanationem_stalum_notare(sutura, piscina, sanationes,
            stala[i], actio,
            FABRICA_SANATUM, ratio, duratio);
        redde;
    }
    si (!_exitus_recentes(sutura, actio, piscina, &causa))
    {
        status[i] = SANANDI_FRACTUM;
        _sanationem_stalum_notare(sutura, piscina, sanationes,
            stala[i], actio,
            FABRICA_FRACTUM,
            fabricae_iungere(piscina, "exitus 0 sed non RECENS: ",
            causa, ""),
            duratio);
        redde;
    }
    status[i] = SANANDI_SANATUM;
    _sanationem_stalum_notare(sutura, piscina, sanationes,
            stala[i], actio,
        FABRICA_SANATUM, chorda_ex_literis("", piscina), duratio);
}

/* via libri normata: radix et './' demptae; NIHIL-chorda si extra
 * arborem (absoluta) */
interior chorda
_viam_libri_normare (
    constans FabricaSutura* sutura,
                    chorda  via)
{
    si (   sutura->radix.mensura > 0
        && via.mensura > sutura->radix.mensura
        && chorda_incipit(via, sutura->radix)
        && via.datum[sutura->radix.mensura] == '/')
    {
        via = chorda_sectio(via, sutura->radix.mensura + I,
            via.mensura);
    }
    dum (via.mensura > II && via.datum[0] == '.' && via.datum[I] == '/')
    {
        via = chorda_sectio(via, II, via.mensura);
    }
    si (via.mensura > 0 && via.datum[0] == '/')
    {
        via.mensura = ZEPHYRUM;
    }
    redde via;
}

interior s32
_vestigia_via_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde chorda_comparare(((constans FabricaVestigium*)a)->via,
        ((constans FabricaVestigium*)b)->via);
}

/* SCRIPTURAE NOTATAE (S) libri membri: viae normatae (extra arborem
 * praetermissae - photographia eas quoque non videt). Xar de chorda;
 * vacua si liber absens. */
interior Xar*
_scripturas_notatas (
    constans FabricaSutura* sutura,
        constans character* liber_via,
                   Piscina* piscina)
{
    chorda  liber;
       Xar* viae;
       i32  i;
       i32  initium;

    viae = fabricae_xar_chordarum(piscina);
    si (   viae == NIHIL || sutura->legere == NIHIL
        || !sutura->legere(sutura->datum, liber_via, piscina, &liber))
    {
        redde viae;
    }
    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; i < liber.mensura; i++)
    {
        chorda linea;
        chorda via;

        si (liber.datum[i] != '\n')
        {
            perge;
        }
        linea    = chorda_sectio(liber, initium, i);
        initium  = i + I;
        si (   linea.mensura < III || linea.datum[0] != 'S'
            || linea.datum[I] != '\t')
        {
            perge;
        }
        via = _viam_libri_normare(sutura,
            chorda_sectio(linea, II, linea.mensura));
        si (via.mensura > ZEPHYRUM)
        {
            fabricae_chordam_addere(viae, via);
        }
    }
    redde viae;
}

/* 'via' directorium MAIUS loci 'locus' (locus = via + '/' + ...)? */
interior b32
_via_maior (
       chorda via,
       chorda locus)
{
    redde via.mensura > ZEPHYRUM && locus.mensura > via.mensura
        && locus.datum[via.mensura] == '/'
        && memcmp(locus.datum, via.datum, (size_t)via.mensura)
            == ZEPHYRUM;
}

/* idem contra litteras C (thesaurus: partes constantes) */
interior b32
_via_maior_literis (
                 chorda  via,
     constans character* locus)
{
    size_t longitudo = strlen(locus);

    redde via.mensura > ZEPHYRUM && longitudo > (size_t)via.mensura
        && locus[via.mensura] == '/'
        && memcmp(locus, via.datum, (size_t)via.mensura) == ZEPHYRUM;
}

/* ANTECESSOR PERMISSUS (iudicium-fumus, 2026-10-24): linea S directorii
 * quod maius est loco ubi actio scribere licet (vestigium, exitus,
 * thesaurus) - creatio semitae ad eum (mkdir -p: compilator thesaurum
 * build/aedilis/obiecta/ in arbore recenti creat). Deletio maioris
 * damnum non celat: contenta deleta lineas S suas ferunt, quae ipsae
 * iudicantur. Photographiae directoria non notant - lineae solae. */
interior b32
_antecessor_permissus (
    constans FabricaActio* actio,
                   chorda  via)
{
    i32 k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        si (_via_maior_literis(via, fabricae_thesauri_partes[k]))
        {
            redde VERUM;
        }
    }
    per (k = ZEPHYRUM; actio->vestigia != NIHIL
         && k < xar_numerus(actio->vestigia); k++)
    {
        si (_via_maior(via,
            ((FabricaLocus*)xar_obtinere(actio->vestigia,
                k))->via))
        {
            redde VERUM;
        }
    }
    per (k = ZEPHYRUM; k < xar_numerus(actio->exitus); k++)
    {
        si (_via_maior(via, ((FabricaExitus*)xar_obtinere(actio->exitus,
                k))->via))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* prima via notata extra vestigium actionis (vacua = nulla) */
interior chorda
_scripturam_alienam (
     constans FabricaActio* actio,
              constans Xar* notatae,
                   Piscina* piscina)
{
    Xar* ante;
    Xar* post;
    Xar* extra;
    i32  i;

    ante = xar_creare(piscina, (i32)magnitudo(FabricaVestigium));
    post = xar_creare(piscina, (i32)magnitudo(FabricaVestigium));
    per (i = ZEPHYRUM; post != NIHIL && i < xar_numerus(notatae); i++)
    {
        FabricaVestigium* v;

        si (_antecessor_permissus(actio, *(chorda*)xar_obtinere(notatae,
            i)))
        {
            perge;
        }
        v = (FabricaVestigium*)xar_addere(post);
        si (v == NIHIL)
        {
            frange;
        }
        v->via        = *(chorda*)xar_obtinere(notatae, i);
        v->tempus_ns  = ZEPHYRUM;
        v->mensura    = ZEPHYRUM;
    }
    si (post == NIHIL || xar_numerus(post) == 0)
    {
        redde chorda_ex_literis("", piscina);
    }
    xar_ordinare(post, _vestigia_via_comparare);
    extra = fabrica_vestigia_comparare(actio, ante, post, piscina);
    si (extra != NIHIL && xar_numerus(extra) > 0)
    {
        redde *(chorda*)xar_obtinere(extra, ZEPHYRUM);
    }
    redde chorda_ex_literis("", piscina);
}

/* CURSUS UNDAE (plan 2 T6; per gradus fabrica-6): membra tuta simul.
 * Tabulae parallelae (per membrum k) ut sutura->agere_simul eas
 * poscit; status, stala, sanationes cursus sanandi totius */
nomen structura {
     constans FabricaSutura*  sutura;
                    Piscina*  piscina;
               constans Xar*  ordo;
                        Xar*  indices;     /* k -> index in ordine */
              StatusSanandi*  status;
                     chorda*  stala;
                        Xar*  sanationes;
                        i32   n;
      constans FabricaActio** actiones;
         constans character** acta_viae;
         constans character** libri;
               FabricaActum*  acta;
                        b32*  incepta;
                        Xar** notatae;     /* scripturae 'S' libri */
                        Xar*  ante;        /* photographia ante undam */
} CursusUndae;

/* membra ordine tituli (cursus et sanationes deterministice) */
interior vacuum
_membra_ordinare (
    constans Xar* ordo,
             Xar* indices)
{
    i32 n;
    i32 k;
    i32 m;

    n = xar_numerus(indices);
    per (k = I; k < n; k++)
    {
        per (m = k; m > ZEPHYRUM; m--)
        {
            i32* a;
            i32* b;
            i32  t;

            a = (i32*)xar_obtinere(indices, m - I);
            b = (i32*)xar_obtinere(indices, m);
            si (chorda_comparare(
                    (*(FabricaActio**)xar_obtinere(ordo, *a))->titulus,
                    (*(FabricaActio**)xar_obtinere(ordo, *b))->titulus)
                <= 0)
            {
                frange;
            }
            t   = *a;
            *a  = *b;
            *b  = t;
        }
    }
}

/* tabulae membrorum: actio, acta (build/fabrica/acta/T.log), liber
 * (T.sanare.tsv); FALSUM si piscina deficit */
interior b32
_undam_parare (
    CursusUndae* c)
{
    memoriae_index n;
               i32 k;

    n             = (memoriae_index)c->n;
    c->actiones   =
        (constans FabricaActio**)piscina_allocare(c->piscina,
        magnitudo(FabricaActio*) * n);
    c->acta_viae  = (constans character**)piscina_allocare(c->piscina,
        magnitudo(character*) * n);
    c->libri      = (constans character**)piscina_allocare(c->piscina,
        magnitudo(character*) * n);
    c->acta       = (FabricaActum*)piscina_allocare(c->piscina,
        magnitudo(FabricaActum) * n);
    c->incepta    = (b32*)piscina_allocare(c->piscina,
        magnitudo(b32) * n);
    si (   c->actiones == NIHIL || c->acta_viae == NIHIL
        || c->libri    == NIHIL || c->acta == NIHIL
        || c->incepta  == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < c->n; k++)
    {
        c->actiones[k] = *(FabricaActio**)xar_obtinere(c->ordo,
            *(i32*)xar_obtinere(c->indices, k));
        c->acta_viae[k] = chorda_ut_cstr(fabricae_iungere(c->piscina,
            "build/fabrica/acta/", c->actiones[k]->titulus, ".log"),
            c->piscina);
        c->libri[k] = chorda_ut_cstr(fabricae_iungere(c->piscina,
            LIBRI_DIRECTORIUM, c->actiones[k]->titulus, ".sanare.tsv"),
            c->piscina);
        c->incepta[k] = FALSUM;
    }
    redde VERUM;
}

/* photographia ante, membra simul, memoriae vacatae, post-condiciones
 * membrorum felicium simul (PRAEVISIO T6b: arbor sanata), scripturae
 * 'S' cuiusque libri. FALSUM si piscina deficit */
interior b32
_undam_currere (
    CursusUndae* c)
{
    constans FabricaSutura* sutura;
                       Xar* sanata;
                       i32  k;

    sutura   = c->sutura;
    c->ante  = NIHIL;
    si (sutura->vestigium_capere != NIHIL)
    {
        (vacuum)sutura->vestigium_capere(sutura->datum, c->piscina,
            &c->ante);
    }
    sutura->agere_simul(sutura->datum,
        (constans FabricaActio* constans*)c->actiones, c->n,
        (constans character* constans*)c->acta_viae,
        (constans character* constans*)c->libri, c->piscina, c->acta,
        c->incepta);
    _memorias_vacare(sutura, VERUM);
    sanata = xar_creare(c->piscina, (i32)magnitudo(FabricaActio*));
    per (k = ZEPHYRUM; sanata != NIHIL && k < c->n; k++)
    {
        si (c->incepta[k] && c->acta[k].codex == ZEPHYRUM)
        {
            *(constans FabricaActio**)xar_addere(sanata) =
                c->actiones[k];
        }
    }
    si (sanata != NIHIL)
    {
        fabrica_regenerationes_praevidere(sutura, sanata, VERUM,
            c->piscina);
    }
    c->notatae = (Xar**)piscina_allocare(c->piscina,
        magnitudo(Xar*) * (memoriae_index)c->n);
    si (c->notatae == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < c->n; k++)
    {
        c->notatae[k] = _scripturas_notatas(sutura, c->libri[k],
            c->piscina);
    }
    redde VERUM;
}

/* continetne 'viae' (Xar de chorda) viam? */
interior b32
_viam_continet (
     constans Xar* viae,
           chorda  via)
{
    i32 f;

    per (f = ZEPHYRUM; f < xar_numerus(viae); f++)
    {
        si (chorda_aequalis(via, *(chorda*)xar_obtinere(viae, f)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* scripturae extra vestigium OMNIUM membrorum (intersectio extra
 * singulorum); NIHIL si comparatio deficit */
interior Xar*
_extra_omnium (
    constans CursusUndae* c,
            constans Xar* post)
{
    Xar* extra;
    i32  k;

    extra = fabrica_vestigia_comparare(c->actiones[0], c->ante, post,
        c->piscina);
    per (k = I; extra != NIHIL && k < c->n; k++)
    {
        Xar* extra_k;
        Xar* communia;
        i32  e;

        extra_k   = fabrica_vestigia_comparare(c->actiones[k], c->ante,
            post, c->piscina);
        communia  = fabricae_xar_chordarum(c->piscina);
        per (e = ZEPHYRUM; extra_k != NIHIL && communia != NIHIL
             && e < xar_numerus(extra); e++)
        {
            chorda via;

            via = *(chorda*)xar_obtinere(extra, e);
            si (_viam_continet(extra_k, via))
            {
                fabricae_chordam_addere(communia, via);
            }
        }
        extra = communia;
    }
    redde extra;
}

/* SCRIPTURAE IGNOTAE undae: extra vestigia omnium membrorum ET a nullo
 * libro notatae (notata membro suo imputatur, non undae). NIHIL sine
 * photographiis */
interior Xar*
_scripturas_ignotas (
    constans CursusUndae* c)
{
    Xar* post;
    Xar* extra;
    Xar* ignotae;
    i32  e;

    post = NIHIL;
    si (   c->ante == NIHIL
        || !c->sutura->vestigium_capere(c->sutura->datum, c->piscina,
               &post))
    {
        redde NIHIL;
    }
    extra = _extra_omnium(c, post);
    si (extra == NIHIL)
    {
        redde NIHIL;
    }
    ignotae = fabricae_xar_chordarum(c->piscina);
    per (e = ZEPHYRUM; ignotae != NIHIL && e < xar_numerus(extra); e++)
    {
        chorda via;
           b32 notata;
           i32 k;

        via     = *(chorda*)xar_obtinere(extra, e);
        notata  = FALSUM;
        per (k = ZEPHYRUM; !notata && k < c->n; k++)
        {
            notata = _viam_continet(c->notatae[k], via);
        }
        si (!notata)
        {
            fabricae_chordam_addere(ignotae, via);
        }
    }
    redde ignotae;
}

/* causa scripturarum ignotarum (prima nominata, '+N' reliquarum);
 * vacua si nullae */
interior chorda
_causa_ignotarum (
    constans CursusUndae* c,
            constans Xar* ignotae)
{
    chorda causa;

    si (ignotae == NIHIL || xar_numerus(ignotae) == 0)
    {
        redde chorda_ex_literis("", c->piscina);
    }
    causa = fabricae_iungere(c->piscina,
        "unda scripsit extra vestigia omnium membrorum: ",
        *(chorda*)xar_obtinere(ignotae, ZEPHYRUM), "");
    si (xar_numerus(ignotae) > I)
    {
        causa = fabricae_iungere(c->piscina, "", causa, " +");
        causa = chorda_concatenare(causa, numerus_romanus_exprimere(
            (i64)(xar_numerus(ignotae) - I), NIHIL, c->piscina),
            c->piscina);
    }
    redde fabricae_iungere(c->piscina, "", causa,
        " (scriptor ignotus - membra omnia fracta)");
}

/* VERDICTUM membri k, prima causa vincit: non inceptum post fracturam
 * (OMISSUM), non actum, exitus non nullus, scriptura 'S' extra
 * vestigium suum, scriptura ignota undae; aliter post-condicio.
 * VERUM si fractum */
interior b32
_membrum_iudicare (
    constans CursusUndae* c,
                     i32  k,
                  chorda  causa_undae)
{
       i32 i;
    chorda causa;
    chorda aliena;

    i = *(i32*)xar_obtinere(c->indices, k);
    si (   !c->incepta[k] && c->acta[k].codex == -I
        && c->acta[k].duratio_ms    == 0
        && c->acta[k].cauda.mensura == 0)
    {
        c->status[i] = SANANDI_OMISSUM;
        _sanationem_notare(c->sutura, c->piscina, c->sanationes,
            c->actiones[k], FABRICA_OMISSUM,
            chorda_ex_literis("non incepta: fractura in unda (nova "
                "non incipiuntur)", c->piscina), ZEPHYRUM);
        redde VERUM;
    }
    si (!c->incepta[k])
    {
        causa = fabricae_iungere(c->piscina, "non actum: ",
            c->acta[k].cauda, "");
    }
    alioquin si (c->acta[k].codex != 0)
    {
        character numerus[XXXII];

        sprintf(numerus, "exitus %d: ", (integer)c->acta[k].codex);
        causa = fabricae_iungere(c->piscina, numerus, c->acta[k].cauda,
            "");
    }
    alioquin
    {
        aliena = _scripturam_alienam(c->actiones[k], c->notatae[k],
            c->piscina);
        causa  = (aliena.mensura > 0)
            ? fabricae_iungere(c->piscina,
                "scripsit extra vestigium (S): ", aliena, "")
            : causa_undae;
    }
    si (causa.mensura > 0)
    {
        _post_agere(c->sutura, c->actiones[k], i, c->status, c->stala,
            c->sanationes, FALSUM, causa, c->acta[k].duratio_ms,
            c->piscina);
        redde VERUM;
    }
    _post_agere(c->sutura, c->actiones[k], i, c->status, c->stala,
        c->sanationes, VERUM, chorda_ex_literis("", c->piscina),
        c->acta[k].duratio_ms, c->piscina);
    redde c->status[i] == SANANDI_FRACTUM;
}

/* UNDA una simul: membra tuta (indices in ordine) ordine tituli,
 * photographia una circa undam, liber per membrum, verdictum per
 * membrum. VERUM si quid fractum. */
interior b32
_undam_agere (
    constans FabricaSutura* sutura,
              constans Xar* ordo,
                       Xar* indices,
             StatusSanandi* status,
                    chorda* stala,
                       Xar* sanationes,
                   Piscina* piscina)
{
    CursusUndae c;
         chorda causa_undae;
            i32 k;
            b32 fractum;

    memset(&c, ZEPHYRUM, magnitudo(c));
    c.sutura      = sutura;
    c.piscina     = piscina;
    c.ordo        = ordo;
    c.indices     = indices;
    c.status      = status;
    c.stala       = stala;
    c.sanationes  = sanationes;
    c.n           = xar_numerus(indices);
    _membra_ordinare(ordo, indices);
    si (!_undam_parare(&c) || !_undam_currere(&c))
    {
        redde VERUM;
    }
    causa_undae  = _causa_ignotarum(&c, _scripturas_ignotas(&c));
    fractum      = FALSUM;
    per (k = ZEPHYRUM; k < c.n; k++)
    {
        si (_membrum_iudicare(&c, k, causa_undae))
        {
            fractum = VERUM;
        }
    }
    redde fractum;
}

/* AMBITUS SANANDI (fabrica.h; parcum …AG7): actiones viarum electarum
 * (aut omnes praeter iudicia si electa NIHIL), deinde omnes supra eas
 * (clausura per fabricae_pendet) */
b32*
fabrica_ambitum_sanandi (
    constans Xar* ordo,
    constans Xar* electa,
         Piscina* piscina,
          chorda* causa_out)
{
    b32* ambitus;
    i32  numerus;
    i32  i;
    i32  j;
    b32  mutatum;

    numerus  = xar_numerus(ordo);
    ambitus  = (b32*)piscina_allocare(piscina,
        magnitudo(b32) * (memoriae_index)(numerus + 1));
    si (ambitus == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        /* iudicium (porta) solum NOMINATUM sanatur: 'sanare' sine
         * argumentis suitas probationum non currit */
        ambitus[i]  = (electa == NIHIL)
            && (*(FabricaActio**)xar_obtinere(ordo, i))->genus
                   != FABRICA_ACTIO_IUDICIUM;
    }
    per (j = ZEPHYRUM; electa != NIHIL && j < xar_numerus(electa); j++)
    {
        chorda via;
           b32 inventa;

        via      = *(chorda*)xar_obtinere(electa, j);
        inventa  = FALSUM;
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            constans FabricaActio* actio;
                              i32  k;

            actio = *(FabricaActio**)xar_obtinere(ordo, i);
            per (k = ZEPHYRUM; k < xar_numerus(actio->exitus); k++)
            {
                si (chorda_aequalis(((FabricaExitus*)xar_obtinere(
                        actio->exitus, k))->via, via))
                {
                    ambitus[i]  = VERUM;
                    inventa     = VERUM;
                }
            }
        }
        si (!inventa)
        {
            fabricae_causam_ponere(causa_out, piscina,
                "artificium a nulla actione productum: ", via, "");
            redde NIHIL;
        }
    }
    fac
    {
        mutatum = FALSUM;
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            si (!ambitus[i])
            {
                perge;
            }
            per (j = ZEPHYRUM; j < numerus; j++)
            {
                si (   !ambitus[j]
                    && fabricae_pendet(
                        *(FabricaActio**)xar_obtinere(ordo, i),
                        *(FabricaActio**)xar_obtinere(ordo, j)))
                {
                    ambitus[j]  = VERUM;
                    mutatum     = VERUM;
                }
            }
        }
    }
    dum (mutatum);
    redde ambitus;
}

interior s32
_chordas_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde chorda_comparare(*(constans chorda*)a, *(constans chorda*)b);
}

/* exemplar intra arborem? ('~' domus et '/' absolutum: regulae
 * consilii eas iudicant, numquam vestigium) */
interior vacuum
_exemplar_addere (
          Xar* exemplaria,
       chorda  via,
      Piscina* piscina,
          b32  directorium)
{
    si (   via.mensura         == ZEPHYRUM || via.datum[ZEPHYRUM] == '~'
        || via.datum[ZEPHYRUM] == '/')
    {
        redde;
    }
    fabricae_chordam_addere(exemplaria, via);
    si (directorium)
    {
        fabricae_chordam_addere(exemplaria, fabricae_iungere(piscina,
            "",
            via, via.datum[via.mensura - I] == '/' ? "*" : "/*"));
    }
}

/* VESTIGIUM SANANDI (fabrica.h; parcum …AG7) */
Xar*
fabrica_vestigium_sanandi (
    constans Xar* ordo,
    constans Xar* electa,
         Piscina* piscina,
          chorda* causa_out)
{
     b32* ambitus;
     b32* iudicum;
     Xar* viae_iudicum;
     Xar* exemplaria;
     Xar* unica;
     i32  i;
     i32  k;

    ambitus = fabrica_ambitum_sanandi(ordo, electa, piscina, causa_out);
    viae_iudicum = fabricae_xar_chordarum(piscina);
    exemplaria = fabricae_xar_chordarum(piscina);
    unica = fabricae_xar_chordarum(piscina);
    si (   ambitus    == NIHIL || viae_iudicum == NIHIL
        || exemplaria == NIHIL
        || unica      == NIHIL)
    {
        redde NIHIL;
    }
    /* STADIUM IUDICUM: iudices stali ante electa sanantur; quis stalus
     * sit ante cursum non scitur - omnes */
    per (i = ZEPHYRUM; i < xar_numerus(ordo); i++)
    {
        constans FabricaActio* actio =
            *(FabricaActio**)xar_obtinere(ordo,
            i);

        per (k = ZEPHYRUM; actio->iudex
            && k < xar_numerus(actio->exitus);
             k++)
        {
            fabricae_chordam_addere(viae_iudicum,
                ((FabricaExitus*)xar_obtinere(actio->exitus, k))->via);
        }
    }
    iudicum = (xar_numerus(viae_iudicum) > ZEPHYRUM)
        ? fabrica_ambitum_sanandi(ordo, viae_iudicum, piscina,
        causa_out)
        : NIHIL;
    per (i = ZEPHYRUM; i < xar_numerus(ordo); i++)
    {
        constans FabricaActio* actio =
            *(FabricaActio**)xar_obtinere(ordo,
            i);

        si (!ambitus[i] && (iudicum == NIHIL || !iudicum[i]))
        {
            perge;
        }
        per (k = ZEPHYRUM; k < xar_numerus(actio->exitus); k++)
        {
            _exemplar_addere(exemplaria, ((FabricaExitus*)xar_obtinere(
                actio->exitus, k))->via, piscina, FALSUM);
        }
        per (k = ZEPHYRUM; actio->vestigia != NIHIL
             && k < xar_numerus(actio->vestigia); k++)
        {
            /* vestigium: plagula aut directorium - utrumque tegitur */
            _exemplar_addere(exemplaria, ((FabricaLocus*)xar_obtinere(
                actio->vestigia, k))->via, piscina, VERUM);
        }
    }
    xar_ordinare(exemplaria, _chordas_comparare);
    per (i = ZEPHYRUM; i < xar_numerus(exemplaria); i++)
    {
        chorda e = *(chorda*)xar_obtinere(exemplaria, i);

        si (   xar_numerus(unica) == ZEPHYRUM
            || !chorda_aequalis(e, *(chorda*)xar_obtinere(unica,
                   xar_numerus(unica) - I)))
        {
            fabricae_chordam_addere(unica, e);
        }
    }
    redde unica;
}

/* PISCINA ACTI (parcum …T77EH4): actum quodque cum post-condicione
 * sua in piscina temporaria currit, post actum vacata (photographiae
 * arboris bis, libri lectionum, acta: ~LX MB per membrum, olim usque
 * ad finem cursus servati - cursus radicis -vis XVI GB). Quod ultra
 * actum vivit sutura->piscina_cursus tenet; sine ea nulla temporaria
 * (NIHIL): omnia in piscina cursus, ut olim */
interior Piscina*
_piscina_acti (
    Piscina* temporaria,
    Piscina* piscina)
{
    redde (temporaria != NIHIL) ? temporaria : piscina;
}

/* vacare PER NULLA (piscina_vacare octetos delet): monstrator errans in
 * actum priorem nihil legit, non valores veteres quasi vivos */
interior vacuum
_actum_vacare (
    Piscina* temporaria)
{
    si (temporaria != NIHIL)
    {
        piscina_vacare(temporaria);
    }
}

/* CURSUS UNDARUM (plan 2 T6; per gradus fabrica-6): status communis
 * omnium undarum. 'fractum' = fractura vera (membrum gradus excepto):
 * post eam nihil novum incipitur */
nomen structura {
    constans FabricaSutura* sutura;
                   Piscina* piscina;
                   Piscina* temporaria;  /* NIHIL = piscina */
              constans Xar* ordo;
             StatusSanandi* status;
                    chorda* stala;
                       Xar* sanationes;
                       b32  fractum;
} CursusUndarum;

/* actio ordinis 'index' */
interior FabricaActio*
_actio_ordinis (
    constans CursusUndarum* cursus,
                       i32  index)
{
    redde *(FabricaActio**)xar_obtinere(cursus->ordo, index);
}

/* post fracturam: non incipitur - OMISSUM 'post fracturam' */
interior vacuum
_post_fracturam_omittere (
    CursusUndarum* cursus,
              i32  index)
{
    cursus->status[index] = SANANDI_OMISSUM;
    _sanationem_notare(cursus->sutura, cursus->piscina,
        cursus->sanationes,
        _actio_ordinis(cursus, index), FABRICA_OMISSUM,
        chorda_ex_literis("post fracturam: nova non incipiuntur",
            cursus->piscina), ZEPHYRUM);
}

/* membrum post fracturam: nihil incipitur, sed iudicatur - dependentia
 * fracta nominatur, recens tacet, agendum OMISSUM 'post fracturam' */
interior vacuum
_membrum_post_fracturam (
    CursusUndarum* cursus,
              i32  index)
{
    FabricaActio* actio;
          chorda  fracta;
          chorda  causa;

    actio = _actio_ordinis(cursus, index);
    causa = chorda_ex_literis("", cursus->piscina);
    si (_dependentia_fracta(cursus->ordo, index, cursus->status,
        &fracta))
    {
        cursus->status[index] = SANANDI_OMISSUM;
        _sanationem_notare(cursus->sutura, cursus->piscina,
            cursus->sanationes,
            actio, FABRICA_OMISSUM, fabricae_iungere(cursus->piscina,
                "dependentia fracta: ", fracta, ""), ZEPHYRUM);
    }
    alioquin si (_exitus_recentes(cursus->sutura, actio,
                 cursus->piscina,
                 &causa))
    {
        cursus->status[index] = SANANDI_RECENS;
    }
    alioquin
    {
        _post_fracturam_omittere(cursus, index);
    }
}

/* actio SOLA (via vetus, photographia sua): actum et post-condicio.
 * VERUM si fracta */
interior b32
_solam_agere (
    CursusUndarum* cursus,
              i32  index)
{
         i32  duratio;
      chorda  causa;
         b32  actum;
     Piscina* acti;

    acti     = _piscina_acti(cursus->temporaria, cursus->piscina);
    duratio  = ZEPHYRUM;
    causa    = chorda_ex_literis("", acti);
    actum    = _actionem_agere(cursus->sutura, _actio_ordinis(cursus,
        index),
        acti, &duratio, &causa);
    _post_agere(cursus->sutura, _actio_ordinis(cursus, index), index,
        cursus->status,
        cursus->stala, cursus->sanationes, actum, causa, duratio,
        acti);
    _actum_vacare(cursus->temporaria);
    redde cursus->status[index] == SANANDI_FRACTUM;
}

/* membra undae dividere: post fracturam iudicantur solum; nihil
 * agendum (_ante_agere) praetermittitur; tutae (lectiones, non
 * iudicium - iudicium numquam simul: scripturae suitae totae) in
 * 'tuta', ceterae in 'ceteri' */
interior vacuum
_undam_dividere (
     CursusUndarum* cursus,
      constans Xar* membra,
               Xar* tuta,
               Xar* ceteri)
{
    i32 m;

    per (m = ZEPHYRUM; m < xar_numerus(membra); m++)
    {
        FabricaActio* actio;
                 s32  index;

        actio = *(FabricaActio**)xar_obtinere(membra, m);
        index = _index_actionis(cursus->ordo, actio->titulus);
        si (index < 0)
        {
            perge;
        }
        si (cursus->fractum)
        {
            _membrum_post_fracturam(cursus, (i32)index);
            perge;
        }
        si (!_ante_agere(cursus->sutura, cursus->ordo, (i32)index,
            cursus->status,
                cursus->stala, cursus->sanationes, FALSUM,
                cursus->piscina))
        {
            perge;
        }
        si (actio->lectiones && actio->genus != FABRICA_ACTIO_IUDICIUM)
        {
            *(i32*)xar_addere(tuta) = (i32)index;
        }
        alioquin
        {
            *(i32*)xar_addere(ceteri) = (i32)index;
        }
    }
}

/* non tutae: SOLAE, ordine. MEMBRA GRADUS (fabrica-6 T8): fractura
 * membri fratres non sistit - membra independentia sunt et verdictum
 * gradus omnia nominare debet; dependentes per _ante_agere OMISSI
 * manent */
interior vacuum
_ceteros_agere (
    CursusUndarum* cursus,
     constans Xar* ceteri)
{
    i32 m;

    per (m = ZEPHYRUM; m < xar_numerus(ceteri); m++)
    {
        i32 index;

        index = *(i32*)xar_obtinere(ceteri, m);
        si (cursus->fractum)
        {
            _post_fracturam_omittere(cursus, index);
        }
        alioquin si (   _solam_agere(cursus, index)
                     && _actio_ordinis(cursus, index)->membrum == NIHIL)
        {
            cursus->fractum = VERUM;
        }
    }
}

/* tutae: post fracturam OMISSAE; sola via vetere (nihil simul); plures
 * simul (_undam_agere) */
interior vacuum
_tutas_agere (
    CursusUndarum* cursus,
              Xar* tuta)
{
    i32 m;

    si (cursus->fractum)
    {
        per (m = ZEPHYRUM; m < xar_numerus(tuta); m++)
        {
            _post_fracturam_omittere(cursus, *(i32*)xar_obtinere(tuta,
                m));
        }
    }
    alioquin si (xar_numerus(tuta) == I)
    {
        si (_solam_agere(cursus, *(i32*)xar_obtinere(tuta, ZEPHYRUM)))
        {
            cursus->fractum = VERUM;
        }
    }
    alioquin si (xar_numerus(tuta) > I)
    {
        /* unda tota una piscina temporaria (photographia una undae) */
        si (_undam_agere(cursus->sutura, cursus->ordo, tuta,
                cursus->status, cursus->stala, cursus->sanationes,
                _piscina_acti(cursus->temporaria, cursus->piscina)))
        {
            cursus->fractum = VERUM;
        }
        _actum_vacare(cursus->temporaria);
    }
}

/* PROGRESSUS (colloquia T8): vocatio una si sutura eam praebet;
 * siccum nihil agit, ergo nihil notat */
interior vacuum
_progressum_notare (
    constans FabricaSutura* sutura,
                       b32  siccum,
                       i32  iudicata,
                       i32  ambitus,
                       i32  unda,
                       i32  undae)
{
    si (!siccum && sutura->progressum_notare != NIHIL)
    {
        sutura->progressum_notare(sutura->datum, iudicata, ambitus,
            unda,
            undae);
    }
}

/* SANARE PER UNDAS (plan 2 T6): undae ex actionibus ambitus (ignotae
 * excluduntur - exitus non iudicati); in unda non tutae SOLAE
 * (photographia sua), tutae (lectiones="verum") simul. Fractura: nova
 * non incipiuntur (OMISSUM), dependentes OMISSUM. */
interior b32
_sanare_per_undas (
    constans FabricaSutura* sutura,
              constans Xar* ordo,
                       b32* ambitus,
             StatusSanandi* status,
                    chorda* stala,
                       Xar* sanationes,
                   Piscina* piscina,
                   Piscina* temporaria)
{
    CursusUndarum  cursus;
              Xar* membra_ambitus;
              Xar* undae;
              i32  iudicata;
              i32  i;
              i32  w;

    membra_ambitus = xar_creare(piscina, (i32)magnitudo(FabricaActio*));
    si (membra_ambitus == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(ordo); i++)
    {
        FabricaActio* actio;

        actio = *(FabricaActio**)xar_obtinere(ordo, i);
        si (ambitus[i] && !_actio_ignota(actio))
        {
            *(FabricaActio**)xar_addere(membra_ambitus) = actio;
        }
    }
    undae = fabrica_undas_formare(membra_ambitus, piscina);
    si (undae == NIHIL)
    {
        redde FALSUM;
    }
    cursus.sutura      = sutura;
    cursus.piscina     = piscina;
    cursus.temporaria  = temporaria;
    cursus.ordo        = ordo;
    cursus.status      = status;
    cursus.stala       = stala;
    cursus.sanationes  = sanationes;
    cursus.fractum     = FALSUM;
    iudicata           = ZEPHYRUM;
    _progressum_notare(sutura, FALSUM, ZEPHYRUM, xar_numerus(
        membra_ambitus), ZEPHYRUM, xar_numerus(undae));
    per (w = ZEPHYRUM; w < xar_numerus(undae); w++)
    {
        Xar* membra;
        Xar* tuta;
        Xar* ceteri;

        membra  = *(Xar**)xar_obtinere(undae, w);
        tuta    = xar_creare(piscina, (i32)magnitudo(i32));
        ceteri  = xar_creare(piscina, (i32)magnitudo(i32));
        si (tuta == NIHIL || ceteri == NIHIL)
        {
            redde FALSUM;
        }
        /* PRAEVISIO (T6b): iudicia membrorum tutorum simul; deinde
         * OMNIA membra iudicantur ANTE quodvis actum (actum memoriam
         * regenerationum vacat) - membra undae independentia sunt */
        fabrica_regenerationes_praevidere(sutura, membra, VERUM,
            piscina);
        _undam_dividere(&cursus, membra, tuta, ceteri);
        _ceteros_agere(&cursus, ceteri);
        _tutas_agere(&cursus, tuta);
        iudicata += xar_numerus(membra);
        _progressum_notare(sutura, FALSUM, iudicata, xar_numerus(
            membra_ambitus), w + I, xar_numerus(undae));
    }
    redde VERUM;
}

Xar*
fabrica_sanare (
    constans FabricaSutura* sutura,
              constans Xar* ordo,
              constans Xar* electa,
                       b32  siccum,
                   Piscina* piscina,
                    chorda* causa_out)
{
               Xar* sanationes;
               b32* ambitus;
     StatusSanandi* status;
            chorda* stala;
           Piscina* temporaria;
               i32  numerus;
               i32  in_ambitu;
               i32  iudicata;
               i32  i;

    numerus     = xar_numerus(ordo);
    sanationes  = xar_creare(piscina, (i32)magnitudo(FabricaSanatio));
    status = (StatusSanandi*)piscina_allocare(piscina,
        magnitudo(StatusSanandi) * (memoriae_index)(numerus + 1));
    stala = (chorda*)piscina_allocare(piscina,
        magnitudo(chorda) * (memoriae_index)(numerus + 1));
    si (sanationes == NIHIL || status == NIHIL || stala == NIHIL)
    {
        redde NIHIL;
    }
    memset(stala, ZEPHYRUM, magnitudo(chorda) * (memoriae_index)(numerus
        + 1));
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        status[i] = SANANDI_NONDUM;
    }
    ambitus = fabrica_ambitum_sanandi(ordo, electa, piscina, causa_out);
    si (ambitus == NIHIL)
    {
        redde NIHIL;
    }

    /* PISCINA ACTI: solum si quod ultra actum vivit domum habet */
    temporaria = (sutura->piscina_cursus != NIHIL && !siccum)
        ? piscina_generare_dynamicum("fabrica_actum", 4194304) : NIHIL;
    si (sutura->agere_simul != NIHIL && !siccum)
    {
        b32 bonum;

        bonum = _sanare_per_undas(sutura, ordo, ambitus, status, stala,
            sanationes, piscina, temporaria);
        si (temporaria != NIHIL)
        {
            piscina_destruere(temporaria);
        }
        redde bonum ? sanationes : NIHIL;
    }
    /* via seriatim: progressus post actionem quamque ambitus */
    in_ambitu  = ZEPHYRUM;
    iudicata   = ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (ambitus[i] && !_actio_ignota(*(FabricaActio**)xar_obtinere(
            ordo, i)))
        {
            in_ambitu++;
        }
    }
    _progressum_notare(sutura, siccum, ZEPHYRUM, in_ambitu, ZEPHYRUM,
        ZEPHYRUM);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        constans FabricaActio* actio;
                       chorda  causa;
                          i32  duratio;
                          b32  actum;

        actio = *(FabricaActio**)xar_obtinere(ordo, i);
        si (!ambitus[i] || _actio_ignota(actio))
        {
            perge;   /* ignota: praecondicione sola realizatur */
        }
        iudicata++;
        si (!_ante_agere(sutura, ordo, i, status, stala, sanationes,
            siccum,
                piscina))
        {
            _progressum_notare(sutura, siccum, iudicata, in_ambitu,
                ZEPHYRUM, ZEPHYRUM);
            perge;
        }
        duratio  = ZEPHYRUM;
        causa    = chorda_ex_literis("", _piscina_acti(temporaria,
            piscina));
        actum = _actionem_agere(sutura, actio, _piscina_acti(temporaria,
            piscina), &duratio, &causa);
        _post_agere(sutura, actio, i, status, stala, sanationes, actum,
            causa,
            duratio, _piscina_acti(temporaria, piscina));
        _actum_vacare(temporaria);
        _progressum_notare(sutura, siccum, iudicata, in_ambitu,
            ZEPHYRUM,
            ZEPHYRUM);
    }
    si (temporaria != NIHIL)
    {
        piscina_destruere(temporaria);
    }
    redde sanationes;
}
