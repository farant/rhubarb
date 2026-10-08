/* tools/fabrica_memoria.c - bin/fabrica: MEMORIA (fabrica-6 H4) -
 * build/fabrica.db per scrinium: verificationes, vestigia lectionum,
 * particulae, verdicta, cursus; suturae_memoriam_nectere. sqlite HIC
 * solum, numquam in lib/. Vide tools/fabrica_sutura.h. */

#include "postulata_posix.h"

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "iter_directoria.h"
#include "processus.h"
#include "internamentum.h"
#include "sigillum.h"
#include "git.h"
#include "tabula_dispersa.h"
#include "chorda_aedificator.h"
#include "fabrica.h"
#include "provenientia.h"
#include "scrinium.h"
#include "numerus_romanus.h"
#include "thesaurus.h"
#include "via.h"
#include "aedilis.h"
#include "aedilis_silva.h"
#include "compilator.h"
#include "fabrica_sutura.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>


/* ==================================================
 * Memoria verificationum (build/fabrica.db; plan 1a T6)
 *
 * Ordo = (titulus, clavis, artificium): clavis = sigillum ingressuum
 * ET mandati, artificium = sigillum commissi - ambo congruere debent
 * (artificium manu mutatum numquam memoria tegitur). Nucleus solus
 * decernit quando scribitur (post regenerationem congruentem actionis
 * memorabilis) et quando legitur (actio memorabilis). sqlite HIC
 * solum, numquam in lib/ (nexus caeci obiectorum build/).
 * ================================================== */

/* migratio II (plan 1b T7): cursus - omne actum sanationis (tempus,
 * duratio, eventus, causa); -siccum ex eo aestimat, lens 'tempus'
 * inventarii impletur */
hic_manens constans character* constans MIGRATIONES_MEMORIAE[VI] = {
    "CREATE TABLE verificationes ("
    " titulus TEXT NOT NULL, clavis TEXT NOT NULL,"
    " artificium TEXT NOT NULL, tempus INTEGER NOT NULL,"
    " PRIMARY KEY (titulus, clavis, artificium))",
    "CREATE TABLE cursus ("
    " titulus TEXT NOT NULL, initium INTEGER NOT NULL,"
    " duratio_ms INTEGER NOT NULL, eventus TEXT NOT NULL,"
    " causa TEXT NOT NULL)",
    /* migratio III (plan 2 T2): vestigia lectionum - clavis ut
     * verificationes; sigilla ut massae (XXXII octeti) */
    "CREATE TABLE lectiones ("
    " titulus TEXT NOT NULL, clavis TEXT NOT NULL,"
    " artificium TEXT NOT NULL, genus TEXT NOT NULL,"
    " via TEXT NOT NULL, sigillum BLOB NOT NULL)",
    /* migratio IV: vestigium per (titulus, EXITUS) - actio exituum
     * multorum vestigia sua inter se delebat (fragmenta_silva) */
    "ALTER TABLE lectiones ADD COLUMN exitus TEXT NOT NULL DEFAULT ''",
    /* migratio V (plan 5 T1): CUR acta - causa iudicii ante actum
     * (vacua = non acta aut ignota) */
    "ALTER TABLE cursus ADD COLUMN stalum TEXT NOT NULL DEFAULT ''",
    /* migratio VI (plan 5 T1): particulae transitus ULTIMI per
     * (titulus, exitus) - quis ingressus transitum mutavit */
    "CREATE TABLE particulae ("
    " titulus TEXT NOT NULL, exitus TEXT NOT NULL,"
    " via TEXT NOT NULL, sigillum BLOB NOT NULL)"
};

interior vacuum
_hex_ligare (
    ScriniumEnuntiatum* enuntiatum,
               integer  index,
     constans Sigillum* sigillum,
               Piscina* piscina)
{
    character hex[SIGILLUM_HEX_MENSURA];

    sigillum_hex(sigillum, hex);
    (vacuum)scrinium_ligare_textum(enuntiatum, index,
        chorda_ex_literis(hex, piscina));
}

interior b32
_meminisse (
                vacuum* datum,
    constans character* titulus,
     constans Sigillum* clavis,
     constans Sigillum* artificium)
{
               Memoria* memoria;
    ScriniumEnuntiatum* enuntiatum;
                   b32  inventum;

    memoria    = (Memoria*)datum;
    enuntiatum = scrinium_praeparare(memoria->scrinium,
        "SELECT 1 FROM verificationes WHERE titulus = ?"
        " AND clavis = ? AND artificium = ?");
    si (enuntiatum == NIHIL)
    {
        redde FALSUM;
    }
    (vacuum)scrinium_ligare_textum(enuntiatum, I,
        chorda_ex_literis(titulus, memoria->piscina));
    _hex_ligare(enuntiatum, II, clavis, memoria->piscina);
    _hex_ligare(enuntiatum, III, artificium, memoria->piscina);
    inventum = (scrinium_gradi(enuntiatum) == SCRINIUM_ORDO);
    scrinium_finire(enuntiatum);
    redde inventum;
}

interior vacuum
_inscribere (
                vacuum* datum,
    constans character* titulus,
     constans Sigillum* clavis,
     constans Sigillum* artificium)
{
               Memoria* memoria;
    ScriniumEnuntiatum* enuntiatum;

    memoria    = (Memoria*)datum;
    enuntiatum = scrinium_praeparare(memoria->scrinium,
        "INSERT OR REPLACE INTO verificationes"
        " (titulus, clavis, artificium, tempus) VALUES (?, ?, ?, ?)");
    si (enuntiatum == NIHIL)
    {
        redde;
    }
    (vacuum)scrinium_ligare_textum(enuntiatum, I,
        chorda_ex_literis(titulus, memoria->piscina));
    _hex_ligare(enuntiatum, II, clavis, memoria->piscina);
    _hex_ligare(enuntiatum, III, artificium, memoria->piscina);
    (vacuum)scrinium_ligare_numerum(enuntiatum, IV,
        (s64)time(NIHIL));
    si (scrinium_gradi(enuntiatum) != SCRINIUM_FACTUM)
    {
        fprintf(stderr, "fabrica: verificatio non scripta (%s): %s\n",
            titulus, scrinium_error(memoria->scrinium));
    }
    scrinium_finire(enuntiatum);
}

/* vestigium lectionum (plan 2 T2) legere: lectiones sub clave exacta;
 * FALSUM = nullum vestigium */
interior b32
_lectiones_legere (
                vacuum*  datum,
    constans character*  titulus,
    constans character*  exitus,
     constans Sigillum*  clavis,
     constans Sigillum*  artificium,
               Piscina*  piscina,
                   Xar** lectiones_out)
{
               Memoria* memoria;
    ScriniumEnuntiatum* enuntiatum;
                   Xar* lectiones;

    memoria    = (Memoria*)datum;
    enuntiatum = scrinium_praeparare(memoria->scrinium,
        "SELECT genus, via, sigillum FROM lectiones WHERE titulus = ?"
        " AND clavis = ? AND artificium = ? AND exitus = ?");
    si (enuntiatum == NIHIL)
    {
        redde FALSUM;
    }
    (vacuum)scrinium_ligare_textum(enuntiatum, I,
        chorda_ex_literis(titulus, piscina));
    _hex_ligare(enuntiatum, II, clavis, piscina);
    _hex_ligare(enuntiatum, III, artificium, piscina);
    (vacuum)scrinium_ligare_textum(enuntiatum, IV,
        chorda_ex_literis(exitus, piscina));
    lectiones = xar_creare(piscina, (i32)magnitudo(FabricaLectio));
    dum (   lectiones                  != NIHIL
         && scrinium_gradi(enuntiatum) == SCRINIUM_ORDO)
    {
               chorda  genus;
               chorda  massa;
        FabricaLectio* lectio;

        genus = scrinium_columna_textus(enuntiatum, 0, piscina);
        massa = scrinium_columna_massa(enuntiatum, II, piscina);
        si (genus.mensura != I || massa.mensura != SIGILLUM_OCTETI)
        {
            perge;
        }
        lectio = (FabricaLectio*)xar_addere(lectiones);
        si (lectio == NIHIL)
        {
            frange;
        }
        commutatio (genus.datum[0])
        {
            casus 'A': lectio->genus = LECTIO_ABSENS;      frange;
            casus 'X': lectio->genus = LECTIO_EXSTAT;      frange;
            casus 'D': lectio->genus = LECTIO_ENUMERAVIT;  frange;
            casus 'E': lectio->genus = LECTIO_AMBITUS;     frange;
            ordinarius: lectio->genus = LECTIO_LEGIT;      frange;
        }
        lectio->via = scrinium_columna_textus(enuntiatum, I, piscina);
        memcpy(lectio->sigillum.octeti, massa.datum, SIGILLUM_OCTETI);
    }
    scrinium_finire(enuntiatum);
    si (lectiones == NIHIL || xar_numerus(lectiones) == 0)
    {
        redde FALSUM;
    }
    *lectiones_out = lectiones;
    redde VERUM;
}

/* vestigia ultima tituli (omnes exitus): viae distinctae - ORDO */
b32
suturae_lectiones_ultimae (
                vacuum*  datum,
    constans character*  titulus,
               Piscina*  piscina,
                   Xar** lectiones_out)
{
               Memoria* memoria;
    ScriniumEnuntiatum* enuntiatum;
                   Xar* lectiones;

    memoria    = (Memoria*)datum;
    enuntiatum = scrinium_praeparare(memoria->scrinium,
        "SELECT DISTINCT genus, via FROM lectiones WHERE titulus = ?");
    si (enuntiatum == NIHIL)
    {
        redde FALSUM;
    }
    (vacuum)scrinium_ligare_textum(enuntiatum, I,
        chorda_ex_literis(titulus, piscina));
    lectiones = xar_creare(piscina, (i32)magnitudo(FabricaLectio));
    dum (   lectiones                  != NIHIL
         && scrinium_gradi(enuntiatum) == SCRINIUM_ORDO)
    {
               chorda  genus;
        FabricaLectio* lectio;

        genus   = scrinium_columna_textus(enuntiatum, 0, piscina);
        lectio  = (FabricaLectio*)xar_addere(lectiones);
        si (lectio == NIHIL || genus.mensura != I)
        {
            frange;
        }
        lectio->genus = (genus.datum[0] == 'D') ? LECTIO_ENUMERAVIT
            : LECTIO_LEGIT;
        lectio->via = scrinium_columna_textus(enuntiatum, I, piscina);
    }
    scrinium_finire(enuntiatum);
    si (lectiones == NIHIL || xar_numerus(lectiones) == 0)
    {
        redde FALSUM;
    }
    *lectiones_out = lectiones;
    redde VERUM;
}

/* vestigium scribere: vetera tituli deleta (vestigium ultimum solum),
 * nova in transactione una */
interior vacuum
_lectiones_scribere (
                vacuum* datum,
    constans character* titulus,
    constans character* exitus,
     constans Sigillum* clavis,
     constans Sigillum* artificium,
          constans Xar* lectiones)
{
               Memoria* memoria;
    ScriniumEnuntiatum* enuntiatum;
                   i32  i;

    memoria = (Memoria*)datum;
    (vacuum)scrinium_incipere(memoria->scrinium);
    enuntiatum = scrinium_praeparare(memoria->scrinium,
        "DELETE FROM lectiones WHERE titulus = ? AND exitus = ?");
    si (enuntiatum != NIHIL)
    {
        (vacuum)scrinium_ligare_textum(enuntiatum, I,
            chorda_ex_literis(titulus, memoria->piscina));
        (vacuum)scrinium_ligare_textum(enuntiatum, II,
            chorda_ex_literis(exitus, memoria->piscina));
        (vacuum)scrinium_gradi(enuntiatum);
        scrinium_finire(enuntiatum);
    }
    per (i = ZEPHYRUM; i < xar_numerus(lectiones); i++)
    {
        constans FabricaLectio* lectio;
                     character  littera[II];
                        chorda  massa;
                            i8  octeti[SIGILLUM_OCTETI];

        lectio = (constans FabricaLectio*)xar_obtinere(lectiones, i);
        enuntiatum = scrinium_praeparare(memoria->scrinium,
            "INSERT INTO lectiones (titulus, clavis, artificium, genus,"
            " via, sigillum, exitus) VALUES (?, ?, ?, ?, ?, ?, ?)");
        si (enuntiatum == NIHIL)
        {
            frange;
        }
        commutatio (lectio->genus)
        {
            casus LECTIO_ABSENS:      littera[0] = 'A'; frange;
            casus LECTIO_EXSTAT:      littera[0] = 'X'; frange;
            casus LECTIO_ENUMERAVIT:  littera[0] = 'D'; frange;
            casus LECTIO_AMBITUS:     littera[0] = 'E'; frange;
            ordinarius:               littera[0] = 'L'; frange;
        }
        littera[I] = '\0';
        memcpy(octeti, lectio->sigillum.octeti, SIGILLUM_OCTETI);
        massa.datum    = octeti;
        massa.mensura  = SIGILLUM_OCTETI;
        (vacuum)scrinium_ligare_textum(enuntiatum, I,
            chorda_ex_literis(titulus, memoria->piscina));
        _hex_ligare(enuntiatum, II, clavis, memoria->piscina);
        _hex_ligare(enuntiatum, III, artificium, memoria->piscina);
        (vacuum)scrinium_ligare_textum(enuntiatum, IV,
            chorda_ex_literis(littera, memoria->piscina));
        (vacuum)scrinium_ligare_textum(enuntiatum, V, lectio->via);
        (vacuum)scrinium_ligare_massam(enuntiatum, VI, massa);
        (vacuum)scrinium_ligare_textum(enuntiatum, VII,
            chorda_ex_literis(exitus, memoria->piscina));
        si (scrinium_gradi(enuntiatum) != SCRINIUM_FACTUM)
        {
            fprintf(stderr,
                "fabrica: vestigium non scriptum (%s): %s\n",
                titulus, scrinium_error(memoria->scrinium));
        }
        scrinium_finire(enuntiatum);
    }
    (vacuum)scrinium_committere(memoria->scrinium);
}

/* PARTICULAE TRANSITUS (plan 5 T1): ultimae per (titulus, exitus) */
interior vacuum
_particulas_scribere (
                vacuum* datum,
    constans character* titulus,
    constans character* exitus,
          constans Xar* particulae)
{
               Memoria* memoria;
    ScriniumEnuntiatum* enuntiatum;
                   i32  i;

    memoria = (Memoria*)datum;
    (vacuum)scrinium_incipere(memoria->scrinium);
    enuntiatum = scrinium_praeparare(memoria->scrinium,
        "DELETE FROM particulae WHERE titulus = ? AND exitus = ?");
    si (enuntiatum != NIHIL)
    {
        (vacuum)scrinium_ligare_textum(enuntiatum, I,
            chorda_ex_literis(titulus, memoria->piscina));
        (vacuum)scrinium_ligare_textum(enuntiatum, II,
            chorda_ex_literis(exitus, memoria->piscina));
        (vacuum)scrinium_gradi(enuntiatum);
        scrinium_finire(enuntiatum);
    }
    per (i = ZEPHYRUM; i < xar_numerus(particulae); i++)
    {
         constans FabricaParticula* particula;
                            chorda  massa;
                                i8  octeti[SIGILLUM_OCTETI];

        particula = (constans FabricaParticula*)xar_obtinere(particulae,
            i);
        enuntiatum = scrinium_praeparare(memoria->scrinium,
            "INSERT INTO particulae (titulus, exitus, via, sigillum)"
            " VALUES (?, ?, ?, ?)");
        si (enuntiatum == NIHIL)
        {
            frange;
        }
        memcpy(octeti, particula->octeti.octeti, SIGILLUM_OCTETI);
        massa.datum    = octeti;
        massa.mensura  = SIGILLUM_OCTETI;
        (vacuum)scrinium_ligare_textum(enuntiatum, I,
            chorda_ex_literis(titulus, memoria->piscina));
        (vacuum)scrinium_ligare_textum(enuntiatum, II,
            chorda_ex_literis(exitus, memoria->piscina));
        (vacuum)scrinium_ligare_textum(enuntiatum, III, particula->via);
        (vacuum)scrinium_ligare_massam(enuntiatum, IV, massa);
        si (scrinium_gradi(enuntiatum) != SCRINIUM_FACTUM)
        {
            fprintf(stderr,
                "fabrica: particulae non scriptae (%s): %s\n",
                titulus, scrinium_error(memoria->scrinium));
        }
        scrinium_finire(enuntiatum);
    }
    (vacuum)scrinium_committere(memoria->scrinium);
}

interior b32
_particulas_legere (
                vacuum*  datum,
    constans character*  titulus,
    constans character*  exitus,
               Piscina*  piscina,
                   Xar** particulae_out)
{
               Memoria* memoria;
    ScriniumEnuntiatum* enuntiatum;
                   Xar* particulae;

    memoria    = (Memoria*)datum;
    enuntiatum = scrinium_praeparare(memoria->scrinium,
        "SELECT via, sigillum FROM particulae WHERE titulus = ?"
        " AND exitus = ?");
    si (enuntiatum == NIHIL)
    {
        redde FALSUM;
    }
    (vacuum)scrinium_ligare_textum(enuntiatum, I,
        chorda_ex_literis(titulus, piscina));
    (vacuum)scrinium_ligare_textum(enuntiatum, II,
        chorda_ex_literis(exitus, piscina));
    particulae = xar_creare(piscina, (i32)magnitudo(FabricaParticula));
    dum (   particulae                 != NIHIL
         && scrinium_gradi(enuntiatum) == SCRINIUM_ORDO)
    {
                  chorda  massa;
        FabricaParticula* particula;

        massa = scrinium_columna_massa(enuntiatum, I, piscina);
        si (massa.mensura != SIGILLUM_OCTETI)
        {
            perge;
        }
        particula = (FabricaParticula*)xar_addere(particulae);
        si (particula == NIHIL)
        {
            frange;
        }
        particula->via = scrinium_columna_textus(enuntiatum, ZEPHYRUM,
            piscina);
        memcpy(particula->octeti.octeti, massa.datum, SIGILLUM_OCTETI);
    }
    scrinium_finire(enuntiatum);
    si (particulae == NIHIL || xar_numerus(particulae) == 0)
    {
        redde FALSUM;
    }
    *particulae_out = particulae;
    redde VERUM;
}

/* VERDICTUM PONERE (plan 5 T3): NIHIL = deletum (absens licet);
 * aliter temporarium + rename (atomice, ut iudicium_currere silvae) */
interior b32
_verdictum_ponere (
                vacuum* datum,
    constans character* via,
       constans chorda* contentum)
{
    character temporarium[IV * MXXIV];

    (vacuum)datum;
    si (contentum == NIHIL)
    {
        si (filum_existit(via))
        {
            redde filum_delere(via);
        }
        redde VERUM;
    }
    si (strlen(via) + V >= magnitudo(temporarium))
    {
        redde FALSUM;
    }
    sprintf(temporarium, "%s.tmp", via);
    (vacuum)filum_directorium_creare_cum_parentibus(
        "build/fabrica/verdicta");
    redde filum_scribere(temporarium, *contentum)
        && filum_movere(temporarium, via);
}

/* radix arboris absoluta (praefixum viarum libri demendum) */
chorda
suturae_radix_absoluta (
    Piscina* piscina)
{
    character sedes[4096];

    si (getcwd(sedes, magnitudo(sedes)) == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    redde chorda_ex_literis(sedes, piscina);
}

/* build/fabrica.db aperire; FALSUM (cautio impressa) = iudicium sine
 * memoria - numquam fractum propter memoriam */
b32
suturae_memoriam_aperire (
    Memoria* memoria,
    Piscina* piscina)
{
    memoria->piscina = piscina;
    si (!filum_directorium_creare_cum_parentibus("build"))
    {
        redde FALSUM;
    }
    memoria->scrinium = scrinium_aperire(piscina, "build/fabrica.db");
    si (   memoria->scrinium == NIHIL
        || !scrinium_migrare(memoria->scrinium, MIGRATIONES_MEMORIAE,
        (integer)(magnitudo(MIGRATIONES_MEMORIAE)
            / magnitudo(MIGRATIONES_MEMORIAE[ZEPHYRUM]))))
    {
        fprintf(stderr, "fabrica: CAUTIO memoria build/fabrica.db "
            "aperiri nequit - iudicium sine memoria\n");
        redde FALSUM;
    }
    redde VERUM;
}

constans character*
suturae_eventus_titulus (
    FabricaEventus eventus)
{
    commutatio (eventus)
    {
        casus FABRICA_SANATUM:
            redde "SANATUM";
        casus FABRICA_PRAEPARATUM:
            redde "PRAEPARATUM";
        casus FABRICA_FRACTUM:
            redde "FRACTUM";
        casus FABRICA_OMISSUM:
            redde "OMISSUM";
        casus FABRICA_AGENDUM:
            redde "AGENDUM";
        casus FABRICA_IUDICIUM:
            redde "IUDICIUM";
        casus FABRICA_AUDITUM_DISCORS:
            redde "AUDITUM_DISCORS";
        ordinarius:
            redde "FORTASSE";
    }
}

/* textus vacuus VERUS: scrinium_ligare_textum chordam vacuam cuius
 * datum NIHIL est ut SQL NULL ligat (sqlite3_bind_text cum NULL) -
 * causa sanati vacua 'NOT NULL' frangebat (1b T7) */
hic_manens character TEXTUS_VACUUS[I] = { '\0' };

/* cursus (1b T7): actum scribere; tempus = initium ~ finis - duratio */
interior vacuum
_cursum_inscribere (
                     vacuum* datum,
    constans FabricaSanatio* sanatio)
{
               Memoria* memoria;
    ScriniumEnuntiatum* enuntiatum;

    memoria    = (Memoria*)datum;
    enuntiatum = scrinium_praeparare(memoria->scrinium,
        "INSERT INTO cursus (titulus, initium, duratio_ms, eventus,"
        " causa, stalum) VALUES (?, ?, ?, ?, ?, ?)");
    si (enuntiatum == NIHIL)
    {
        redde;
    }
    (vacuum)scrinium_ligare_textum(enuntiatum, I,
        sanatio->actio->titulus);
    (vacuum)scrinium_ligare_numerum(enuntiatum, II,
        (s64)time(NIHIL) - (s64)(sanatio->duratio_ms / M));
    (vacuum)scrinium_ligare_numerum(enuntiatum, III,
        (s64)sanatio->duratio_ms);
    (vacuum)scrinium_ligare_textum(enuntiatum, IV, chorda_ex_literis(
        suturae_eventus_titulus(sanatio->eventus), memoria->piscina));
    si (sanatio->causa.mensura > 0)
    {
        (vacuum)scrinium_ligare_textum(enuntiatum, V, sanatio->causa);
    }
    alioquin
    {
        chorda vacua;

        vacua.datum    = (i8*)TEXTUS_VACUUS;
        vacua.mensura  = ZEPHYRUM;
        (vacuum)scrinium_ligare_textum(enuntiatum, V, vacua);
    }
    /* chorda vacua (datum NIHIL) ut NULL ligaretur (quaestio ...FET2):
     * 'NOT NULL' frangeret - textus vacuus expressus */
    si (sanatio->stalum.mensura > 0)
    {
        (vacuum)scrinium_ligare_textum(enuntiatum, VI, sanatio->stalum);
    }
    alioquin
    {
        chorda vacua;

        vacua.datum    = (i8*)TEXTUS_VACUUS;
        vacua.mensura  = ZEPHYRUM;
        (vacuum)scrinium_ligare_textum(enuntiatum, VI, vacua);
    }
    si (scrinium_gradi(enuntiatum) != SCRINIUM_FACTUM)
    {
        fprintf(stderr, "fabrica: cursus non scriptus: %s\n",
            scrinium_error(memoria->scrinium));
    }
    scrinium_finire(enuntiatum);
}

/* duratio cursus ULTIMI sanati (aut praeparati) tituli */
interior b32
_cursum_legere (
                vacuum* datum,
    constans character* titulus,
                   i32* duratio_ms_out)
{
               Memoria* memoria;
    ScriniumEnuntiatum* enuntiatum;
                   b32  inventum;

    memoria    = (Memoria*)datum;
    enuntiatum = scrinium_praeparare(memoria->scrinium,
        "SELECT duratio_ms FROM cursus WHERE titulus = ?"
        " AND eventus IN ('SANATUM', 'PRAEPARATUM')"
        " ORDER BY initium DESC, rowid DESC LIMIT 1");
    si (enuntiatum == NIHIL)
    {
        redde FALSUM;
    }
    (vacuum)scrinium_ligare_textum(enuntiatum, I,
        chorda_ex_literis(titulus, memoria->piscina));
    inventum = (scrinium_gradi(enuntiatum) == SCRINIUM_ORDO);
    si (inventum)
    {
        *duratio_ms_out = (i32)scrinium_columna_numerus(enuntiatum,
            ZEPHYRUM);
    }
    scrinium_finire(enuntiatum);
    redde inventum;
}

/* MEMORIA VERA in sutura (fabrica-6 H4): datum = memoria, ansae
 * memoriae omnes (cursum_legere solum sanare legit) */
vacuum
suturae_memoriam_nectere (
    FabricaSutura* sutura,
          Memoria* memoria)
{
    sutura->datum                = memoria;
    sutura->meminisse            = _meminisse;
    sutura->inscribere           = _inscribere;
    sutura->cursum_inscribere    = _cursum_inscribere;
    sutura->cursum_legere        = _cursum_legere;
    sutura->lectiones_legere     = _lectiones_legere;
    sutura->lectiones_scribere   = _lectiones_scribere;
    sutura->lectiones_ultimae    = suturae_lectiones_ultimae;
    sutura->particulas_scribere  = _particulas_scribere;
    sutura->particulas_legere    = _particulas_legere;
    sutura->verdictum_ponere     = _verdictum_ponere;
}
