import pt.citar.diablu.nxt.brick.*;
import pt.citar.diablu.nxt.protocol.*;
import pt.citar.diablu.processing.nxt.*;
import processing.serial.*;

color cor;
ArrayList obstaculos;
int estado; //0 = inicial, 1 = setando obstáculos, 2 = pronto pra gerar rota
String obx, oby, obLar, obAlt;
int escrever; //0 = faz nada 1 = seta x, 2 = seta y, 3 = seta Largura, 4 = seta Altura

void setup() {
  obstaculos = new ArrayList();
  clean();
  estado = 0; //Estado inicial
  escrever = 0;
  background(168,168,168);//fundo Cinza Brilhante
  size(1000, 700);
  
  desenhaRet(20,20,810,660, color(255));//Área de trabalho, cada cm são 3 pixeis
  desenhaRet(850, 20,120, 40, color(112,219,219));//Botão adicionar obstáculo
  desenhaRet(850,70,120,40, color(112,219,147));//Botão Confirma
  desenhaRet(850,640,120,40, color(255));//Coordenada mouse
    
  fill(0);  
  textSize(14);
  text("Adicionar", 878, 39);
  text("Obstáculo", 875, 54);
  
  text("Confirma", 875, 89);
  text("Ambiente", 875, 104);
  
  text("Coordenadas:", 865, 659); 
  
}

void draw() {
  if (mouseX >= 850 && mouseX <=970 && mouseY>=20 && mouseY<=60 ) {//mouse no botão Adicionar Obstáculo
    cursor(HAND);
    if(mousePressed == true && mouseButton == LEFT){
      estado=1;
      escrever=1;
      cor = color(50,153,204);
    }
  } else if(mouseX >= 850 && mouseX <=970 && mouseY>=70 && mouseY<=110){//mouse no botão Confirmar Ambiente
    cursor(HAND);
    if(mousePressed == true && mouseButton == LEFT){
      estado=2;
    }
  } else {
    cursor(ARROW);
  }
  switch(estado){
    case 0:
      if (mouseX >= 850 && mouseX <=970 && mouseY>=20 && mouseY<=60 ) {//mouse no botão Adicionar Obstáculo
        cursor(HAND);
        if(mousePressed == true && mouseButton == LEFT){
          estado=1;
          cor = color(50,153,204);
        }
      } else if(mouseX >= 850 && mouseX <=970 && mouseY>=70 && mouseY<=110){//mouse no botão Confirmar Ambiente
        cursor(HAND);
        if(mousePressed == true && mouseButton == LEFT){
          estado=2;
        }
      } else {
        cursor(ARROW);
      }
    break;
    case 1:
      desenhaRet(850, 130, 50, 30, 255);//X
      desenhaRet(920, 130, 50, 30, 255);//Y
      desenhaRet(850, 180, 50, 30, 255);//largura
      desenhaRet(920, 180, 50, 30, 255);//altura
      desenhaRet(900, 230, 70, 30, 0);
      
      textSize(14);
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
          desenhaRet(850, 130, 125, 135, 168);
          desenhaRet(20+(int(obx)*3), 20+(int(oby)*3), int(obLar)*3, int(obAlt)*3, cor);
          
          stroke(0);
          Obstaculo ob = new Obstaculo();
          ob.setCoordenadas(int(obx), int(oby), int(obx)+int(obLar), int(oby), int(obx)+int(obLar), int(oby)+int(obAlt), int(obx), int(oby)+int(obAlt));
          
          obstaculos.add(ob);
          
          println(ob.println());
          println(obstaculos);          
          
          clean();
        }
      }
    break;
    case 2:
    break;
  }
  
  if (mouseX >= 20 && mouseX <=830 && mouseY>=20 && mouseY<=680 ) {
    desenhaRet(850,640,120,40, color(255));//Coordenada mouse
    
    fill(0);  
    textSize(14); 
    text("Coordenadas:", 865, 659);
    text(posicaoCM(mouseX)+":"+posicaoCM(mouseY)+" cm", 875, 674);
  }
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
