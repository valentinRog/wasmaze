function draw_line(x0, y0, x1, y1) {
  const ctx = canvas.getContext("2d");
  ctx.beginPath();
  ctx.moveTo(x0, y0);
  ctx.lineTo(x1, y1);
  ctx.stroke();
}

function draw_rect(x, y, width, height) {
  const ctx = canvas.getContext("2d");
  ctx.fillRect(x, y, width, height);
}
