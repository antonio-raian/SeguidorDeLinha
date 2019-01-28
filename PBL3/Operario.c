#pragma config(Sensor, S1,     LightSensorLeft, sensorLightActive)
#pragma config(Sensor, S2,     LightSensorRight, sensorLightActive)
#pragma config(Sensor, S3,     sonar,          sensorSONAR)
#pragma config(Motor,  motorA,          LeftMotor,     tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorB,          RightMotor,    tmotorNormal, PIDControl, encoder)
#pragma platform(NXT)
/////////////////////////////////////////////////////////////////////////////////////////

char comando = 'n';

const int maxTamMessage = 5;
const TMailboxIDs entrada1 = mailbox1;

//Bluetooth ---------------------------------
/*task checaConec()
{
   if (nBTCurrentStreamIndex >= 0){
     nxtDisplayCenteredTextLine(1, "Conectado!!");
     return;
   }

   PlaySound(soundLowBuzz);
   eraseDisplay();
   nxtDisplayCenteredTextLine(3, "BT nao");
   nxtDisplayCenteredTextLine(4, "Conectado");
   wait1Msec(2000);
   //StopAllTasks();
}

task readDataMsg(){
  int messageTam;
  char bufferEntrada[maxTamMessage];

      messageTam = cCmdMessageGetSize(entrada1);
      if(messageTam<=0)
        return;

     cCmdMessageRead(bufferEntrada, messageTam, entrada1);
     nxtDisplayCenteredTextLine(5, "MSG %c", bufferEntrada[0]);

     if(bufferEntrada[0] == 'C' || bufferEntrada[0] == 'c')
       //iniciar = true;
     else if(bufferEntrada[0] == 'P' || bufferEntrada[0] == 'p')
       //iniciar = false;
   return;
}*/

task readDataMsg(){
  setBluetoothOn();
  setBluetoothRawDataMode();
  while(!bBTRawMode){ //Espera entrar no modo
        wait1Msec(50);
  }
  while(true){
    nxtReadRawBluetooth(&comando, 20);
  }
  EndTimeSlice();
}

void sendMessage(){
  ubyte bytesEnviados = 10;

  nxtWriteRawBluetooth(bytesEnviados,8);
  wait1Msec(100);
}
//////////////////////////////////////////////////////////////////////////////////////

//PRINCIPAL-----------------------------------------------------------------------------
task main{
  StartTask(readDataMsg);

  while(true){
	  if(nNxtButtonPressed == 3){//button orange pressed
	    sendMessage();
	  }
	}
}
