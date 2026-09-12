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
/* strcmp, memset: lector eis utitur. In opere II consulto omissum quia
 * reddere sola nullo eorum egebat - recte tunc, falsum nunc. 'Include
 * solum quod uteris' cum plagula functionem alteram accipit iterum
 * quaerendum est, non semel constituendum. */
#include <string.h>

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

/* ORDINARIA UNO LOCO. reddere ea scribit, aequalis ea supponit: si
 * duobus locis viverent, aequalitas 'effectiva' vera esset casu, non
 * constructione. Literae ipsae hic solae vivunt. */
#define FASCICULUM_VERSIO_ORDINARIA        "1.0"
#define FASCICULUM_AEDIFICATIO_ORDINARIA   "1"

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

/* Clavis quam valor ipse fert, ergo in plista_extra NON manet */
interior b32
_clavis_levata (
    constans character* clavis)
{
    constans character* levatae[X];
                   i32  i;

    levatae[0] = "CFBundleIdentifier";
    levatae[1] = "CFBundleExecutable";
    levatae[2] = "CFBundleName";
    levatae[3] = "CFBundlePackageType";
    levatae[4] = "CFBundleInfoDictionaryVersion";
    levatae[5] = "CFBundleShortVersionString";
    levatae[6] = "CFBundleVersion";
    levatae[7] = "CFBundleIconFile";
    levatae[8] = "LSUIElement";
    levatae[9] = "NSHighResolutionCapable";

    per (i = ZEPHYRUM; i < X; i++)
    {
        si (strcmp(clavis, levatae[i]) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior chorda
_textum_clavis (
             PlistValor* dictio,
     constans character* clavis)
{
     PlistValor* v;
         chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    v = plist_dictio_capere(dictio, clavis);
    si (!v || v->genus != PLIST_CHORDA)
    {
        redde vacua;
    }
    redde v->textus;
}

b32
fasciculum_legere (
     constans character* via_radicis,
             Fasciculum* f,
       FasciculumStatus* status,
                 chorda* sedes_vitii,
                Piscina* piscina,
    InternamentumChorda* intern)
{
           chorda  vacua;
           chorda  semita_contents;
           chorda  semita_plistae;
           chorda  titulus_exs;
    PlistResultus  r;
       PlistValor* icon_valor;
       PlistValor* scandalum;
      FilumStatus  st;
              i32  i;

    si (!status || !sedes_vitii)
    {
        redde FALSUM;
    }

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    *status        = FASCICULUM_SUCCESSUS;
    *sedes_vitii   = vacua;

    si (!via_radicis || !f || !piscina || !intern)
    {
        redde _recusare(status, sedes_vitii, FASCICULUM_ERROR_DESUNT,
                        vacua);
    }

    memset(f, ZEPHYRUM, magnitudo(Fasciculum));

    /* Contents/ ANTE plistam: directorium quod fasciculus non est
     * 'non fasciculus' dicendum est, non 'plista fracta' */
    semita_contents = _semita_iuncta(via_radicis,
        chorda_ex_literis("Contents", piscina), piscina);
    si (!filum_directorium_existit(
            chorda_ut_cstr(semita_contents, piscina)))
    {
        redde _recusare(status, sedes_vitii,
                        FASCICULUM_ERROR_NON_FASCICULUM,
                        chorda_ex_literis("Contents", piscina));
    }

    semita_plistae = _semita_iuncta(via_radicis,
        chorda_ex_literis("Contents/Info.plist", piscina), piscina);
    r = plist_legere_plagulam(chorda_ut_cstr(semita_plistae, piscina),
                              piscina, intern);
    si (   r.status       != PLIST_SUCCESSUS || !r.radix
        || r.radix->genus != PLIST_DICTIO)
    {
        /* Par. VI statum ipsius plist quoque iter facere vult, sed
         * signatura probata locum PlistStatus non habet: semita
         * elementi in sede vehitur, numerus non (nominatum, non
         * tacitum). */
        chorda sedes_plistae;

        sedes_plistae = chorda_ex_literis("Contents/Info.plist",
                                          piscina);
        si (!chorda_vacua(r.semita))
        {
            chorda partes_sedis[II];

            partes_sedis[ZEPHYRUM]  = sedes_plistae;
            partes_sedis[I]         = r.semita;
            sedes_plistae = chorda_iungere(partes_sedis, II,
                chorda_ex_literis(":", piscina), piscina);
        }
        redde _recusare(status, sedes_vitii, FASCICULUM_ERROR_PLISTA,
                        sedes_plistae);
    }

    f->identitas  = _textum_clavis(r.radix, "CFBundleIdentifier");
    f->titulus    = _textum_clavis(r.radix, "CFBundleName");
    f->versio    = _textum_clavis(r.radix,
                                  "CFBundleShortVersionString");
    f->versio_aedificationis = _textum_clavis(r.radix,
                                              "CFBundleVersion");

    scandalum = plist_dictio_capere(r.radix, "LSUIElement");
    f->sine_scandali = (scandalum
        && scandalum->genus == PLIST_VERITAS
        && scandalum->veritas) ? VERUM : FALSUM;

    /* Fasciculus vias SUAS novit, non fontium unde copiatae sunt */
    titulus_exs = _textum_clavis(r.radix, "CFBundleExecutable");
    si (chorda_vacua(titulus_exs))
    {
        redde _recusare(status, sedes_vitii,
                        FASCICULUM_ERROR_EXSECUTABILE,
                        chorda_ex_literis("CFBundleExecutable",
                                          piscina));
    }
    f->exsecutabile = chorda_ut_cstr(_semita_iuncta(via_radicis,
        _semita_iuncta("Contents/MacOS", titulus_exs, piscina),
        piscina), piscina);

    icon_valor = plist_dictio_capere(r.radix, "CFBundleIconFile");
    si (   icon_valor && icon_valor->genus == PLIST_CHORDA
        && !chorda_vacua(icon_valor->textus))
    {
        /* Icon PRAESENTIA non poscitur: par. IV exsecutabile solum
         * confirmandum dicit, et par. XI notat nos .icns numquam
         * iudicare. Asymmetria consulta est. */
        f->icon = chorda_ut_cstr(_semita_iuncta(via_radicis,
            _semita_iuncta("Contents/Resources", icon_valor->textus,
                           piscina), piscina), piscina);
    }

    /* Claves NON levatae in plista_extra manent (par. IV) */
    f->plista_extra = plist_dictio_creare(piscina);
    si (!f->plista_extra)
    {
        redde _recusare(status, sedes_vitii, FASCICULUM_ERROR_MEMORIA,
                        vacua);
    }
    per (i = ZEPHYRUM; i < plist_numerus(r.radix); i++)
    {
         PlistPar* par;
        character* clavis_c;

        par = (PlistPar*)xar_obtinere(r.radix->liberi, i);
        si (!par)
        {
            perge;
        }
        clavis_c = chorda_ut_cstr(par->clavis, piscina);
        si (clavis_c && !_clavis_levata(clavis_c))
        {
            plist_dictio_ponere(f->plista_extra, par->clavis,
                                par->valor, piscina);
        }
    }

    /* EXSECUTABILE: fasciculus sine eo duplici ictu TACET */
    si (!filum_status(f->exsecutabile, &st) || !st.est_filum)
    {
        redde _recusare(status, sedes_vitii,
                        FASCICULUM_ERROR_EXSECUTABILE,
                        _semita_iuncta("Contents/MacOS", titulus_exs,
                                       piscina));
    }
    redde VERUM;
}

/* Aequalitas EFFECTIVA: campus vacuus ordinario suo aequalis est.
 * Aliter scriptor fasciculum iam rectum semper rescriberet, quod huius
 * functionis causa ipsa est (par. IV). */
interior b32
_chordae_effectivae_aequales (
                  chorda  a,
                  chorda  b,
      constans character* ordinarium)
{
    b32 a_vacua;
    b32 b_vacua;

    a_vacua = chorda_vacua(a);
    b_vacua = chorda_vacua(b);

    si (a_vacua && b_vacua)
    {
        redde VERUM;
    }
    si (a_vacua)
    {
        redde chorda_aequalis_literis(b, ordinarium);
    }
    si (b_vacua)
    {
        redde chorda_aequalis_literis(a, ordinarium);
    }
    redde chorda_aequalis(a, b);
}

/* Nomen solum: fasciculus relectus vias fontium numquam novit */
interior constans character*
_basis_semitae (
    constans character* via)
{
    constans character* ultima;

    si (!via)
    {
        redde NIHIL;
    }
    ultima = strrchr(via, '/');
    redde ultima ? ultima + I : via;
}

interior b32
_nomina_aequalia (
    constans character* a,
    constans character* b)
{
    si (!a && !b)
    {
        redde VERUM;
    }
    si (!a || !b)
    {
        redde FALSUM;
    }
    /* NOMEN, non semita: via_nomen allocat et aequalis piscinam non
     * accipit, ergo basis per indicem sola sumitur. Prima forma huius
     * functionis semitas TOTAS conferebat dum nomina conferre se
     * dicebat - et iter reditus H5 id statim rubrum fecit. */
    redde (strcmp(_basis_semitae(a), _basis_semitae(b)) == ZEPHYRUM)
        ? VERUM : FALSUM;
}

b32
fasciculum_aequalis (
    constans Fasciculum* a,
    constans Fasciculum* b)
{
    si (!a || !b)
    {
        redde FALSUM;
    }
    si (!chorda_aequalis(a->identitas, b->identitas))
    {
        redde FALSUM;
    }
    si (!_chordae_effectivae_aequales(a->versio, b->versio,
            FASCICULUM_VERSIO_ORDINARIA))
    {
        redde FALSUM;
    }
    si (!_chordae_effectivae_aequales(a->versio_aedificationis,
            b->versio_aedificationis,
            FASCICULUM_AEDIFICATIO_ORDINARIA))
    {
        redde FALSUM;
    }
    si ((a->sine_scandali ? VERUM : FALSUM)
        != (b->sine_scandali ? VERUM : FALSUM))
    {
        redde FALSUM;
    }
    /* Titulus ordinarium suum ex exsecutabili trahit, ergo nomina
     * ante titulos conferenda sunt */
    si (!_nomina_aequalia(a->exsecutabile, b->exsecutabile))
    {
        redde FALSUM;
    }
    si (!_nomina_aequalia(a->icon, b->icon))
    {
        redde FALSUM;
    }
    /* TITULUS: ordinarium suum ex nomine exsecutabilis trahit, ergo
     * POST nomina confertur (ibi iam aequalia probata sunt). Prima
     * forma hanc collationem OMNINO omittebat - duo fasciculi solo
     * CFBundleName differentes aequales habebantur, et nulla probatio
     * id cepit quia dissimilitudo unum campum solum variabat. */
    si (!_chordae_effectivae_aequales(a->titulus, b->titulus,
            a->exsecutabile ? _basis_semitae(a->exsecutabile) : ""))
    {
        redde FALSUM;
    }
    /* NIHIL et dictio VACUA idem sunt: fasciculus e valore minimo
     * scriptus dictionem vacuam relegit (plist_numerus NIHIL fert,
     * plist_aequalis non) */
    si (   plist_numerus(a->plista_extra) == ZEPHYRUM
        && plist_numerus(b->plista_extra) == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde plist_aequalis(a->plista_extra, b->plista_extra);
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
