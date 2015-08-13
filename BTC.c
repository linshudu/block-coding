# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <conio.h>
void medi();
void blockcode();
void blockdecode();
int pick();
void yuzhi();

static int dat1[256][256]={0};		//����ȫ�ֱ���
int k1;
main()
{
	int k;
	do
	{
		printf("\n\t*********************************");
		printf("\n\t*     1.��ֵ�˲�                *");
		printf("\n\t*     2.�������                *");
		printf("\n\t*     3.��������                *");
		printf("\n\t*     4.Ԥֵ�ָ�                *");
		printf("\n\t*********************************");
		printf("\n\t-------���������ѡ��(1,2,3,4):  ---\b\b\b\b\b");
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

	printf("\t�������ļ�·��: ");		scanf("%s", infile);
	if((in=fopen(infile,"rb"))==NULL)
	{	printf("\t���ܴ򿪸����ļ�!\n");
		exit(0);      
	}
	printf("\t��ֵ���ڴ�СN(����):  ");	scanf("%d",&k1);
	printf("\t1.���δ�  2.ʮ�ִ�  3.'һ'�δ�  4.'|'�δ�:  ");scanf("%d",&k2);
	printf("\t�����������ļ���:   ");		scanf("%s",outfile);	
	out=fopen(outfile, "wb+");
	rewind(in);
	rewind(out);
	while(!feof(in))										//��ȡ����
	{	if(i<256) a1[j][i++]=fgetc(in);
		else 
			{i=0;j++;}
	}

	m1=k1/2;
	for(i=0;i<m1;i++)										//��ͼ����б�Ե����
		for(j=0;j<256+2*m1;j++)
			if(j<m1)			a2[i][j]=a1[0][0];			//��һ����չ
			else if(j<256+m1)	a2[i][j]=a1[0][j-m1];
			else				a2[i][j]=a1[0][255];
	for(i=0;i<256;i++)
		for(j=0;j<256+2*m1;j++)
			if(j<m1)			a2[i+m1][j]=a1[i][0];		//��һ����չ
			else if(j<256+m1)	a2[i+m1][j]=a1[i][j-m1];	//ͼ������
			else				a2[i+m1][j]=a1[i][255];		//���һ����չ
	for(i=0;i<m1;i++)
		for(j=0;j<256+2*m1;j++)	
			if(j<m1)			a2[i+256+m1][j]=a1[255][0];	//���һ����չ
			else if(j<256+m1)	a2[i+256+m1][j]=a1[255][j-m1];
			else				a2[i+256+m1][j]=a1[255][255];
	switch(k2)
	{	case	1:	m2=k1*k1;
					for(i=m1;i<256+m1;i++)
					for(j=m1;j<256+m1;j++)					//ѡ����δ���ֵ�˲�
					{	t=0;
						for(i1=i;i1<i+k1;i1++)
						for(j1=j;j1<j+k1;j1++)
							b[t++]=a2[i1-m1][j1-m1];
						a3[i-m1][j-m1]=pick(b,m2);
					}
					break;
		case	2:	m2=2*k1-1;
					for(i=m1;i<256+m1;i++)					//ѡ��ʮ�ִ���ֵ�˲�
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
					for(i=m1;i<256+m1;i++)					//'һ'�δ���ֵ�˲�
					for(j=m1;j<256+m1;j++)					//�����˳���������
					{	t=0;
						for(j1=j;j1<j+k1;j1++)
							b[t++]=a2[i][j1-m1];
						a3[i-m1][j-m1]=pick(b,m2);
					}
					break;
		case	4:	m2=k1;
					for(i=m1;i<256+m1;i++)					//'|'�δ���ֵ�˲�
					for(j=m1;j<256+m1;j++)					//�����˳���������
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
			if(arry[j]<arry[k])k=j;							//ð�ݷ�����С��������
				t=arry[k];
				arry[k]=arry[i];
				arry[i]=t; 
	}
	m=arry[(n-1)/2];
	return(m);
}

void blockcode()								//�������
{
	FILE   *in,*out;
	char infile[20],outfile[20];
	static int dat2[65536]={0};
	int b[65536]={0};
	int i=0,j=0,t,m,q,k2,a0,a1,hang,lie;
	int m1,m2,yuantu,yasuotu,x;
	float c;

	printf("\t�������ļ�·��:   ");						scanf("%s", infile);
	if((in=fopen(infile,"rb"))==NULL)
	{	printf("\t���ܴ򿪸����ļ�!\n");
		exit(0);      
	}
	printf("\t������봰�ڴ�СN(2��������) :  ");		scanf("%d",&k1);
	printf("\t�����������ļ���:   ");					scanf("%s",outfile);		
	out=fopen(outfile, "wb+");
	rewind(in);
	rewind(out);
	while(!feof(in))										//��ȡ����
	{	if(i<256) dat1[j][i++]=fgetc(in);
		else 
			{i=0;j++;}
	}
	m=k1*k1;k2=0;
	yuantu=256*256;
	x=m/8;
	if(x==0)	x=1;
	yasuotu=(2+x)*(256*256/m);
	c=(float)yuantu/yasuotu;								//cΪ����ѹ����

	hang=0;lie=0;
	for(hang=0;hang<256;hang=hang+k1)
	for(lie=0;lie<256;lie=lie+k1)
	{	t=0;m1=0;m2=0;
		for(i=0+hang;i<k1+hang;i++)								//���ֵ
		for(j=0+lie;j<k1+lie;j++)
			{	b[t++]=dat1[i][j];
				m1=m1+dat1[i][j];
			}
		m1=m1/m;
		
		for(i=0;i<m;i++)										//�󷽲�
				m2+=(b[i]-m1)*(b[i]-m1);
		m2=m2/m;	
		q=0;
		for(i=0;i<m;i++)										//ͳ�Ʒ������q
			if(b[i]>m1)	q++;
		if(q==m||q==0)											//�����������ȫ�����
		{	a0=m1;
			a1=a0;
		}
		else
		{	a0=(int)(m1-sqrt(m2*q/(m-q))+0.5);						//��a0,a1
			if(a0<0)	a0=0;
			a1=(int)(m1+sqrt(m2*(m-q)/m)+0.5);
			if(a1>255)	a1=255;
		}
		dat2[k2++]=a0;
		dat2[k2]=a1;
		for(i=0;i<m;i++)
		{	if(i%8==0)	k2++;						//�趨ÿλ���δ�16λ����
			if(b[i]>m1)	dat2[k2]+=(int)pow(2,i%8);
		}
		k2++;
	}
	rewind(out);
	for(i=0;i<k2;i++)
		fputc(dat2[i],out);
	printf("����������,ѹ����Ϊ: %f\n",c);
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
	int i2,j2,a0,a1,hang,lie,fg,mse;	//fgΪ����MSE���м����
	float PSNR;							//mseΪ�������
	printf("\t�������ļ�·��:   ");						scanf("%s", infile);
	if((in=fopen(infile,"rb"))==NULL)
	{	printf("\t���ܴ򿪸����ļ�!\n");
		exit(0);      
	}
	printf("\t������봰�ڴ�СN(2��������) :  ");		scanf("%d",&k1);

	printf("\t���������������ļ���:   ");				scanf("%s",outfile);		
	out=fopen(outfile, "wb+");
	m=k1*k1;k2=0;										//mΪ���ڵ������С
	t=m/8;												//tΪͼ��������ռ����λ��

	rewind(in);
	rewind(out);
	i=0;j=0;hang=0;lie=0;

	if(t>0)
	while(!feof(in))									//��ȡ����
	{	if(i%(t+2)>1)
		{	k2=fgetc(in);j=0;							//k2Ϊ8λ��ͼ������
			for(j=0;j<8;j++)
			{ k3=k2%2;									//k3Ϊ����������
			  b2[j]=k3;
			  k2=k2/2;
			}
			i++;
			for(k2=0;k2<8;k2++)								//k4Ϊ�������ظ���
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
	//�����жϴ�������λ������8,���ڴ�СΪ2,1ʱ�ĳ���
	if(t==0)
	{	t=1;
	while(!feof(in))									//��ȡ����
	{	if(i%(t+2)>1)
		{	k2=fgetc(in);j=0;							//k2Ϊ8λ��ͼ������
			for(j=0;j<m;j++)
			{ k3=k2%2;									//k3Ϊ����������
			  b2[j]=k3;
			  k2=k2/2;
			}
			i++;
			for(k2=0;k2<m;k2++)								//k4Ϊ�������ظ���
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
		printf("\t��ֵ�����Ϊ: %f\n",PSNR);}
	else	printf("\t���ܼ����ֵ�����\n");
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

	printf("\n***************��ֵ�ָ�****************\n");
	printf(" �����ļ�·��:");
	scanf("%s",filename);
	printf("\n ���������ļ���:");
	scanf("%s",filename1);
	if ((fp=fopen(filename,"rb"))==0)	{printf("���ܴ򿪸��ļ�\n");exit(0);}
	out=fopen(filename1,"wb");

	i=0;j=0;
	while(!feof(fp))					 /* ͳ�Ƹ����ؼ��Ҷȸ��� *///
	{	c=fgetc(fp);					//��ȡ��256*256
		if(j<256) a1[i][j++]=c;
		else {j=0;i++;a1[i][j++]=c;}
		a[c]++;
	}
	
	t0=0;
	for(i=0;i<256;i++)					//��ƽ���Ҷ�
	{	t0=t0+i*a[i];
	}
	t0=t0/65536;						//t0��ʼ��ֵ
	c=9;
	while(c*c>0)
	{	c=0;
		u0=0;u1=0;t=0;
		for(i=0;i<256;i++)
		for(j=0;j<256;j++)	
		{	if(a1[i][j]>t0)
				u1+=a1[i][j];
			else
			{	t++;					//tΪf(x,y)<t0��������
				u0+=a1[i][j];
			}
		}
		u0/=t;
		u1/=65536-t;
		t1=(u0+u1)/2;
		c=t1-t0;						//cΪt0,t1��ֵ
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