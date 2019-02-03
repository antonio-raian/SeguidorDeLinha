class Frame {
  /*Coordenadas do centro do frame*/
  int centerX;
  int centerY;
  
  int X1;
  int Y1;
  int weight;
  int setObstaculo; //Variável que determina se o obstáculo já está setado com algum peso
  
  public Frame(int X1, int Y1, int largura, int altura){
      this.centerX = (largura-int(27/2));
      this.centerY = (altura + int(22/2));
      this.X1      = X1;
      this.Y1      = Y1;
      this.setObstaculo = 0;
  }
  
  public int getX1(){
    return this.X1;
  }
  
  public int getY1(){
      return this.Y1;
  }  
  
  public int getCenterX(){
    return this.centerX;
  }
  
  public int getCenterY(){
      return this.centerY;
  }  
  
  public int getWeight(){
      return this.weight;
  }
  
  public int setWeight(int value){
      return this.weight = value;
  } 
  
  public int getSetObstaculo(){
      return this.setObstaculo;
  }  
  
  public int setObstaculo(int value){
      return this.setObstaculo = value;
  }  
}
