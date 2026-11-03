# ave: melodia primi (modus I, d dorius, metrum liberum) cum verbis Ave
# Maria - schola super ison D. Incisa primi verbis respondent.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import primus

VERBA = [
    "A-ve Ma-ri-a_ _ gra-ti-a ple-na_ _",
    "Do-mi_ _ nus_ te_ _ _ cum_ _",
    "be-ne-dic-ta tu in mu-li-e-ri-bus_ _",
    "et be-ne-dic-tus fruc-tus ven-tris tu-i, Ie-sus.",
]
COMMENTARIUM = '''
  <!-- ave: melodia primi (modus I, d dorius, metrum liberum) cum
       verbis Ave Maria - primum carmen cantatum (carmen.h VERBA).
       Incisa: Ave Maria | gratia plena, Dominus | tecum, benedicta |
       tu in mulieribus, et benedictus fructus | ventris tui | Iesus.
       Melismata in "Ma-ri-a", "ple-na", "Do-mi-nus te-cum".
       Generator: musica/fontes/ave.py -->'''

if __name__ == '__main__':
    open(sys.argv[1], 'w').write(
        primus.carmen('ave', COMMENTARIUM, ' '.join(VERBA)))
