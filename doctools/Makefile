DOC_CLASSES := $(abspath ../doc_classes)
GODOT_DOCS_SOURCE ?= $(or $(firstword $(wildcard $(HOME)/cvs/master-godot/doc/classes $(HOME)/cvs/master-godot/editor/doc/classes)),$(HOME)/cvs/master-godot/doc/classes)
RST_OUTPUT ?= docs
HTML_OUTPUT ?= ../docs

run:
	python3 make_rst.py -o "$(RST_OUTPUT)" -l "en" --filter "$(DOC_CLASSES)" --allow-warnings "$(GODOT_DOCS_SOURCE)" "$(DOC_CLASSES)"

html: run
	sphinx-build -b html -c . "$(RST_OUTPUT)" "$(HTML_OUTPUT)"
