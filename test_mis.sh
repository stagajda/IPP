#!/bin/bash

make

if [[ $# != 2 ]]
then
	echo "sposob uzycia: ./test_mis prog dir"
	exit
fi

for f in ./$2/*.in
do
	echo ""
	valgrind --log-fd=3 3>>./$2/test.log ./$1 <$f >${f%in}outtmp 2>${f%in}errtmp
	echo "Dla pliku $f program zakonczyl sie kodem $?."

	if ! diff -q ${f%in}out ${f%in}outtmp&>/dev/null;
		then echo "Standardowe wyjscie bledne"
		else echo "Standardowe wyjscie poprawne"
	fi

	if ! diff -q ${f%in}err ${f%in}errtmp&>/dev/null;
		then echo "Standardowe wyjscie bledne"
		else echo "Standardowe wyjscie poprawne"
	fi

	rm ${f%in}outtmp
	rm ${f%in}errtmp
	rm ./$2/test.log

done