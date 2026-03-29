# Introduction

This documentation is built using [Zensical](https://zensical.org/docs/get-started/), a static site generator that is geared towards building project documentation. The documentation is created in the [Markdown](http://en.wikipedia.org/wiki/Markdown) format. All of the documentation resides in the [`docs`](https://github.com/Islandora/documentation/tree/main/docs) directory in the repository. The organization of the documentation is controlled by the [`mkdocs.yml`](https://github.com/Islandora/documentation/blob/main/mkdocs.yml) in the root of the repository.

## Prerequisites

On your local machine you will need to have

- [Docker](https://docs.docker.com/engine/install/) 
- `git`
- `make`

If you're an Islandora committer or have access to edit Islandora Documentation, you can checkout [https://github.com/Islandora/documentation](https://github.com/Islandora/documentation)

```
git clone git@github.com:Islandora/documentation
cd documentation
git checkout -B "$(whoami)-patch-1"
```

If you can not make edits to the documentation you will need to fork [https://github.com/Islandora/documentation](https://github.com/Islandora/documentation) and run a similar command as above using your fork repo instead of `git@github.com:Islandora/documentation`

## Make edits to the documentation locally

You can view the docs at [http://localhost:8080](http://localhost:8080) by running

```
make serve
```

This will create a live preview of Islandora's docs. While `make serve` is running you can then make edits to the markdown files and your web browser will automatically reload when you save the file.

To stop the `make serve` command just type the key combination "Control-c".

### Glossary and Snippets

Islandora's docs leverage [Zensical's glossary](https://zensical.org/docs/authoring/tooltips/#add-a-glossary) feature.

> The Snippets extension can be used to implement a simple glossary by moving all abbreviations in a dedicated file, and auto-append this file to all pages.

Our docs are configured so any term in [Islandora's Glossary](https://islandora.github.io/documentation/user-documentation/glossary/) can be referenced in any markdown file by wrapping the term in brackets `[]`. Adding brackets to a term will add a rich snippet for the term when someone hovers their mouse over the word. A snippet provides the definition of a term without needing to look it up in the glossary.

If you a create a new term in `glossary.md` you can run the following `make` helper to get the snippets to work properly. Namely, it populates `./includes/abbreviations.md` with the correct markdown to the glossary terms so docs can reference them. This also runs in GitHub Actions before deploying incase this step is forgotten by a docs editor.

```
make abbreviations
```

Only add terms to the glossary if they will be referenced by other documentation pages. If you just want a couple snippets on a particular docs page and they aren't generally useful you can define snippets on individual markdown pages. 

Once you're done with your edits, put up a PR and a preview site will be created to show your changes. Make sure the changes are the same as your local build was showing you. If something doesn't look right, you can debug with the steps below.

## Debug GitHub Pages Preview

Islandora's documentation is deployed to GitHub Pages using GitHub Actions. When you create a PR on [https://github.com/Islandora/documentation](https://github.com/Islandora/documentation) a preview site of your changes will be made available by an automated comment on the PR.

If something in GitHub Pages doesn't look the same as it did on your local machine, you can use the steps below to help troubleshoot:

```
make preview
```

This command will create a static `site` folder in the root of the repository. That set of HTML files is what GitHub Pages shows. The HTML will be available at [http://localhost:8080](http://localhost:8080).

`make preview` won't have live edits like `make serve` does but you can use this to more closely replicate GitHub Pages' infrastructure and troubleshoot bugs or subtle differences between zensical's `serve` and `build` commands that may be undocumented or unknown.

To stop the `make preview` command just type the key combination "Control-c".
