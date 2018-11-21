String portname = null;

void setup()
{
  size(300, 150);
  
}

void draw()
{
  background(555555);  
  fill(255);
  int y = 24, dy = 12;
  text("INSTRUÇÕES", 12, y); y += dy;
  text("p: selecionar porta serial", 20, y); y += dy;
  text("h: ponto inicial", 20, y); y += dy;
  text("g: abrir arquivo", 20, y); y += dy;
  text("x: parar desenho", 20, y); y += dy;
  y = height - dy;
  text("Porta Serial: " + portname, 12, y); y -= dy;
}
