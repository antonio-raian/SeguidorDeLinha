#pragma config(Sensor, S1,     LightSensorLeft, sensorLightActive)
#pragma config(Sensor, S2,     LightSensorRight, sensorLightActive)
#pragma config(Sensor, S3,     sonar,          sensorSONAR)
#pragma config(Motor, motorA, motorEsq, tmotorNormal, PIDControl, encoder)
#pragma config(Motor, motorB,  motorDir, tmotorNormal, PIDControl, encoder)
#pragma platform(NXT)
/////////////////////////////////////////////////////////////////////////////////////////

//Estados
const int PARADO = 0;
const int EM_FRENTE_22 = 1;
const int EM_FRENTE_27 = 2;
const int VIRA_DIREITA = 3;
const int VIRA_ESQUERDA = 4;

//PROTOCOLO
const int FRENTE_22 = 1;
const int FRENTE_27 = 2;
const int DIREITA = 3;
const int ESQUERDA = 4;

//Variaveis
int estado = PARADO;
int tickRobot = 20;
int circunferenciaRobot = 628;

////////////BlueTooth////////////////////////////////////////////
void readBTMsg() {
  ubyte bufferEntrada[1];                                       // create a ubyte array, 'BytesRead' of size 'bufferSize'.
  int nNumbBytesRead;
  nNumbBytesRead = nxtReadRawBluetooth(&bufferEntrada[0], 1);   // store 'bufferSize' amount of bytes into 'BytesRead[0]'.
  nxtDisplayCenteredTextLine(3, "%d", estado);
  nxtDisplayCenteredTextLine(5, "MSG %d", bufferEntrada[0]);

  switch (bufferEntrada[0]){
    case FRENTE_22:
        estado = EM_FRENTE_22;
        break;
    case FRENTE_27:
        estado = EM_FRENTE_27;
        break;
    case ESQUERDA:
        estado = VIRA_ESQUERDA;
        break;
    case DIREITA:
        estado = VIRA_DIREITA;
        break;
    default:
        estado = PARADO;
        break;
  }
  bufferEntrada[0]=0;
  wait1Msec(500);
}

task configBT(){
  //setBluetoothOn();
  setBluetoothRawDataMode();
  while (!bBTRawMode) {
    nxtDisplayCenteredTextLine(4, "Inside RAW");
		wait1Msec(1);
  }
	nxtDisplayCenteredTextLine(4, "After RAW");
  while(true) {
    readBTMsg();
    wait1Msec(100);
  }
}
void sendBTMessage(){
  setBluetoothRawDataMode();
  ubyte myMsg[1];
	myMsg[0] = 5;
	nxtWriteRawBluetooth(nBTCurrentStreamIndex , myMsg, 1);
}
//////////////////////////////////////////////////////////////////////////
////////////////Funcoes///////////////////////////////////////////////////
void parar(){
	motor[motorDir] = 0;
	motor[motorEsq] = 0;
	wait1Msec(50);
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
//////////////////////////////////////////////////
//PRINCIPAL-----------------------------------------------------------------------------
task main{
  StartTask(configBT);

  while(true){
	  switch(estado){
	    case PARADO:
	      parar();
        break;
      case EM_FRENTE_22:
        walk(220, 50);
	      sendBTMessage();
        estado = PARADO;
        break;
      case EM_FRENTE_27:
        walk(270, 50);
	      sendBTMessage();
        estado = PARADO;
        break;
      case VIRA_DIREITA:
        viraGraus(90, 0, 50);
	      sendBTMessage();
        estado = PARADO;
        break;
      case VIRA_ESQUERDA:
        viraGraus(90, 1, 50);
	      sendBTMessage();
        estado = PARADO;
        break;
      //default:
	  }
	}
}
