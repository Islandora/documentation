.PHONY: docs docs-build

docs-build:
	docker build -t islandora-docs .

docs: docs-build
	docker run --rm -it \
		-p 8080:8080 \
		-v "$(CURDIR):/work" \
		-w /work \
		islandora-docs
