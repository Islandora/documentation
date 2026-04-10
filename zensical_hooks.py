"""
Python-Markdown extension: fix glossary link depth and inject data-preview.

Background
----------
Zensical's LinksProcessor always prepends exactly one extra "../" to non-index
relative links when use_directory_urls=True.  Because includes/abbreviations.md
uses "../user-documentation/glossary.md#term" (one leading ".."), LinksProcessor
always emits "../../user-documentation/glossary/#term" in HTML regardless of
actual page depth:

  depth-2 page (something/page/):        ../../  -> root  OK
  depth-4 page (a/b/c/page/):            ../../  -> a/b/  WRONG

Separately, zensical.extensions.preview adds data-preview="" by resolving the
original .md href relative to the current source file.  For deeply nested pages
the relative "../user-documentation/glossary.md" doesn't resolve to
"user-documentation/glossary.md", so data-preview is never added.

Fix
---
Register a Treeprocessor at priority -1 (lower than the priority-0 processors
PreviewProcessor and LinksProcessor). It runs after both, reads the current
page's source path from Zensical's `zrelpath` processor, computes the correct
depth, rewrites every glossary href, and adds data-preview="" where absent.
"""

from __future__ import annotations

import re
from typing import TYPE_CHECKING

from markdown import Extension
from markdown.treeprocessors import Treeprocessor
from zensical.extensions.links import LinksProcessor

if TYPE_CHECKING:
    from xml.etree.ElementTree import Element

# Matches the relative prefix (any number of ../) before the glossary path
_GLOSSARY_RE = re.compile(r"^(?:\.\./)*user-documentation/glossary/")


def _url_depth(src_path: str) -> int:
    """Return URL depth with use_directory_urls=True.

    - index.md / README.md at directory depth D  ->  D
    - any other .md at directory depth D          ->  D + 1
    """
    parts = src_path.replace("\\", "/").split("/")
    is_index = parts[-1].lower() in ("index.md", "readme.md")
    return len(parts) - 1 if is_index else len(parts)


def _find_page_path(md) -> str | None:
    """Return the source path of the page being rendered.

    This repo ships a Docker-pinned Zensical version that always registers the
    links treeprocessor as ``zrelpath``.
    """
    try:
        idx = md.treeprocessors.get_index_for_name("zrelpath")
        proc = md.treeprocessors[idx]
        if isinstance(proc, LinksProcessor) and isinstance(proc.path, str):
            return proc.path
    except Exception:  # noqa: BLE001
        pass

    return None


class GlossaryFixProcessor(Treeprocessor):
    """Rewrite glossary hrefs to the correct depth and ensure data-preview."""

    def run(self, root: Element) -> None:
        try:
            path = _find_page_path(self.md)
            if path is None:
                return
            depth = _url_depth(path)
            prefix = "../" * depth
        except Exception:  # noqa: BLE001
            return

        for el in root.iter("a"):
            href = el.get("href", "")
            if "user-documentation/glossary/" not in href:
                continue

            new_href = _GLOSSARY_RE.sub(
                prefix + "user-documentation/glossary/", href
            )
            if new_href != href:
                el.set("href", new_href)

            if "data-preview" not in el.attrib:
                el.set("data-preview", "")


class GlossaryFixExtension(Extension):
    def extendMarkdown(self, md) -> None:  # noqa: N802
        md.treeprocessors.register(GlossaryFixProcessor(md), "glossary_fix", -1)


def makeExtension(**kwargs):  # noqa: N802
    return GlossaryFixExtension(**kwargs)
