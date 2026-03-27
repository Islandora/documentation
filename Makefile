.PHONY: docs docs-build build abbreviations preview

abbreviations:
	./scripts/generate-abbreviations.sh

docs-build:
	docker build -t islandora-docs .

build: docs-build
	rm -rf site
	docker run --rm \
		$(if $(SITE_URL),-e SITE_URL=$(SITE_URL)) \
		-v "$(CURDIR):/work" \
		-w /work \
		islandora-docs \
		build --clean

docs: docs-build
	docker run --rm -it \
		-p 8080:8080 \
		-v "$(CURDIR):/work" \
		-w /work \
		islandora-docs

preview:
	$(MAKE) build SITE_URL=http://localhost:8080
	docker run --rm -it \
		-p 8080:8080 \
		-v "$(CURDIR)/site:/site" \
		-w /site \
		--entrypoint python3 \
		islandora-docs \
		-m http.server 8080
