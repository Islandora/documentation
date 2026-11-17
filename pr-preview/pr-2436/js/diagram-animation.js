/**
 * Sequential flow animation for Mermaid diagrams.
 */
(() => {
  const STEP_DURATION = 1500;
  const STEP_GAP = 150;
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
    const escaped = window.CSS && window.CSS.escape ? window.CSS.escape(edgeId) : edgeId;
    return '[id="' + escaped + '"], [id$="-' + escaped + '"]';
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

  const ensureAnimatedPath = (edgePath) => {
    if (edgePath.__islandoraAnimatedPath && edgePath.__islandoraAnimatedPath.isConnected) {
      return edgePath.__islandoraAnimatedPath;
    }

    const animatedPath = edgePath.cloneNode(false);
    const computed = window.getComputedStyle(edgePath);
    const strokeWidth = parseFloat(computed.strokeWidth || edgePath.getAttribute("stroke-width") || "2") || 2;

    animatedPath.removeAttribute("id");
    animatedPath.removeAttribute("marker-start");
    animatedPath.removeAttribute("marker-end");
    animatedPath.setAttribute("aria-hidden", "true");
    animatedPath.style.fill = "none";
    animatedPath.style.opacity = "0";
    animatedPath.style.pointerEvents = "none";
    animatedPath.style.stroke = computed.stroke;
    animatedPath.style.strokeWidth = String(Math.max(strokeWidth * 1.75, strokeWidth + 1));
    animatedPath.style.strokeLinecap = "round";
    animatedPath.style.strokeLinejoin = "round";
    animatedPath.style.strokeDasharray = "0 1";
    animatedPath.style.strokeDashoffset = "0";

    edgePath.parentNode && edgePath.parentNode.appendChild(animatedPath);
    edgePath.__islandoraAnimatedPath = animatedPath;
    return animatedPath;
  };

  const attachAnimationListeners = (container, syncUI) => {
    (container.__islandoraAnimations || []).forEach((animation) => {
      if (animation.__islandoraSyncAttached) {
        return;
      }
      animation.addEventListener("finish", syncUI);
      animation.addEventListener("cancel", syncUI);
      animation.__islandoraSyncAttached = true;
    });
  };

  const animateContainer = (container) => {
    const steppedEdges = getSteppedEdges(container);
    if (steppedEdges.length === 0) return false;

    const maxStep = Math.max(...steppedEdges.map(({ step }) => step));
    const totalCycle = (maxStep + 1) * STEP_DURATION + maxStep * STEP_GAP;

    container.__islandoraAnimations = steppedEdges.map(({ el, step }) => {
      const animatedPath = ensureAnimatedPath(el);
      const pathLength = Math.max(el.getTotalLength(), 1);
      const segmentLength = Math.max(Math.min(pathLength * 0.35, 120), 18);
      const hiddenOffset = pathLength + segmentLength;
      const visibleDash = String(segmentLength) + ' ' + String(pathLength);
      const stepDelay = step * (STEP_DURATION + STEP_GAP);
      const startOffset = Math.min(stepDelay / totalCycle, 1);
      const finishOffset = Math.min((stepDelay + STEP_DURATION) / totalCycle, 1);
      const hideOffset = Math.min(finishOffset + 0.0005, 1);

      animatedPath.__islandoraDashAnimation && animatedPath.__islandoraDashAnimation.cancel();
      animatedPath.style.strokeDasharray = visibleDash;
      animatedPath.style.strokeDashoffset = String(hiddenOffset);
      animatedPath.style.opacity = '0';

      const animation = animatedPath.animate(
        [
          { strokeDashoffset: hiddenOffset, opacity: 0, offset: 0 },
          { strokeDashoffset: hiddenOffset, opacity: 0, offset: startOffset },
          { strokeDashoffset: hiddenOffset, opacity: 1, offset: startOffset },
          { strokeDashoffset: segmentLength * 2, opacity: 1, offset: finishOffset },
          { strokeDashoffset: segmentLength * 2, opacity: 0, offset: hideOffset },
          { strokeDashoffset: hiddenOffset, opacity: 0, offset: 1 },
        ],
        {
          duration: totalCycle,
          easing: 'linear',
          fill: 'forwards',
          iterations: Infinity,
        }
      );
      animatedPath.__islandoraDashAnimation = animation;
      return animation;
    });

    return true;
  };

  const stopContainer = (container) => {
    container.querySelectorAll(EDGE_SELECTOR).forEach((el) => {
      if (el.__islandoraAnimatedPath && el.__islandoraAnimatedPath.__islandoraDashAnimation) {
        el.__islandoraAnimatedPath.__islandoraDashAnimation.cancel();
      }
      el.__islandoraAnimatedPath && el.__islandoraAnimatedPath.remove();
      el.__islandoraAnimatedPath = null;
    });
    container.__islandoraAnimations = null;
    delete container.dataset.islandoraAnimated;
  };

  const injectControls = (container) => {
    container.querySelector('.mermaid-controls') && container.querySelector('.mermaid-controls').remove();

    const controls = document.createElement('div');
    controls.className = 'mermaid-controls';
    controls.setAttribute('role', 'group');
    controls.setAttribute('aria-label', 'Diagram animation controls');

    const makeBtn = (text, label) => {
      const btn = document.createElement('button');
      btn.className = 'mermaid-btn';
      btn.type = 'button';
      btn.textContent = text;
      btn.setAttribute('aria-label', label);
      return btn;
    };

    const playBtn = makeBtn('▶', 'Play animation');
    const pauseBtn = makeBtn('⏸', 'Pause animation');
    const restartBtn = makeBtn('↺', 'Restart animation');
    const enlargeBtn = makeBtn('+', 'Enlarge diagram');

    const syncUI = () => {
      const activeAnimations = container.__islandoraAnimations || [];
      const running = activeAnimations.some((animation) => animation.playState === 'running');
      playBtn.disabled = running || activeAnimations.length === 0;
      pauseBtn.disabled = !running;
    };

    playBtn.addEventListener('click', () => {
      (container.__islandoraAnimations || []).forEach((animation) => animation.play());
      syncUI();
    });

    pauseBtn.addEventListener('click', () => {
      (container.__islandoraAnimations || []).forEach((animation) => animation.pause());
      syncUI();
    });

    restartBtn.addEventListener('click', () => {
      animateContainer(container);
      attachAnimationListeners(container, syncUI);
      (container.__islandoraAnimations || []).forEach((animation) => {
        animation.currentTime = 0;
        animation.play();
      });
      syncUI();
    });

    enlargeBtn.addEventListener('click', () => {
      container.__islandoraOpenLightbox && container.__islandoraOpenLightbox();
    });

    attachAnimationListeners(container, syncUI);
    syncUI();
    controls.append(playBtn, pauseBtn, restartBtn, enlargeBtn);
    container.prepend(controls);
  };

  const activateContainer = (container) => {
    if (reduceMotion.matches) return;
    if (container.dataset.islandoraAnimated === 'true') return;

    const hasSteps = animateContainer(container);
    if (!hasSteps) return;

    container.dataset.islandoraAnimated = 'true';
    if (!container.querySelector('.mermaid-controls')) {
      injectControls(container);
    }
  };

  document.addEventListener('mermaid:rendered', ({ detail: container }) => {
    activateContainer(container);
  });

  const onMotionChange = () => {
    document.querySelectorAll('.mermaid').forEach((container) => {
      if (reduceMotion.matches) {
        stopContainer(container);
        container.querySelector('.mermaid-controls') && container.querySelector('.mermaid-controls').remove();
      } else {
        activateContainer(container);
      }
    });
  };

  const observeMermaidContainers = (root) => {
    if (!(root instanceof Element || root instanceof Document)) return;
    if (root instanceof Element && root.matches('.mermaid')) {
      activateContainer(root);
    }
    root.querySelectorAll && root.querySelectorAll('.mermaid').forEach((container) => {
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

  if (typeof reduceMotion.addEventListener === 'function') {
    reduceMotion.addEventListener('change', onMotionChange);
  } else if (typeof reduceMotion.addListener === 'function') {
    reduceMotion.addListener(onMotionChange);
  }
})();
