DOC_CLASSES := $(abspath ../doc_classes)
GODOT_DOCS_SOURCE ?= $(or $(firstword $(wildcard $(HOME)/cvs/master-godot/doc/classes $(HOME)/cvs/master-godot/editor/doc/classes)),$(HOME)/cvs/master-godot/doc/classes)
GODOT_SOURCE_ROOT := $(patsubst %/editor/doc/classes,%,$(patsubst %/doc/classes,%,$(GODOT_DOCS_SOURCE)))
RST_OUTPUT ?= docs
HTML_OUTPUT ?= ../docs

run:
	@if [ ! -d "$(GODOT_DOCS_SOURCE)" ] || [ ! -d "$(GODOT_SOURCE_ROOT)/misc" ]; then \
		echo "Skipping docs generation: Godot sources not found at $(GODOT_DOCS_SOURCE)"; \
		rm -rf "$(RST_OUTPUT)"; \
		mkdir -p "$(RST_OUTPUT)"; \
		printf '%s\n' \
			'GodotApplePlugins Documentation' \
			'===============================' \
			'' \
			'Documentation generation was skipped because the upstream Godot sources were not available at the expected path.' \
			'' \
			'Expected docs path:' \
			'' \
			'- ``$(GODOT_DOCS_SOURCE)``' \
			> "$(RST_OUTPUT)/index.rst"; \
	else \
		rm -rf "$(RST_OUTPUT)"; \
		GODOT_SOURCE_ROOT="$(GODOT_SOURCE_ROOT)" \
		python3 make_rst.py -o "$(RST_OUTPUT)" -l "en" --filter "$(DOC_CLASSES)" --allow-warnings "$(GODOT_DOCS_SOURCE)" "$(DOC_CLASSES)"; \
	fi

html: run
	sphinx-build -b html -c . "$(RST_OUTPUT)" "$(HTML_OUTPUT)"
