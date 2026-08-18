/* dialectus domus - fixtura scripta manu: ES5 + class, sagittae,
   formulae, let/const. Munda esse debet: imperfecta et ignota
   NULLA (porta corporis id iudicat). */
'use strict';

class Salutator extends Object {
  constructor(nomen) {
    super();
    this.nomen = nomen;
    this.numeri = [12, 1.5, .5, 1e3, 1E+3, 0x1F, 0b101, 0o17];
  }
  saluta(quis) {
    const forma = `salve ${quis}, a ${this.nomen}`;
    let plures = `${quis} habet ${[1, 2].length} partes`;
    return forma + plures;
  }
}

const par = (a, b) => a + b;
const impar = x => x * 2;

function probare(s) {
  var re = /^ab[/]c$/gi;
  var divisio = s.length / 2 / 3;
  var post = this ? 1 : 2;
  if (re.test(s) && s !== 'x') {
    return /salve/.test(s) ? par(1, 2) : impar(3);
  }
  var multiforma = `linea una
linea altera ${divisio}
linea tertia`;
  var obiectum = { a: 1, b: { c: 2 } };
  var intra = `valor: ${ { x: 3 }.x }`;
  var nidus = `extra ${`intra ${post}`} finis`;
  var effugium = 'citatio \' intra' + "et \" altera";
  var continuata = 'prima \
secunda';
  var x = 10;
  x /= 2;
  x >>>= 1;
  var y = x === 10 ? x++ / 2 : ~x;
  return obiectum.b.c + multiforma.length + intra.length +
    nidus.length + effugium.length + continuata.length + y;
}
