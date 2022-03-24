#include<stdio.h> 
#include<stdlib.h>
#include<fcntl.h> 
#include <unistd.h>
#include<errno.h> 
#include<string.h>
extern int errno;

//convert character array to integer array with space as delimiter
int* convert(char* str, int* size)
{
    	int i=0;
	int j=0;
	int k=0;
	int* arr;
	while(str[i]!='\0')
	{
		if(str[i]==' ')
		{
			i++;
		}
		else
		{
			j=i;
			while(str[i]!=' ' && str[i]!='\0')
			{
				i++;
			}
			arr[k]=atoi(&str[j]);
			k++;
		}
	}
	*size=k;
	return arr;

}

//comparison function
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}



int main() {
    //reading data from f1.txt
    int fd1 = open("f1.txt", O_RDONLY);


    char buff[100];


    int x = read(fd1, buff, 100);


    int size = 0;
    int *arr;

    arr = convert(buff, &size);

    close(fd1);


    

    //reading data from f2.txt
    int fd2 = open("f2.txt", O_RDONLY);
    char buff2[100];
    read(fd2, buff2, 100);
    int size2 = 0;
    int *arr2;
    arr2 = convert(buff2, &size2);
    printf("\n");
    close(fd2);

    
    // //concatenating two arrays and sorting
    int arr3[200];
    for(int i = 0; i< size+size2 ;i++) {
        if(i < size){
            arr3[i] = arr[i];
        }
        else{
            arr3[i] = arr2[i-size];
        }
    }
    qsort(arr3, size + size2, sizeof(int), cmpfunc);


    //put arr3 in f3.txt
    int fd3 = open("f3.txt", O_WRONLY | O_CREAT, 0644);

    char buff3[100];
    
    for(int i = 0; i< size+size2 ;i++) {
        sprintf(buff3, "%d ", arr3[i]);
        write(fd3, buff3, strlen(buff3));
    }
    close(fd3);

    exit(0);
}