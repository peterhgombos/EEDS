typedef struct
{
  int x;
  int y;
} position;

typedef struct
{
  int width;
  int height;
  position pos;
} puck;

typedef struct
{
  int radius;
  position pos;
} ball;

struct gameboard
{
  position dimensions;
};
