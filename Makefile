LATEXCMD = pdflatex -shell-escape -output-directory build/
export TEXINPUTS=.:content/tex/:
export max_print_line = 1048576

# Extract the job count from MAKEFLAGS
JOBS ?= $(shell echo $(MAKEFLAGS) | sed -n 's/.*-j\([0-9][0-9]*\).*/\1/p')
JOBS ?= 1  # Default to 1 if no -j flag with a number is found
export JOBS

help:
	@echo "This makefile builds KACTL++"
	@echo ""
	@echo "Available commands are:"
	@echo "	make fast		- to build KACTL++, quickly (only runs LaTeX once)"
	@echo "	make kactl		- to build KACTL++"
	@echo "	make snippets	- to build VSCode snippets"
	@echo "	make clean		- to clean up the build process"
	@echo "	make veryclean		- to clean up and remove kactl.pdf"
	@echo "	make test		- to run all the stress tests in stress-tests/"
	@echo "	make test-group-<group>	- to run all the stress tests in stress-tests/<group>"
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

.PHONY: help fast kactl clean veryclean format snippets test test-group-% test-compiles test-style test-test-problems test-stress-contents

build:
	mkdir -p build/

test:
	bash ./doc/scripts/checkInlineContents-stress-test.sh
	./doc/scripts/run-stress-tests.sh .

test-group-%:
	./doc/scripts/run-stress-tests.sh . $*

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

test-stress-contents:
	bash ./doc/scripts/checkInlineContents-stress-test.sh

showexcluded: build
	grep -RoPh '^\s*\\kactlimport(\[[^]]*\])?{\K.*' content/ | sed 's/.$$//' > build/headers_included
	find ./content -not -path "./content/tex/*" \( -name "*.h" -o -name "*.cpp" -o -name "*.py" -o -name "*.java" \) | grep -vFf build/headers_included

snippets:
	find content/ -type f -name "*.h" ! -path "*/tex/*" ! -path "*/contest/*" -print0 \
		| xargs -0 -n 1 -I{} sh -c \
			'python3 content/tex/preprocessor.py -s -i {} > /dev/null || exit 0'
	python3 content/tex/snippets.py
