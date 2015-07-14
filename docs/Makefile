PANDOC = pandoc
PANDOC_OPTS = -f --toc markdown+yaml_metadata_block+definition_lists
MDS = $(wildcard *.md)
PDFS = $(MDS:.md=.pdf)
HTMLS = $(MDS:.md=.html)

all: $(PDFS) $(HTMLS)

%.html: %.md
	$(PANDOC) -s -S -c style.css $(PANDOC_OPTS)+auto_identifiers $< -o $@

%.pdf: %.md
	$(PANDOC) $(PANDOC_OPTS) $< -o $@

.PHONY: clean

clean:
	find . -name "*.pdf" -delete

