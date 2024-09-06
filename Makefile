LATEXCMD = pdflatex -shell-escape -output-directory build/
export TEXINPUTS=.:content/tex/:
export max_print_line = 1048576

help:
	@echo "This makefile builds KACTL (KTH Algorithm Competition Template Library)"
	@echo ""
	@echo "Available commands are:"
	@echo "	make fast		- to build KACTL, quickly (only runs LaTeX once)"
	@echo "	make kactl		- to build KACTL"
	@echo "	make snippets	- to build VSCode snippets"
	@echo "	make clean		- to clean up the build process"
	@echo "	make veryclean		- to clean up and remove kactl.pdf"
	@echo "	make test		- to run all the stress tests in stress-tests/"
	@echo "	make test-compiles	- to test compiling all headers"
	@echo "	make help		- to show this information"
	@echo "	make showexcluded	- to show files that are not included in the doc"
	@echo ""
	@echo "For more information see the file 'doc/README'"

fast: | build
	$(LATEXCMD) content/kactl.tex </dev/null
	cp build/kactl.pdf kactl.pdf

kactl: test-session.pdf | build
	$(LATEXCMD) content/kactl.tex && $(LATEXCMD) content/kactl.tex
	cp build/kactl.pdf kactl.pdf

clean:
	cd build && rm -f kactl.aux kactl.log kactl.tmp kactl.toc kactl.pdf kactl.ptc

veryclean: clean
	rm -f kactl.pdf test-session.pdf
	rm -rf snippets

.PHONY: help fast kactl clean veryclean format snippets

build:
	mkdir -p build/

test:
	./doc/scripts/run-all.sh .
	bash ./doc/scripts/checkInlineContents-stress-test.sh

test-compiles:
	./doc/scripts/compile-all.sh .

test-session.pdf: content/test-session/test-session.tex content/test-session/chapter.tex | build
	$(LATEXCMD) content/test-session/test-session.tex
	cp build/test-session.pdf test-session.pdf

test-style:
	python3 doc/scripts/styleCheck.py --path=./content --tabs --lineLength 63
	python3 doc/scripts/styleCheck.py --path=./stress-tests --tabs
	python3 doc/scripts/styleCheck.py --path=./test-problems --tabs

test-test-problems:
	bash ./doc/scripts/checkAllTestProblems.sh

showexcluded: build
	grep -RoPh '^\s*\\kactlimport{\K.*' content/ | sed 's/.$$//' > build/headers_included
	find ./content -name "*.h" -o -name "*.py" -o -name "*.java" | grep -vFf build/headers_included

snippets:
	find content/ -type f -name "*.*" ! -name "*.tex" ! -name "*.txt" ! -name "*.pdf" ! -path "*/tex/*" ! -path "*/contest/*" -print0 \
		| xargs -0 -n 1 -I{} sh -c \
			'python3 content/tex/preprocessor.py -s -i {} > /dev/null || exit 0'
	python3 content/tex/snippets.py
