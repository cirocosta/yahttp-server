PANDOC = pandoc
PANDOC_OPTS = -f markdown+yaml_metadata_block+definition_lists --toc
MDS = $(wildcard *.md)
PDFS = $(MDS:.md=.pdf)
HTMLS = $(MDS:.md=.html)

all: $(PDFS) $(HTMLS)

%.html: %.md
	$(PANDOC) $(PANDOC_OPTS) $< -o $@

%.pdf: %.md
	$(PANDOC) $(PANDOC_OPTS) $< -o $@

.PHONY: clean

clean:
	find . -name "*.pdf" -delete

