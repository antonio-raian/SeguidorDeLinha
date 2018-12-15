#pragma config(Sensor, S1,     LightSensorLeft, sensorLightActive)
#pragma config(Sensor, S2,     LightSensorRight, sensorLightActive)
#pragma config(Sensor, S3,     sonar,          sensorSONAR)
#pragma config(Motor,  motorA,          LeftMotor,     tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorB,          RightMotor,    tmotorNormal, PIDControl, encoder)
#pragma platform(NXT)

//////////////////////////////FUNCTIONS THAT CHECK THE CALIBRATION//////////////////////////////////////////////

int tickRobot = 20;
int circunferenciaRobot = 628;               // 628 mm
int white[2] = {54,78};                      //Array that store the intervals correspondent to white.
int red[2]   = {70,80};                      //Array that store the intervals correspondent to red.
int black[2] = {24,51};                      //Array that store the intervals correspondent to black.

const int maxTamMessage = 5;
const TMailboxIDs entrada1 = mailbox1;

bool iniciar = false;

/////////////////////////////////////////////////////////////////////////////////////////////

int identifieColors(int sensor){
  if(sensor >= white[0] && sensor <= white[1]){//white color
    return 0;
  }
  if(sensor >= black[0] && sensor <= black[1]){//black color
    return 1;
  }
  if(sensor >= red[0] && sensor <= red[1]){//red color
    return 2;
  }
  return -1;
}

void LineAlign(int right, int left){
  if(right ==1 && left==1){
    motor[motorA] = -20;
    motor[motorB] = 50;
    wait1Msec(125);
  }
  if(right == 1){
    motor[motorA] = 50;
    motor[motorB] = -25;
    wait1Msec(125);
  }else if(left == 1){
    motor[motorA] = -25;
    motor[motorB] = 50;
    wait1Msec(125);
  }
}
void stopAllMotors(){
  motor[motorA] = 0;
  motor[motorB] = 0;
  wait1Msec(50);
}

void walk(int distance, int powerMotor){
    nMotorEncoder[LeftMotor]  = 0;
    nMotorEncoder[RightMotor] = 0;
    int tickGoal = (tickRobot * distance)/10;
    //nMotorEncoder[motorA] = tickGoal;
    //nMotorEncoder[motorB] = tickGoal;
    while(nMotorEncoder[motorB] < tickGoal && nMotorEncoder[motorA] < tickGoal){
      motor[LeftMotor] = powerMotor;
      motor[RightMotor] = powerMotor;
    }
    stopAllMotors();
}

void turnDegress(int degress, int direction, int powerMotor){
  int distanceInDegress = (degress * (circunferenciaRobot/10))/ 360; //calculo para verificar a distancia para percorrer em torno do seu proprio eixo.
  distanceInDegress*=10;                                             //transformo para milimetro novamente.
  nxtDisplayCenteredTextLine(2,"%d", distanceInDegress);
  nMotorEncoder[LeftMotor]  = 0;
  nMotorEncoder[RightMotor] = 0;
  int tickGoal = ((tickRobot-2) * distanceInDegress)/10;
  if(direction == 1){ //turn left
      while(nMotorEncoder[motorB] < tickGoal && nMotorEncoder[motorA] > (tickGoal*(-1))){
        motor[LeftMotor] = (-1)*(powerMotor);//
        motor[RightMotor] = powerMotor;
      }
  }else{ //turn Right
      while(nMotorEncoder[motorB] > (tickGoal*(-1)) && nMotorEncoder[motorA] < tickGoal){
        motor[LeftMotor] = powerMotor;
        motor[RightMotor] = (-1)*(powerMotor);//
      }
  }
  stopAllMotors();
}

int findLine(){
  int colorRight  = identifieColors(SensorValue[LightSensorRight]);
  int colorLeft   = identifieColors(SensorValue[LightSensorLeft]);

  if(colorRight == 0 || colorLeft == 0){
    motor[motorA] = 50;
    motor[motorB] = 50;
  }
  if(colorRight == 1 || colorLeft == 1){
    stopAllMotors();
    return 1;
  }
  return 0;
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
   wait1Msec(3000);
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

     if(bufferEntrada[0] == 'C' || bufferEntrada[0] == 'c')
       iniciar = true;
     else if(bufferEntrada[0] == 'P' || bufferEntrada[0] == 'p')
       iniciar = false;
   return;
}
//////////////////////////////////////////////////////////////////////////////////////

//TASKS -------------------------------------------------------------------------------
task tBTmensage(){

  while(true){
     checaConec();
     readDataMsg();
     wait1Msec(1);
  }
}


//PRINCIPAL-----------------------------------------------------------------------------
task main{
  StartTask(tBTmensage);

  int sonarValue = 0;
  /*Initialize with 3 because the number three haven't any function of motion*/
  int colorLeft   = 3;
  int colorRight  = 3;
  int direcao;
  //routineCalibration(); //calibration
  wait1Msec(2000); //wait 10 seconds before of start.
  while(true){
    if(iniciar){
      /*Check the color of the sensors*/
      colorRight  = identifieColors(SensorValue[LightSensorRight]);
      colorLeft   = identifieColors(SensorValue[LightSensorLeft]);

      sonarValue  = SensorValue[sonar];

      if(sonarValue>=10){
        if(colorRight == 0 && colorLeft == 0){
           motor[motorA] = 50;
           motor[motorB] = 50;
        }
        if(colorRight == 1 || colorLeft == 1){
          LineAlign(colorRight,colorLeft);
        }
        if(colorRight == 2 || colorLeft == 2){
          iniciar = false;
        }
      }else{
        stopAllMotors();
        direcao =1;
        turnDegress(90,1,25); //turn Left
           if(SensorValue[sonar] <= 20){
             turnDegress(180,0,25); //turn Right
             direcao =0;
             walk(250,50);
             turnDegress(90,1,25);
             walk(500,50);
             turnDegress(90,1,25);
           }else{
             walk(250,50);
             turnDegress(90,0,25);
             walk(500,50);
             turnDegress(90,0,25);
           }
           while(true){
             if(findLine()==1){
               walk(50,50);
               turnDegress(90,direcao,50);
               break;
             }
           }
           wait1Msec(500);
      }
    }else{
      stopAllMotors();
    }
  }
}
