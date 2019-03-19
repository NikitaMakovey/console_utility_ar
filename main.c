#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    FILE *output_file, *reading_file;
    if(*(argv[1] + 1) != 0)
    {
        //create verification of correct key
        //if pointer of next character is NULL then
        //x == 0 and everything is ok
        //else x != 0 and catch the exception for this error
        printf("%s is incorrect key\n", argv[1]);
    } else
    switch (*argv[1])
    {
        case 'r':
            //create archive file that includes itself a file from current directory
            output_file = fopen(argv[2], "w"); //create archive file for recording of files
            fprintf(output_file, "!<arch>\n");

            struct stat st;
            if (stat(argv[2], &st) == -1) // if not successful filling of structure
            {
                perror("stat");
                return 1;
            }

            FILE *input_file;

            for(int j = 3; j < argc; j++)
            {
                input_file = fopen(argv[j], "r");

                int c;
                long size_count = 0;
                char *file_array = malloc(sizeof(c) / 4);
                while ((c = getc(input_file)) != EOF)
                {
                    size_count += sizeof(c);
                    file_array = realloc(file_array, size_count / 4 * sizeof(c) / 4);
                    file_array[size_count / 4 - 1] = (char)c;
                }

                char header[60]; //create header of archive file
                memset(header, 0x20, 60); //fill spaces all 60 bytes
                header[sprintf(header, "%s%c", argv[j], '/')] = 0x20;
                header[sprintf(header + 16, "%ld", st.st_mtime) + 16] = 0x20; //1552712807 change this?
                header[sprintf(header + 28, "%d", st.st_uid) + 28] = 0x20;
                header[sprintf(header + 34, "%d", st.st_gid) + 34] = 0x20;
                header[sprintf(header + 40, "%o", st.st_mode & 0xFFF) + 40] = 0x20;
                header[sprintf(header + 48, "%ld", size_count / 4) + 48] = 0x20;
                header[sprintf(header + 58, "%c%c", 0x60, 0x0A) + 58] = 0x20;

                for (int i = 0; i < 60; i++)
                {
                    fprintf(output_file, "%c", header[i]);
                }

                for (long i = 0; i < size_count / 4; i++)
                {
                    fprintf(output_file, "%c", file_array[i]);
                }
                if(file_array[size_count / 4 - 1] != '\n' && j != argc - 1)
                    fprintf(output_file, "%c", '\n');

                free(file_array);

                fclose(input_file);

            }
            fclose(output_file);
            break;
        case 't':
            //show filenames of all files in archive file
            reading_file = fopen(argv[2], "r");
            int fc;
            char * filename = malloc(100000);
            int index = 0;
            bool ifPoint_ = false;
            bool ifSlash_ = false;
            int control_slash = 0;
            while ((fc = getc(reading_file)) != EOF)
            {
                filename[index] = (char)fc;
                index++;
                if(ifPoint_ && ifSlash_ && fc == '\n')
                {
                    for(int i = 0; i < control_slash; i++)
                    {
                        printf("%c", filename[i]);
                    }
                    printf("\n");
                    ifPoint_ = false;
                    ifSlash_ = false;
                    index = 0;
                }
                if(fc == '/')
                {
                    ifSlash_ = true;
                    control_slash = index - 1;
                }
                if(fc == 0x60 && index == 59)
                    ifPoint_ = true;
                if(fc == '\n')
                {
                    index = 0;
                }
            }
            free(filename);

            fclose(reading_file);
            break;
        case 'p':
            //show content of all files in archive file
            reading_file = fopen(argv[2], "r");
            char * stroke = malloc(100000);
            int i = 0;
            int ch;
            bool ifPoint = false;
            bool ifSlash = false;
            bool ifForbid = false;
            bool ifOutput = false;
            while((ch = getc(reading_file)) != EOF)
            {
                stroke[i] = (char)ch;
                if(ch == '/')
                    ifSlash = true;
                if(ch == 0x60 && i == 58)
                    ifPoint = true;
                if(ch == 0x0A && ifPoint && ifSlash)
                {
                    ifOutput = false;
                    ifForbid = true;
                }
                i++;
                if(ch == '\n' && ifOutput)
                {
                    for(int j = 0; j < i; j++)
                        printf("%c", stroke[j]);
                    ifSlash = false;
                    ifPoint = false;
                }
                if(ifForbid && ch == '\n')
                {
                    ifForbid = false;
                    ifOutput = true;
                    ifSlash = false;
                    ifPoint = false;
                }
                if(ch == '\n')
                    i = 0;
            }
            free(stroke);

            fclose(reading_file);
            break;
        case 'd':
            //delete choice file in archive file
            reading_file = fopen(argv[2], "r");
            output_file = fopen("h6R6i6ttTweBD40jokeTy1288tyd.ar", "w");
            char * active_stroke = malloc(100000);
            int j = 0;
            bool ifNoWrite = false;
            int sizeof_file = 0;
            int cur_ch;
            int dex = 10;
            int size_index = 48;
            int len_filename = 0;
            while(*(argv[3] + j) != '\0')
            {
                len_filename++;
                j++;
            }
            j = 0;

            while((cur_ch = getc(reading_file)) != EOF)
            {
                active_stroke[j] = (char)cur_ch;
                j++;
                if(cur_ch == '\n')
                {
                    int tmp_i = 0;
                    while(active_stroke[tmp_i] == *(argv[3] + tmp_i))
                    {
                        if(tmp_i + 1 == len_filename && active_stroke[tmp_i + 1] == '/')
                        {
                            while (active_stroke[size_index] != ' ')
                            {
                                sizeof_file = sizeof_file * dex + ((int)active_stroke[size_index] - 48);
                                size_index++;
                            }
                            size_index = 48;
                            ifNoWrite = true;
                            sizeof_file++;
                        }
                        tmp_i++;
                    }
                    if(!ifNoWrite)
                    {
                        for(int i = 0; i < j; i++)
                        {
                            fprintf(output_file, "%c", active_stroke[i]);
                        }
                    }
                    j = 0;
                }

                if(ifNoWrite)
                {
                    if(sizeof_file == 0)
                        ifNoWrite = false;
                    sizeof_file--;
                }
            }
            free(active_stroke);
            fclose(reading_file);
            fclose(output_file);

            output_file = fopen(argv[2], "w");
            reading_file = fopen("h6R6i6ttTweBD40jokeTy1288tyd.ar", "r");
            while((cur_ch = getc(reading_file)) != EOF)
            {
                fprintf(output_file, "%c", cur_ch);
            }
            fclose(reading_file);
            fclose(output_file);

            //delete time file in the current directory
            if (-1 == remove("h6R6i6ttTweBD40jokeTy1288tyd.ar"))
                printf("Error with deleted file\n");

            break;
        case 'x':
            //export all files from archive file
            reading_file = fopen(argv[2], "r");
            char filenames[100][100];
            char *work_line = malloc(1000);
            int f_index = 0;
            int t_index = 0;
            int char_f;
            while((char_f = getc(reading_file)) != EOF)
            {
                work_line[t_index] = (char)char_f;
                t_index++;
                if(
                        char_f == '\n' &&
                        t_index == 60)
                {
                    if(work_line[58] == '`')
                    {
                        int i = 0;
                        while (work_line[i] != '/')
                        {
                            if (i > 15)
                            {
                                i = 0;
                                break;
                            }
                            filenames[f_index][i] = work_line[i];
                            i++;
                        }
                        if (i != 0)
                        {
                            int j = 48;
                            int size_of = 0;
                            while (work_line[j] != ' ')
                            {
                                size_of = size_of * 10 + ((int) work_line[j] - 48);
                                j++;
                            }
                            output_file = fopen(filenames[f_index], "w");
                            f_index++;
                            for (int q = 0; q < size_of; q++)
                            {
                                int c = getc(reading_file);
                                fprintf(output_file, "%c", c);
                            }
                            fclose(output_file);
                        }
                    }
                    t_index = 0;
                }

                if(char_f == '\n')
                {
                    t_index = 0;
                }
            }
            free(work_line);
            fclose(reading_file);

            break;
        default:
            // if user write down incorrect key
            printf("%s is incorrect key\n", argv[1]);
    }

	return 0;
}
