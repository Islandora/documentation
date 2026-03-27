import mermaid from "https://unpkg.com/mermaid@11.13.0/dist/mermaid.esm.min.mjs";

mermaid.initialize({
  startOnLoad: false,
  securityLevel: "loose",
});

const LIGHTBOX_ID = "mermaid-lightbox";

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

const ensureLightbox = () => {
  let lightbox = document.getElementById(LIGHTBOX_ID);
  if (lightbox) {
    return lightbox;
  }

  lightbox = document.createElement("dialog");
  lightbox.id = LIGHTBOX_ID;
  lightbox.className = "mermaid-lightbox";
  lightbox.setAttribute("aria-label", "Expanded Mermaid diagram");

  const frame = document.createElement("div");
  frame.className = "mermaid-lightbox__frame";

  const closeButton = document.createElement("button");
  closeButton.type = "button";
  closeButton.className = "mermaid-lightbox__close";
  closeButton.setAttribute("aria-label", "Close expanded diagram");
  closeButton.textContent = "Close";
  closeButton.addEventListener("click", () => {
    lightbox.close();
  });

  const body = document.createElement("div");
  body.className = "mermaid-lightbox__body";

  frame.append(closeButton, body);
  lightbox.append(frame);

  lightbox.addEventListener("click", (event) => {
    if (event.target === lightbox) {
      lightbox.close();
    }
  });

  document.body.append(lightbox);
  return lightbox;
};

const openLightbox = (container) => {
  const svg = container.querySelector("svg");
  if (!svg) {
    return;
  }

  const lightbox = ensureLightbox();
  const body = lightbox.querySelector(".mermaid-lightbox__body");
  const clone = svg.cloneNode(true);

  clone.removeAttribute("width");
  clone.removeAttribute("height");
  clone.style.width = "100%";
  clone.style.height = "auto";

  body.replaceChildren(clone);

  if (!lightbox.open) {
    lightbox.showModal();
  }
};

const attachExpandBehavior = (container) => {
  if (container.dataset.mermaidExpandable === "true") {
    return;
  }
  container.dataset.mermaidExpandable = "true";
  container.classList.add("mermaid-expandable");
  container.tabIndex = 0;
  container.setAttribute("role", "button");
  container.setAttribute("aria-label", "Open Mermaid diagram in a larger view");
  container.title = "Click to enlarge diagram";
  container.__islandoraOpenLightbox = () => {
    openLightbox(container);
  };

  container.addEventListener("click", (event) => {
    if (event.target.closest(".mermaid-controls, button, a")) {
      return;
    }
    openLightbox(container);
  });

  container.addEventListener("keydown", (event) => {
    if (event.key !== "Enter" && event.key !== " ") {
      return;
    }
    event.preventDefault();
    openLightbox(container);
  });
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
    attachExpandBehavior(renderTarget);
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
