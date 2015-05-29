Read more: http://www.studytable.in/2013/03/c-program-to-implement-floodfill.html#ixzz3YfyhjHeU

#include<stdio.h>
#include<conio.h>
#include<math.h>
#include<graphics.h>
4-connected boundary floodfill
4-way floodfill using a stack for storage

void push(int,int);
void pop(int *,int *);
int s[12000][2];
long top=-1;
void main()
{
int gd=DETECT,gm,i,x,y,xi,n,yi,sx,sy,a[10][10];

initgraph(&gd,&gm," ");
setcolor(RED);
printf("\n\n\tEnter the no. of edges of polygon : ");
scanf("%d",&n);
printf("\n\n\tEnter the cordinates of polygon :\n\n\n ");

for(i=0;i<n;i++)

{
printf("\tX%d Y%d : ",i,i);
scanf("%d %d",&a[i][0],&a[i][1]);
}
a[n][0]=a[0][0];
a[n][1]=a[0][1];
printf("enter seed pt:");
scanf("%d\t%d",&sx,&sy);

clrscr();
for(i=0;i<n;i++)
{
line(a[i][0],a[i][1],a[i+1][0],a[i+1][1]);
}
x=sx;
y=sy;
push(x,y);
while(top!=-1)
{
pop(&x,&y);

if(getpixel(x,y)!=WHITE)
putpixel(x,y,WHITE);

if(getpixel(x+1,y)!=WHITE && getpixel(x+1,y)!=RED)
push(x+1,y);

if(getpixel(x,y+1)!=WHITE && getpixel(x,y+1)!=RED)
push(x,y+1);

if(getpixel(x-1,y)!=WHITE && getpixel(x-1,y)!=RED)
push(x-1,y);

if(getpixel(x,y-1)!=WHITE && getpixel(x,y-1)!=RED)
push(x,y-1);
delay(20);
}
getch();
}

void push(int x,int y)
{
top++;
s[top][0]=x;
s[top][1]=y;
}

void pop(int *x,int *y)
{
*x=s[top][0];
*y=s[top][1];
top--;
}