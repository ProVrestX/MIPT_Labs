all: build

.PHONY: build
build:
	@mkdir -p tmp
	xelatex -shell-escape _main.tex

.PHONY: git
git:
	git add .
	git commit -m "."

.PHONY: push
push: git
	git push -u origin

.PHONY: bib
bib: 
	biber _main

.PHONY: clean
clean:
	@rm -rf tmp
	@rm -rf *.gnuplot *-fig*.pdf *.log *.aux *.bcf *.out *.xml *.toc w18-test-*.tex
