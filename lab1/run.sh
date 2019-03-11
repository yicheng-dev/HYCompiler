flex ./Code/lexical.l
gcc ./lex.yy.c -ll -o scanner
./scanner ./Test/sample1.cmm
