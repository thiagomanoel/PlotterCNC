import java.awt.event.KeyEvent;
import javax.swing.JOptionPane;
import processing.serial.*;

Serial port = null;

String portname = null;

boolean desenhando = false;
String[] gcode;
int i = 0;

void abrirPortaSerial()
{
  if (portname == null) return;
  if (port != null) port.stop();
  
  port = new Serial(this, portname, 9600);
  
  port.bufferUntil('\n');

}


void selecionarPortaSerial()
{
   String result = (String) JOptionPane.showInputDialog(frame,
    "Selecione a porta serial correspondente ao seu Arduino.",
    "Selecione a porta serial",
    JOptionPane.QUESTION_MESSAGE,
    null,
    Serial.list(),
    0);
    
  if (result != null) {
    portname = result;
    abrirPortaSerial();
  }
}

void setup()
{
  size(300, 150);
  abrirPortaSerial();
}

void draw()
{
  background(555555);  
  fill(255);
  int y = 24, dy = 12;
  text("INSTRUÇÕES", 12, y); y += dy;
  text("p: selecionar porta serial", 20, y); y += dy;
  text("g: abrir arquivo", 20, y); y += dy;
  text("x: parar desenho", 20, y); y += dy;
  y = height - dy;
  text("Porta Serial: " + portname, 12, y); y -= dy;
}

void keyPressed()
{
    
  if (!desenhando) {
    if (key == 'p') selecionarPortaSerial();
    
  }
  
  if (!desenhando && key == 'g') {
    gcode = null; i = 0;
    File file = null; 
    println("Carregando arquivo...");
    selectInput("Selecione o arquivo:", "selectFile", file);
  }
  
  if (key == 'x') {
   desenhando = false;
   port.write("M300 S50.00 \n");
   port.write("G90\nG20\nG00 X0.000 Y0.000 Z0.000\n");
}
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

void desenho()
{
  if (!desenhando) return;
  
  while (true) {
    if (i == gcode.length) {
      desenhando = false;
      return;
    }
    
    if (gcode[i].trim().length() == 0) i++;
    else break;
  }
  
  println(gcode[i]);
  port.write(gcode[i] + '\n');
  i++;
}

void serialEvent(Serial p)
{
  String s = p.readStringUntil('\n');
  println(s.trim());
  
  if (s.trim().startsWith("ok")) desenho();
  if (s.trim().startsWith("erro")) desenho(); 
}
