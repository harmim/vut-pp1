CO := 2020-PP1FIT-xharmi00


all: $(CO).pdf


$(CO).pdf: clean
	pdflatex $(CO)
	bibtex $(CO)
	pdflatex $(CO)
	pdflatex $(CO)


clean:
	rm -f *.dvi *.log $(CO).blg $(CO).bbl $(CO).toc *.aux $(CO).out \
		$(CO).lof $(CO).ptc $(CO).pdf *~


pack:
	tar czvf $(CO).tar.gz *.tex *.bib *.bst ./images/* $(CO).pdf Makefile


vlna:
	vlna -l *.tex


normostrany:
	echo "scale=2; `detex -n *.tex | wc -c`/1800;" | bc
