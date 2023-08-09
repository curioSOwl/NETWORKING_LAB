#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define min(x,y) ((x)<(y))?x:y

int rem=0,b_size,rate;


int main(){
    printf("\nEnter the bucket size: ");
    scanf("%d",&b_size);
    printf("\nEnter the output rate: ");
    scanf("%d",&rate);
    int array[100];
    int i=0,size,b=0;
    do{
        printf("\nEnter the size of incoming packet: ");
        scanf("%d",&array[i]);
        i++;
        printf("\nDo you want to continue: (0-YES 1-NO): ");
        scanf("%d",&b);
    }while(b==0);

    for(int k=0;k<i || rem;k++){
        printf("\n%d\t",k+1);
        printf("%d\t",array[k]);

        if(array[k]+rem > b_size){      //drop condition
            printf("%d\t",min(rate,rem));
            printf("%d\t",array[k]);
            if(rate>rem){
                rem=0;
            }
            else{
                rem-=rate;
            }
            printf("%d\t",rem);
            continue;
        }
        else{
            printf("%d\t",min(rate,array[k]+rem));
            printf("0\t");
            if(rate>(array[k]+rem)){
                rem=0;
            }
            else{
                rem=(array[k]+rem)-rate;
            }
            printf("%d\t",rem);
            continue;
        }

    }
    printf("\n");
}