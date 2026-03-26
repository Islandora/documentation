/**
 * Sequential flow animation for Mermaid diagrams.
 *
 * Listens for the `mermaid:rendered` custom event dispatched by mermaid-init.mjs
 * and animates any edges assigned to flow steps such as `flow0`, `flow1`, etc.
 *
 * Author diagrams by assigning step classes to edge IDs:
 *   class e1 flow0;
 *   class e2 flow1;
 */
(() => {
  const STEP_DURATION = 1500;
  const PAUSE_DURATION = 100;
  const EDGE_SELECTOR = "path";

  const reduceMotion = window.matchMedia("(prefers-reduced-motion: reduce)");

  const getStep = (element) => {
    let el = element;
    while (el) {
      for (const cls of el.classList) {
        const match = cls.match(/^flow(\d+)$/);
        if (match) return parseInt(match[1], 10);
      }
      el = el.parentElement;
    }
    return null;
  };

  const getEdgeSelector = (edgeId) => {
    const escaped = window.CSS?.escape ? window.CSS.escape(edgeId) : edgeId;
    return `[id="${escaped}"], [id$="-${escaped}"]`;
  };

  const getSteppedEdges = (container) => {
    const flowSteps = container.__islandoraFlowSteps || {};
    const mappedEdges = Object.entries(flowSteps)
      .map(([edgeId, step]) => {
        const el = container.querySelector(getEdgeSelector(edgeId));
        return el ? { el, step } : null;
      })
      .filter(Boolean);

    if (mappedEdges.length > 0) {
      return mappedEdges;
    }

    return Array.from(new Set(container.querySelectorAll(EDGE_SELECTOR)))
      .map((el) => ({ el, step: getStep(el) }))
      .filter(({ step }) => step !== null);
  };

  const animateContainer = (container) => {
    const steppedEdges = getSteppedEdges(container);
    if (steppedEdges.length === 0) return false;

    const maxStep = Math.max(...steppedEdges.map(({ step }) => step));
    const totalCycle = (maxStep + 1) * STEP_DURATION + PAUSE_DURATION;
    const activeRatio = STEP_DURATION / totalCycle;

    container.__islandoraAnimations = steppedEdges.map(({ el, step }) => {
      el.__islandoraDashAnimation?.cancel();
      el.style.animation = "none";
      el.style.strokeDasharray = "9,5";
      el.style.strokeDashoffset = "900";

      const anim = el.animate(
        [
          { strokeDashoffset: 900, offset: 0 },
          { strokeDashoffset: -100, offset: activeRatio },
          { strokeDashoffset: 900, offset: Math.min(activeRatio + 0.001, 1) },
          { strokeDashoffset: 900, offset: 1 },
        ],
        {
          duration: totalCycle,
          delay: step * STEP_DURATION,
          easing: "linear",
          iterations: Infinity,
        }
      );

      el.__islandoraDashAnimation = anim;
      return anim;
    });

    return true;
  };

  const stopContainer = (container) => {
    container.querySelectorAll(EDGE_SELECTOR).forEach((el) => {
      el.__islandoraDashAnimation?.cancel();
      el.__islandoraDashAnimation = null;
      el.style.animation = "";
      el.style.strokeDasharray = "";
      el.style.strokeDashoffset = "";
    });
    container.__islandoraAnimations = null;
  };

  const injectControls = (container) => {
    container.querySelector(".mermaid-controls")?.remove();

    const controls = document.createElement("div");
    controls.className = "mermaid-controls";
    controls.setAttribute("role", "group");
    controls.setAttribute("aria-label", "Diagram animation controls");

    let playing = true;

    const makeBtn = (text, label) => {
      const btn = document.createElement("button");
      btn.className = "mermaid-btn";
      btn.type = "button";
      btn.textContent = text;
      btn.setAttribute("aria-label", label);
      return btn;
    };

    const playBtn    = makeBtn("▶", "Play animation");
    const pauseBtn   = makeBtn("⏸", "Pause animation");
    const restartBtn = makeBtn("↺", "Restart animation");
    const syncUI = () => {
      playBtn.disabled = playing;
      pauseBtn.disabled = !playing;
    };

    playBtn.addEventListener("click", () => {
      playing = true;
      syncUI();
      container.__islandoraAnimations?.forEach((animation) => animation.play());
    });

    pauseBtn.addEventListener("click", () => {
      playing = false;
      syncUI();
      container.__islandoraAnimations?.forEach((animation) => animation.pause());
    });

    restartBtn.addEventListener("click", () => {
      playing = true;
      syncUI();
      animateContainer(container);
    });

    syncUI();
    controls.append(playBtn, pauseBtn, restartBtn);
    container.prepend(controls);
  };

  const activateContainer = (container) => {
    if (reduceMotion.matches) return;
    const hasSteps = animateContainer(container);
    if (hasSteps && !container.querySelector(".mermaid-controls")) {
      injectControls(container);
    }
  };

  document.addEventListener("mermaid:rendered", ({ detail: container }) => {
    activateContainer(container);
  });

  const onMotionChange = () => {
    document.querySelectorAll(".mermaid").forEach((container) => {
      if (reduceMotion.matches) {
        stopContainer(container);
        container.querySelector(".mermaid-controls")?.remove();
      } else {
        activateContainer(container);
      }
    });
  };

  const observeMermaidContainers = (root) => {
    if (!(root instanceof Element || root instanceof Document)) return;
    if (root instanceof Element && root.matches(".mermaid")) {
      activateContainer(root);
    }
    root.querySelectorAll?.(".mermaid").forEach((container) => {
      activateContainer(container);
    });
  };

  observeMermaidContainers(document);

  const observer = new MutationObserver((records) => {
    records.forEach((record) => {
      record.addedNodes.forEach((node) => {
        observeMermaidContainers(node);
      });
    });
  });

  if (document.body) {
    observer.observe(document.body, { childList: true, subtree: true });
  }

  if (typeof reduceMotion.addEventListener === "function") {
    reduceMotion.addEventListener("change", onMotionChange);
  } else if (typeof reduceMotion.addListener === "function") {
    reduceMotion.addListener(onMotionChange);
  }
})();
