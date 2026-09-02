/* officina_conexio.c - Conexio: nexor moduli medullae (M2a)
 *
 * Internamentum globale in addere (conflictus ibi capti); collocatio
 * + sarcimen relocationum in nectere. Tituli chordae NON
 * zephyrum-terminatae in arenis modulorum PROPRIIS vivunt -
 * comparatio semper per mensuram, copiae in piscinam conexionis.
 *
 * Sarcimen relocationis per memcpy (locellus fortasse non ordinatus
 * intra datum ordinationis I - nulla assumptio ordinationis).
 */

#include "officina_conexio.h"
#include "tabula_dispersa.h"

#include <string.h>

nomen structura {
    constans character* titulus;
                   s64  valor;
} CellaNota;

/* symbola data externa nota (M2a; M2c/M2d addent) - ansae opacae
 * (interview Q1): stdin 0, stdout 1, stderr 2; errno cella 0 */
interior constans CellaNota CELLAE_NOTAE[] = {
    { "stdin",  0 },
    { "stdout", 1 },
    { "stderr", 2 },
    { "errno",  0 }
};
#define CELLAE_NOTAE_NUMERUS 4

structura Conexio {
           Piscina* piscina;
             Regio* regio;
               Xar* moduli;            /* MedullaModulus* */
               Xar* symbola;           /* ConexioSymbolum valore */
    TabulaDispersa* index_titulorum;   /* titulus -> index+1 */
               Xar* translationes;     /* s32* (una per modulum) */
               Xar* functiones_nexae;  /* ConexioFunctioNexa valore */
               Xar* decipulae;         /* chorda valore (tituli) */
               b32  nexum;
            chorda  querela;
            chorda  querela_symbolum;
               i32  numerus_fracturarum;
               i32  numerus_datorum;
               i32  numerus_cellarum;
};


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
_titulum_copiare (
    Conexio* conexio,
     chorda  titulus)
{
    chorda  copia;
        i8* datum;

    datum = piscina_allocare(conexio->piscina,
        (memoriae_index)titulus.mensura);
    memcpy(datum, titulus.datum, (memoriae_index)titulus.mensura);
    copia.datum    = datum;
    copia.mensura  = titulus.mensura;
    redde copia;
}

interior vacuum
_querelam_ponere (
               Conexio* conexio,
    constans character* nuntius)
{
    conexio->querela = chorda_ex_literis(nuntius, conexio->piscina);
}

/* invenire aut creare; -I in fractura allocationis */
interior s32
_globale_internare (
    Conexio* conexio,
     chorda  titulus)
{
             vacuum* valor;
    ConexioSymbolum* symbolum;
                s32  index;

    si (tabula_dispersa_invenire(conexio->index_titulorum, titulus,
        &valor))
    {
        redde (s32)(memoriae_index)valor - I;
    }

    index     = (s32)xar_numerus(conexio->symbola);
    symbolum  = xar_addere(conexio->symbola);
    si (symbolum == NIHIL)
    {
        redde -I;
    }
    symbolum->titulus        = _titulum_copiare(conexio, titulus);
    symbolum->genus          = CONEXIO_SYMBOLUM_IGNOTUM;
    symbolum->modulus_index  = -I;
    symbolum->index_localis  = -I;
    symbolum->sedes          = NIHIL;

    si (!tabula_dispersa_inserere(conexio->index_titulorum,
        symbolum->titulus,
        (vacuum*)(memoriae_index)(index + I)))
    {
        redde -I;
    }
    redde index;
}


/* ==================================================
 * Vita + nexus
 * ================================================== */

Conexio*
conexio_creare (
    Piscina* piscina,
      Regio* regio)
{
    Conexio* conexio;

    si (piscina == NIHIL || regio == NIHIL)
    {
        redde NIHIL;
    }
    conexio = piscina_allocare(piscina, magnitudo(Conexio));
    memset(conexio, ZEPHYRUM, magnitudo(Conexio));

    conexio->piscina  = piscina;
    conexio->regio    = regio;
    conexio->moduli = xar_creare(piscina,
        (i32)magnitudo(MedullaModulus*));
    conexio->symbola = xar_creare(piscina,
        (i32)magnitudo(ConexioSymbolum));
    conexio->index_titulorum = tabula_dispersa_creare_chorda(piscina,
        256);
    conexio->translationes = xar_creare(piscina,
        (i32)magnitudo(s32*));
    conexio->functiones_nexae = xar_creare(piscina,
        (i32)magnitudo(ConexioFunctioNexa));
    conexio->decipulae = xar_creare(piscina, (i32)magnitudo(chorda));

    si (   conexio->moduli == NIHIL || conexio->symbola == NIHIL
        || conexio->index_titulorum == NIHIL
        || conexio->translationes == NIHIL
        || conexio->functiones_nexae == NIHIL
        || conexio->decipulae == NIHIL)
    {
        redde NIHIL;
    }
    redde conexio;
}

b32
conexio_modulum_addere (
           Conexio* conexio,
    MedullaModulus* modulus)
{
               s32   modulus_index;
               i32   numerus;
               s32*  translatio;
               i32   i;
    MedullaModulus** locellus;
               s32** locellus_translationis;

    si (conexio == NIHIL || modulus == NIHIL || conexio->nexum)
    {
        redde FALSUM;
    }

    modulus_index  = (s32)xar_numerus(conexio->moduli);
    locellus       = xar_addere(conexio->moduli);
    si (locellus == NIHIL)
    {
        redde FALSUM;
    }
    *locellus = modulus;

    numerus     = xar_numerus(modulus->symbola);
    translatio  = NIHIL;
    si (numerus > ZEPHYRUM)
    {
        translatio = piscina_allocare(conexio->piscina,
            (memoriae_index)numerus * magnitudo(s32));
    }

    /* fractura statum NON corrumpit: omnes duplicationes colliguntur
     * (census instrumenti), translatio semper impletur et ponitur -
     * nectere postea recusat si fracturae > 0 */
    {
        b32 fractum = FALSUM;

        per (i = ZEPHYRUM; i < numerus; i += I)
        {
            constans MedullaSymbolum* localis =
                medulla_symbolum_obtinere(modulus, (s32)i);
            s32 globale = _globale_internare(conexio,
                localis->titulus);
            ConexioSymbolum* symbolum;

            si (globale < ZEPHYRUM)
            {
                _querelam_ponere(conexio,
                    "allocatio symboli fracta");
                conexio->numerus_fracturarum += I;
                redde FALSUM;
            }
            translatio[i] = globale;

            si (localis->genus == MEDULLA_SYMBOLUM_EXTERNUM)
            {
                perge;
            }

            symbolum = xar_obtinere(conexio->symbola, (i32)globale);
            si (symbolum->genus != CONEXIO_SYMBOLUM_IGNOTUM)
            {
                _querelam_ponere(conexio,
                    "definitio duplex symboli");
                conexio->querela_symbolum     = symbolum->titulus;
                conexio->numerus_fracturarum  += I;
                fractum                       = VERUM;
                perge;
            }
            symbolum->genus =
                (localis->genus == MEDULLA_SYMBOLUM_FUNCTIO)
                ? CONEXIO_SYMBOLUM_FUNCTIO
                : CONEXIO_SYMBOLUM_DATUM;
            symbolum->modulus_index = modulus_index;
            symbolum->index_localis = localis->index;
        }

        locellus_translationis = xar_addere(conexio->translationes);
        si (locellus_translationis == NIHIL)
        {
            redde FALSUM;
        }
        *locellus_translationis = translatio;
        redde !fractum;
    }
}

interior b32
_cellam_notam_quaerere (
    chorda  titulus,
       s64* valor_out)
{
    i32 i;

    per (i = ZEPHYRUM; i < CELLAE_NOTAE_NUMERUS; i += I)
    {
        si (chorda_aequalis_literis(titulus, CELLAE_NOTAE[i].titulus))
        {
            *valor_out = CELLAE_NOTAE[i].valor;
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
conexio_nectere (
    Conexio* conexio)
{
    i32 numerus;
    i32 i;

    si (   conexio == NIHIL || conexio->nexum
        || conexio->numerus_fracturarum > ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* gradus I: collocatio - omne symbolum sedem accipit */
    numerus = xar_numerus(conexio->symbola);
    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        ConexioSymbolum* symbolum = xar_obtinere(conexio->symbola, i);
         MedullaModulus* modulus;

        si (symbolum->genus == CONEXIO_SYMBOLUM_FUNCTIO)
        {
             ConexioDescriptor* descriptor;
            ConexioFunctioNexa* nexa;
                           s64  index_nexae;

            modulus = *(MedullaModulus**)xar_obtinere(conexio->moduli,
                (i32)symbolum->modulus_index);
            index_nexae  = (s64)xar_numerus(conexio->functiones_nexae);
            nexa         = xar_addere(conexio->functiones_nexae);
            nexa->functio = *(MedullaFunctio**)xar_obtinere(
                modulus->functiones, (i32)symbolum->index_localis);
            nexa->modulus_index = symbolum->modulus_index;

            descriptor = regio_globalia_allocare(conexio->regio,
                magnitudo(ConexioDescriptor), XVI);
            si (descriptor == NIHIL)
            {
                _querelam_ponere(conexio, "globalia exhausta");
                redde FALSUM;
            }
            descriptor->signum  = CONEXIO_SIGNUM_INTERPRETATUM;
            descriptor->index   = index_nexae;
            symbolum->sedes     = descriptor;
        }
        alioquin si (symbolum->genus == CONEXIO_SYMBOLUM_DATUM)
        {
            constans MedullaDatum* datum;

            modulus = *(MedullaModulus**)xar_obtinere(conexio->moduli,
                (i32)symbolum->modulus_index);
            datum = *(MedullaDatum**)xar_obtinere(modulus->data,
                (i32)symbolum->index_localis);
            symbolum->sedes = regio_globalia_allocare(conexio->regio,
                (memoriae_index)datum->magnitudo_octetorum,
                (memoriae_index)datum->ordinatio);
            si (symbolum->sedes == NIHIL)
            {
                _querelam_ponere(conexio, "globalia exhausta");
                redde FALSUM;
            }
            memcpy(symbolum->sedes, datum->imago,
                (memoriae_index)datum->magnitudo_octetorum);
            conexio->numerus_datorum += I;
        }
        alioquin
        {
            s64 valor_cellae;

            si (_cellam_notam_quaerere(symbolum->titulus,
                &valor_cellae))
            {
                s64* cella = regio_globalia_allocare(conexio->regio,
                    magnitudo(s64), VIII);
                si (cella == NIHIL)
                {
                    _querelam_ponere(conexio, "globalia exhausta");
                    redde FALSUM;
                }
                *cella                     = valor_cellae;
                symbolum->genus            = CONEXIO_SYMBOLUM_CELLA;
                symbolum->sedes            = cella;
                conexio->numerus_cellarum  += I;
            }
            alioquin
            {
                ConexioDescriptor* descriptor;
                           chorda* titulus_decipulae;
                              s64  index_decipulae;

                index_decipulae = (s64)xar_numerus(conexio->decipulae);
                titulus_decipulae = xar_addere(conexio->decipulae);
                *titulus_decipulae = symbolum->titulus;

                descriptor = regio_globalia_allocare(conexio->regio,
                    magnitudo(ConexioDescriptor), XVI);
                si (descriptor == NIHIL)
                {
                    _querelam_ponere(conexio, "globalia exhausta");
                    redde FALSUM;
                }
                descriptor->signum  = CONEXIO_SIGNUM_DECIPULA;
                descriptor->index   = index_decipulae;
                symbolum->genus     = CONEXIO_SYMBOLUM_DECIPULA;
                symbolum->sedes     = descriptor;
            }
        }
    }

    /* gradus II: relocationes ADDITIVE sarcire (locellus = sedes
     * symboli + addendum prius; memcpy - nulla assumptio
     * ordinationis) */
    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        constans ConexioSymbolum* symbolum =
            xar_obtinere(conexio->symbola, i);
               MedullaModulus* modulus;
        constans MedullaDatum* datum;
                          s32* translatio;
                          i32  r;

        si (symbolum->genus != CONEXIO_SYMBOLUM_DATUM)
        {
            perge;
        }
        modulus = *(MedullaModulus**)xar_obtinere(conexio->moduli,
            (i32)symbolum->modulus_index);
        datum = *(MedullaDatum**)xar_obtinere(modulus->data,
            (i32)symbolum->index_localis);
        translatio = *(s32**)xar_obtinere(conexio->translationes,
            (i32)symbolum->modulus_index);

        per (r = ZEPHYRUM; r < xar_numerus(datum->relocationes);
            r += I)
        {
            constans MedullaRelocatio* relocatio =
                xar_obtinere(datum->relocationes, r);
            s32 globale = translatio[relocatio->symbolum];
            constans ConexioSymbolum* petitum =
                xar_obtinere(conexio->symbola, (i32)globale);
             i8* locellus = (i8*)symbolum->sedes + relocatio->offset;
            s64  addendum;
            s64  valor;

            memcpy(&addendum, locellus, magnitudo(s64));
            valor = (s64)(memoriae_index)petitum->sedes + addendum;
            memcpy(locellus, &valor, magnitudo(s64));
        }
    }

    conexio->nexum = VERUM;
    redde VERUM;
}

constans chorda*
conexio_querela (
    constans Conexio* conexio)
{
    redde &conexio->querela;
}

constans chorda*
conexio_querela_symbolum (
    constans Conexio* conexio)
{
    redde &conexio->querela_symbolum;
}


/* ==================================================
 * Quaestio
 * ================================================== */

s32
conexio_symbolum_quaerere (
    constans Conexio* conexio,
              chorda  titulus)
{
    vacuum* valor;

    si (tabula_dispersa_invenire(conexio->index_titulorum, titulus,
        &valor))
    {
        redde (s32)(memoriae_index)valor - I;
    }
    redde -I;
}

constans ConexioSymbolum*
conexio_symbolum_obtinere (
    constans Conexio* conexio,
                 s32  index)
{
    redde xar_obtinere_s(conexio->symbola, index);
}

i32
conexio_numerus_symbolorum (
    constans Conexio* conexio)
{
    redde xar_numerus(conexio->symbola);
}

vacuum*
conexio_sedes_quaerere (
    constans Conexio* conexio,
              chorda  titulus)
{
    s32 index = conexio_symbolum_quaerere(conexio, titulus);
    constans ConexioSymbolum* symbolum;

    si (index < ZEPHYRUM)
    {
        redde NIHIL;
    }
    symbolum = conexio_symbolum_obtinere(conexio, index);
    redde symbolum->sedes;
}

s32
conexio_symbolum_globale (
    constans Conexio* conexio,
                 s32  modulus_index,
                 s32  index_localis)
{
    constans MedullaModulus* modulus;
                        s32* translatio;

    si (   modulus_index < ZEPHYRUM
        || modulus_index >= (s32)xar_numerus(conexio->moduli))
    {
        redde -I;
    }
    modulus = *(MedullaModulus**)xar_obtinere(conexio->moduli,
        (i32)modulus_index);
    si (   index_localis < ZEPHYRUM
        || index_localis >= (s32)xar_numerus(modulus->symbola))
    {
        redde -I;
    }
    translatio = *(s32**)xar_obtinere(conexio->translationes,
        (i32)modulus_index);
    redde translatio[index_localis];
}

constans ConexioFunctioNexa*
conexio_functionem_obtinere (
    constans Conexio* conexio,
                 s64  index)
{
    redde xar_obtinere_s(conexio->functiones_nexae, (s32)index);
}

i32
conexio_numerus_functionum (
    constans Conexio* conexio)
{
    redde xar_numerus(conexio->functiones_nexae);
}

i32
conexio_numerus_modulorum (
    constans Conexio* conexio)
{
    redde xar_numerus(conexio->moduli);
}

constans MedullaModulus*
conexio_modulum_obtinere (
    constans Conexio* conexio,
                 s32  index)
{
    MedullaModulus** locellus = xar_obtinere_s(conexio->moduli,
        index);

    redde (locellus == NIHIL) ? NIHIL : *locellus;
}


/* ==================================================
 * Census
 * ================================================== */

i32
conexio_numerus_datorum (
    constans Conexio* conexio)
{
    redde conexio->numerus_datorum;
}

i32
conexio_numerus_cellarum (
    constans Conexio* conexio)
{
    redde conexio->numerus_cellarum;
}

i32
conexio_numerus_decipularum (
    constans Conexio* conexio)
{
    redde xar_numerus(conexio->decipulae);
}

constans chorda*
conexio_decipulam_obtinere (
    constans Conexio* conexio,
                 s32  index)
{
    redde xar_obtinere_s(conexio->decipulae, index);
}
