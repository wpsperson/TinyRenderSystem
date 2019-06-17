class Block{
 private:
  int posX, posY; // Block's position
  
 public:
  void Create();
  void Draw();
  void Destroy();
  
  Block();
  virtual ~Block();
  // Get position
  int GetPosX();
  int GetPosY();
  // Set position
  void SetPosX(int x);
  void SetPosY(int y);
  
  // Transformation
  int Rotate(int direction, int originX, int originY);
  int CanRotate(int direction, int originX, int originY);
  int Translate(int offsetX, int offsetY);
  int CanTranslate(int offsetX, int offsetY);
  
  
  // Assignment operator
  Block & operator = (Block & other){
    if(this!=&other){
      SetPosX(other.GetPosX());
      SetPosY(other.GetPosY());
    }
    return *this;
  }
};


class Figure{
 private:
  int posX, posY;
  int canRotate;
 public:
  Block *data;
  void Create(int numFigure); // Create figures, 
  void Draw();
  void Destroy();

  Figure(int numFigure);
  Figure();
  virtual ~Figure();
  
  // Get/Set figure
  int GetPosX();
  int GetPosY();
  void SetPosX(int x);
  void SetPosY(int y);
  Block *GetFigure();
  void New(int numFigure);
  void Clear();
  // Transformation
  int Rotate(int direction);
  int Translate(int offsetX, int offsetY);
};

class Game{
 private:
  int num_row, num_col;
  int map[100][100];
 public:
  // Constructor, Destructor
  Game();
  virtual ~Game();
  
  // Draw
  void Draw();
  
  // Update
  void Update(Figure &figure);
  int RowIsFull(int row);
  void ClearRow(int row);
  void New();
  void Print();
  int GetMap(int x, int y);
};
