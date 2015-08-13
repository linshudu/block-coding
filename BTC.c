# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <conio.h>
void medi();
void blockcode();
void blockdecode();
int pick();
void yuzhi();

static int dat1[256][256]={0};		//设置全局变量
int k1;
main()
{
	int k;
	do
	{
		printf("\n\t*********************************");
		printf("\n\t*     1.中值滤波                *");
		printf("\n\t*     2.方块编码                *");
		printf("\n\t*     3.方块译码                *");
		printf("\n\t*     4.预值分割                *");
		printf("\n\t*********************************");
		printf("\n\t-------请输入你的选择(1,2,3,4):  ---\b\b\b\b\b");
		scanf("%d",&k);
		system("cls");
		switch(k)
		{
		case 1:	medi();
				break;
		case 2:	blockcode();
				break;
		case 3:	blockdecode();
				break;
		case 4:	yuzhi();
				break;
		default:exit(0);
		}
		printf("\n******************************");
	}while(k>=1&&k<=4);
}

void medi()
{
	FILE   *in,*out;
	char infile[20],outfile[20];
	static int a1[256][256],a2[512][512],a3[256][256];
	int b[65536];
	int i=0,j=0,t,m1,m2,k1,k2,i1,j1;

	printf("\t请输入文件路径: ");		scanf("%s", infile);
	if((in=fopen(infile,"rb"))==NULL)
	{	printf("\t不能打开该类文件!\n");
		exit(0);      
	}
	printf("\t中值窗口大小N(奇数):  ");	scanf("%d",&k1);
	printf("\t1.矩形窗  2.十字窗  3.'一'形窗  4.'|'形窗:  ");scanf("%d",&k2);
	printf("\t请输入生成文件名:   ");		scanf("%s",outfile);	
	out=fopen(outfile, "wb+");
	rewind(in);
	rewind(out);
	while(!feof(in))										//读取数据
	{	if(i<256) a1[j][i++]=fgetc(in);
		else 
			{i=0;j++;}
	}

	m1=k1/2;
	for(i=0;i<m1;i++)										//对图像进行边缘处理
		for(j=0;j<256+2*m1;j++)
			if(j<m1)			a2[i][j]=a1[0][0];			//第一行扩展
			else if(j<256+m1)	a2[i][j]=a1[0][j-m1];
			else				a2[i][j]=a1[0][255];
	for(i=0;i<256;i++)
		for(j=0;j<256+2*m1;j++)
			if(j<m1)			a2[i+m1][j]=a1[i][0];		//第一列扩展
			else if(j<256+m1)	a2[i+m1][j]=a1[i][j-m1];	//图像数据
			else				a2[i+m1][j]=a1[i][255];		//最后一列扩展
	for(i=0;i<m1;i++)
		for(j=0;j<256+2*m1;j++)	
			if(j<m1)			a2[i+256+m1][j]=a1[255][0];	//最后一行扩展
			else if(j<256+m1)	a2[i+256+m1][j]=a1[255][j-m1];
			else				a2[i+256+m1][j]=a1[255][255];
	switch(k2)
	{	case	1:	m2=k1*k1;
					for(i=m1;i<256+m1;i++)
					for(j=m1;j<256+m1;j++)					//选择矩形窗中值滤波
					{	t=0;
						for(i1=i;i1<i+k1;i1++)
						for(j1=j;j1<j+k1;j1++)
							b[t++]=a2[i1-m1][j1-m1];
						a3[i-m1][j-m1]=pick(b,m2);
					}
					break;
		case	2:	m2=2*k1-1;
					for(i=m1;i<256+m1;i++)					//选择十字窗中值滤波
					for(j=m1;j<256+m1;j++)
					{	t=0;
						for(i1=i;i1<i+k1;i1++)
						for(j1=j;j1<j+k1;j1++)
						{	if(i1<i+m1)	
							if(j1==j+m1)	b[t++]=a2[i1-m1][j1-m1];
							if(i1==i+m1)	b[t++]=a2[i1-m1][j1-m1];
							if(i1>i+m1)
							if(j1==j+m1)	b[t++]=a2[i1-m1][j1-m1];	
						}
						a3[i-m1][j-m1]=pick(b,m2);
					}
					break;
		case	3:	m2=k1;
					for(i=m1;i<256+m1;i++)					//'一'形窗中值滤波
					for(j=m1;j<256+m1;j++)					//可以滤除纵向噪声
					{	t=0;
						for(j1=j;j1<j+k1;j1++)
							b[t++]=a2[i][j1-m1];
						a3[i-m1][j-m1]=pick(b,m2);
					}
					break;
		case	4:	m2=k1;
					for(i=m1;i<256+m1;i++)					//'|'形窗中值滤波
					for(j=m1;j<256+m1;j++)					//可以滤除横向噪声
					{	t=0;
						for(i1=i;i1<i+k1;i1++)
						for(j1=j;j1<j+k1;j1++)
						{	if(j1==j+m1)
								b[t++]=a2[i1-m1][j1-m1];
							a3[i-m1][j-m1]=pick(b,m2);
						}
					}
					break;
		default	:	exit(0);
	}
	rewind(out);
	for(i=0;i<256;i++)
		for(j=0;j<256;j++)
			fputc(a3[i][j],out);
	fclose(out);
	fclose(in);
}

int pick(int arry[],int n )
{
	int i,j,k,t,m;
	for(i=0;i<n-1;i++)
	{	k=i;
		for(j=i+1;j<n;j++)
			if(arry[j]<arry[k])k=j;							//冒泡法按从小到大排序
				t=arry[k];
				arry[k]=arry[i];
				arry[i]=t; 
	}
	m=arry[(n-1)/2];
	return(m);
}

void blockcode()								//方块编码
{
	FILE   *in,*out;
	char infile[20],outfile[20];
	static int dat2[65536]={0};
	int b[65536]={0};
	int i=0,j=0,t,m,q,k2,a0,a1,hang,lie;
	int m1,m2,yuantu,yasuotu,x;
	float c;

	printf("\t请输入文件路径:   ");						scanf("%s", infile);
	if((in=fopen(infile,"rb"))==NULL)
	{	printf("\t不能打开该类文件!\n");
		exit(0);      
	}
	printf("\t方块编码窗口大小N(2的整次幂) :  ");		scanf("%d",&k1);
	printf("\t请输入生成文件名:   ");					scanf("%s",outfile);		
	out=fopen(outfile, "wb+");
	rewind(in);
	rewind(out);
	while(!feof(in))										//读取数据
	{	if(i<256) dat1[j][i++]=fgetc(in);
		else 
			{i=0;j++;}
	}
	m=k1*k1;k2=0;
	yuantu=256*256;
	x=m/8;
	if(x==0)	x=1;
	yasuotu=(2+x)*(256*256/m);
	c=(float)yuantu/yasuotu;								//c为计算压缩比

	hang=0;lie=0;
	for(hang=0;hang<256;hang=hang+k1)
	for(lie=0;lie<256;lie=lie+k1)
	{	t=0;m1=0;m2=0;
		for(i=0+hang;i<k1+hang;i++)								//求均值
		for(j=0+lie;j<k1+lie;j++)
			{	b[t++]=dat1[i][j];
				m1=m1+dat1[i][j];
			}
		m1=m1/m;
		
		for(i=0;i<m;i++)										//求方差
				m2+=(b[i]-m1)*(b[i]-m1);
		m2=m2/m;	
		q=0;
		for(i=0;i<m;i++)										//统计非零个数q
			if(b[i]>m1)	q++;
		if(q==m||q==0)											//如果块内像素全部相等
		{	a0=m1;
			a1=a0;
		}
		else
		{	a0=(int)(m1-sqrt(m2*q/(m-q))+0.5);						//求a0,a1
			if(a0<0)	a0=0;
			a1=(int)(m1+sqrt(m2*(m-q)/m)+0.5);
			if(a1>255)	a1=255;
		}
		dat2[k2++]=a0;
		dat2[k2]=a1;
		for(i=0;i<m;i++)
		{	if(i%8==0)	k2++;						//设定每位整形传16位数据
			if(b[i]>m1)	dat2[k2]+=(int)pow(2,i%8);
		}
		k2++;
	}
	rewind(out);
	for(i=0;i<k2;i++)
		fputc(dat2[i],out);
	printf("方块编码完成,压缩比为: %f\n",c);
	fclose(out);
	fclose(in);
}

void blockdecode()
{
	FILE   *in,*out;
	char	infile[20],outfile[20];
	static int dat3[256][256]={0};
	int b[65536]={0},b2[32]={0};
	int i=0,j=0,t,m,k1,k2,k3,k4;
	int i2,j2,a0,a1,hang,lie,fg,mse;	//fg为计算MSE的中间变量
	float PSNR;							//mse为均方误差
	printf("\t请输入文件路径:   ");						scanf("%s", infile);
	if((in=fopen(infile,"rb"))==NULL)
	{	printf("\t不能打开该类文件!\n");
		exit(0);      
	}
	printf("\t方块解码窗口大小N(2的整次幂) :  ");		scanf("%d",&k1);

	printf("\t请输入译码生成文件名:   ");				scanf("%s",outfile);		
	out=fopen(outfile, "wb+");
	m=k1*k1;k2=0;										//m为窗口的数组大小
	t=m/8;												//t为图像数据所占数组位数

	rewind(in);
	rewind(out);
	i=0;j=0;hang=0;lie=0;

	if(t>0)
	while(!feof(in))									//读取数据
	{	if(i%(t+2)>1)
		{	k2=fgetc(in);j=0;							//k2为8位长图像数据
			for(j=0;j<8;j++)
			{ k3=k2%2;									//k3为二进制数据
			  b2[j]=k3;
			  k2=k2/2;
			}
			i++;
			for(k2=0;k2<8;k2++)								//k4为窗口像素个数
				b[k4++]=b2[k2];
		}
		else
		{	k4=0;
			a0=fgetc(in);
			a1=fgetc(in);
			i+=2;
		}
		if(k4==m)
		{	k4=0;
			for(i2=0+hang;i2<k1+hang;i2++)
			for(j2=0+lie;j2<k1+lie;j2++)
			{	if(b[k4]==1)	dat3[i2][j2]=a1;
				else			dat3[i2][j2]=a0;
				k4++;
			}
			if(hang<256)
				if(lie+k1<256)	lie+=k1;
				else			{hang+=k1;lie=0;}
			k4=0;
		}
	}
	//这是判断窗口数据位数少于8,窗口大小为2,1时的程序
	if(t==0)
	{	t=1;
	while(!feof(in))									//读取数据
	{	if(i%(t+2)>1)
		{	k2=fgetc(in);j=0;							//k2为8位长图像数据
			for(j=0;j<m;j++)
			{ k3=k2%2;									//k3为二进制数据
			  b2[j]=k3;
			  k2=k2/2;
			}
			i++;
			for(k2=0;k2<m;k2++)								//k4为窗口像素个数
				b[k4++]=b2[k2];
		}
		else
		{	k4=0;
			a0=fgetc(in);
			a1=fgetc(in);
			i+=2;
		}
		if(k4==m)
		{	k4=0;
			for(i2=0+hang;i2<k1+hang;i2++)
			for(j2=0+lie;j2<k1+lie;j2++)
			{	if(b[k4]==1)	dat3[i2][j2]=a1;
				else			dat3[i2][j2]=a0;
				k4++;
			}
			if(hang<256)
				if(lie+k1<256)	lie+=k1;
				else			{hang+=k1;lie=0;}
			k4=0;
		}
	}
	}
	rewind(out);
	mse=0;
	for(i=0;i<256;i++)
		for(j=0;j<256;j++)
		{	fg=dat1[i][j]-dat3[i][j];
			mse+=fg*fg;
			fputc(dat3[i][j],out);
		}
	mse/=65536;
	if(mse!=0)
	{	PSNR=log(255*255/mse)/log(10);
		PSNR*=10;
		printf("\t峰值信噪比为: %f\n",PSNR);}
	else	printf("\t不能计算峰值信噪比\n");
	rewind(in);rewind(out);
	fclose(out);
	fclose(in);
}

void yuzhi()
{	int c,i,j,u0,u1,t,t0,t1;
	static int a[256],h[256],a1[256][256];
	FILE *fp;
	FILE *out;
	char filename[20],filename1[20];

	printf("\n***************阈值分割****************\n");
	printf(" 输入文件路径:");
	scanf("%s",filename);
	printf("\n 输入生成文件名:");
	scanf("%s",filename1);
	if ((fp=fopen(filename,"rb"))==0)	{printf("不能打开该文件\n");exit(0);}
	out=fopen(filename1,"wb");

	i=0;j=0;
	while(!feof(fp))					 /* 统计个像素级灰度个数 *///
	{	c=fgetc(fp);					//读取成256*256
		if(j<256) a1[i][j++]=c;
		else {j=0;i++;a1[i][j++]=c;}
		a[c]++;
	}
	
	t0=0;
	for(i=0;i<256;i++)					//求平均灰度
	{	t0=t0+i*a[i];
	}
	t0=t0/65536;						//t0初始阈值
	c=9;
	while(c*c>0)
	{	c=0;
		u0=0;u1=0;t=0;
		for(i=0;i<256;i++)
		for(j=0;j<256;j++)	
		{	if(a1[i][j]>t0)
				u1+=a1[i][j];
			else
			{	t++;					//t为f(x,y)<t0的像素数
				u0+=a1[i][j];
			}
		}
		u0/=t;
		u1/=65536-t;
		t1=(u0+u1)/2;
		c=t1-t0;						//c为t0,t1差值
		if(c*c>0)
			t0=t1;
	}
	rewind(out);
	for(i=0;i<256;i++)
	for(j=0;j<256;j++)
	{	if(a1[i][j]<=t0)	a1[i][j]=255;
		fputc(a1[i][j],out);
	}
	fclose(fp);
	fclose(out);
}