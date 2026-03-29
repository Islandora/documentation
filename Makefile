.PHONY: docker-build build abbreviations help preview serve

help: ## Show this help message
	echo 'Usage: make [target]'
	echo ''
	echo 'Available targets:'
	awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z_-]+:.*?## / {printf "  \033[36m%s\033[0m\t%s\n", $$1, $$2}' $(MAKEFILE_LIST) | sort | column -t -s $$'\t'

abbreviations: ## Parse ./docs/user-documentation/glossary.md and create Zensical Snippets other docs can reference
	./scripts/generate-abbreviations.sh

docker-build: ## Build the Zensical python application
	docker build -t islandora-docs .

build: docker-build ## Run zensical build --clean
	rm -rf site
	docker run --rm \
		$(if $(SITE_URL),-e SITE_URL=$(SITE_URL)) \
		-v "$(CURDIR):/work" \
		-w /work \
		islandora-docs \
		build --clean

serve: docker-build ## Run zensical serve to make the docs available at http://localhost:8080. You can then make live updates to the docs.
	docker run --rm -it \
		-p 8080:8080 \
		-v "$(CURDIR):/work" \
		-w /work \
		islandora-docs

preview: ## Make the contents from zensical build --clean available at http://localhost:8080. Use for troubleshooting GitHuub Pages issues.
	$(MAKE) build SITE_URL=http://localhost:8080
	docker run --rm -it \
		-p 8080:8080 \
		-v "$(CURDIR)/site:/site" \
		-w /site \
		--entrypoint python3 \
		islandora-docs \
		-m http.server 8080
