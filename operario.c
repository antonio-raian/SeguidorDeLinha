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
//CALIBRACAO
void insertSort();
void checkIntervals(int counter);
int routineCalibration();
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
	//////////////ROTINA DE CALIBRACAO////////////////////////////
  	//nxtDisplayCenteredTextLine(3,"%d", SensorValue[sonar]);
	//nxtDisplayCenteredTextLine(2,"%d", colorRight);
	//nxtDisplayCenteredTextLine(3,"%d", colorLeft);
	/*nxtDisplayCenteredTextLine(2,"%d", white[0]);
	nxtDisplayCenteredTextLine(3,"%d", white[1]);
	waitInMilliseconds(2000);
	eraseDisplay();
	nxtDisplayCenteredTextLine(2,"%d", red[0]);
	nxtDisplayCenteredTextLine(3,"%d", red[1]);
	waitInMilliseconds(2000);
	eraseDisplay();
	nxtDisplayCenteredTextLine(2,"%d", black[0]);
	nxtDisplayCenteredTextLine(3,"%d", black[1]);
	waitInMilliseconds(2000);
	eraseDisplay();*/
	///////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////

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
// MÉTODOS DE CALIBRAÇÃO //////////////////////////////////////////////////////////
int tickRobot = 18;
int circunferenciaRobot = 628;               // 628 mm
int white[2] = {54,78};                      //Array that store the intervals correspondent to white.
int red[2]   = {70,80};                      //Array that store the intervals correspondent to red.
int black[2] = {24,51};                      //Array that store the intervals correspondent to black.
int intervals[10] = {0,0,0,0,0,0,0,0,0,0}; //Array used for calculate the intervals of colors.

void insertSort() {
  for(int i = 0; i < 10; i++){
    int j = i;
    while(j>0 && intervals[j] < intervals[j-1]){
      int temp  = intervals[j];
      intervals[j] = intervals[j-1];
      intervals[j-1] = temp;
      j--;
    }
  }
}

void checkIntervals(int counter){
   int amount = 0;                     //Amount of reading
   int i = 0;                          //Counter of position to intervals
   while(amount < 5){
      if(nNxtButtonPressed == 1){
        nxtDisplayCenteredTextLine(2,"Sensor Right");
        nxtDisplayCenteredTextLine(3,"%d", SensorValue[LightSensorRight]);
      }else if(nNxtButtonPressed == 2){
        nxtDisplayCenteredTextLine(2,"Sensor Left");
        nxtDisplayCenteredTextLine(3,"%d", SensorValue[LightSensorLeft]);
      }else if(nNxtButtonPressed == 3){//button orange pressed
          intervals[i]  = SensorValue[LightSensorRight];
          intervals[i+1]= SensorValue[LightSensorLeft];
          waitInMilliseconds(1000);
          nxtDisplayCenteredTextLine(2,"Reading done");
          waitInMilliseconds(1000);
          nxtDisplayClearTextLine(3);
          i+=2;                         //Now, two positions are with values, so, increase 2.
          amount += 1;                  //After that finish a reading, let's to next.
      }
   }
   //Perform Ordering values of intervals
   insertSort();
   switch(counter){
        /*Now, with the intervals, we pass the values to the matching colors.*/
        case 0:                          //color White
            nxtDisplayCenteredTextLine(1,"Color White");
            white[0] = intervals[0] - 8; //take the less value
            white[1] = intervals[9] + 8; //take the maijor value
            writeDebugStreamLine("white: %d - %d", white[0], white[1]);
            break;

        case 1:                          //color Black
            nxtDisplayCenteredTextLine(2,"Color Black");
            black[0] = intervals[0] - 8; //take the less value
            black[1] = intervals[9] + 8; //take the maijor value
            writeDebugStreamLine("black: %d - %d", black[0], black[1]);
            break;

        case 2:                        //color Red
            nxtDisplayCenteredTextLine(3,"Color Red");
            red[0] = intervals[0] - 8; //take the less value
            red[1] = intervals[9] + 8; //take the maijor value
            writeDebugStreamLine("red: %d - %d", red[0], red[1]);
            break;
        default:
            break;
   }
   waitInMilliseconds(500);
   eraseDisplay();
   /*set intervals with 0*/
   for(int j = 0; j < 10; j++){
     intervals[j] = 0;
   }
}

int routineCalibration(){
    checkIntervals(0); //white
    checkIntervals(1); //black
    checkIntervals(2); //red
    return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////