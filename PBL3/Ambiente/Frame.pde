class Frame {
  /*Coordenadas do centro do frame*/
  int centerX;
  int centerY;
  
  int X1;
  int Y1;
  
  public Frame(int X1, int Y1, int largura, int altura){
      this.centerX = (largura-int(27/2));
      this.centerY = (altura + int(22/2));
      this.X1      = X1;
      this.Y1      = Y1;
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
}
