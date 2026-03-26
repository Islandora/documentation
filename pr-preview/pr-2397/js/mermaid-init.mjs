import mermaid from "https://cdn.jsdelivr.net/npm/mermaid@11/dist/mermaid.esm.min.mjs";
import elkLayouts from "https://cdn.jsdelivr.net/npm/@mermaid-js/layout-elk@0/dist/mermaid-layout-elk.esm.min.mjs";

mermaid.registerLayoutLoaders(elkLayouts);
mermaid.initialize({
  startOnLoad: false,
  securityLevel: "loose",
  layout: "elk",
});

const renderDiagram = async (container, index) => {
  if (container.dataset.mermaidRendered === "true") {
    return;
  }

  const code = container.querySelector("code");
  const source = code ? code.textContent : container.textContent;
  if (!source || !source.trim()) {
    return;
  }

  const renderTarget = document.createElement("div");
  renderTarget.className = "mermaid";

  try {
    const result = await mermaid.render(`islandora-mermaid-${index}`, source);
    renderTarget.innerHTML = result.svg;
    container.replaceWith(renderTarget);
    document.dispatchEvent(new CustomEvent("mermaid:rendered", { detail: renderTarget }));
  } catch (error) {
    console.error("Mermaid render failed", error);
  }
};

const renderAll = () => {
  document.querySelectorAll("pre.mermaid").forEach((container, index) => {
    void renderDiagram(container, index);
  });
};

if (document.readyState === "loading") {
  document.addEventListener("DOMContentLoaded", renderAll, { once: true });
} else {
  renderAll();
}
