#include "xml_commune.h"
/* Constructio -> scriptio -> relectio. Effusio cruda etiam in
 * build/sonda/xml_scribere_reditus/out_<n>.xml pro oraculo expat. */
static Piscina* p; static InternamentumChorda* it; static int nfile = 0;
static void emitte(const char* t, XmlNodus* n, b32 pul)
{
    chorda s = xml_scribere(n, p, pul);
    XmlResultus r; char via[256]; FILE* f;
    printf("%-26s out=", t); sd_chorda(&s);
    r = xml_legere(s, p, it);
    printf(" | relectum succ=%d status=%d : ", (int)r.successus, (int)r.status);
    sd_arbor(r.radix); printf("\n");
    sprintf(via, "build/sonda/xml_scribere_reditus/out_%02d.xml", nfile++);
    f = fopen(via, "wb"); if (f) { if (s.mensura) fwrite(s.datum, 1, s.mensura, f); fclose(f); }
}
int main(void)
{
    XmlNodus *a, *b, *c;
    b32 rv;
    p = piscina_generare_dynamicum("sonda", 65536);
    it = internamentum_creare(p);

    a = xml_elementum_creare(p, it, "a");
    rv = xml_attributum_addere(a, p, it, "x", "");
    printf("addere x=\"\" -> %d, habet=%d, capere=%p\n", (int)rv,
           (int)xml_attributum_habet(a, "x"), (void*)xml_attributum_capere(a, "x"));
    emitte("attr vacuum", a, FALSUM);

    a = xml_elementum_creare(p, it, "a");
    xml_attributum_addere(a, p, it, "x", "1");
    xml_attributum_addere(a, p, it, "x", "2");
    printf("duplex capere -> "); sd_chorda(xml_attributum_capere(a, "x")); printf("\n");
    emitte("attr duplex", a, FALSUM);

    a = xml_elementum_creare(p, it, "a");
    xml_attributum_addere(a, p, it, "x", "q\"<&>'\t\n");
    emitte("attr effugium", a, FALSUM);

    a = xml_elementum_creare(p, it, "a");
    xml_textum_addere(a, p, it, " lead");
    emitte("textus spatium initiale", a, FALSUM);

    a = xml_elementum_creare(p, it, "a");
    xml_textum_addere(a, p, it, " ");
    emitte("textus spatium solum", a, FALSUM);

    a = xml_elementum_creare(p, it, "a");
    rv = xml_textum_addere(a, p, it, "");
    printf("textum_addere \"\" -> %d\n", (int)rv);
    c = xml_textum_creare(p, it, "");
    printf("textum_creare \"\" -> valor=%p\n", c ? (void*)c->valor : (void*)0);
    emitte("textus vacuus", a, FALSUM);

    a = xml_elementum_creare(p, it, "a");
    xml_textum_addere(a, p, it, "x");
    xml_textum_addere(a, p, it, "y");
    emitte("textus duo", a, FALSUM);

    a = xml_elementum_creare(p, it, "a");
    xml_liberum_addere(a, xml_commentum_creare(p, it, "a -- b -"));
    emitte("commentum --", a, FALSUM);

    a = xml_elementum_creare(p, it, "a");
    xml_liberum_addere(a, xml_commentum_creare(p, it, "x --> <b/> <!-- y"));
    emitte("commentum -->", a, FALSUM);

    a = xml_elementum_creare(p, it, "a");
    xml_liberum_addere(a, xml_commentum_creare(p, it, "salve"));
    emitte("commentum reditus", a, FALSUM);

    a = xml_elementum_creare(p, it, "a b");
    emitte("nomen cum spatio", a, FALSUM);
    a = xml_elementum_creare(p, it, "a><z");
    emitte("nomen cum <>", a, FALSUM);

    {
        chorda cn; cn.datum = (i8*)"x\0y"; cn.mensura = 3;
        a = xml_elementum_creare(p, it, "a");
        xml_liberum_addere(a, xml_textum_creare_ex_chorda(p, it, cn));
        emitte("textus NUL", a, FALSUM);
        a = xml_elementum_creare(p, it, "a");
        xml_liberum_addere(a, xml_textum_creare_ex_chorda(p, it, chorda_ex_literis("\x01\x1b", p)));
        emitte("textus C0", a, FALSUM);
    }

    /* pulchrum: mixtum */
    a = xml_elementum_creare(p, it, "a");
    xml_textum_addere(a, p, it, "x");
    b = xml_elementum_creare(p, it, "b");
    xml_liberum_addere(a, b);
    xml_textum_addere(a, p, it, "y");
    emitte("mixtum compactum", a, FALSUM);
    emitte("mixtum pulchrum", a, VERUM);

    /* radix textus */
    c = xml_textum_creare(p, it, "<t>");
    emitte("radix textus", c, FALSUM);

    /* elementum_creare "" -> titulus NIHIL? scriptor ruit? */
    a = xml_elementum_creare(p, it, "");
    printf("elementum_creare \"\" -> nodus=%p titulus=%p\n", (void*)a, a ? (void*)a->titulus : (void*)0);
    fflush(stdout);
    if (a) emitte("nomen vacuum", a, FALSUM);
    return 0;
}
