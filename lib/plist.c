/* plist.c - tabulae proprietatum (contractus in capite).
 *
 * Scriptor textum directe emittit (forma scriptoris XML veteris
 * octetim servata); prologus et declaratio generis hic praefiguntur.
 * Lector visionem coctam xml_documentum.h ambulat.
 */

#include "plist.h"
#include "xml_documentum.h"
#include "base64.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include <string.h>

hic_manens constans character PLIST_PROLOGUS[] =
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" "
    "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";

interior PlistValor*
_valorem_creare (
    PlistGenus  genus,
       Piscina* piscina)
{
    PlistValor* v;

    si (!piscina)
    {
        redde NIHIL;
    }
    v = (PlistValor*)piscina_allocare(piscina, magnitudo(PlistValor));
    si (!v)
    {
        redde NIHIL;
    }
    memset(v, ZEPHYRUM, magnitudo(PlistValor));
    v->genus = genus;
    redde v;
}

PlistValor*
plist_dictio_creare (
    Piscina* piscina)
{
    PlistValor* v = _valorem_creare(PLIST_DICTIO, piscina);

    si (v)
    {
        v->liberi = xar_creare(piscina, (i32)magnitudo(PlistPar));
    }
    redde v;
}

PlistValor*
plist_lista_creare (
    Piscina* piscina)
{
    PlistValor* v = _valorem_creare(PLIST_LISTA, piscina);

    si (v)
    {
        v->liberi = xar_creare(piscina, (i32)magnitudo(PlistValor*));
    }
    redde v;
}

PlistValor*
plist_chordam_creare (
      chorda  textus,
     Piscina* piscina)
{
    PlistValor* v = _valorem_creare(PLIST_CHORDA, piscina);

    si (v)
    {
        v->textus = textus;
    }
    redde v;
}

PlistValor*
plist_veritatem_creare (
         b32  valor,
     Piscina* piscina)
{
    PlistValor* v = _valorem_creare(PLIST_VERITAS, piscina);

    si (v)
    {
        v->veritas = valor;
    }
    redde v;
}

PlistValor*
plist_integrum_creare (
         s64  numerus,
     Piscina* piscina)
{
    PlistValor* v = _valorem_creare(PLIST_INTEGER, piscina);

    si (v)
    {
        v->integrum = numerus;
    }
    redde v;
}

PlistValor*
plist_realem_creare (
         f64  numerus,
     Piscina* piscina)
{
    PlistValor* v = _valorem_creare(PLIST_REALIS, piscina);

    si (v)
    {
        v->realis = numerus;
    }
    redde v;
}

PlistValor*
plist_diem_creare (
    DiesHora  dh,
     Piscina* piscina)
{
    PlistValor* v = _valorem_creare(PLIST_DIES, piscina);

    si (v)
    {
        v->dies = dh;
    }
    redde v;
}

PlistValor*
plist_data_creare (
      chorda  octeti,
     Piscina* piscina)
{
    PlistValor* v = _valorem_creare(PLIST_DATA, piscina);

    si (v)
    {
        v->textus = octeti;
    }
    redde v;
}

b32
plist_dictio_ponere (
     PlistValor* dictio,
         chorda  clavis,
     PlistValor* valor,
        Piscina* piscina)
{
    PlistPar* par;
         i32  i;

    (vacuum)piscina;
    si (   !dictio || dictio->genus != PLIST_DICTIO || !dictio->liberi
        || !valor)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(dictio->liberi); i++)
    {
        par = (PlistPar*)xar_obtinere(dictio->liberi, i);
        si (par && chorda_aequalis(par->clavis, clavis))
        {
            par->valor = valor;   /* IN LOCO: ordo servatur */
            redde VERUM;
        }
    }
    par = (PlistPar*)xar_addere(dictio->liberi);
    si (!par)
    {
        redde FALSUM;
    }
    par->clavis  = clavis;
    par->valor   = valor;
    redde VERUM;
}

b32
plist_lista_addere (
    PlistValor* lista,
    PlistValor* valor,
       Piscina* piscina)
{
    PlistValor** sedes;

    (vacuum)piscina;
    si (   !lista || lista->genus != PLIST_LISTA || !lista->liberi
        || !valor)
    {
        redde FALSUM;
    }
    sedes = (PlistValor**)xar_addere(lista->liberi);
    si (!sedes)
    {
        redde FALSUM;
    }
    *sedes = valor;
    redde VERUM;
}

PlistValor*
plist_dictio_capere (
            PlistValor* dictio,
    constans character* clavis)
{
    PlistPar* par;
         i32  i;

    si (   !dictio || dictio->genus != PLIST_DICTIO || !dictio->liberi
        || !clavis)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(dictio->liberi); i++)
    {
        par = (PlistPar*)xar_obtinere(dictio->liberi, i);
        si (par && chorda_aequalis_literis(par->clavis, clavis))
        {
            redde par->valor;
        }
    }
    redde NIHIL;
}

i32
plist_numerus (
    PlistValor* valor)
{
    si (   !valor || !valor->liberi
        || (   valor->genus != PLIST_DICTIO
            && valor->genus != PLIST_LISTA))
    {
        redde ZEPHYRUM;
    }
    redde xar_numerus(valor->liberi);
}

PlistValor*
plist_ad_indicem (
     PlistValor* lista,
            i32  index)
{
    PlistValor** sedes;

    si (!lista || lista->genus != PLIST_LISTA || !lista->liberi)
    {
        redde NIHIL;
    }
    sedes = (PlistValor**)xar_obtinere(lista->liberi, index);
    redde sedes ? *sedes : NIHIL;
}

b32
plist_aequalis (
    PlistValor* a,
    PlistValor* b)
{
    i32 i;

    si (!a || !b || a->genus != b->genus)
    {
        redde FALSUM;
    }
    commutatio (a->genus)
    {
        casus PLIST_CHORDA:
        casus PLIST_DATA:
            redde chorda_aequalis(a->textus, b->textus);

        casus PLIST_VERITAS:
            redde ((a->veritas ? VERUM : FALSUM)
                == (b->veritas ? VERUM : FALSUM)) ? VERUM : FALSUM;

        casus PLIST_INTEGER:
            redde (a->integrum == b->integrum) ? VERUM : FALSUM;

        casus PLIST_REALIS:
            redde (a->realis == b->realis) ? VERUM : FALSUM;

        casus PLIST_DIES:
            redde (   fasti_aequalis(a->dies.dies, b->dies.dies)
                   && a->dies.hora.hora == b->dies.hora.hora
                   && a->dies.hora.minutum == b->dies.hora.minutum
                   && a->dies.hora.secundum == b->dies.hora.secundum)
                ? VERUM : FALSUM;

        casus PLIST_LISTA:
            si (plist_numerus(a) != plist_numerus(b))
            {
                redde FALSUM;
            }
            per (i = ZEPHYRUM; i < plist_numerus(a); i++)
            {
                si (!plist_aequalis(plist_ad_indicem(a, i),
                                    plist_ad_indicem(b, i)))
                {
                    redde FALSUM;
                }
            }
            redde VERUM;

        casus PLIST_DICTIO:
            si (plist_numerus(a) != plist_numerus(b))
            {
                redde FALSUM;
            }
            per (i = ZEPHYRUM; i < plist_numerus(a); i++)
            {
                PlistPar* pa;
                PlistPar* pb;

                pa = (PlistPar*)xar_obtinere(a->liberi, i);
                pb = (PlistPar*)xar_obtinere(b->liberi, i);
                si (   !pa || !pb
                    || !chorda_aequalis(pa->clavis, pb->clavis)
                    || !plist_aequalis(pa->valor, pb->valor))
                {
                    redde FALSUM;
                }
            }
            redde VERUM;

        ordinarius:
            redde FALSUM;
    }
}

/* Signum C0 vetitum? XML 1.0 solum tabulationem, lineam novam,
 * reditum admittit infra 0x20; cetera stratum escapandi nostrum
 * exprimere NON potest (par. VI.7 specificationis). */
interior b32
_signum_vetitum (
    chorda s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        i8 c = s.datum[i];

        si (   c < (i8)0x20 && c != (i8)'\t' && c != (i8)'\n'
            && c != (i8)'\r')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Scriptor directus: forma octetim eadem ac scriptor 'pulcher'
 * include/xml.h veteris - gradus duo spatia, folium in linea una,
 * folium vacuum '<t/>', textus & < > solum effugit. */
interior vacuum
_effugere (
    ChordaAedificator* aed,
               chorda  s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        character c = (character)s.datum[i];

        si (c == '<')
        {
            chorda_aedificator_appendere_literis(aed, "&lt;");
        }
        alioquin si (c == '>')
        {
            chorda_aedificator_appendere_literis(aed, "&gt;");
        }
        alioquin si (c == '&')
        {
            chorda_aedificator_appendere_literis(aed, "&amp;");
        }
        alioquin
        {
            chorda_aedificator_appendere_character(aed, c);
        }
    }
}

interior vacuum
_indentare (
    ChordaAedificator* aed,
                  i32  gradus)
{
    i32 i;

    per (i = ZEPHYRUM; i < gradus; i++)
    {
        chorda_aedificator_appendere_literis(aed, "  ");
    }
}

/* '<titulus>textus</titulus>' in linea una; textus vacuus ->
 * '<titulus/>' */
interior vacuum
_folium_scribere (
     ChordaAedificator* aed,
                   i32  gradus,
    constans character* titulus,
                chorda  textus)
{
    _indentare(aed, gradus);
    chorda_aedificator_appendere_character(aed, '<');
    chorda_aedificator_appendere_literis(aed, titulus);
    si (textus.mensura == ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(aed, "/>\n");
        redde;
    }
    chorda_aedificator_appendere_character(aed, '>');
    _effugere(aed, textus);
    chorda_aedificator_appendere_literis(aed, "</");
    chorda_aedificator_appendere_literis(aed, titulus);
    chorda_aedificator_appendere_literis(aed, ">\n");
}

interior b32
_valorem_scribere (
           PlistValor* v,
    ChordaAedificator* aed,
                  i32  gradus,
              Piscina* piscina,
          PlistStatus* status)
{
                 chorda  vacua;
                 chorda  textus;
     constans character* titulus;
                    i32  i;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    /* gradus I = radix sub <plist>; ultra limitem (aut cyclus) */
    si (!v || gradus > PLIST_PROFUNDITAS_MAXIMA)
    {
        *status = PLIST_ERROR_STRUCTURA;
        redde FALSUM;
    }
    commutatio (v->genus)
    {
        casus PLIST_CHORDA:
            si (_signum_vetitum(v->textus))
            {
                *status = PLIST_ERROR_SIGNUM;
                redde FALSUM;
            }
            _folium_scribere(aed, gradus, "string", v->textus);
            redde VERUM;

        casus PLIST_VERITAS:
            _folium_scribere(aed, gradus, v->veritas ? "true" : "false",
                             vacua);
            redde VERUM;

        casus PLIST_INTEGER:
            _folium_scribere(aed, gradus, "integer",
                             chorda_ex_s64(v->integrum, piscina));
            redde VERUM;

        casus PLIST_REALIS:
            _folium_scribere(aed, gradus, "real",
                             chorda_ex_f64_exacta(v->realis, piscina));
            redde VERUM;

        casus PLIST_DIES:
            textus = fasti_ad_iso(v->dies, piscina);
            si (chorda_vacua(textus))
            {
                *status = PLIST_ERROR_DIES;
                redde FALSUM;
            }
            _folium_scribere(aed, gradus, "date", textus);
            redde VERUM;

        casus PLIST_DATA:
            _folium_scribere(aed, gradus, "data",
                             base64_codificare(v->textus.datum,
                                 v->textus.mensura, piscina));
            redde VERUM;

        casus PLIST_LISTA:
        casus PLIST_DICTIO:
            titulus = v->genus == PLIST_LISTA ? "array" : "dict";
            si (plist_numerus(v) == ZEPHYRUM)
            {
                _folium_scribere(aed, gradus, titulus, vacua);
                redde VERUM;
            }
            _indentare(aed, gradus);
            chorda_aedificator_appendere_character(aed, '<');
            chorda_aedificator_appendere_literis(aed, titulus);
            chorda_aedificator_appendere_literis(aed, ">\n");
            per (i = ZEPHYRUM; i < plist_numerus(v); i++)
            {
                PlistValor* filius;

                si (v->genus == PLIST_LISTA)
                {
                    filius = plist_ad_indicem(v, i);
                }
                alioquin
                {
                    PlistPar* par = (PlistPar*)xar_obtinere(v->liberi,
                        i);

                    si (!par)
                    {
                        *status = PLIST_ERROR_STRUCTURA;
                        redde FALSUM;
                    }
                    si (_signum_vetitum(par->clavis))
                    {
                        *status = PLIST_ERROR_SIGNUM;
                        redde FALSUM;
                    }
                    _folium_scribere(aed, gradus + I, "key",
                        par->clavis);
                    filius = par->valor;
                }
                si (!_valorem_scribere(filius, aed, gradus + I, piscina,
                                       status))
                {
                    redde FALSUM;
                }
            }
            _indentare(aed, gradus);
            chorda_aedificator_appendere_literis(aed, "</");
            chorda_aedificator_appendere_literis(aed, titulus);
            chorda_aedificator_appendere_literis(aed, ">\n");
            redde VERUM;

        ordinarius:
            *status = PLIST_ERROR_STRUCTURA;
            redde FALSUM;
    }
}

chorda
plist_scribere (
     PlistValor* radix,
    PlistStatus* status,
        Piscina* piscina)
{
               chorda  vacua;
    ChordaAedificator* aed;
          PlistStatus  interius = PLIST_SUCCESSUS;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    si (!status)
    {
        redde vacua;
    }
    *status = PLIST_SUCCESSUS;
    si (!radix || !piscina)
    {
        *status = PLIST_ERROR_STRUCTURA;
        redde vacua;
    }
    aed = chorda_aedificator_creare(piscina, MXXIV);
    si (!aed)
    {
        *status = PLIST_ERROR_MEMORIA;
        redde vacua;
    }
    chorda_aedificator_appendere_literis(aed, PLIST_PROLOGUS);
    chorda_aedificator_appendere_literis(aed,
        "<plist version=\"1.0\">\n");
    si (!_valorem_scribere(radix, aed, I, piscina, &interius))
    {
        *status = (interius == PLIST_SUCCESSUS)
                ? PLIST_ERROR_STRUCTURA : interius;
        redde vacua;
    }
    /* linea vacua finalis: forma scriptoris veteris servatur */
    chorda_aedificator_appendere_literis(aed, "</plist>\n\n");
    redde chorda_aedificator_finire(aed);
}

b32
plist_scribere_plagulam (
            PlistValor* radix,
    constans character* via,
           PlistStatus* status,
               Piscina* piscina)
{
    chorda textus = plist_scribere(radix, status, piscina);

    si (chorda_vacua(textus) || !via)
    {
        redde FALSUM;
    }
    redde filum_scribere(via, textus);
}


/* ==================================================
 * Legere
 * ================================================== */

/* "basis/nomen" aut "basis/nomen[index]" - semita sedis (contractus
 * plist.h) */
interior chorda
_semita_liberi (
                 chorda  basis,
     constans character* titulus,
                    i32  index,
                    b32  cum_indice,
                Piscina* piscina)
{
    ChordaAedificator* aed;

    aed = chorda_aedificator_creare(piscina,
        (memoriae_index)basis.mensura + LXIV);
    si (!aed)
    {
        redde basis;
    }
    chorda_aedificator_appendere_chorda(aed, basis);
    chorda_aedificator_appendere_literis(aed, "/");
    chorda_aedificator_appendere_literis(aed, titulus);
    si (cum_indice)
    {
        chorda_aedificator_appendere_literis(aed, "[");
        chorda_aedificator_appendere_i32(aed, index);
        chorda_aedificator_appendere_literis(aed, "]");
    }
    redde chorda_aedificator_finire(aed);
}

interior b32
_textus_albus (
    chorda s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        i8 c = s.datum[i];

        si (   c != (i8)' ' && c != (i8)'\t' && c != (i8)'\n'
            && c != (i8)'\r')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Apple data per lineas frangit (tabulationibus indentata), ergo
 * spatium album ante decodificationem tollendum est. */
interior chorda
_album_tollere (
      chorda  s,
     Piscina* piscina)
{
    chorda  fructus;
        i8* purgata;
       i32  i;
       i32  n = ZEPHYRUM;

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;

    si (s.mensura == ZEPHYRUM)
    {
        redde fructus;
    }
    purgata = (i8*)piscina_allocare(piscina,
        (memoriae_index)s.mensura);
    si (!purgata)
    {
        redde fructus;
    }
    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        i8 c = s.datum[i];

        si (   c != (i8)' ' && c != (i8)'\t' && c != (i8)'\n'
            && c != (i8)'\r')
        {
            purgata[n] = c;
            n++;
        }
    }
    fructus.datum    = purgata;
    fructus.mensura  = n;
    redde fructus;
}

/* Liberum proximum quod ELEMENTUM est; textum album praeterit,
 * textum aliud VITIUM nominat (numquam tacite deicit). */
interior constans XmlCoctus*
_elementum_proximum (
    constans XmlCoctus* parens,
                   i32* index,
                   b32* vitium)
{
    *vitium = FALSUM;
    dum (*index < xml_liberorum_numerus(parens))
    {
        constans XmlCoctus* liberum = xml_liberum(parens, *index);

        (*index)++;
        si (xml_nodi_genus(liberum) == XML_COCTUS_ELEMENTUM)
        {
            redde liberum;
        }
        si (   xml_nodi_genus(liberum) == XML_COCTUS_TEXTUS
            && !_textus_albus(xml_textus(liberum)))
        {
            *vitium = VERUM;
            redde NIHIL;
        }
    }
    redde NIHIL;
}

/* textus omnis elementi; vacuus si memoria deficit */
interior chorda
_textus (
    constans XmlCoctus* nodus,
               Piscina* piscina)
{
    chorda textus;

    si (!xml_textus_totus(nodus, piscina, &textus))
    {
        textus.datum    = NIHIL;
        textus.mensura  = ZEPHYRUM;
    }
    redde textus;
}

/* textus in internamento: valores fontem non retinent */
interior chorda
_textus_internatus (
     constans XmlCoctus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    chorda  textus = _textus(nodus, piscina);
    chorda* internata;

    si (textus.mensura == ZEPHYRUM)
    {
        redde textus;
    }
    internata = chorda_internare(intern, textus);
    redde internata ? *internata : textus;
}

interior PlistValor*
_recusare (
          PlistResultus* r,
            PlistStatus  status,
                 chorda  semita,
     constans character* causa,
                Piscina* piscina)
{
    r->status  = status;
    r->semita  = semita;
    r->causa   = chorda_ex_literis(causa, piscina);
    redde NIHIL;
}

interior PlistValor*
_valorem_ex_nodo (
     constans XmlCoctus* nodus,
                 chorda  semita,
                    i32  gradus,
                Piscina* piscina,
    InternamentumChorda* intern,
          PlistResultus* r)
{
         PlistValor* v;
             chorda  textus;
             chorda  titulus  = xml_titulus(nodus);
                i32  index    = ZEPHYRUM;
                b32  vitium;

    si (!nodus)
    {
        redde _recusare(r, PLIST_ERROR_STRUCTURA, semita,
                        "nodus sine titulo", piscina);
    }
    si (gradus > PLIST_PROFUNDITAS_MAXIMA)
    {
        redde _recusare(r, PLIST_ERROR_STRUCTURA, semita,
            "nidi nimis profundi (PLIST_PROFUNDITAS_MAXIMA)", piscina);
    }
    si (chorda_aequalis_literis(titulus, "string"))
    {
        redde plist_chordam_creare(
            _textus_internatus(nodus, piscina, intern), piscina);
    }
    si (chorda_aequalis_literis(titulus, "true"))
    {
        redde plist_veritatem_creare(VERUM, piscina);
    }
    si (chorda_aequalis_literis(titulus, "false"))
    {
        redde plist_veritatem_creare(FALSUM, piscina);
    }
    si (chorda_aequalis_literis(titulus, "integer"))
    {
        s64 numerus;

        si (!chorda_ut_s64(_textus(nodus, piscina),
                           &numerus))
        {
            redde _recusare(r, PLIST_ERROR_NUMERUS, semita,
                            "integer non legitur", piscina);
        }
        redde plist_integrum_creare(numerus, piscina);
    }
    si (chorda_aequalis_literis(titulus, "real"))
    {
        f64 numerus;

        si (!chorda_ut_f64(_textus(nodus, piscina),
                           &numerus))
        {
            redde _recusare(r, PLIST_ERROR_NUMERUS, semita,
                            "real non legitur", piscina);
        }
        redde plist_realem_creare(numerus, piscina);
    }
    si (chorda_aequalis_literis(titulus, "date"))
    {
        DiesHora dh;

        si (!fasti_ex_iso(_textus(nodus, piscina), &dh))
        {
            redde _recusare(r, PLIST_ERROR_DIES, semita,
                            "date non est instans ISO-8601 UTC",
                            piscina);
        }
        redde plist_diem_creare(dh, piscina);
    }
    si (chorda_aequalis_literis(titulus, "data"))
    {
        Base64Fructus decocta;
               chorda purgata;

        purgata = _album_tollere(
            _textus(nodus, piscina), piscina);
        /* data vacua licet (nulli octeti); decodificatorem de
         * ingressu vacuo non interrogamus */
        si (purgata.mensura == ZEPHYRUM)
        {
            textus.datum    = NIHIL;
            textus.mensura  = ZEPHYRUM;
            redde plist_data_creare(textus, piscina);
        }
        decocta = base64_decodificare(purgata, piscina);
        si (!decocta.datum)
        {
            redde _recusare(r, PLIST_ERROR_BASE64, semita,
                            "data non est base64", piscina);
        }
        textus.datum    = decocta.datum;
        textus.mensura  = decocta.mensura;
        redde plist_data_creare(textus, piscina);
    }
    si (chorda_aequalis_literis(titulus, "array"))
    {
        i32 ordo = ZEPHYRUM;

        v = plist_lista_creare(piscina);
        si (!v)
        {
            redde _recusare(r, PLIST_ERROR_MEMORIA, semita,
                            "lista non creata", piscina);
        }
        per (;;)
        {
            constans XmlCoctus* liberum = _elementum_proximum(nodus,
                                              &index, &vitium);
                    PlistValor* filius;

            si (vitium)
            {
                redde _recusare(r, PLIST_ERROR_STRUCTURA, semita,
                                "textus non albus in array", piscina);
            }
            si (!liberum)
            {
                frange;
            }
            filius = _valorem_ex_nodo(liberum,
                _semita_liberi(semita, "array", ordo, VERUM, piscina),
                gradus + I, piscina, intern, r);
            si (!filius)
            {
                redde NIHIL;
            }
            plist_lista_addere(v, filius, piscina);
            ordo++;
        }
        redde v;
    }
    si (chorda_aequalis_literis(titulus, "dict"))
    {
        i32 ordo = ZEPHYRUM;

        v = plist_dictio_creare(piscina);
        si (!v)
        {
            redde _recusare(r, PLIST_ERROR_MEMORIA, semita,
                            "dictio non creata", piscina);
        }
        per (;;)
        {
    constans XmlCoctus* nodus_clavis;
    constans XmlCoctus* nodus_valoris;
            PlistValor* filius;
                chorda  semita_clavis;

            nodus_clavis = _elementum_proximum(nodus, &index, &vitium);
            si (vitium)
            {
                redde _recusare(r, PLIST_ERROR_STRUCTURA, semita,
                                "textus non albus in dict", piscina);
            }
            si (!nodus_clavis)
            {
                frange;
            }
            semita_clavis = _semita_liberi(semita, "key", ordo, VERUM,
                                           piscina);
            si (!chorda_aequalis_literis(xml_titulus(nodus_clavis),
                                         "key"))
            {
                redde _recusare(r, PLIST_ERROR_STRUCTURA,
                                semita_clavis,
                                "valor sine clave", piscina);
            }
            nodus_valoris = _elementum_proximum(nodus, &index,
                                                &vitium);
            si (vitium || !nodus_valoris)
            {
                redde _recusare(r, PLIST_ERROR_STRUCTURA,
                                semita_clavis,
                                "clavis sine valore", piscina);
            }
            filius = _valorem_ex_nodo(nodus_valoris, semita_clavis,
                                      gradus + I, piscina, intern, r);
            si (!filius)
            {
                redde NIHIL;
            }
            plist_dictio_ponere(v,
                _textus_internatus(nodus_clavis, piscina, intern),
                filius,
                piscina);
            ordo++;
        }
        redde v;
    }
    redde _recusare(r, PLIST_ERROR_STRUCTURA,
                    _semita_liberi(semita,
                        chorda_ut_cstr(titulus, piscina),
                        ZEPHYRUM, FALSUM, piscina),
                    "elementum ignotum", piscina);
}

PlistResultus
plist_legere (
                 chorda  textus,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    PlistResultus r;
          XmlDocumentum* documentum;
    constans XmlCoctus* radix_xml;
                 chorda  versio;
    constans XmlCoctus* unicum;
    constans XmlCoctus* superfluum;
                    i32  index = ZEPHYRUM;
                    b32  vitium;

    r.status         = PLIST_SUCCESSUS;
    r.radix          = NIHIL;
    r.causa.datum    = NIHIL;
    r.causa.mensura  = ZEPHYRUM;
    r.semita         = chorda_ex_literis("plist", piscina);

    si (!piscina || !intern || chorda_vacua(textus))
    {
        (vacuum)_recusare(&r, PLIST_ERROR_STRUCTURA, r.semita,
                          "textus vacuus aut plagula non legibilis",
                          piscina);
        redde r;
    }
    /* plist BINARIA: forma alia, ante omnia recusatur */
    si (   textus.mensura                       >= VIII
        && memcmp(textus.datum, "bplist0", VII) == ZEPHYRUM)
    {
        (vacuum)_recusare(&r, PLIST_ERROR_BINARIUM, r.semita,
                          "bplist00 - forma binaria non tractatur",
                          piscina);
        redde r;
    }
    /* NUL post finem (plagulae ad mensuram impletae): CFPropertyList
     * et plutil accipiunt, XML non - sola lenitas plist propria */
    dum (   textus.mensura > ZEPHYRUM
         && textus.datum[textus.mensura - I] == (i8)ZEPHYRUM)
    {
        textus.mensura--;
    }
    documentum  = xml_documentum_legere(textus, piscina);
    radix_xml   = xml_documentum_radix(documentum);
    si (!xml_documentum_successus(documentum) || !radix_xml)
    {
        ChordaAedificator* aed = chorda_aedificator_creare(piscina,
                                                           CCLVI);

        r.status = PLIST_ERROR_XML;
        si (   aed && xml_documentum_diagnostica_scribere(documentum,
                                                       "plist", aed))
        {
            r.causa = chorda_aedificator_finire(aed);
        }
        redde r;
    }
    si (!chorda_aequalis_literis(xml_titulus(radix_xml), "plist"))
    {
        (vacuum)_recusare(&r, PLIST_ERROR_NON_PLIST, r.semita,
                          "radix non est plist", piscina);
        redde r;
    }
    si (   xml_attributum(radix_xml, "version", &versio)
        && !chorda_aequalis_literis(versio, "1.0"))
    {
        (vacuum)_recusare(&r, PLIST_ERROR_VERSIO, r.semita,
                          "versio praeter 1.0", piscina);
        redde r;
    }
    unicum = _elementum_proximum(radix_xml, &index, &vitium);
    si (vitium || !unicum)
    {
        (vacuum)_recusare(&r, PLIST_ERROR_STRUCTURA, r.semita,
                          "plist liberum unum poscit", piscina);
        redde r;
    }
    superfluum = _elementum_proximum(radix_xml, &index, &vitium);
    si (superfluum || vitium)
    {
        (vacuum)_recusare(&r, PLIST_ERROR_STRUCTURA, r.semita,
                          "plist plus uno libero fert", piscina);
        redde r;
    }
    r.radix = _valorem_ex_nodo(unicum, r.semita, I, piscina, intern,
                               &r);
    si (r.status != PLIST_SUCCESSUS)
    {
        r.radix = NIHIL;
    }
    redde r;
}

PlistResultus
plist_legere_plagulam (
     constans character* via,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    si (!via || !piscina)
    {
        PlistResultus r;

        r.status         = PLIST_ERROR_STRUCTURA;
        r.radix          = NIHIL;
        r.semita         = chorda_ex_literis("plist", piscina);
        r.causa.datum    = NIHIL;
        r.causa.mensura  = ZEPHYRUM;
        redde r;
    }
    redde plist_legere(filum_legere_totum(via, piscina), piscina,
                       intern);
}
