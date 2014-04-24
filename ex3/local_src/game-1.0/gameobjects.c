typedef struct
{
  int x;
  int y;
} dimension;

struct puck
{
  dimension pos;
  int height;
  int width;
};

struct ball
{
  dimension pos;
  int radius;
};

struct gameboard
{
  dimension dimensions;
};
