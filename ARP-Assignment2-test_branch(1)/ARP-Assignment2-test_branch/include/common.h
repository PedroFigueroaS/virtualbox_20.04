#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/shm.h>
#include <bmpfile.h>
#include <math.h>


#define WIDTH 1600
#define HEIGHT 600
#define DEPTH 4
#define RADIUS 20

#define SHMOBJ_PATH "/shm_AOS"
#define SEM_PATH "/sem_AOS_1"

void draw_circle_bmp(bmpfile_t *bmp, int x0, int y0)
{
    // Configuration about the circle:
    int radius = RADIUS;
    rgb_pixel_t pixel = {255, 0, 0, 0};  // Color of the circle (BGRA)
    rgb_pixel_t center = {0, 0, 255, 0}; // Color of the center (BGRA)

    // Check if x0,y0 are inside boudaries:
    x0 = x0 < radius ? radius : x0 > WIDTH - radius ? WIDTH - radius : x0;
    y0 = y0 < radius ? radius : y0 > HEIGHT - radius ? HEIGHT - radius : y0;

    // Draw the circle
    for (int x = -radius; x <= radius; x++)
    {
        for (int y = -radius; y <= radius; y++)
        {
            if (x * x + y * y < radius * radius)
            {
                if (x == 0 && y == 0)
                    bmp_set_pixel(bmp, x0, y0, center); // Color center pixel
                else
                    bmp_set_pixel(bmp, x0 + x, y0 + y, pixel);
            }
        }
    }
}

void find_center(rgb_pixel_t *matrix, int *pos)
{
    int max_len = 0, count = 1; 
    for(int x = 0; x < WIDTH; x++) {
        int len = 0;
        for(int y = 0; y < HEIGHT; y++) {
            rgb_pixel_t pixel = matrix[x + y*WIDTH];
            if (pixel.blue + pixel.green + pixel.red < 765) {
                rgb_pixel_t prev_pixel  = matrix[x + (y-1)*WIDTH];
                if (prev_pixel.blue == 255 && prev_pixel.green == 255 && prev_pixel.red == 255) {
                    pos[0] = x;
                    pos[1] = y;
                }
                len++;
            }
        }
        if (len > max_len) {
            max_len = len;
            count = 1;
        }
        else if (len == max_len) count++;
        else break;
    }
    pos[0] = pos[0] - count / 2 - 1;
    pos[1] = pos[1] + max_len / 2 - 1;
}

void save_bmp(bmpfile_t *bmp, rgb_pixel_t *matrix)
{
    // Iterate over all pixels
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {

            rgb_pixel_t *pixel = bmp_get_pixel(bmp, x, y); // Get pixel

            matrix[x + y * WIDTH] = *pixel; // Store the pixel in the matrix
        }
    }
}

void load_bmp(bmpfile_t *bmp, rgb_pixel_t *matrix)
{
    // Iterate over all pixels
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            bmp_set_pixel(bmp, x, y, matrix[x + y * WIDTH]); // Set pixel at x,y from matrix
        }
    }
}