#include "xml_commune.h"

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* it = internamentum_creare(p);

    /* vacuum / prologus */
    SD_LEGERE("vacuum", "");
    SD_LEGERE("spatium solum", "   \n ");
    SD_LEGERE("decl sola", "<?xml version=\"1.0\"?>");
    SD_LEGERE("commentum solum", "<!-- x -->");
    SD_LEGERE("doctype solum", "<!DOCTYPE x>");
    /* longitudo 1 */
    SD_LEGERE("<", "<");
    SD_LEGERE(">", ">");
    SD_LEGERE("&", "&");
    SD_LEGERE("a", "a");
    sd_legere_n("NUL", "\0", 1, p, it);
    /* radix */
    SD_LEGERE("textus ante radicem", "hello<a/>");
    SD_LEGERE("BOM", "\xEF\xBB\xBF<a/>");
    SD_LEGERE("duae radices", "<a/><b/>");
    SD_LEGERE("textus post radicem", "<a/>trailing");
    SD_LEGERE("claudens vagus post", "<a/></b>");
    SD_LEGERE("claudens vagus solus", "</b>");
    /* clausura */
    SD_LEGERE("non clausum", "<a><b></b>");
    SD_LEGERE("impropria", "<a></b>");
    SD_LEGERE("impropria nidus", "<a><b></a></b>");
    SD_LEGERE("praefix claudens", "<ab></a>");
    /* attributa */
    SD_LEGERE("attr sine =", "<a x/>");
    SD_LEGERE("attr sine quot", "<a x=1/>");
    SD_LEGERE("attr quot mixtae", "<a x=\"1'/>");
    SD_LEGERE("attr sine valor", "<a x=/>");
    SD_LEGERE("attr sine spatio", "<a x=\"1\"y=\"2\"/>");
    SD_LEGERE("attr < in valor", "<a x=\"<\"/>");
    SD_LEGERE("attr vacuum x=\"\"", "<a x=\"\"/>");
    SD_LEGERE("attr vacuum x=''", "<a x=''/>");
    SD_LEGERE("attr duplex", "<a x=\"1\" x=\"2\"/>");
    SD_LEGERE("attr newline", "<a x=\"1\n2\"/>");
    /* entitates */
    SD_LEGERE("ent quinque", "<a>&lt;&gt;&amp;&quot;&apos;</a>");
    SD_LEGERE("ent ignota", "<a>&foo;</a>");
    SD_LEGERE("ent numerica", "<a>&#65;&#x41;</a>");
    SD_LEGERE("ent nuda", "<a>a & b</a>");
    SD_LEGERE("ent sine ;", "<a>&amp</a>");
    /* textus spatia */
    SD_LEGERE("textus spatium initiale", "<a>  x</a>");
    SD_LEGERE("textus spatium finale", "<a>x  </a>");
    SD_LEGERE("textus spatium solum", "<a> </a>");
    SD_LEGERE("mixtum spatia", "<a>x <b/> y</a>");
    SD_LEGERE("textus NUL", "<a>x\0y</a>");
    SD_LEGERE("> in textu", "<a>a>b]]>c</a>");
    /* commenta */
    SD_LEGERE("commentum in elemento", "<a><!--c--></a>");
    SD_LEGERE("commentum <!-->", "<a><!--></a>");
    SD_LEGERE("commentum <!--->", "<a><!---></a>");
    SD_LEGERE("commentum --", "<a><!-- a -- b --></a>");
    SD_LEGERE("commentum non clausum", "<a><!-- x");
    SD_LEGERE("commentum prolog non cl", "<!-- x <a/>");
    /* PI, CDATA, DOCTYPE */
    SD_LEGERE("PI in elemento", "<a><?pi x?></a>");
    SD_LEGERE("decl non prima", "<a/><?xml version=\"1.0\"?>");
    SD_LEGERE("CDATA", "<a><![CDATA[x<y]]></a>");
    SD_LEGERE("DOCTYPE in elemento", "<a><!DOCTYPE x></a>");
    SD_LEGERE("DOCTYPE > in SYSTEM", "<!DOCTYPE x SYSTEM \"a><b/>\"><c/>");
    SD_LEGERE("DOCTYPE [ in SYSTEM", "<!DOCTYPE x SYSTEM \"a[b\"><c/>");
    SD_LEGERE("DOCTYPE interna", "<!DOCTYPE x [ <!ENTITY a \"b\"> ]><x/>");
    /* nomina */
    SD_LEGERE("nomen digitus", "<1a/>");
    SD_LEGERE("nomen -", "<-a/>");
    SD_LEGERE("nomen vacuum", "<>");
    SD_LEGERE("< a>", "< a/>");
    SD_LEGERE("</ a>", "<a></ a>");
    SD_LEGERE("</a >", "<a></a >");
    SD_LEGERE("nomen colon", "<x:a xmlns:x=\"u\"/>");
    SD_LEGERE("nomen UTF-8", "<caf\xC3\xA9/>");
    /* self-closing */
    SD_LEGERE("<a />", "<a />");
    SD_LEGERE("<a></a>", "<a></a>");
    /* error positio */
    SD_LEGERE("err lin1 col1", "</a>");
    SD_LEGERE("err post LF", "<a>\n\n<b></c></a>");
    SD_LEGERE("err post spatia", "<a>    </c>");
    SD_LEGERE("err post textum LF", "<a>x\ny\n<b></c></a>");
    SD_LEGERE("err post CRLF", "<a>x\r\ny\r\n</c>");
    SD_LEGERE("err post UTF-8", "<a>\xC3\xA9\xC3\xA9</c>");
    SD_LEGERE("err EOF", "<a>");
    SD_LEGERE("err decl LF", "<?xml\n\n?>\n<a></c>");
    return 0;
}
