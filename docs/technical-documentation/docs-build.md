# Introduction

This documentation is built using [mkdocs](http://www.mkdocs.org/), a static site generator that is geared towards building project documentation. The documentation is created in the [Markdown](http://en.wikipedia.org/wiki/Markdown) format, and it all resides in the [`docs`](https://github.com/Islandora-CLAW/CLAW/tree/master/docs) directory in the repository. The organization of the documenation is controlled by the [`mkdocs.yml`](https://github.com/Islandora-CLAW/CLAW/blob/master/mkdocs.yml) in the root of the repository.

## Build and Deploy documentation

Documentation is build by running to the following command in the root of the repository:

`mkdocs build --clean`

This command will create a static `site` folder in the root of the repository.

You can preview any changes you have made to the documentation by running the following command:

`mkdocs serve`

And then visiting http://localhost:8111 in your browser.

To deploy documenation to GitHub Pages, issue the following command:

`mkdocs gh-deploy --clean`
