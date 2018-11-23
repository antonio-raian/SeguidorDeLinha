import pt.citar.diablu.nxt.brick.*;
import pt.citar.diablu.nxt.protocol.*;
import pt.citar.diablu.processing.nxt.*;
import processing.serial.*;

PVector posAtual;
PVector posAnterior;
float grau = 6.22;//1º do robo no eixo = 6.22º de uma roda quando a outra está parada
LegoNXT lego;
boolean comecar;

void setup() {
  comecar = false;
  posAtual = new PVector(0, 0);
  posAnterior = new PVector(0, 0);
  lego = new LegoNXT(this, Serial.list()[1]);//conecta ao lego
  background(255);//fundo branco
  size(900, 900);
  frameRate(20);
  noSmooth();
  loop();
  //reseta motores
  lego.resetMotorPosition(LegoNXT.MOTOR_A, false);
  lego.resetMotorPosition(LegoNXT.MOTOR_B, false);
}
void draw() {
  if(comecar == true){
    //obtem rotacao dos dois motores
    float legoRTC_A = lego.getMotorRotationCount(LegoNXT.MOTOR_A);
    float legoRTC_B = lego.getMotorRotationCount(LegoNXT.MOTOR_B);
    
    float angulo = (legoRTC_A - legoRTC_B)/grau; //obtem o angulo atraves da diferença dos giros do motor
    PVector direcao = PVector.fromAngle(radians(angulo));//cria vetor apontando para frente do robo
    posAtual.add(direcao);
    
    //desenha a linha
    translate(300,600);
    scale(5);
    line(posAnterior.x, posAnterior.y, posAtual.x , posAtual.y);
    
    posAnterior = posAtual.copy();
    
    //verifica se esta em movimento
    float temp_A = lego.getMotorRotationCount(LegoNXT.MOTOR_A);
    float temp_B = lego.getMotorRotationCount(LegoNXT.MOTOR_B);
    if(legoRTC_A == temp_A && legoRTC_B == temp_B)
     comecar = false;
  }
}

void keyPressed(){
  if(key == 'W' || key == 'w')
    comecar = lego.sendMsg(0, "C");
  if(key == 'S' || key == 's')
    comecar = !(lego.sendMsg(0, "P"));
}
