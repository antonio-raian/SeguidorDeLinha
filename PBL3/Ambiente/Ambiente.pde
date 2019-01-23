import pt.citar.diablu.nxt.brick.*;
import pt.citar.diablu.nxt.protocol.*;
import pt.citar.diablu.processing.nxt.*;
import processing.serial.*;

color cor;
ArrayList <Obstaculo> obstaculos;
int estado; //0 = inicial, 1 = setando obstáculos, 2 = pronto pra gerar rota, 3 = apagar último obstáculo, 4 = definir ponto final e inicial e gerar tragetória 
String obx, oby, obLar, obAlt;
int escrever; //0 = faz nada 1 = seta x, 2 = seta y, 3 = seta Largura, 4 = seta Altura
int frames;
int sizeLinha;
int sizeColuna;
int positionMouseX;
int positionMouseY;
int pointIF; //Ponto inicial = 0, ponto final = 1;

Frame [][] frameScreen = new Frame[10][10];

void setup() {
  pointIF = 0;
  sizeLinha   = 27;
  sizeColuna  = 22;
  obstaculos = new ArrayList();
  clean();
  estado = 0; //Estado inicial
  escrever = 0;
  frames = 10;
  background(168,168,168);//fundo Cinza Brilhante
  size(1000, 700);
  
  desenhaRet(20,20,810,660, color(255));          //Área de trabalho, cada cm são 3 pixeis
  desenhaRet(850, 20,120, 40, color(112,219,219));//Botão adicionar obstáculo
  desenhaRet(850,70,120,40, color(112,219,147));  //Botão Confirma
  desenhaRet(850,640,120,40, color(255));         //Coordenada mouse
  desenhaRet(850,590,120,40, color(112,219,219)); //Botão de apagar obstáculos  
  fill(0);  
  textSize(13);
  text("Adicionar", 878, 39);
  text("Obstáculo", 875, 54);
  text("Confirmar", 875, 89);
  text("Ambiente", 876, 104);
  text("Coordenadas:", 865, 659); 
  text("Apagar obstáculos", 852, 615); 
  divideScreen(); //Divide a tela em frames
  storeFrames();  //Armazena os frames das telas
}

void draw() {
  /* Código responsável pela checagem do movimento e ação do curso do mouse*/
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  switch(estado){
    case 0:
        if (mouseX >= 850 && mouseX <=970 && mouseY>=20 && mouseY<=60) {//mouse no botão Adicionar Obstáculo
          cursor(HAND);
          if(mousePressed == true && mouseButton == LEFT){
            noStroke();
            desenhaRet(850, 130, 125, 135, 168); //Cobre a mensagem de erro dos obstáculos fora dos espaços.
            estado=1;
            escrever = 1;
            cor = color(50,153,204);
          }
        } else if(mouseX >= 850 && mouseX <=970 && mouseY>=70 && mouseY<=110){//mouse no botão Confirmar Ambiente
          cursor(HAND);
          if(mousePressed == true && mouseButton == LEFT){
            estado=2;
          }
        }else if(mouseX >= 850 && mouseX <= 970 && mouseY>=590 && mouseY<=630){//mouse no botão Apagar obstáculos
          cursor(HAND);
          if(mousePressed == true && mouseButton == LEFT){
            estado=3;
          }
        }else {
        cursor(ARROW);
        }
        break;
    case 1:
        desenhaRet(850, 130, 50, 30, 255);//X
        desenhaRet(920, 130, 50, 30, 255);//Y
        desenhaRet(850, 180, 50, 30, 255);//largura
        desenhaRet(920, 180, 50, 30, 255);//altura
        desenhaRet(900, 230, 70, 30, 0);
      
      textSize(13);
      fill(0);
      text(obx+"", 853, 150);
      text(oby+"", 923, 150);
      text(obLar+"", 853, 195);
      text(obAlt+"", 923, 195);
      
      fill(255);
      text("OK", 925, 250);
      if (mouseX >= 900 && mouseX <=970 && mouseY>=230 && mouseY<=260 ) {//mouse no botão Adicionar Obstáculo
        cursor(HAND);
        if(mousePressed == true && mouseButton == LEFT){
          estado=0;
          noStroke();
          desenhaRet(850, 130, 125, 135, 168); //Cobre novamente os espaços de coordenadas após inserí-los.
          if( checkValueCoordenate() == 1) {   //Realiza checagem para verificar se o obstáculo está dentro do espaço desejado.
            desenhaRet(20+(int(obx)*3), 20+(int(oby)*3), int(obLar)*3, int(obAlt)*3, cor);//Desenha o obstáculo desejado.
            stroke(0);
            Obstaculo ob = new Obstaculo();
            ob.setCoordenadas(int(obx), int(oby), int(obx)+int(obLar), int(oby), int(obx)+int(obLar), int(oby)+int(obAlt), int(obx), int(oby)+int(obAlt)); 
            obstaculos.add(ob);
            println(ob.println());          
          } else {
            fill(0);  
            textSize(15);
            text("Obstáculo fora", 858, 150);
            text("do espaço!!", 875, 170);
          }
          clean();
        }
      }
      break;
    case 2: //gerar ambiente de rotas
      /*Pintar guadros ocupados por obstáculos*/
      int x1,y1,x3,y3;
      int frX1,frY1,frX3,frY3;
      for (int i = obstaculos.size() - 1; i >= 0; i--) {
        Obstaculo part = obstaculos.get(i);
        x1 = part.getX();
        y1 = part.getY();
        x3 = part.getX3();
        y3 = part.getY3();
        /*Agora pinta os frames específcos ocupados pelo obstáculo*/
        frX1 = (27*(searchFrame(x1,1)-1));
        frX3 = (27*searchFrame(x3,1));
        frY1 = (22*(searchFrame(y1,0)-1));
        frY3 = (22*searchFrame(y3,0));
        noStroke();
        desenhaRet(20+(frX1*3),20+(frY1*3),(frX3-frX1)*3,(frY3-frY1)*3, color(0));
        divideScreen();//divide novamente a tela
      }
      /*--------------------------------------*/
      estado = 4;
      break;
      
    case 3: //apagar último obstáculo
      delay(100); //Atrasa a execução para dar o tempo do click do mouse
      if(obstaculos.size() == 0){
        estado = 0;
      }else{
        Obstaculo aux = obstaculos.remove((obstaculos.size()-1));
        noStroke();
        desenhaRet(20+(aux.getX()*3), 20+(aux.getY()*3), aux.getWidth()*3, aux.getHeight()*3, color(255));//Apaga o obstáculo
        estado = 0;
      }
      divideScreen();//divide novamente a tela
      /*Laço que redesenha os obstáculos para evitar danos em algum, caso tenha ocorrido sobreposição.*/
      for(int i = 0; i < obstaculos.size(); i++){
        Obstaculo aux = obstaculos.get(i);
        noStroke();
        desenhaRet(20+(aux.getX()*3), 20+(aux.getY()*3), aux.getWidth()*3, aux.getHeight()*3, cor);
      }
      break;
      
    case 4:
      /*--------------Definir ponto inicial e final da tragetória-------------------*/
      delay(100);
      if(pointIF == 2){
        estado = 0;
        break;
      }
      if (mouseX >= 20 && mouseX <=830 && mouseY>=20 && mouseY<=680 ) {
          cursor(HAND);
          if(mousePressed == true && mouseButton == LEFT){
             positionMouseX = posicaoCM(mouseX);
             positionMouseY = posicaoCM(mouseY);
             int mouseX1Frame = searchFrame(positionMouseX,1)-1;
             int mouseY1Frame = searchFrame(positionMouseY,0)-1;
             Frame aux = frameScreen[mouseX1Frame][mouseY1Frame];
             if(pointIF == 0){       //Initial point
               ellipseMode(CENTER);  // Set ellipseMode to CENTER
               fill(100);            // Set fill to gray
             }else{                  //Final point
               ellipseMode(CENTER);  // Set ellipseMode to CENTER
               fill(204, 102, 0);    // Set fill to orange 
             }
             ellipse(20+(aux.getCenterX()*3), 20+(aux.getCenterY()*3), 10, 10);
             pointIF++;
          }
      }  
      break;
  }
  
  if (mouseX >= 20 && mouseX <=830 && mouseY>=20 && mouseY<=680 ) {
    desenhaRet(850,640,120,40, color(255));//Coordenada mouse
    
    fill(0);  
    textSize(14); 
    text("Coordenadas:", 865, 659);
    positionMouseX = posicaoCM(mouseX);
    positionMouseY = posicaoCM(mouseY);
    text(positionMouseX+":"+positionMouseY+" cm", 875, 674);
  }
}


void storeFrames(){
  int x;
  int y;
  int coordenateX = 0;
  int coordenateY = 0;
  int largura = 27;
  int altura  = 0;
   for(x = 0; x < frames; x++){
     for(y = 0; y < frames; y++ ){
       frameScreen[x][y] = new Frame(coordenateX,coordenateY,largura,altura);
        /*print(coordenateX + " ");
        println(coordenateY);
        println(largura);
        println(altura);
        println("------------------------");*/
        coordenateY+=sizeColuna;
        altura += sizeColuna;
     }
     altura = 0;
     largura += sizeLinha;
     coordenateX+=sizeLinha;
     coordenateY = 0;
   }
}

void divideScreen(){
  int x1,x2,y1,y2;
  /*----------Divisão vertical------------*/
  x1 = (20 + (27*3)); 
  x2 = (20 + (27*3)); 
  y1 = 20; 
  y2 = 20 + 660;
  for(int i = 27; i < 270; i+=27){
      stroke(255, 140, 100);
      line(x1, y1, x2, y2);
      x1+=27*3;
      x2+=27*3;
  }
  /*----------Divisão horizontal-----------*/  
  x1 = 20; 
  x2 = 20+810; 
  y1 = (20 + (22*3)); 
  y2 = (20 + (22*3));
  for(int i = 22; i < 220; i+=22){
    stroke(255, 140, 100);
    line(x1, y1, x2, y2);
    y1+=22*3;
    y2+=22*3;
  }
}

int searchFrame( int value, int coordenate ){
  int point1 = 0;
  int point2;
  int result = 0;
  if(coordenate == 1){ //search frame x
    point2 = 27;
  }else{
    point2 = 22; //search frame y
  }
  
  for(int i = 1; i <= frames; i++){
     if( (value >= point1) && (value <= point2) ){
       result = i;
       break;
     }else{
         if(coordenate == 1){
           point1+= 27;
           point2+= 27;
         }else{
           point1+= 22;
           point2+= 22;
         }
     }
  }  
  return result;
}



int posicaoCM(int x){
  if(x!=0){
    return((x-20)/3);
  }else{
    return 0;  
  }
}

void desenhaRet(int x, int y, int largura, int altura, color c){
  fill(c);
  rect(x, y, largura, altura);
}

void keyTyped() {
  //println("typed " + int(key) + " " + keyCode);
  if(int(key)>=48 && int(key)<=57){
    switch(escrever){
      case 1:
        obx = obx + key;
        println(int(obx));
        break;
      case 2:
        oby = oby + key;
        break;
      case 3:
        obLar = obLar + key;
        break;
      case 4:
        obAlt = obAlt + key;
        break;
      default:
        break;
    }
  }
}

void keyPressed(){
  if(key == ENTER){
    escrever++;
  }
  if(key == BACKSPACE){
    if(escrever==1) obx="";
    if(escrever==2) oby="";
    if(escrever==3) obLar="";  
    if(escrever==4) obAlt="";
  }
}

void clean(){  
  obx = "X";
  oby = "Y";
  obLar = "largura";
  obAlt = "altura";
}

int checkValueCoordenate(){
  if(int(obx) <= 270 && int(oby) <= 220){
    if( (int(obx) + int(obLar)) <= 270 && (int(oby) + int(obAlt)) <=220){
      return 1;
    }
  }  
  return 0;
}
