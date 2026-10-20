#include "xml_commune.h"
int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* it = internamentum_creare(p);
    XmlResultus r; XmlNodus *rad, *t; Xar* xs; chorda s;
    r = xml_legere_ex_literis("<r k=\"v\" kk=\"w\">a<ab/><a id=\"1\"/>m<!--z--><b>c<i>d</i></b><a id=\"2\"/>e</r>", p, it);
    rad = r.radix;
    printf("succ=%d n=%u\n", (int)r.successus, xml_numerus_liberorum(rad));
    printf("invenire a -> "); sd_arbor(xml_invenire_liberum(rad, "a")); printf("\n");
    printf("invenire ab -> "); sd_arbor(xml_invenire_liberum(rad, "ab")); printf("\n");
    printf("invenire abc -> "); sd_arbor(xml_invenire_liberum(rad, "abc")); printf("\n");
    printf("invenire A -> "); sd_arbor(xml_invenire_liberum(rad, "A")); printf("\n");
    printf("invenire \"\" -> "); sd_arbor(xml_invenire_liberum(rad, "")); printf("\n");
    printf("invenire i (descendens) -> "); sd_arbor(xml_invenire_liberum(rad, "i")); printf("\n");
    printf("invenire NIHIL nodus -> %p\n", (void*)xml_invenire_liberum(NIHIL, "a"));
    printf("invenire NIHIL titulus -> %p\n", (void*)xml_invenire_liberum(rad, NIHIL));
    t = xml_liberum_ad_indicem(rad, 0);
    printf("invenire in textu -> %p\n", (void*)xml_invenire_liberum(t, "a"));
    xs = xml_invenire_omnes_liberos(rad, "a", p);
    printf("omnes a -> %u:", xs ? xar_numerus(xs) : 9999u);
    if (xs) { i32 k; for (k = 0; k < xar_numerus(xs); k++) { printf(" "); sd_chorda(xml_attributum_capere(*(XmlNodus**)xar_obtinere(xs, k), "id")); } }
    printf("\n");
    xs = xml_invenire_omnes_liberos(rad, "nihil", p);
    printf("omnes nullum -> %s %u\n", xs ? "Xar" : "NIHIL", xs ? xar_numerus(xs) : 0u);
    xs = xml_invenire_omnes_liberos(t, "a", p);
    printf("omnes in textu -> %s\n", xs ? "Xar" : "NIHIL");
    printf("capere k -> "); sd_chorda(xml_attributum_capere(rad, "k")); printf("\n");
    printf("capere kk -> "); sd_chorda(xml_attributum_capere(rad, "kk")); printf("\n");
    printf("capere kkk -> "); sd_chorda(xml_attributum_capere(rad, "kkk")); printf("\n");
    printf("capere textus -> %p habet=%d\n", (void*)xml_attributum_capere(t, "k"), (int)xml_attributum_habet(t, "k"));
    printf("capere NIHIL -> %p\n", (void*)xml_attributum_capere(NIHIL, "k"));
    printf("ad_indicem n -> %p\n", (void*)xml_liberum_ad_indicem(rad, xml_numerus_liberorum(rad)));
    printf("ad_indicem n+5 -> %p\n", (void*)xml_liberum_ad_indicem(rad, xml_numerus_liberorum(rad) + 5));
    printf("ad_indicem 0-1 -> %p\n", (void*)xml_liberum_ad_indicem(rad, (i32)0 - 1));
    printf("ad_indicem NIHIL -> %p\n", (void*)xml_liberum_ad_indicem(NIHIL, 0));
    printf("numerus NIHIL -> %u, textus -> %u\n", xml_numerus_liberorum(NIHIL), xml_numerus_liberorum(t));
    s = xml_textus_internus(rad, p); printf("textus_internus r -> "); sd_chorda(&s); printf("\n");
    s = xml_textus_internus(t, p); printf("textus_internus textus -> "); sd_chorda(&s); printf("\n");
    s = xml_textus_internus(xml_invenire_liberum(rad, "ab"), p); printf("textus_internus vacuum -> datum=%p mens=%u\n", (void*)s.datum, s.mensura);
    s = xml_textus_internus(NIHIL, p); printf("textus_internus NIHIL -> datum=%p mens=%u\n", (void*)s.datum, s.mensura);
    return 0;
}
