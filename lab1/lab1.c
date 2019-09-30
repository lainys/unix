#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int arc, char **argv) {
    int fd[2];
	pipe(fd);
	if(fork()) { //родитель
	    close(fd[1]);
	    FILE *f = fopen("text","a");
	    if (f == NULL){
	    	printf("Can't open file");
	    	exit(0);
	    }else{
	    	char c = 0;
    		while(read(fd[0],&c,1) > 0){
    			fprintf(f,"%c",c);
    		}
	    }
	    close(fd[0]);
	    fclose(f);
	} else { //дочерний
		close(fd[0]);
		char str[10];
		scanf("%s",&str);
		while(strcmp(str,"0") != 0){
			char c = 0;
			while(str[c] > 0){
				write(fd[1],&str[c],1);
				c++;
			}
			write(fd[1],&("\n"),1);
			scanf("%s",&str);
		}
		
		close(fd[1]);
	}
}
