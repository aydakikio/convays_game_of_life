#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0, -1, A5, A4);

#define grid_size 4

bool grid[2][16][32];
int cur = 0;

void draw_grid();
void neighbors();
void threshold();

void setup() {
  u8g2.begin();
  randomSeed(analogRead(0));

  for (int I = 0; I < 16; I++)
    for (int J = 0; J < 32; J++)
      grid[0][I][J] = (random(10) < 3);
}

void loop() {
  draw_grid();
  neighbors();
  cur = 1 - cur;
  threshold();
  delay(100);
}

void neighbors() {
  for (int I = 0; I < 16; I++) {
    for (int J = 0; J < 32; J++) {
      int count = 0;
      for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
          if (di == 0 && dj == 0) continue;
          int ni = (I + di + 16) % 16;
          int nj = (J + dj + 32) % 32;
          count += grid[cur][ni][nj];
        }
      }
      grid[1 - cur][I][J] = (count == 3 || (count == 2 && grid[cur][I][J]));
    }
  }
}

void threshold() {
  // count alive cells
  int alive = 0;
  for (int I = 0; I < 16; I++)
    for (int J = 0; J < 32; J++)
      alive += grid[cur][I][J];

  // check stagnation
  bool changed = false;
  for (int I = 0; I < 16; I++)
    for (int J = 0; J < 32; J++)
      if (grid[0][I][J] != grid[1][I][J])
        changed = true;

  // reseed if dead or stuck
  if (alive < 10 || !changed) {
    for (int I = 0; I < 16; I++)
      for (int J = 0; J < 32; J++)
        grid[cur][I][J] = (random(10) < 3);
  }
}

void draw_grid() {
  u8g2.firstPage();
  do {
    for (int I = 0; I < 16; I++) {
      for (int J = 0; J < 32; J++) {
        if (grid[cur][I][J]) {
          u8g2.drawBox(J * 4, I * 4, grid_size, grid_size);
        }
      }
    }
  } while (u8g2.nextPage());
}