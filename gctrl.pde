import java.awt.event.KeyEvent;

String portname = null;

boolean desenhando = false;
String[] gcode;
int i = 0;


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

void keyPressed()
{
    
  if (!desenhando) {
    if (key == 'h') port.write("G90\nG20\nG00 X0.000 Y0.000 Z0.000\n");
    if (key == 'p') portSerial();
    
  }
  
  if (!desenhando && key == 'g') {
    gcode = null; i = 0;
    File file = null; 
    println("Carregando arquivo...");
    selectInput("Selecione o arquivo:", "selectFile", file);
  }
  
  if (key == 'x') desenhando = false;
}

void selectFile(File selection) {
  if (selection == null) {
    println("A janela foi fechada ou a seleçao foi cancelada.");
  } else {
    println("Usuario selecionou " + selection.getAbsolutePath());
    gcode = loadStrings(selection.getAbsolutePath());
    if (gcode == null) return;
    desenhando = true;
  }
}
