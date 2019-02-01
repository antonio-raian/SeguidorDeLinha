# NXTSeguidorLinha
LegoNXT seguidor de linha programado com RobotC, realizando comunicação via Bluethoot usando NXTComm Processing.

http://www.robotc.net/wikiarchive/NXT

http://help.robotc.net/WebHelpMindstorms/index.htm

http://jorgecardoso.eu/processing/NXTComm/

https://processing.org/


PBL3

Esse projeto permite colocar um robô em um sistema virtual e gerar rotas entre dois pontos.
O método usado para calcular rotas foi o Geometria de Manhattan.

Protocolo  de comunicação:
	- 10: Robô seguirá em frente por 22cm;
	- 11: Robô seguirá em frente por 27cm;
	- 2: Robô gira 90º para a direita sobre o próprio eixo;
	- 3: Robô gira 90º para a esquerda sobre o próprio eixo;

Estados:
	- PARADO: Estado para o robô e envia mensagem para o supervisor informando que terminou a execução solicitada;
	- EM_FRENTE_22: Estado fará com que o robô ande por 22cm em frente;
	- EM_FRENTE_27: Estado fará com que o robô ande por 27cm em frente; 
	- VIRA_DIREITA: Estado fará o robô girar 90º para a direita no próprio eixo;
	- VIRA_ESQUERDA: Estado fará o robô girar 90º para a esquerda no próprio eixo;