import mermaid from "https://unpkg.com/mermaid@11.13.0/dist/mermaid.esm.min.mjs";

mermaid.initialize({
  startOnLoad: false,
  securityLevel: "loose",
});

const parseFlowSteps = (source) => {
  const flowSteps = {};
  const pattern = /^\s*class\s+([^;]+?)\s+flow(\d+)\s*;/gm;
  let match;

  while ((match = pattern.exec(source)) !== null) {
    const ids = match[1].split(",").map((id) => id.trim()).filter(Boolean);
    const step = parseInt(match[2], 10);
    ids.forEach((id) => {
      flowSteps[id] = step;
    });
  }

  return flowSteps;
};

const renderDiagram = async (container, index) => {
  if (container.dataset.mermaidRendered === "true") {
    return;
  }
  container.dataset.mermaidRendered = "true";

  const code = container.querySelector("code");
  const source = code ? code.textContent : container.textContent;
  if (!source || !source.trim()) {
    return;
  }

  const flowSteps = parseFlowSteps(source);
  const renderTarget = document.createElement("div");
  renderTarget.className = "mermaid";
  renderTarget.__islandoraFlowSteps = flowSteps;

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

// Required for Zensical/Material to find the mermaid instance.
window.mermaid = mermaid;
