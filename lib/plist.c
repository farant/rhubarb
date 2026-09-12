/* plist.c - tabulae proprietatum (contractus in capite).
 *
 * Scriptor arborem XmlNodus struit et xml_scribere vocat, ergo
 * escapatio UNO loco vivit (strato XML); prologus et declaratio
 * generis hic praefiguntur. Lector eandem arborem ambulat (opus V).
 */

#include "plist.h"
#include "xml.h"
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

interior XmlNodus*
_nodum_ex_valore (
             PlistValor* v,
                Piscina* piscina,
    InternamentumChorda* intern,
            PlistStatus* status)
{
    XmlNodus* nodus;
         i32  i;

    si (!v)
    {
        *status = PLIST_ERROR_STRUCTURA;
        redde NIHIL;
    }
    commutatio (v->genus)
    {
        casus PLIST_CHORDA:
            si (_signum_vetitum(v->textus))
            {
                *status = PLIST_ERROR_SIGNUM;
                redde NIHIL;
            }
            nodus = xml_elementum_creare(piscina, intern, "string");
            si (nodus)
            {
                xml_textum_addere(nodus, piscina, intern,
                    chorda_ut_cstr(v->textus, piscina));
            }
            redde nodus;

        casus PLIST_VERITAS:
            redde xml_elementum_creare(piscina, intern,
                v->veritas ? "true" : "false");

        casus PLIST_INTEGER:
            nodus = xml_elementum_creare(piscina, intern, "integer");
            si (nodus)
            {
                xml_textum_addere(nodus, piscina, intern,
                    chorda_ut_cstr(
                        chorda_ex_s64(v->integrum, piscina), piscina));
            }
            redde nodus;

        casus PLIST_REALIS:
            nodus = xml_elementum_creare(piscina, intern, "real");
            si (nodus)
            {
                xml_textum_addere(nodus, piscina, intern,
                    chorda_ut_cstr(
                        chorda_ex_f64_exacta(v->realis, piscina),
                        piscina));
            }
            redde nodus;

        casus PLIST_DIES:
        {
            chorda textus = fasti_ad_iso(v->dies, piscina);

            si (chorda_vacua(textus))
            {
                *status = PLIST_ERROR_DIES;
                redde NIHIL;
            }
            nodus = xml_elementum_creare(piscina, intern, "date");
            si (nodus)
            {
                xml_textum_addere(nodus, piscina, intern,
                    chorda_ut_cstr(textus, piscina));
            }
            redde nodus;
        }

        casus PLIST_DATA:
            nodus = xml_elementum_creare(piscina, intern, "data");
            si (nodus)
            {
                xml_textum_addere(nodus, piscina, intern,
                    chorda_ut_cstr(
                        base64_codificare(v->textus.datum,
                            v->textus.mensura, piscina), piscina));
            }
            redde nodus;

        casus PLIST_LISTA:
            nodus = xml_elementum_creare(piscina, intern, "array");
            si (!nodus)
            {
                *status = PLIST_ERROR_MEMORIA;
                redde NIHIL;
            }
            per (i = ZEPHYRUM; i < plist_numerus(v); i++)
            {
                XmlNodus* liberum = _nodum_ex_valore(
                    plist_ad_indicem(v, i), piscina, intern, status);

                si (!liberum)
                {
                    redde NIHIL;
                }
                xml_liberum_addere(nodus, liberum);
            }
            redde nodus;

        casus PLIST_DICTIO:
            nodus = xml_elementum_creare(piscina, intern, "dict");
            si (!nodus)
            {
                *status = PLIST_ERROR_MEMORIA;
                redde NIHIL;
            }
            per (i = ZEPHYRUM; i < plist_numerus(v); i++)
            {
                PlistPar* par;
                XmlNodus* clavis;
                XmlNodus* liberum;

                par = (PlistPar*)xar_obtinere(v->liberi, i);
                si (!par)
                {
                    *status = PLIST_ERROR_STRUCTURA;
                    redde NIHIL;
                }
                si (_signum_vetitum(par->clavis))
                {
                    *status = PLIST_ERROR_SIGNUM;
                    redde NIHIL;
                }
                clavis = xml_elementum_creare(piscina, intern, "key");
                si (!clavis)
                {
                    *status = PLIST_ERROR_MEMORIA;
                    redde NIHIL;
                }
                xml_textum_addere(clavis, piscina, intern,
                    chorda_ut_cstr(par->clavis, piscina));
                xml_liberum_addere(nodus, clavis);

                liberum = _nodum_ex_valore(par->valor, piscina, intern,
                                           status);
                si (!liberum)
                {
                    redde NIHIL;
                }
                xml_liberum_addere(nodus, liberum);
            }
            redde nodus;

        ordinarius:
            *status = PLIST_ERROR_STRUCTURA;
            redde NIHIL;
    }
}

chorda
plist_scribere (
     PlistValor* radix,
    PlistStatus* status,
        Piscina* piscina)
{
                  chorda  vacua;
                  chorda  corpus;
                XmlNodus* nodus_plist;
                XmlNodus* nodus_radix;
     InternamentumChorda* intern;
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
    intern = internamentum_creare(piscina);
    si (!intern)
    {
        *status = PLIST_ERROR_MEMORIA;
        redde vacua;
    }
    nodus_plist = xml_elementum_creare(piscina, intern, "plist");
    si (   !nodus_plist
        || !xml_attributum_addere(nodus_plist, piscina, intern,
                                  "version", "1.0"))
    {
        *status = PLIST_ERROR_MEMORIA;
        redde vacua;
    }
    nodus_radix = _nodum_ex_valore(radix, piscina, intern, &interius);
    si (!nodus_radix)
    {
        *status = (interius == PLIST_SUCCESSUS)
                ? PLIST_ERROR_STRUCTURA : interius;
        redde vacua;
    }
    xml_liberum_addere(nodus_plist, nodus_radix);

    corpus = xml_scribere(nodus_plist, piscina, VERUM);
    si (chorda_vacua(corpus))
    {
        *status = PLIST_ERROR_MEMORIA;
        redde vacua;
    }
    aed = chorda_aedificator_creare(piscina,
        (memoriae_index)corpus.mensura + CCLVI);
    si (!aed)
    {
        *status = PLIST_ERROR_MEMORIA;
        redde vacua;
    }
    chorda_aedificator_appendere_literis(aed, PLIST_PROLOGUS);
    chorda_aedificator_appendere_chorda(aed, corpus);
    chorda_aedificator_appendere_literis(aed, "\n");
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

/* "basis/nomen" aut "basis/nomen[index]" - semita sedis, quia
 * XmlNodus lineam non fert */
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
interior XmlNodus*
_elementum_proximum (
    XmlNodus* parens,
         i32* index,
         b32* vitium)
{
    *vitium = FALSUM;
    dum (*index < xml_numerus_liberorum(parens))
    {
        XmlNodus* liberum = xml_liberum_ad_indicem(parens, *index);

        (*index)++;
        si (!liberum)
        {
            perge;
        }
        si (liberum->genus == XML_NODUS_ELEMENTUM)
        {
            redde liberum;
        }
        si (   liberum->genus == XML_NODUS_TEXTUS && liberum->valor
            && !_textus_albus(*liberum->valor))
        {
            *vitium = VERUM;
            redde NIHIL;
        }
    }
    redde NIHIL;
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
               XmlNodus* nodus,
                 chorda  semita,
                Piscina* piscina,
    InternamentumChorda* intern,
          PlistResultus* r)
{
         PlistValor* v;
             chorda  textus;
                i32  index = ZEPHYRUM;
                b32  vitium;

    si (!nodus || !nodus->titulus)
    {
        redde _recusare(r, PLIST_ERROR_STRUCTURA, semita,
                        "nodus sine titulo", piscina);
    }
    si (chorda_aequalis_literis(*nodus->titulus, "string"))
    {
        redde plist_chordam_creare(
            xml_textus_internus(nodus, piscina), piscina);
    }
    si (chorda_aequalis_literis(*nodus->titulus, "true"))
    {
        redde plist_veritatem_creare(VERUM, piscina);
    }
    si (chorda_aequalis_literis(*nodus->titulus, "false"))
    {
        redde plist_veritatem_creare(FALSUM, piscina);
    }
    si (chorda_aequalis_literis(*nodus->titulus, "integer"))
    {
        s64 numerus;

        si (!chorda_ut_s64(xml_textus_internus(nodus, piscina),
                           &numerus))
        {
            redde _recusare(r, PLIST_ERROR_NUMERUS, semita,
                            "integer non legitur", piscina);
        }
        redde plist_integrum_creare(numerus, piscina);
    }
    si (chorda_aequalis_literis(*nodus->titulus, "real"))
    {
        f64 numerus;

        si (!chorda_ut_f64(xml_textus_internus(nodus, piscina),
                           &numerus))
        {
            redde _recusare(r, PLIST_ERROR_NUMERUS, semita,
                            "real non legitur", piscina);
        }
        redde plist_realem_creare(numerus, piscina);
    }
    si (chorda_aequalis_literis(*nodus->titulus, "date"))
    {
        DiesHora dh;

        si (!fasti_ex_iso(xml_textus_internus(nodus, piscina), &dh))
        {
            redde _recusare(r, PLIST_ERROR_DIES, semita,
                            "date non est instans ISO-8601 UTC",
                            piscina);
        }
        redde plist_diem_creare(dh, piscina);
    }
    si (chorda_aequalis_literis(*nodus->titulus, "data"))
    {
        Base64Fructus decocta;
               chorda purgata;

        purgata = _album_tollere(
            xml_textus_internus(nodus, piscina), piscina);
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
    si (chorda_aequalis_literis(*nodus->titulus, "array"))
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
            XmlNodus* liberum = _elementum_proximum(nodus, &index,
                                                    &vitium);
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
                piscina, intern, r);
            si (!filius)
            {
                redde NIHIL;
            }
            plist_lista_addere(v, filius, piscina);
            ordo++;
        }
        redde v;
    }
    si (chorda_aequalis_literis(*nodus->titulus, "dict"))
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
               XmlNodus* nodus_clavis;
               XmlNodus* nodus_valoris;
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
            si (   !nodus_clavis->titulus
                || !chorda_aequalis_literis(*nodus_clavis->titulus,
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
                                      piscina, intern, r);
            si (!filius)
            {
                redde NIHIL;
            }
            plist_dictio_ponere(v,
                xml_textus_internus(nodus_clavis, piscina), filius,
                piscina);
            ordo++;
        }
        redde v;
    }
    redde _recusare(r, PLIST_ERROR_STRUCTURA,
                    _semita_liberi(semita,
                        chorda_ut_cstr(*nodus->titulus, piscina),
                        ZEPHYRUM, FALSUM, piscina),
                    "elementum ignotum", piscina);
}

PlistResultus
plist_legere (
                 chorda  textus,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    PlistResultus  r;
      XmlResultus  fructus_xml;
         XmlNodus* radix_xml;
           chorda* versio;
         XmlNodus* unicum;
         XmlNodus* superfluum;
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
    fructus_xml = xml_legere(textus, piscina, intern);
    si (!fructus_xml.successus || !fructus_xml.radix)
    {
        r.status  = PLIST_ERROR_XML;
        r.causa   = chorda_ex_s32((s32)fructus_xml.status,
            piscina);
        redde r;
    }
    radix_xml = fructus_xml.radix;
    si (   !radix_xml->titulus
        || !chorda_aequalis_literis(*radix_xml->titulus, "plist"))
    {
        (vacuum)_recusare(&r, PLIST_ERROR_NON_PLIST, r.semita,
                          "radix non est plist", piscina);
        redde r;
    }
    versio = xml_attributum_capere(radix_xml, "version");
    si (versio && !chorda_aequalis_literis(*versio, "1.0"))
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
    r.radix = _valorem_ex_nodo(unicum, r.semita, piscina, intern, &r);
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
