#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<signal.h>
#include<fcntl.h>
#include<errno.h>

#include "find_min_max.h"
#include "utils.h"

int pnum = -1;
int active_child_processes = 0;
pid_t *child_pid;

static void sig_alarm(int sign)
{
	printf("Timeout exceeded  \n");
	for(int i = 0; i < pnum; ++i)
	{
	  kill(child_pid[i], SIGKILL);
	}
	while (active_child_processes >= 0)
	{
	  int wpid = waitpid(-1, NULL, WNOHANG);
	  printf("%d\n", active_child_processes);
	  if(wpid == -1)
	  {
		if(errno == ECHILD) break;
	  }
	  else
	  {
		active_child_processes -= 1;
	  }
	}
	printf("Exit prog\n");

	exit(0);
}

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int timeout = 0;
  bool with_files = false;

	while (true) {
		//int current_optind = optind ? optind : 1;

        const char* short_options = "s:a:p:t:f";

		static struct option options[] = {{"seed", required_argument, 0, 's'},
										{"array_size", required_argument, 0, 'a'},
										{"pnum", required_argument, 0, 'p'},
										{"by_files", no_argument, 0, 'f'},
										{"timeout", required_argument, 0, 't'},
										{0, 0, 0, 0}};

		int option_index = 0;
		int c = getopt_long(argc, argv, short_options, options, &option_index);

		if (c == -1) break;

		switch (c) {
			case 0:
				switch (option_index) {
                    case 0:
                        seed = atoi(optarg);
                        // your code here
			 //сид должен быть больше нуля
                        // error handling
                        if (seed <= 0) {
                            printf("seed is a positive number\n");
                            return 1;
                        }
                        break;
                    case 1:
                        array_size = atoi(optarg);
			 //размер массива тоже не нулевой или отр    
                        // your code here
                        // error handling
                        if (array_size <= 0) {
                            printf("array_size is a positive number\n");
                            return 1;
                        }
                        break;
                    case 2:
                        pnum = atoi(optarg);
                        // your code here
                        // error handling
                        if (pnum <= 0) {
                            printf("pnum is a positive number\n");
                            return 1;
                        }
                        break;
                    case 3:
                        with_files = true;//переменная условия 
                        break;
		    case 4:
			timeout = atoi(optarg);
			if(timeout <= 0){
			  printf("timeout is a positive\n");
			  return 1;
			}
			  break;

		    defalut:
         		    printf("Index %d is out of options\n", option_index);
                 	}
				break;
			case 'f':
				with_files = true;
				break;

			case 's':
					seed = atoi(optarg);
					// your code here
					// error handling
					if (seed <= 0) {
						printf("seed is a positive number\n");
						return 1;
					}
					break;
			case 'a':
					array_size = atoi(optarg);
					// your code here
					// error handling
					if (array_size <= 0) {
						printf("array_size is a positive number\n");
						return 1;
					}
					break;
			case 'p':
					pnum = atoi(optarg);
					// your code here
					// error handling
					if (pnum <= 0) {
						printf("pnum is a positive number\n");
						return 1;
					}
					break;
			case 't':
					timeout = atoi(optarg);
					if(timeout <= 0){
					printf("timeout is positive num\n");
					return 1;
					}
					break;


			case '?':
				break;

			default:
				printf("getopt returned character code 0%o?\n", c);
		}
	}

	if (optind < argc) {
		printf("Has at least one no option argument\n");//нет арг выбора
		return 1;
	}

	if (seed == -1 || array_size == -1 || pnum == -1) {
		printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
			argv[0]);
		return 1;
	}

	int *array = malloc(sizeof(int) * array_size);
	GenerateArray(array, array_size, seed);//созд массив

	for(int i=0;i<array_size;i++){

	    //printf("%d \n",array[i]);

	}

//int active_child_processes = 0;

	if (pnum > array_size){
		if(array_size > 3){
			pnum = array_size/2;
		}
		else{
			pnum = 1;
		}
	}

	const char path_to_save[] = "file_desc";


	int file_desc[pnum][2][2];

	if(!with_files){

		for (int i = 0; i < pnum; i++){

			pipe( file_desc[i][0] );
			pipe( file_desc[i][1] );
		}
	}

	if(timeout != 0){
		printf("\nTimeout is %d", timeout);
		printf("Sleep is %d second\n\n", timeout + 1);
		if(signal(SIGALRM, sig_alarm));
		{
			printf("Cannot catch 	sigalarm\n");
		}
		alarm(timeout);
	}

	child_pid = malloc(pnum*sizeof(pid_t));

	struct timeval start_time;
	gettimeofday(&start_time, NULL);
  
	for (int i = 0; i < pnum; i++) {
		pid_t child_pid = fork();//запускаем форк
		if (child_pid >= 0) {
		// successful fork
		active_child_processes += 1;
		if (child_pid == 0) {//если ноль то это дочерний процесс
			// child process
//должны вызвать в-ю гет мин макс записать резкльтаты в структуру

			// parallel somehow

			int begin = i * array_size / pnum;

			int end = (i + 1) * array_size / pnum;
			end = end > array_size ? array_size : end;

		   // printf("%d %d \n",begin,end);
			struct MinMax min_max = GetMinMax(array, begin, end);
            //printf("%d %d \n\n",min_max.min, min_max.max);

			if (with_files) {
		//заблокировать файл чтоб поцессы писали в разное вре мя ф-ей флок(клок хз крч) врайт туда мин
		// макс и
		// флок разблокирем файл и делаем ф клоз 
				char file_name[32];

				sprintf(file_name, "%s%d", path_to_save, i+1);

				FILE *fp = fopen(file_name, "w");

				if(fp != NULL){
				    fprintf(fp, "%d %d", min_max.min, min_max.max);
				    fclose(fp);
			    }


			} else {
			    // use pipe here
		// тоже используем врайт пишем знач мин и макс в фал дескриптор  
		//функцией вейт завершаем
			    close(file_desc[i][0][0]);
			    close(file_desc[i][1][0]);

				write(file_desc[i][0][1], &min_max.min, sizeof(int));
				write(file_desc[i][1][1], &min_max.max, sizeof(int));
			}
			return 0;
		}

		} else {
		printf("Fork failed!\n");
		return 1;
		}
	}

	int status;
	while (active_child_processes > 0) {
		// your code here
// завершаем дочерние процессы вызовом функции вейт 
//
		wait(&status);
		if(!WIFEXITED(status))
		{
			printf("Error in child proc");
			return 1;
		}

		active_child_processes -= 1;
	}

	struct MinMax min_max;
	min_max.min = INT_MAX;
	min_max.max = INT_MIN;


	for (int i = 0; i < pnum; i++) {
		int min = INT_MAX;
		int max = INT_MIN;

		if (with_files) {
		// read from files
		//фсканэфом считываем  то что мы записали и сравнить с мин мак  
			char file_name[32];

			sprintf(file_name, "%s%d", path_to_save, i+1);

			FILE *fp = fopen(file_name, "r");

			if(fp != NULL){
			   fscanf(fp, "%d %d", &min, &max);
		  	   fclose(fp);
			   remove(file_name);
		    }


		} else {
		// read from pipes
		//функциейей рид считываем мин и макс  из файла дескриптора в буфферную пер-ю и сравн
		// с мин и макс 
		    close(file_desc[i][0][1]);
			close(file_desc[i][1][1]);

			read(file_desc[i][0][0], &min, sizeof(int));
			read(file_desc[i][1][0], &max, sizeof(int));

		}

		if (min < min_max.min) min_max.min = min;
		if (max > min_max.max) min_max.max = max;
	}

	struct timeval finish_time;
	gettimeofday(&finish_time, NULL);

	double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
	elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

	free(array);

	printf("Min: %d\n", min_max.min);
	printf("Max: %d\n", min_max.max);
	printf("Elapsed time: %fms\n", elapsed_time);
	fflush(NULL);
	return 0;
}
