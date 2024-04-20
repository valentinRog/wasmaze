Module.onRuntimeInitialized = () => {
  var init = Module.init;
  init(width, height);
  acc = 0;
  t = 0;
  function render() {
    if (t == 0) t = Date.now();
    acc++;
    if (Date.now() - t >= 1000) {
      console.log(`${acc}fps`);
      t = Date.now();
      acc = 0;
    }
    Module.render();
    requestAnimationFrame(render);
  }
  render();

  canvas.addEventListener("mousemove", (e) => {
    const rect = canvas.getBoundingClientRect();
    let x = e.clientX - rect.left;
    let y = e.clientY - rect.top;
    Module.handle_mouse(x, y);
  });
  canvas.addEventListener("mouseleave", () => Module.handle_mouse(0, 0));
};
