# Author: Dominik Harmim <harmim6@gmail.com>

CO := 2020-PP1FIT-xharmi00
PDF := $(CO).pdf
PACK := $(CO).tar.gz


.PHONY: all
all: $(PDF)

$(PDF): clean
	pdflatex $(CO)
	bibtex $(CO)
	pdflatex $(CO)
	pdflatex $(CO)


.PHONY: clean
clean:
	rm -f *.dvi *.log $(CO).blg $(CO).bbl $(CO).toc *.aux $(CO).out \
		$(CO).lof $(CO).ptc $(PDF) *~


.PHONY: pack
pack: $(PACK)

$(PACK): *.tex *.bib *.bst *.cls ./images/* $(PDF) Makefile
	COPYFILE_DISABLE=1 tar czvf $@ $^


.PHONY: vlna
vlna:
	vlna -l *.tex *.bib


.PHONY: normostrany
normostrany:
	echo "scale=2; `detex -n *.tex | wc -c`/1800;" | bc
