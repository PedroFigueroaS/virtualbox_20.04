#include "./../include/processA_utilities.h"
#include "./../include/common.h"

int shmfd;
char chared_data[10];
sem_t *sem;
rgb_pixel_t *ptr;
bmpfile_t *bmp;

int init(int mem_size)
{
    bmp = bmp_create(WIDTH, HEIGHT, DEPTH);
    if (bmp == NULL)
    {
        perror("Error creating bitmap");
        return -1;
    }

    int shmfd = shm_open(SHMOBJ_PATH, O_CREAT | O_RDWR, DEFFILEMODE);
    if (shmfd < 0)
    {
        perror("Error opening shared memory");
        return -1;
    }
    //
    int trunc = ftruncate(shmfd, mem_size);
    if (trunc < 0)
    {
        perror("Error configuring shared memory size");
        return -1;
    }

    // Shared memory mapping
    ptr = (rgb_pixel_t *)mmap(NULL, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("Error mapping shared memory");
        return -1;
    }

    // Opening semaphore
    sem = sem_open(SEM_PATH, O_CREAT, S_IRUSR | S_IWUSR, 1);
    {
        if (sem == SEM_FAILED)
        {
            perror("Error opening semaphore");
            return -1;
        }
    }
    return 0;
}

int close_all(int mem_size)
{
    shm_unlink(SHMOBJ_PATH);
    sem_close(sem);
    sem_unlink(SEM_PATH);
    munmap(ptr, mem_size);
}

int main(int argc, char *argv[])
{

    int mem_size = WIDTH * HEIGHT * sizeof(rgb_pixel_t);
    if (init(mem_size) != 0)
    {
        perror("Initialization failed");
        close_all(mem_size);
    }

    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    // Initialize UI
    init_console_ui();

    set_circle();
    draw_circle_bmp(bmp, circle.x/RADIUS, circle.y/RADIUS);
    save_bmp(bmp, ptr);
    
    if (sem_post(sem) < 0)
    {
        perror("Semaphore error");
    }

    // Infinite loop
    while (TRUE)
    {
        // Get input in non-blocking mode
        int cmd = getch();

        // If user resizes screen, re-draw UI...
        if (cmd == KEY_RESIZE)
        {
            if (first_resize)
            {
                first_resize = FALSE;
            }
            else
            {
                reset_console_ui();
            }
        }

        // Else, if user presses print button...
        else if (cmd == KEY_MOUSE)
        {
            if (getmouse(&event) == OK)
            {
                if (check_button_pressed(print_btn, &event))
                {
                    bmp_save(bmp, "./out/snapshot.bmp");
                    mvprintw(LINES - 1, 1, "Print button pressed");
                    refresh();
                    sleep(1);
                    for (int j = 0; j < COLS - BTN_SIZE_X - 2; j++)
                    {
                        mvaddch(LINES - 1, j, ' ');
                    }
                }
            }
        }

        // If input is an arrow key, move circle accordingly...
        else if (cmd == KEY_LEFT || cmd == KEY_RIGHT || cmd == KEY_UP || cmd == KEY_DOWN)
        {
            move_circle(cmd);
            draw_circle();

            // Reset bitmap
            bmp_destroy(bmp);
            bmp = bmp_create(WIDTH, HEIGHT, DEPTH);
            if (bmp == NULL)
            {
                perror("Error creating bitmap");
                break;
            }

            draw_circle_bmp(bmp, circle.x * RADIUS, circle.y * RADIUS);
            save_bmp(bmp, ptr);
            if (sem_post(sem) < 0)
            {
                perror("Semaphore error");
            }
        }
    }
    endwin();
    close_all(mem_size);
    return 0;
}