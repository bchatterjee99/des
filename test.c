#include <stdio.h>


int main()
{
    int n;
    scanf("%d", &n);
    printf("aaa\n");
    char ch[100];
    char a;
    scanf("%c", &a);
    for(int i=0; i<n; i++)
    {
	scanf("%c", &ch[i]);
    }
    for(int i=0; i<n; i++)
    {
	printf("%d ", ch[i]);
    }
    return 0;
}
