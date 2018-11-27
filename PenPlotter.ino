#define LINE_BUFFER_LENGTH 512

struct ponto { 
  float x; 
  float y; 
  float z; 
};

struct ponto posAtuador;

void setup(){}


void loop() 
{
  
  delay(100);
  char line[ LINE_BUFFER_LENGTH ];
  char c;
  int lineIndex;
  bool lineComentario, linePontoEVirgula;

  lineIndex = 0;
  linePontoEVirgula = false;
  lineComentario = false;

 //receber Gcode
  while (1) {

    while ( Serial.available()>0 ) {
      c = Serial.read();
      if (( c == '\n') || (c == '\r') ) {             // fim da linha
        if ( lineIndex > 0 ) {                       
          line[ lineIndex ] = '\0';                   // fim da string
          processarLine( line, lineIndex );
          lineIndex = 0;
        } 
        else { 
          // vazio
        }
        lineComentario = false;
        linePontoEVirgula = false;
        Serial.println("ok");    
      } 
      else {
        if ( (lineComentario) || (linePontoEVirgula) ) {   // dispensar todos caracteres de comentarios
          if ( c == ')' )  lineComentario = false;     // fim do comentario.
        } 
        else {
          if ( c <= ' ' ) {                           // dispensa caracteres em branco
          } 
          else if ( c == '/' ) {                    // ingorar caractere
          } 
          else if ( c == '(' ) {                    // habilitar comentarios e ignorar caracteres ate ')'
            lineComentario = true;
          } 
          else if ( c == ';' ) {
            linePontoEVirgula = true;
          } 
          else if ( lineIndex >= LINE_BUFFER_LENGTH-1 ) {
            Serial.println( "ERRO - Limite do buffer ultrapassado" );
            lineComentario = false;
            linePontoEVirgula = false;
          } 
          else if ( c >= 'a' && c <= 'z' ) {        // transforma minusculas em maiusculas
            line[ lineIndex++ ] = c-'a'+'A';
          } 
          else {
            line[ lineIndex++ ] = c;
          }
        }
      }
    }
  }
}

void processarLine( char* line, int endLine ) {
  int currentIndex = 0;
  char buffer[ 64 ];                                
  struct ponto novaPos;

  novaPos.x = 0.0;
  novaPos.y = 0.0;

  while( currentIndex < endLine ) {
    switch ( line[ currentIndex++ ] ) {              // seleciona o comando
    case 'U':
      penSobe(); 
      break;
    case 'D':
      penDesce(); 
      break;
    case 'G':
      buffer[0] = line[ currentIndex++ ];          
      //      buffer[1] = line[ currentIndex++ ];
      //      buffer[2] = '\0';
      buffer[1] = '\0';

      switch ( atoi( buffer ) ){                   // selecionar comando G
      case 0:                                   // G00 - movimento rápido. G01 - movimento coordenado
      case 1:
        char* indexX = strchr( line+currentIndex, 'X' );  // obtem posiçao X/Y na string, se houver
        char* indexY = strchr( line+currentIndex, 'Y' );
        if ( indexY <= 0 ) {
          novaPos.x = atof( indexX + 1);  //pega coordenada X e guarga na nova posiçao
          novaPos.y = posAtuador.y;       //se n encontrar Y, posiçao nova é igual posição anterior
        } 
        else if ( indexX <= 0 ) {
          novaPos.y = atof( indexY + 1);
          novaPos.x = posAtuador.x;
        } 
        else {
          novaPos.y = atof( indexY + 1);
          indexY = '\0';
          novaPos.x = atof( indexX + 1);
        }
        desenhar(novaPos.x, novaPos.y );
        //        Serial.println("ok");
        posAtuador.x = novaPos.x;
        posAtuador.y = novaPos.y;
        break;
      }
      break;
    case 'M':
      buffer[0] = line[ currentIndex++ ];        
      buffer[1] = line[ currentIndex++ ];
      buffer[2] = line[ currentIndex++ ];
      buffer[3] = '\0';
      switch ( atoi( buffer ) ){
      case 300:                                                 //M300 SG50 - pen sobe  ----M300 SG30 - pen desce 
        {
          char* indexS = strchr( line+currentIndex, 'S' );
          float Spos = atof( indexS + 1);
          //         Serial.println("ok");
          if (Spos == 30) { 
            penDesce(); 
          }
          if (Spos == 50) { 
            penSobe(); 
          }
          break;
        }
      case 114:                                // M114 -mostrar posiçao
        Serial.print( "Posiçao : X = " );
        Serial.print( posAtuador.x );
        Serial.print( "  -  Y = " );
        Serial.println( posAtuador.y );
        break;
      case 18:
        Serial.println( "Motores desligados." );
        Serial.println( "Fim do desenho!");
        break;
      default:
        Serial.print( "Comando nao reconhecido : M");
        Serial.println( buffer );
      }
    }
  }



}
