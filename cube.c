#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600
#define COLOUR_WHITE 0xffffffff
#define COLOUR_BLACK 0x00000000
#define COLOUR_LIGHT 0xC7F0D8
#define COLOUR_DARK  0x43523D
#define POINT_SIZE 4

#define COORD_SYS_OFFSET_X SCREEN_WIDTH / 2
#define COORD_SYS_OFFSET_Y SCREEN_HEIGHT / 2

typedef struct Point {
  double x, y, z;
} Point;

void apply_rotation(struct Point *point, double alpha, double beta,
                    double theta) {
  double rotation_matrix[3][3] = {
      {cos(alpha) * cos(beta),
       cos(alpha) * sin(beta) * sin(theta) - sin(alpha) * cos(theta),
       cos(alpha) * sin(beta) * cos(theta) + sin(alpha) * sin(theta)},
      {sin(alpha) * cos(beta),
       sin(alpha) * sin(beta) * sin(theta) + cos(alpha) * cos(theta),
       sin(alpha) * sin(beta) * cos(theta) - cos(alpha) * sin(theta)},
      {-sin(beta), cos(beta) * sin(theta), cos(beta) * cos(theta)}};
  //
  double point_vec[3] = {point->x, point->y, point->z};
  double result_point[3];
  //
  for (int i = 0; i < 3; i++) {
    double dot = 0.0;
    for (int j = 0; j < 3; j++) {
      dot += rotation_matrix[i][j] * point_vec[j];
    }
    result_point[i] = dot;
  }
  point->x = result_point[0];
  point->y = result_point[1];
  point->z = result_point[2];
}

void draw_point(SDL_Surface *surface, int x, int y) {
  SDL_Rect rect = (SDL_Rect){x, y, POINT_SIZE, POINT_SIZE};
  SDL_FillRect(surface, &rect, COLOUR_DARK);
}

void draw_point_3d(SDL_Surface *surface, Point point) {
  int x_2d = point.x + COORD_SYS_OFFSET_X;
  int y_2d = point.y + COORD_SYS_OFFSET_Y;
  draw_point(surface, x_2d, y_2d);
}

void draw_points_3d(SDL_Surface *surface, Point points[], int arr_len) {
  for (int i = 0; i < arr_len; i++) {
    int x_2d = points[i].x + COORD_SYS_OFFSET_X;
    int y_2d = points[i].y + COORD_SYS_OFFSET_Y;
    draw_point(surface, x_2d, y_2d);
  }
}

void initialise_cube(struct Point points[], int num) {
  int p_side = num / 12;
  int side_length = 200;
  int step_size = side_length / p_side;
  //
  // side 1
  for (int i = 0; i < p_side; i++) {
    points[i] = (struct Point){-side_length / 2 + i * step_size,
                               -side_length / 2, +side_length / 2};
  }
  // side 2
  for (int i = 0; i < p_side; i++) {
    points[i + p_side] = (struct Point){-side_length / 2 + i * step_size,
                                        +side_length / 2, +side_length / 2};
  }
  // side 3
  for (int i = 0; i < p_side; i++) {
    points[i + 2 * p_side] = (struct Point){
        -side_length / 2, -side_length / 2 + i * step_size, +side_length / 2};
  }
  // side 4
  for (int i = 0; i < p_side; i++) {
    points[i + 3 * p_side] = (struct Point){
        +side_length / 2, -side_length / 2 + i * step_size, +side_length / 2};
  }
  //
  // side 5
  for (int i = 0; i < p_side; i++) {
    points[i + 4 * p_side] = (struct Point){-side_length / 2 + i * step_size,
                                            -side_length / 2, -side_length / 2};
  }
  // side 6
  for (int i = 0; i < p_side; i++) {
    points[i + 5 * p_side] = (struct Point){-side_length / 2 + i * step_size,
                                            +side_length / 2, -side_length / 2};
  }
  // side 7
  for (int i = 0; i < p_side; i++) {
    points[i + 6 * p_side] = (struct Point){
        -side_length / 2, -side_length / 2 + i * step_size, -side_length / 2};
  }
  // side 8
  for (int i = 0; i < p_side; i++) {
    points[i + 7 * p_side] = (struct Point){
        +side_length / 2, -side_length / 2 + i * step_size, -side_length / 2};
  }
  // side 9
  for (int i = 0; i < p_side; i++) {
    points[i + 8 * p_side] = (struct Point){-side_length / 2, -side_length / 2,
                                            -side_length / 2 + i * step_size};
  }
  // side 10
  for (int i = 0; i < p_side; i++) {
    points[i + 9 * p_side] = (struct Point){side_length / 2, -side_length / 2,
                                            -side_length / 2 + i * step_size};
  }
  // side 11
  for (int i = 0; i < p_side; i++) {
    points[i + 10 * p_side] = (struct Point){side_length / 2, side_length / 2,
                                             -side_length / 2 + i * step_size};
  }
  // side 12
  for (int i = 0; i < p_side; i++) {
    points[i + 11 * p_side] = (struct Point){-side_length / 2, side_length / 2,
                                             -side_length / 2 + i * step_size};
  }
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("ERROR: %s\n", SDL_GetError());
    exit(1);
  }
  //
  SDL_Window *window =
      SDL_CreateWindow("3D Cube", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  SDL_Surface *surface = SDL_GetWindowSurface(window);
  SDL_Rect back_rect = (SDL_Rect){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  //
  int num = 1200;
  struct Point points[num];
  initialise_cube(points, num);
  draw_points_3d(surface, points, num);
  SDL_Event event;
  int is_running = 1;
  const long max_rand = 10000L;
  double alpha = 0.02;
  double beta = 0.04;
  double gamma = 0.03;
  while (is_running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        is_running = 0;
    }
    SDL_FillRect(surface, &back_rect, COLOUR_LIGHT);
    //alpha =+ -0.02 + (0.02 - -0.02) * (random() % max_rand) / max_rand;
    //beta =+ -0.02 + (0.02 - -0.02) * (random() % max_rand) / max_rand;
    //gamma =+ -0.02 + (0.02 - -0.02) * (random() % max_rand) / max_rand;
    for (int i = 0; i < num; i++)
      apply_rotation(&points[i], alpha, beta, gamma);
    draw_points_3d(surface, points, num);
    //
    SDL_UpdateWindowSurface(window);
    SDL_Delay(20);
  }
  exit(0);
}
