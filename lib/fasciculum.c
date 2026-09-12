/* fasciculum.c - fasciculi .app (contractus in capite).
 *
 * Info.plist dictio PlistValor est quam plist_scribere serializat;
 * reddere nihil de disco legit (vias solum exsistere probat), ergo
 * dispositio tota sine systemate plagularum probari potest.
 *
 * FUSIO UNO MODO TANTUM: claves generatae primum ponuntur, deinde par
 * omne plistae_extra super eas - plist_dictio_ponere clavem
 * exsistentem IN LOCO substituit (include/plist.h:118), ergo vocans
 * vincit (D8) et ordo clavium generatarum servatur. Custodia altera
 * (clavem generatam OMITTERE si vocans eam iam habet) DELETA est:
 * idem effecit, et duae viae idem exprimentes portam H2 viridem sub
 * culpa plantata tenuissent. Vocans claves NOVAS quoque adfert, ergo
 * ansa necessaria est et custodia non erat.
 */

#include "fasciculum.h"
#include "filum.h"
#include "via.h"

interior b32
_recusare (
    FasciculumStatus* status,
              chorda* sedes_vitii,
    FasciculumStatus  valor,
              chorda  sedes)
{
    *status       = valor;
    *sedes_vitii  = sedes;
    redde FALSUM;
}

/* Clavem ponere. Fructus NON per clavem probatur sed per invariantem
 * post structuram (claves necessariae adsunt): piscina defectura
 * omnes pariter frangeret, et decem probationes idem dicerent. */
interior vacuum
_clavem_ponere (
             PlistValor* dictio,
     constans character* clavis,
             PlistValor* valor,
                Piscina* piscina)
{
    plist_dictio_ponere(dictio, chorda_ex_literis(clavis, piscina),
                        valor, piscina);
}

interior chorda
_semita_iuncta (
     constans character* a,
                 chorda  b,
                Piscina* piscina)
{
    chorda partes[II];

    partes[ZEPHYRUM]  = chorda_ex_literis(a, piscina);
    partes[I]         = b;
    redde via_iungere(partes, II, piscina);
}

b32
fasciculum_reddere (
    constans Fasciculum*  f,
                    Xar** partes,
       FasciculumStatus*  status,
                 chorda*  sedes_vitii,
                Piscina*  piscina)
{
          PlistValor* dictio;
         PlistStatus  plista_status;
              chorda  titulus_exsecutabilis;
              chorda  octeti;
              chorda  vacua;
      FasciculumPars* pars;
         FilumStatus  st;

    /* status et sedes ANTE omnia: aliter scriptio prima per indicem
     * non probatum iret */
    si (!status || !sedes_vitii)
    {
        redde FALSUM;
    }

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    *status        = FASCICULUM_SUCCESSUS;
    *sedes_vitii   = vacua;

    si (!f || !partes || !piscina)
    {
        redde _recusare(status, sedes_vitii, FASCICULUM_ERROR_DESUNT,
                        vacua);
    }
    /* Recusatio clavem DEFICIENTEM nominat, non chordam vacuam quam
     * modo respuit (par. III: refusio quae rem suam non nominat
     * agibilis non est) */
    si (chorda_vacua(f->identitas))
    {
        redde _recusare(status, sedes_vitii, FASCICULUM_ERROR_DESUNT,
                        chorda_ex_literis("CFBundleIdentifier",
                                          piscina));
    }
    si (!f->exsecutabile)
    {
        redde _recusare(status, sedes_vitii, FASCICULUM_ERROR_DESUNT,
                        chorda_ex_literis("CFBundleExecutable",
                                          piscina));
    }
    si (!filum_status(f->exsecutabile, &st) || !st.est_filum)
    {
        redde _recusare(status, sedes_vitii,
                        FASCICULUM_ERROR_EXSECUTABILE,
                        chorda_ex_literis(f->exsecutabile, piscina));
    }
    si (f->icon && (!filum_status(f->icon, &st) || !st.est_filum))
    {
        redde _recusare(status, sedes_vitii, FASCICULUM_ERROR_ICON,
                        chorda_ex_literis(f->icon, piscina));
    }

    titulus_exsecutabilis = via_nomen(
        chorda_ex_literis(f->exsecutabile, piscina), piscina);

    /* ---- Info.plist ---- */
    dictio = plist_dictio_creare(piscina);
    si (!dictio)
    {
        redde _recusare(status, sedes_vitii, FASCICULUM_ERROR_MEMORIA,
                        vacua);
    }
    _clavem_ponere(dictio, "CFBundleIdentifier",
        plist_chordam_creare(f->identitas, piscina), piscina);
    _clavem_ponere(dictio, "CFBundleExecutable",
        plist_chordam_creare(titulus_exsecutabilis, piscina), piscina);
    _clavem_ponere(dictio, "CFBundleName",
        plist_chordam_creare(chorda_vacua(f->titulus)
            ? titulus_exsecutabilis : f->titulus, piscina), piscina);
    _clavem_ponere(dictio, "CFBundlePackageType",
        plist_chordam_creare(chorda_ex_literis("APPL", piscina),
                             piscina), piscina);
    _clavem_ponere(dictio, "CFBundleInfoDictionaryVersion",
        plist_chordam_creare(chorda_ex_literis("6.0", piscina),
                             piscina), piscina);
    _clavem_ponere(dictio, "CFBundleShortVersionString",
        plist_chordam_creare(chorda_vacua(f->versio)
            ? chorda_ex_literis("1.0", piscina) : f->versio, piscina),
        piscina);
    _clavem_ponere(dictio, "CFBundleVersion",
        plist_chordam_creare(chorda_vacua(f->versio_aedificationis)
            ? chorda_ex_literis("1", piscina)
            : f->versio_aedificationis, piscina), piscina);
    si (f->icon)
    {
        _clavem_ponere(dictio, "CFBundleIconFile",
            plist_chordam_creare(via_nomen(
                chorda_ex_literis(f->icon, piscina), piscina),
                piscina), piscina);
    }
    si (f->sine_scandali)
    {
        _clavem_ponere(dictio, "LSUIElement",
            plist_veritatem_creare(VERUM, piscina), piscina);
    }
    /* SEMPER: sine hoc fenestra in scrinio retinae scalata et obscura
     * redditur - vitium quod consilium simulat */
    _clavem_ponere(dictio, "NSHighResolutionCapable",
        plist_veritatem_creare(VERUM, piscina), piscina);

    /* Claves vocantis SUPER generatas (D8), etiam generatas
     * substituentes: plist_dictio_ponere in loco substituit. */
    si (f->plista_extra)
    {
        i32 i;

        per (i = ZEPHYRUM; i < plist_numerus(f->plista_extra); i++)
        {
            PlistPar* par = (PlistPar*)xar_obtinere(
                f->plista_extra->liberi, i);

            si (par)
            {
                plist_dictio_ponere(dictio, par->clavis, par->valor,
                                    piscina);
            }
        }
    }

    /* Invarians post structuram: claves sine quibus fasciculus
     * fractus est ADSUNT. Hoc omnem positionem fractam capit sine
     * decem probationibus quae idem dicerent. */
    si (   !plist_dictio_capere(dictio, "CFBundleIdentifier")
        || !plist_dictio_capere(dictio, "CFBundleExecutable"))
    {
        redde _recusare(status, sedes_vitii, FASCICULUM_ERROR_MEMORIA,
                        vacua);
    }

    octeti = plist_scribere(dictio, &plista_status, piscina);
    si (plista_status != PLIST_SUCCESSUS || chorda_vacua(octeti))
    {
        redde _recusare(status, sedes_vitii, FASCICULUM_ERROR_PLISTA,
                        chorda_ex_literis("Contents/Info.plist",
                                          piscina));
    }

    /* ---- planum ---- */
    *partes = xar_creare(piscina, (i32)magnitudo(FasciculumPars));
    si (!*partes)
    {
        redde _recusare(status, sedes_vitii, FASCICULUM_ERROR_MEMORIA,
                        vacua);
    }

    pars = (FasciculumPars*)xar_addere(*partes);
    si (!pars)
    {
        redde _recusare(status, sedes_vitii, FASCICULUM_ERROR_MEMORIA,
                        vacua);
    }
    pars->origo              = FASCICULUM_GENITUM;
    pars->semita             = chorda_ex_literis(
        "Contents/Info.plist", piscina);
    pars->octeti             = octeti;
    pars->fons               = NIHIL;
    pars->exsecutabile_fiat  = FALSUM;

    pars = (FasciculumPars*)xar_addere(*partes);
    si (!pars)
    {
        redde _recusare(status, sedes_vitii, FASCICULUM_ERROR_MEMORIA,
                        vacua);
    }
    pars->origo              = FASCICULUM_COPIATUM;
    pars->semita             = _semita_iuncta("Contents/MacOS",
        titulus_exsecutabilis, piscina);
    pars->octeti             = vacua;
    pars->fons               = f->exsecutabile;
    pars->exsecutabile_fiat  = VERUM;

    si (f->icon)
    {
        pars = (FasciculumPars*)xar_addere(*partes);
        si (!pars)
        {
            redde _recusare(status, sedes_vitii,
                            FASCICULUM_ERROR_MEMORIA, vacua);
        }
        pars->origo              = FASCICULUM_COPIATUM;
        pars->semita             = _semita_iuncta("Contents/Resources",
            via_nomen(chorda_ex_literis(f->icon, piscina), piscina),
            piscina);
        pars->octeti             = vacua;
        pars->fons               = f->icon;
        pars->exsecutabile_fiat  = FALSUM;
    }
    redde VERUM;
}

b32
fasciculum_scribere (
           constans Xar* partes,
     constans character* via_radicis,
       FasciculumStatus* status,
                 chorda* sedes_vitii,
                Piscina* piscina)
{
    chorda vacua;
       i32 i;

    si (!status || !sedes_vitii)
    {
        redde FALSUM;
    }

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    *status        = FASCICULUM_SUCCESSUS;
    *sedes_vitii   = vacua;

    si (!partes || !via_radicis || !piscina)
    {
        redde _recusare(status, sedes_vitii, FASCICULUM_ERROR_DESUNT,
                        vacua);
    }

    per (i = ZEPHYRUM; i < xar_numerus(partes); i++)
    {
        FasciculumPars* pars;
                chorda  absoluta;
             character* via_c;
                chorda  directorium;

        pars = (FasciculumPars*)xar_obtinere(partes, i);
        si (!pars)
        {
            redde _recusare(status, sedes_vitii,
                            FASCICULUM_ERROR_MEMORIA, vacua);
        }
        absoluta     = _semita_iuncta(via_radicis, pars->semita,
                                      piscina);
        via_c        = chorda_ut_cstr(absoluta, piscina);
        directorium  = via_directorium(absoluta, piscina);
        si (!via_c || chorda_vacua(directorium))
        {
            redde _recusare(status, sedes_vitii,
                            FASCICULUM_ERROR_MEMORIA, pars->semita);
        }

        /* CUM PARENTIBUS: Contents/MacOS/ duos gradus ab area virgine
         * poscit, et creator unius gradus hic de ENOENT deficeret
         * (filum, par modi II) */
        si (!filum_directorium_creare_cum_parentibus(
                chorda_ut_cstr(directorium, piscina)))
        {
            redde _recusare(status, sedes_vitii,
                            FASCICULUM_ERROR_DIRECTORIUM,
                            pars->semita);
        }

        si (pars->origo == FASCICULUM_GENITUM)
        {
            si (!filum_scribere(via_c, pars->octeti))
            {
                redde _recusare(status, sedes_vitii,
                                FASCICULUM_ERROR_SCRIPTIO,
                                pars->semita);
            }
        }
        alioquin
        {
            si (!pars->fons || !filum_copiare(pars->fons, via_c))
            {
                redde _recusare(status, sedes_vitii,
                                FASCICULUM_ERROR_SCRIPTIO,
                                pars->semita);
            }
        }

        /* Modus SOLUM ubi planum eum poscit: copia octetorum nuda
         * modum non servat (filum, par modi I) */
        si (   pars->exsecutabile_fiat
            && !filum_modum_ponere(via_c, 0755))
        {
            redde _recusare(status, sedes_vitii, FASCICULUM_ERROR_MODUS,
                            pars->semita);
        }
    }
    redde VERUM;
}
