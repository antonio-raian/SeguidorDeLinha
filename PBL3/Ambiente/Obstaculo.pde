class Obstaculo{
  PVector p1, p2, p3, p4;
  int x,y;
  public PVector[] getCoordenadas(){
    PVector [] cord = {p1, p2, p3, p4}; 
    return cord;
  }
  
  public void setCoordenadas(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
    this.p1 = new PVector(x1, y1);
    this.p2 = new PVector(x2, y2);
    this.p3 = new PVector(x3, y3);
    this.p4 = new PVector(x4, y4);
    this.x  = x1;
    this.y  = y1;
  }
  
  public String println(){
    return p1+","+p2+","+p3+","+p4;
  }
  
  public PVector getP1(){
    return p1;
  }
  
  public PVector getP2(){
    return p2;
  }
  
  public PVector getP3(){
    return p3;
  }
  
  public PVector getP4(){
    return p4;
  }
  
  public int getX(){
    return x;
  }
  
  public int getY(){
    return y;
  }
  public int getWidth(){
    return int(p1.dist(p2)); 
  }
  
  public int getHeight(){
    return int(p1.dist(p4)) ;
  }
}
