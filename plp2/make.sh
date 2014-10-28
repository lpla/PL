flex plp2.l
bison -d plp2.y
g++ -o plp2 plp2.tab.c lex.yy.c
./plp2 codigo.fnt > codigo.m2r
./m2r codigo.m2r
