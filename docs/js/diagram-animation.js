(() => {
  const timings = [
    [3000, 0],
    [3000, 1000],
    [3000, 2000],
    [3000, 3000],
    [3000, 4000],
  ];

  const reduceMotion = window.matchMedia("(prefers-reduced-motion: reduce)");

  const findEdges = () => Array.from(document.querySelectorAll(".mermaid svg path[data-edge=true]"));

  const stopAnimations = () => {
    findEdges().forEach((element) => {
      if (element.__islandoraDashAnimation) {
        element.__islandoraDashAnimation.cancel();
        element.__islandoraDashAnimation = null;
      }
      element.style.animation = "none";
      element.style.strokeDasharray = "9,5";
      element.style.strokeDashoffset = "0";
    });
  };

  const startAnimations = () => {
    findEdges().forEach((element, index) => {
      const [duration, delay] = timings[index % timings.length];

      if (element.__islandoraDashAnimation) {
        element.__islandoraDashAnimation.cancel();
      }

      element.style.animation = "none";
      element.style.strokeDasharray = "9,5";
      element.style.strokeDashoffset = "900";

      element.__islandoraDashAnimation = element.animate(
        [
          { strokeDashoffset: 900 },
          { strokeDashoffset: 0 },
        ],
        {
          duration,
          delay,
          easing: "linear",
          iterations: Infinity,
          fill: "forwards",
        },
      );
    });
  };

  const applyEdgeMotion = () => {
    if (reduceMotion.matches) {
      stopAnimations();
      return;
    }

    startAnimations();
  };

  const run = () => window.requestAnimationFrame(applyEdgeMotion);

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", run, { once: true });
  } else {
    run();
  }

  new MutationObserver(run).observe(document.body, { childList: true, subtree: true });

  if (typeof reduceMotion.addEventListener === "function") {
    reduceMotion.addEventListener("change", run);
  } else if (typeof reduceMotion.addListener === "function") {
    reduceMotion.addListener(run);
  }
})();
