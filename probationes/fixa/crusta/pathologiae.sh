# pathologiae.sh - casus pathologici crustae (spec crusta-arbor-spec.md
# appendix A.IX et additamenta P6); forma Oils: '#### titulus'.
# Plagula tota etiam octetim per corpus parsatur.
#### positio-verba
echo if; x=if; A=1 echo B=2
#### heredoc-bini
cat <<A <<B | wc -l
one
A
two
B
#### substitutio-commentum
echo $(echo ")" # )
echo "x")
#### arith-subcrusta
echo $((1+1)); (echo $( (echo nested) ))
#### sublinguae
[[ abc =~ ^a.c$ ]]; case x in a|b) ;; esac; echo $'\t'; echo ${x:-d}
#### fd-glutinatio
exec 2>/dev/null; echo 2 >/dev/null; echo x2>/dev/null
#### gravis-nidus
echo `echo \`echo deep\``
#### extglob
shopt -s extglob; echo @(a|b)
#### alias
alias x='if true; then'; x echo hi; fi
#### processus-substitutio
diff <(a) >(b)
#### pipa-erroris
a |& b
#### redirectio-ambo
a &> f; b &>> g
#### fd-variabilis
exec {fd}>f; echo x >&$fd; exec {fd}>&-
#### optio-termini
case x in a) ;& b) ;;& c) ;; esac
#### heredoc-tabulae
cat <<-E
	one
		two
	E
#### herestring
cat <<< "a b"
#### regula
[[ a =~ ^a(b|c)$ ]] && [[ $x =~ a|b'c d'$v ]]
#### socius
coproc c { :; }; coproc cmd a b
#### electio-select
select x in a b; do echo $x; done
#### tempus
time -p ls | wc; ! true; time { a; }
#### cyclus
for ((i=0; i<3; i++)); do :; done; for ((;;)) do break; done
#### tabulata
a=(1 [2]=x) b+=(y) declare -a c=([0]=z)
#### versa
echo $"x" $'y\n'
#### crlf
echo a
if true; then
  echo b
fi
#### arith-ambiguum
echo $((1+1)); echo $((echo a); b)
#### heredoc-in-substitutione
echo $(cat <<A
x
A
); echo $(cat <<A)
y
A
#### continuatio
ab\
cd \
  ef
#### functio-formae
f() { :; }; function g { :; }; function h () ( : )
#### grex-sine-separatore
{ a }
#### si-apertum
if true; then
#### esac-solum
esac
#### parenthesis-vaga
a ) b
