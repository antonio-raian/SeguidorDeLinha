#pragma config(Sensor, S1, sensorLuzEsq, sensorLightActive)
#pragma config(Sensor, S2, sensorLuzDir, sensorLightActive)
#pragma config(Sensor, S3, sonar, sensorSONAR)
#pragma config(Motor, motorA, motorEsq, tmotorNormal, PIDControl, encoder)
#pragma config(Motor, motorB,  motorDir, tmotorNormal, PIDControl, encoder)
#pragma platform(NXT)

//ESTADOS
#define PARADO 0
#define FRENTE 1
#define CURVA_DIR 2
#define CURVA_ESQ 3
#define BIFURCA 4
#define OBSTACULO_INICIO 5
#define OBSTACULO_DIR 6
#define OBSTACULO_ESQ 7
#define OBSTACULO_ANDA 8
#define OBSTACULO_FIM 9

#define B_DIREITA 1
#define B_ESQUERDA 2
#define B_LARANJA 3

#define VELOCIDADE 40

#define PRATA 75
#define BRANCO 55
#define PRETO 25

const int maxTamMessage = 5;
const TMailboxIDs entrada1 = mailbox1;

//TASKS
task tBTmensage();
task controle();
//////////////////
//FUNCOES
int obterCor(int sensor);
void andar(int direita,int esquerda);
void curva(char sentido);
void emFrente();
void parar();
void viraGraus(int graus, int direcao, int forcaMotor);
void checaConec();
void readDataMsg();
void walk(int distance, int powerMotor);
////////////////////
//Variaveis de controle
bool caminhar = false;
int estado = PARADO;
int tickRobot = 20;
int circunferenciaRobot = 628;
int direcao = 0;

// ----- TASK PRINCIPAL -------------------
task main(){
	//StartTask(controle);
	StartTask(tBTmensage);
	int cont;
	for(;;){
		if(caminhar){
			switch(estado){
				case PARADO:
					nxtDisplayString(1,"PARADO");
					parar();
				break;
				case FRENTE:
					nxtDisplayString(1,"FRENTE");
					emFrente();
				break;
				case CURVA_DIR:
					nxtDisplayString(1,"CURVA DIREITA");
					curva('D');
				break;
				case CURVA_ESQ:
					nxtDisplayString(1,"CURVA ESQUERDA");
					curva('E');
				break;
				case BIFURCA:
					nxtDisplayString(1,"BIFURCA");
					//viraGraus(10,0,50);
					curva('D');
				break;
				case OBSTACULO_INICIO:
					nxtDisplayString(1,"OBSTACULO");
					StopTask(controle);
					viraGraus(90,1,50);
					cont =1;
					direcao = OBSTACULO_DIR;
					if(SensorValue[sonar]<=15){
						viraGraus(180,0,50);
						direcao = OBSTACULO_ESQ;
					}
					estado = OBSTACULO_ANDA;
				break;
				case OBSTACULO_DIR:
					viraGraus(90,0,50);
					if(cont <2){
						estado = OBSTACULO_ANDA;
						cont++;
					}else
						estado = OBSTACULO_FIM;
				break;
				case OBSTACULO_ESQ:
					viraGraus(90,1,50);
					if(cont <2){
						estado = OBSTACULO_ANDA;
						cont++;
					}else
						estado = OBSTACULO_FIM;
				break;
				case OBSTACULO_ANDA:
					emFrente();
					if(cont!=2)
					  wait1Msec(1500);
					else
					  wait1Msec(2700);
					parar();
					estado = direcao;
				break;
				case OBSTACULO_FIM:
					while(true){
             if(obterCor(SensorValue[sensorLuzEsq])==PRETO){
               parar();
               walk(50,50);
               if(direcao==OBSTACULO_DIR)
                 viraGraus(90, 1, 50);
               else
                 viraGraus(90, 0, 50);
               break;
             }
				     emFrente();
           }
          StartTask(controle);
				break;
				default:
					nxtDisplayString(1,"ERRO");
			}
		}
	}
}
//TASKS -------------------------------------------------------------------------------
task tBTmensage(){
  while(true){
     checaConec();
     readDataMsg();
     wait1Msec(1);
  }
}

task controle(){
	int luzEsq;
	int luzDir;
	int distancia;
	for(;;){
		luzEsq = obterCor(SensorValue[sensorLuzEsq]);
		luzDir = obterCor(SensorValue[sensorLuzDir]);
		distancia = SensorValue[sonar];
		if(distancia <= 10)
			estado = OBSTACULO_INICIO;
		else
			if(luzEsq == BRANCO && luzDir == BRANCO)
			  estado = FRENTE;
			else if (luzDir == PRETO && luzEsq == BRANCO)
			  estado = CURVA_DIR;
			else if (luzDir == BRANCO && luzEsq == PRETO)
			  estado = CURVA_ESQ;
			else if(luzDir == PRETO && luzEsq == PRETO)
			  estado = BIFURCA;
			else if(luzDir == PRATA && luzEsq == PRATA)
			  estado = PARADO;
	}
}

int obterCor(int sensor){
	if(sensor < PRATA)
		if(sensor > BRANCO)
			return BRANCO;
		else if(sensor > PRETO)
			return PRETO;
		else
			return -1;
	else
		return PRATA;
}

void curva(char sentido){
	if(sentido == 'D')
		andar(-2,1);
	else if(sentido == 'E')
		andar(1,-2);
}

void emFrente(){
	andar(1,1);
}

void parar(){
	motor[motorDir] = 0;
	motor[motorEsq] = 0;
	wait1Msec(50);
}

void andar(int direita, int esquerda){
	motor[motorDir] = VELOCIDADE/direita;
	motor[motorEsq] = VELOCIDADE/esquerda;
}

void viraGraus(int graus, int direcao, int forcaMotor){
	int distanciaGraus = (graus * (circunferenciaRobot/10))/ 360; //calculo para verificar a distancia para percorrer em torno do seu proprio eixo.
	distanciaGraus*=10;                                             //transformo para milimetro novamente.
	nxtDisplayCenteredTextLine(2,"%d", distanciaGraus);
	nMotorEncoder[motorEsq]  = 0;
	nMotorEncoder[motorDir] = 0;
	int tickGoal = ((tickRobot-2) * distanciaGraus)/10;
	if(direcao == 1){ //turn left
		while(nMotorEncoder[motorB] < tickGoal && nMotorEncoder[motorA] > (tickGoal*(-1))){
			motor[motorEsq] = (-1)*(forcaMotor);//
			motor[motorDir] = forcaMotor;
		}
	}else{ //turn Right
		while(nMotorEncoder[motorB] > (tickGoal*(-1)) && nMotorEncoder[motorA] < tickGoal){
			motor[motorEsq] = forcaMotor;
			motor[motorDir] = (-1)*(forcaMotor);//
		}
	}
	parar();
}

void walk(int distance, int powerMotor){
    nMotorEncoder[motorEsq]  = 0;
    nMotorEncoder[motorDir] = 0;
    int tickGoal = (tickRobot * distance)/10;
    //nMotorEncoder[motorA] = tickGoal;
    //nMotorEncoder[motorB] = tickGoal;
    while(nMotorEncoder[motorB] < tickGoal && nMotorEncoder[motorA] < tickGoal){
      motor[motorEsq] = powerMotor;
      motor[motorDir] = powerMotor;
    }
    parar();
}

//Bluetooth ---------------------------------
void checaConec()
{
   if (nBTCurrentStreamIndex >= 0){
     nxtDisplayCenteredTextLine(1, "Conectado!!");
     return;
   }

   PlaySound(soundLowBuzz);
   eraseDisplay();
   nxtDisplayCenteredTextLine(3, "BT nao");
   nxtDisplayCenteredTextLine(4, "Conectado");
   wait1Msec(1000);
   //StopAllTasks();
}

void readDataMsg(){
  int messageTam;
  char bufferEntrada[maxTamMessage];

      messageTam = cCmdMessageGetSize(entrada1);
      if(messageTam<=0)
        return;

     cCmdMessageRead(bufferEntrada, messageTam, entrada1);
     nxtDisplayCenteredTextLine(5, "MSG %c", bufferEntrada[0]);

     if(bufferEntrada[0] == 'C' || bufferEntrada[0] == 'c'){
       caminhar = true;
       StartTask(controle);
    }else if(bufferEntrada[0] == 'P' || bufferEntrada[0] == 'p'){
       caminhar = false;
       StopTask(controle);
       parar();
     }
   return;
}
//////////////////////////////////////////////////////////////////////////////////////
