all: build

.PHONY: build
build:
	xelatex --enable-write18 _main.tex

.PHONY: git
git:
	git add .
	git commit -m "."

.PHONY: push
push: git
	git push -u origin

.PHONY: clean
clean:
	@rm -rf *.gnuplot *-fig*.pdf *.log *.aux *.bcf *.out *.xml *.toc w18-test-*.tex