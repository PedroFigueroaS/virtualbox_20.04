#include "./../include/processB_utilities.h"
#include "./../include/common.h"

int main()
{
    // Initialize UI
    init_console_ui();
    int center_of_circle[2];
    int mem_size = WIDTH * HEIGHT * sizeof(rgb_pixel_t); // define shared memory size

    int share_mem = shm_open(SHMOBJ_PATH, O_CREAT | O_RDWR, 0666); // open share memory
    if (share_mem < 0)                                           // in case of failure
    {
        perror("Cannot open shared memory!");
        exit(1);
    }

    rgb_pixel_t *rgb_matrix = (rgb_pixel_t *)mmap(NULL, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, share_mem, 0); // mapping shared memory
    if (rgb_matrix == MAP_FAILED)                                                                                    // in case of failure
    {
        perror("Cannot map shared memory!");
        exit(1);
    }

    sem_t *semaphor = sem_open(SEM_PATH, 1); // opening semaphore
    if (semaphor == SEM_FAILED)
    {
        perror("Cannot open semaphore!");
        exit(1);
    }

    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    while (TRUE)
    {

        int cmd = getch(); // getting input from keyboard

        if (cmd == KEY_RESIZE) // if widnow resize
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

        else
        {
            find_center(rgb_matrix, center_of_circle);
            mvaddch(center_of_circle[1]/RADIUS, center_of_circle[0]/RADIUS, '0');
        }
    }
    close(share_mem);
    shm_unlink(SHMOBJ_PATH);
    endwin();
    return 0;
}