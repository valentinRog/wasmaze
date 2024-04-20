function _get_rbg_string(color) {
  r = color >> 16;
  g = (color >> 8) & 0xff;
  b = color & 0xff;
  return `rgb(${r} ${g} ${b})`;
}

function draw_line(x0, y0, x1, y1, color) {
  ctx.strokeStyle = _get_rbg_string(color);
  ctx.beginPath();
  ctx.moveTo(x0, y0);
  ctx.lineTo(x1, y1);
  ctx.stroke();
}

function draw_rect(x, y, width, height, color) {
  ctx.fillStyle = _get_rbg_string(color);
  ctx.fillRect(x, y, width, height);
}
