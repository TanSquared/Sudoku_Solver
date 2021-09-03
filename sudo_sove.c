#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>

int **arr = NULL;

void print()
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
            printf("%d ", arr[i][j]);
        printf("\n");
    }
}

int same_block(int i, int j)
{
    if((i%10)%3 == 0 && (i / 10) %3 == 0)
    {
        if(j == i + 11 || j == i + 12 || j == i + 21 || j == i + 22)
            return 1;
    }
    if((i / 10)% 3 == 0 && (i % 10) % 3 == 1)
    {
        if(j == i + 9 || j == i + 11 || j == i + 19 || j == i + 21)
            return 1;
    }
    if((i / 10)% 3 == 0 && (i % 10) % 3 == 2)
    {
        if(j == i + 8 || j == i + 18 || j == i + 9 || j == i + 19)
            return 1;
    }
    if((i / 10)% 3 == 1 && (i % 10) % 3 == 0)
    {
        if(j == i - 9 || j == i - 8 || j == i + 11 || j == i + 12)
            return 1;
    }
    if((i / 10)% 3 == 1 && (i % 10) % 3 == 1)
    {
        if(j == i - 9 || j == i - 11 || j == i + 9 || j == i + 11)
            return 1;
    }
    if((i / 10)% 3 == 1 && (i % 10) % 3 == 2)
    {
        if(j == i + 9 || j == i + 8 || j == i - 12 || j == i - 11)
            return 1;
    }
    if((i / 10)% 3 == 2 && (i % 10) % 3 == 0)
    {
        if(j == i - 9 || j == i - 8 || j == i - 19 || j == i - 18)
            return 1;
    }
    if((i / 10)% 3 == 2 && (i % 10) % 3 == 1)
    {
        if(j == i - 9 || j == i - 11 || j == i - 19 || j == i - 21)
            return 1;
    }
    if((i%10)%3 == 2 && (i / 10) %3 == 2)
    {
        if(j == i - 11 || j == i - 12 || j == i - 21 || j == i - 22)
            return 1;
    }
    return -1;
}

int safe(int x, int y)
{
    if(arr[x][y] < 1 || arr[x][y] > 9) return -1;
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(i == x && j == y)
                continue;
            if(i == x && arr[i][j] == arr[x][y])
                return -1;
            if(y == j && arr[i][j] == arr[x][y])
                return -1;
            if(same_block(i * 10 + j, x * 10 + y) > 0 && arr[x][y] == arr[i][j])
                return -1;
        }
    }
    return 1;
}

int next(int x, int y)
{
    if(x == 8 && y == 8) return -1;
    int index = x * 10 + y;
    index++;
    if(y == 8) index++;
    while(arr[index/10][index % 10] != 0)
    {
        index++;
        if(index % 10 == 9) index ++;
        if(index > 88) return -1;
    }
    return index;          
}

int solve(int x, int y)
{
    while(safe(x, y) < 0)
    {
        arr[x][y]++;
        if(arr[x][y] > 9)
        {
            arr[x][y] = 0;
            return -1;
        }
    }
        if(next(x, y) < 0) return 1;
        int index = next(x, y);
        int i = index/10,j = index % 10;
        while((safe(x, y) > 0 && solve(i, j) > 0) != 1)
        {
            arr[x][y]++;
            if(arr[x][y] > 9)
            {
                arr[x][y] = 0;
                return -1;
            }
        }
    return 1;
}

int main()
{
    arr = (int **)malloc(9 * sizeof(int *));
    int i, j;
    char k;
    for(i = 0; i < 9; i++)
        *(arr + i) = (int *)calloc(9, sizeof(int));
    int fd = open("puzzle.txt", O_RDONLY, 0600);
    if(fd < 0)
    {
        perror("puzzle.txt doesn't exist\n");
        return 0;
    }
        for(i = 0; i < 9; i++)
        {
            for(j = 0; j < 9; j++)
            {
                read(fd, &k, 1);
                arr[i][j] = k - '0';
                lseek(fd, 1, SEEK_CUR);
            }
        }
    close(fd);
    printf("Data read from puzzle.txt\n...............Processing data............\n");
    for(i = 0; i < 9; i++)
    {
        for(j = 0; j < 9; j++)
        {
            if(arr[i][j] == 0) continue;
            if(safe(i, j) < 0)
            {
                printf("Sudoku input not valid\n");
                return 0;
            }
        }
    }
    int ans = 0;
    if(arr[0][0] == 0)
        ans = solve(0, 0);
    else 
    {
        int index = next(0, 0);
        ans = solve(index/10, index % 10);
    }
    if(ans < 0)
    {
        printf("No solution\n");
        return 0;
    }
    fd = open("sol.txt", O_CREAT | O_WRONLY, 0600);
    if(fd < 0)
        perror("Can't create solution file\n");
    else
    {
        printf("Data processed and written into solution.txt\n");
        for(i = 0; i < 9; i++)
        {
            for(j = 0; j < 9; j++)
            {
                k = arr[i][j] + '0';
                write(fd, &k, 1);
                write(fd, " ", 1);
            }
            write(fd, "\n", 1);
        }
    }
    return 0;
}