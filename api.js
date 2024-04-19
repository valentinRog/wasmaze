Module.onRuntimeInitialized = () => {
  var init = Module.init;
  init(width, height);
  Module.render();
};
