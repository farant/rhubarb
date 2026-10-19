#include "xml_commune.h"
int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* it = internamentum_creare(p);
    SD_LEGERE("PI prolog LF", "<?pi\n\n?>\n<a></c>");
    SD_LEGERE("comm prolog LF", "<!--\n\n-->\n<a></c>");
    SD_LEGERE("decl LF", "<?xml version=\"1.0\"?>\n\n<a></c>");
    SD_LEGERE("plist-like", "<?xml version=\"1.0\"?>\n<plist>\n  <dict>\n    <key>k</key>\n    <string>v</strin>\n  </dict>\n</plist>");
    return 0;
}
