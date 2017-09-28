#include "stdio.h"
#include "iostream"
using namespace std;

#include "math.h"
#include "string.h"
#include "windows.h"

/******************************************************ȫ�ֱ�������*******************************************************/
int node_sum, phd;					//�ڵ�������ƽ��ڵ��
double uph, e;						//ƽ����ѹ�����㾫��
int kk;								//��Ϊ��ʶ���������Ƿ���������·�����ϵ��ӣ�������ӾͲ��ÿ��½ڵ���
double v[1500] = { 0 };						//��ѹ��ֵ
double rad[1500] = { 0 };			//��ѹ���
double radv[3000] = { 0 };			//����Ϊ ��� ż��Ϊ ��ѹ��ֵ
double err = 0;						//���
int pvyjbz[1500] = { 0 };			//pv��Խ���־
double p[1500] = { 0 };			// Pi
double q[1500] = { 0 };			// Qi
double gmin[1500] = { 0 };
double gmax[1500] = { 0 };

/*******************************************************��������********************************************************/
double readdata();					//���ݶ�ȡ�봦��ֱ��������ʽ��������Y��
char option();
struct Yz *insert1(struct Yz * , struct Yz * );
struct Ycb *insert2(struct Ycb * , struct Ycb * );
struct Ycb *insert3(struct Ycb * , struct Ycb * );
void ycbmatrix( struct Yz *[] );
void ycbdata( struct Yz *[] );
void luss();
void fixradv();
void deltapq();
void fixphdpvd();
void deletepvd(int);
void result();
void finaldeal();

/*******************************************************�ṹ�嶨��******************************************************/
struct Ygb		// Y��ֱ�����꣩����
{
	int row, lnxt;			//����
	double g, b;
	struct Ygb *next;
} *ygb[50] = { NULL };

struct Ygbd 						//Y��Խ�Ԫ�أ�ֱ�����꣩
{
	double g, b;
} ygbd[50] = { 0 };

struct Yz		// Y�󣨷�ֵ��ǣ�����
{
	int row, lnxt;			//����
	double y, rad;
	struct Yz *next;
} *y[50] = { NULL };

struct Yd 						//Y��Խǣ���ֵ��ǣ�Ԫ��
{
	double y, rad;
} yd[50] = { 0 };

struct Ycb
{
	int row, lnxt;
	double zh;
	struct Ycb *next;
} *ycb[100] = { NULL };

struct pqd
{
	int node;
	double p, q;
	struct pqd *next;
};			//pd������
struct pqd *headpqd;

struct pvd
{
	int node;
	double vi, qmin, qmax;
	struct pvd *next;
};			//pd������
struct pvd *headpvd;

/*******************************************************�������ٴ˴�********************************************************/
void main()
{
	char c;						// record input choice
	int ynum = 0;				// ͳ��y�������������Ľڵ�ţ�Ϊ�˽�Լ�ռ䣬ֻ��¼�����������ݣ������ԳƲ���û�м�¼��

	opt: c = option();			// opt��ָ֧�������溯��

	if (c == '1')				//����1ʱ���г�������
		goto opt1;

	else if(c=='2')
	{
	system( "mode con:cols=100 lines=30 & color 07" );

		system("cls");
		printf("         ################################��������ѯ################################\n");
		printf("\n\n");
		result();
		goto opt;
	}

	else if (c == '0')
	{
		char ext;
		printf("\tȷ���˳��� --��[Y]ȷ�ϣ�������ȡ���� ");
		fflush(stdin);
		ext = getchar();
		if (ext == 'Y' || ext == 'y')
		{
			printf("\n\t�����˳�����.");
			Sleep(333);
			printf(".");
			Sleep(333);
			printf(".");
			Sleep(333);
			exit(0);
		}
		else
			goto opt;
	}

	else
	{
		printf("\t\t\t������ָ����������䣡������\n");
		Sleep(1000);
		goto opt;
	}

	opt1 : e = readdata();

	deltapq();						//��ƽ������P ��Q ������radv[]��ͳ��err

	int loop = 0;					//ѭ������
	while( err > e && loop < 20 )
	{
		err = 0;

		ycbmatrix(y);
		ycbdata(y);
		luss();				// LU �ֽ�õ� ���� ��V ������radv[]

		fixradv();			// ����������������� �� V ������ rad[] v[]
		//fixphdpvd();		//�µ� phd��pvd �� Pi Qi ������ p[i] q[i]

		deltapq();			//��ƽ������P ��Q ������radv[]��ͳ��err
		loop++;
	}


	finaldeal();
/*******************************************�ļ����**********************************/
	if(loop >= 20)
	{
		printf("\t\t\t�������������㲻������������\n");
		Sleep(1000);
	}

	FILE *fp;
	if((fp=fopen("solution.txt","w")) == NULL)
	{
	printf("�ļ�д����󣡣�");
	Sleep(1000);
	exit(0);
	}
	fprintf(fp , "Node Amount:%d\n" , node_sum);
	if(loop >= 20)
	{
		printf("\t\t\t�������������㲻������������\n");
		goto end;
	}

	fprintf(fp, "Converged, Iterations:%d\n", loop);
	fprintf(fp, "Node,    Voltage,    Radian,    P,    Q\n");
	fprintf(fp, "-\n");
	for( int i = 1 ; i <= node_sum ; i++ )
		fprintf(fp , "%d,  %f,  %f,  %f,  %f\n" , i , v[i] , rad[i] , p[i] , q[i]);


	printf("\n\t\t\t�����������!\n");
end:fclose(fp);
	Sleep(500);
	printf("\n\t\t\t�ļ�д�����!\n");
	printf("\t\t\t��[0]�˳�����\n");
	printf("\t\t\t��[b]�˻�������\n");


	while (1)
	{
		c = getchar();
		if (c == '0')
			break;
		if (c == 'b' || c == 'B')
			goto opt;
	}
}



/********************************************* ������phd pvd��ֵ *************************************/
void finaldeal()
{
	double x = 0, r = 0;
	struct Yz *p1 = NULL;								//ƽ��㴦��
	p1 = y[phd];

	while (p1 != NULL)
	{
		x = x + p1->y * v[p1->lnxt] * cos(rad[phd] - rad[p1->lnxt] - p1->rad);
		r = r + p1->y * v[p1->lnxt] * sin(rad[phd] - rad[p1->lnxt] - p1->rad);
		p1 = p1->next;
	}
	/******************�Ƿ�Ҫ�ѶԽ�Ԫ�ص������ǣ�**************************/
	x = x + ygbd[phd].g *  v[phd] * cos(rad[phd] - rad[phd] - ygbd[phd].b);
	r = r + ygbd[phd].g *  v[phd] * sin(rad[phd] - rad[phd] - ygbd[phd].b);
	p[phd] = x * v[phd];
	q[phd] = r * v[phd];

	//*****************************PV�㴦��p2Ϊ pvd ����***************************

	struct pvd *p2 = headpvd;
	struct Yz *p3 = NULL;

	while (p2 != NULL)
	{
		r = 0;
		p3 = y[p2->node];		// p2Ϊ pvd ����, ȡ��ڵ�ţ��õ��������ͷ
		while (p3 != NULL)
		{
			r = r + p3->y * v[p3->lnxt] * sin(rad[p3->row] - rad[p3->lnxt] - p3->rad);
			p3 = p3->next;
		}
		p2 = p2->next;
	}
}

/****************************************************���˵�ѡ��ҳ��*********************************************************/
char option()
{
	char c;
	system("mode con:cols=75 lines=25 &color 70");
	puts("\n");
	puts("      ***************************************************************");
	puts("		          ��������γ����		");
	puts("      ***************************************************************\n\n");
	printf("\t����ָ�\n\n\t\t1.��������      2.�������鿴      0.�˳�����\n\n");
	fflush(stdin);
	printf("\t����ָ�");
	c = getchar();
	return c;
}

/****************************************************�����ѯҳ��*********************************************************/
void result()
{
	int ch;
	char cint = NULL;
	FILE *fp;
	system("mode con:cols=75 lines=25 &color 70");
	puts("\n\n");
	puts("���������ڵ�ţ�");
	
	cin >> ch;
	
	if (0 < ch && ch <32767)
	{
		
	}
	else
	{
		printf("\t����������Ľڵ�ţ�������\n");
		printf("\n\t��[b]���������棺");
		while (1)
		{
			cint = NULL;
			cint = getchar();
			if (cint == 'b' || cint == 'B')
				return;
		}
	}

	if((fp = fopen("solution.txt", "rb")) == NULL)
	{
		printf("\n\t�������ļ���ʧ������");
		Sleep(1000);
		exit(0);
	}


	while (cint != '-')
		fscanf(fp, "%c", &cint);

	printf("\n\n\t�ڵ�      ��ѹ      ���          P          Q\n\n");

	int node = 0;
	double volt, degree, P, Q;

	while (node != ch )
	{
		fscanf(fp, "%d,  %lf,  %lf,  %lf,  %lf", &node, &volt, &degree, &P, &Q);
	}
	printf("\t%d   || %.5lf || %.5lf || %.5lf || %.5lf", node, volt, degree, P, Q);
	
	printf("\n\n\n\n\t��[0]�˳�����");
	printf("\n\t��[b]���������棺");
	while (1)
	{
		cint = getchar();
		if (cint == 'b' || cint == 'B')
			break;
		if (cint == '0' )
			exit(0);
	}

	fclose(fp);
}

/***************************************************��ȡԴ�ļ�����**********************************************************/
double readdata()
{
	FILE *fp;
	int i, j, m;
	char  ch;
	double r, x, bb, ee, k0;
	double gij, bij;
	char fsource[20];

	printf("\n		�����볱������������ļ�\n		--");
	getf: fflush(stdin);
	gets(fsource);
	if ((fp = fopen(fsource, "rb")) == NULL)
	{
		printf("\t�޴��ļ���  --��[Y]�������룬�������˳�:");
		fflush(stdin);
		ch = getchar();
		if (ch == 'Y' || ch == 'y')
		{
			printf("\n\t\t��������������ļ���");
			goto getf;
		}
		else
			printf("\n\t��ȷ�������ļ����ں��������������򣡣���");
		Sleep(1000);
		exit(0);
	}

	fscanf(fp, "%d,%d,%lf,%lf", &node_sum, &phd, &uph, &ee);		//1���ڵ�������ƽ��ڵ�ţ�ƽ����ѹ�����㾫��
	fscanf(fp, "%d", &m);									//�����м��0
	fscanf(fp, "%d", &m);									//�����ж��Ƿ�������

	for (int ii = 1; ii <= node_sum; ii++)
		v[ii] = 1;

	/***********************2����ȥ�Խǵĵ�������Խ�Ԫ��**************************/
	struct Yz *p00, *p11;
	struct Yz *yz_tail = y[1];
	p00 = new Yz;				//ֱ�Ӱ����������Ϊ��ֵ��ǣ���������ϰ벿��
	p11 = new Yz;				//ֱ�Ӱ����������Ϊ��ֵ��ǣ�i j�ߵ�����Ϊ��������°벿��
	while (m != 0)
	{
		fscanf(fp, ",%d,%d,%lf,%lf,%lf", &i, &j, &r, &x, &bb);		// ��·����
		gij = r / (r*r + x*x);
		bij = -x / (r*r + x*x);

		p00->row = i;
		p00->lnxt = j;
		p00->y = sqrt(gij * gij + bij * bij);
		p00->rad = atan2( - bij , - gij );
		y[i] = insert1(y[i], p00);
		if (kk == 0)
			p00 = new Yz;				//ֱ�Ӱ����������Ϊ��ֵ��ǣ���������ϰ벿��

		p11->row = j;
		p11->lnxt = i;
		p11->y = sqrt(gij * gij + bij * bij);
		p11->rad = atan2(- bij, - gij);
		y[j] = insert1(y[j], p11);
		if (kk == 0)
			p11 = new Yz;				//ֱ�Ӱ����������Ϊ��ֵ��ǣ�i j�ߵ�����Ϊ��������°벿��

		ygbd[i].g += gij;
		ygbd[i].b += bij + bb;
		ygbd[j].g += gij;
		ygbd[j].b += bij + bb;
		fscanf(fp, "%d", &m);
	}
	delete p00, p11;

	/****************************************3��������·�м��ѹ��*********************************/
	fscanf(fp, "%d", &m);
	double x0, b0;
	p00 = new Yz;
	p11 = new Yz;
	while (m != 0)
	{
		fscanf(fp, ",%d,%d,%lf,%lf,%lf", &i, &j, &r, &x, &k0);
		gij = r / (r*r + x*x);
		bij = -x / (r*r + x*x);
		x0 = -gij / k0;
		b0 = -bij / k0;

		p00->row = i;
		p00->lnxt = j;
		p00->y = sqrt( x0 * x0 + b0 * b0 );
		p00->rad = atan2( b0 , x0 );
		y[i] = insert1( y[i] , p00 );
		if (kk == 0)
			p00 = new Yz;

		p11->row = j;
		p11->lnxt = i;
		p11->y = sqrt(x0 * x0 + b0 * b0);
		p11->rad = atan2(b0, x0);
		y[j] = insert1( y[j] , p11 );
		if (kk == 0)
			p11 = new Yz;

		ygbd[i].g += gij / k0 / k0;
		ygbd[i].b += bij / k0 / k0;
		ygbd[j].g += gij;										//��ѹ���ѹ��������
		ygbd[j].b += bij;
		fscanf(fp, "%d", &m);
	}
	delete p00, p11;

	/******************************4�����Ͻӵ�֧·�Ե�***************************/
	fscanf(fp, "%d", &m);
	while (m != 0)
	{
		fscanf(fp, ",%d,%lf,%lf", &i, &r, &x);
		ygbd[i].g += r;
		ygbd[i].b += x;
		fscanf(fp, "%d", &m);
	}

	for (int ii = 1; ii <= node_sum; ii++)
	{
		r = sqrt(ygbd[ii].g * ygbd[ii].g + ygbd[ii].b * ygbd[ii].b);
		ygbd[ii].b = atan2(ygbd[ii].b , ygbd[ii].g);					//��Ϊ�Խ��ߵ����
		ygbd[ii].g = r;													//��Ϊ�Խ��ߵķ�ֵ
	}

	j = 0;
	for (int ii = 1; ii <= node_sum; ii++)								//�жϹµ�
	{
		if(ygbd[ii].g == 0)
			j++;
	}
	if(j > 0)
	{
		printf("\n\t\t\t������ϵͳ�д��ڹµ㣡������" );
		printf("\n\t\t\t������������������򣡡�����");
		Sleep(3000);
		exit(0);
	}


	/*
	for (int ii = 1; ii <= node_sum; ii++)
	{
		p00 = new Yz;
		p00->row = ii;
		p00->lnxt = ii;
		p00->y = sqrt( ygbd[ii].g * ygbd[ii].g + ygbd[ii].b * ygbd[ii].b );
		p00->rad = atan2( ygbd[ii].b , ygbd[ii].g );
		y[ii] = insert1(y[ii], p00);
	}
	delete p00;
	*/

	/********************************5����ȡ���нڵ㹦������***************************/
	fscanf(fp, "%d", &m);
	/*	struct pqd *p1;
	struct pqd *tailpqd = headpqd;*/
	while (m != 0)
	{
		fscanf(fp, ",%d,%lf,%lf,%lf,%lf", &i, &r, &x, &bb, &k0);

		p[i] = r - bb;						// Pi
		q[i] = x - k0;						// Qi

	/*	p1 = new pqd;
		p1->node = i;
		p1->p = r - bb;
		p1->q = x - k0;

		if (headpqd == NULL)
			headpqd = p1;
		else
			tailpqd->next = p1;
		tailpqd = p1;*/
		fscanf(fp, "%d", &m);
	}
	//tailpqd->next = NULL;

	/********************************6����ȡPV�ڵ�����***************************/
	fscanf(fp, "%d", &m);
	struct pvd *p2;
	struct pvd *tailpvd = headpvd;
	while (m != 0)
	{
		fscanf(fp, ",%d,%lf,%lf,%lf", &i, &r, &x, &bb);
		p2 = new pvd;
		p2->node = i;
		p2->vi = r;
		p2->qmin = x;
		p2->qmax = bb;

		gmax[i] = bb;
		gmin[i] = x;
		//pvyjbz[i] = 0;					//pv��Խ���־

		v[i] = p2->vi;

		if (headpvd == NULL)
			headpvd = p2;
		else
			tailpvd->next = p2;
		tailpvd = p2;
		fscanf(fp, "%d", &m);
	}
	p[phd] = 0;
	q[phd] = 0;
	radv[2 * phd - 1] = 0;		// phd, ��Pi  ��Qi = 0
	radv[2 * phd] = 0;
	tailpvd->next = NULL;
	fclose(fp);

	return ee;
}

/***************************************************�γ��ſɱȾ���ϡ��������ţ�*******************************************/
void ycbmatrix(struct Yz *tp[ ])
{
	struct Yz *p1;
	struct Ycb *p3, *p4;
	int i, j;

	for( i = 1 ; i <= node_sum ; i++ )		//��ֻ�Ƕ�λ�ã�
	{
		p1 = tp[i];
		j = 0;								//���б�־
		if( p1 != NULL )
		{
			while(( p1->row == i ) && ( j == 0 ))
			{
				p3 = new Ycb;					//Hij,Nij
				p4 = new Ycb;
				p3->row = p1->row * 2 - 1;
				p4->row = p1->row * 2 - 1;
				p3->lnxt = p1->lnxt * 2 - 1;
				p4->lnxt = p1->lnxt * 2;
				p3->zh = 1;
				p4->zh = 1;
				p3->next = NULL;
				p4->next = NULL;
				ycb[p3->lnxt] = insert2( ycb[p3->lnxt] , p3 );
				ycb[p4->lnxt] = insert2( ycb[p4->lnxt] , p4 );
				if( p1->next == NULL )
					j = 1;
				else
					p1 = p1->next;
			}
		}

		j = 0;									//���б�־
		p1 = tp[i];
		if( p1 != NULL )
		{
			while(( p1->row == i ) && ( j == 0 ))
			{
				p3 = new Ycb;					//Jij,Lij
				p4 = new Ycb;
				p3->row = p4->row = p1->row * 2;
				p3->lnxt = p1->lnxt * 2 - 1;
				p4->lnxt = p1->lnxt * 2;
				p3->zh = 1;
				p4->zh = 1;
				p3->next = NULL;
				p4->next = NULL;
				ycb[p3->lnxt] = insert2( ycb[p3->lnxt] , p3 );
				ycb[p4->lnxt] = insert2( ycb[p4->lnxt] , p4) ;
				if( p1->next == NULL )
					j = 1;
				else
					p1 = p1->next;
			}
		}
		/***************************************�Խ�Ԫ�ص�������******************************/
		p3 = new Ycb;							//Hii
		p3->row = 2 * i - 1;
		p3->lnxt = 2 * i - 1;
		p3->zh = 1;
		p3->next = NULL;
		ycb[p3->lnxt] = insert2( ycb[p3->lnxt] , p3 );

		p3 = new Ycb;							//Lii
		p3->row = 2 * i;
		p3->lnxt = 2 * i;
		p3->zh = 1;
		p3->next = NULL;
		ycb[p3->lnxt] = insert2( ycb[p3->lnxt] , p3 );

		p3 = new Ycb;							//Nii
		p3->row = 2 * i - 1;
		p3->lnxt = 2 * i;
		p3->zh = 1;
		p3->next = NULL ;
		ycb[p3->lnxt] = insert2( ycb[p3->lnxt] , p3 );

		p3 = new Ycb;							//Jii
		p3->row = 2 * i;
		p3->lnxt= 2 * i - 1;
		p3->zh = 1;
		p3->next = NULL;
		ycb[p3->lnxt] = insert2( ycb[p3->lnxt] , p3 );
	}
}

/***************************************************�γ��ſɱȾ���********************************************************/
void ycbdata(struct Yz *tp[ ])
{
	struct Yz *p1;
	struct Ycb *p3;
	int i, j, n, k;
	double x;

	for( i = 1 ; i <= node_sum ; i++ )
	{
		p3 = ycb[ 2 * i - 1 ];				// �� H J ���н��д���
		while( p3 != NULL )
		{
			j = p3->row;					// j jcb[]���к�
			n = ( j + 1 ) / 2;						// int nΪ �ڵ㵼�ɾ��� Y���кţ�i���к�

			if( fmod( j , 2 ) != 0 )				//������ΪH
			{
				if( n == i )							//**************** ��ʾHii *******************
				{
					p1 = tp[n];
					x = 0;
					k = 0;
					while(( p1->row == n ) && ( k == 0 ))			// ȡ���к�Ϊn�� y ��Ԫ��ֵ
					{
						x = x + p1->y * v[p1->lnxt] * sin( rad[p1->row] - rad[p1->lnxt] - p1->rad );
						if( p1->next == NULL )
							k = 1;
						else
							p1 = p1->next;
					}
					x = x * v[n];
					p3->zh = x;
				}
				else									//**************** ��ʾHij *******************
				{
					p1 = tp[n];
					while( p1->lnxt != i )				// int nΪ �ڵ㵼�ɾ��� Y���кţ�i���к�
						p1 = p1->next;
					p3->zh = - v[p1->row] * p1->y * v[p1->lnxt] * sin( rad[p1->row] - rad[p1->lnxt] - p1->rad );
				}
			}

			else								//ż����ΪJ
			{	if( n == i )						//**************** ��ʾJii *******************
				{
					p1 = tp[n];
					x = 0;
					k = 0;
					while(( p1->row == n ) && ( k == 0 ))
					{
						x = x + p1->y * v[p1->lnxt] * cos( rad[p1->row] - rad[p1->lnxt] - p1->rad );
						if( p1->next == NULL )
							k = 1;
						else
							p1 = p1->next;
					}
					x = x * v[n];
					p3->zh = -x;
				}
				else				//**************** ��ʾJij  *******************
				{
					p1 = tp[n];
					while( p1->lnxt != i )
						p1 = p1->next;
					p3->zh = v[p1->row] * p1->y * v[p1->lnxt] * cos( rad[p1->row] - rad[p1->lnxt] - p1->rad );
				}
			}
			p3 = p3->next;
		}


		p3 = ycb[ 2 * i ];		//�ⲿ����N��L
		while( p3 != NULL )
		{
			j = p3->row;
			n = ( j + 1 ) / 2;		//n��Ӧ���кţ�i���к�
			if( fmod( j , 2 ) != 0)		//��ʾN
			{
				if( n == i )			//��ʾNii
				{
					p1 = tp[n];
					x = 0;
					k = 0;
					while(( p1->row == n ) && ( k == 0 ))
					{
						x = x + p1->y * v[p1->lnxt] * cos( rad[p1->row] - rad[p1->lnxt] - p1->rad );
						if( p1->next == NULL )
							k = 1;
						else
							p1 = p1->next;
					}
	/*##################################### ע�� N��L�Ķ��� ################################*/
	/*##################################### ע�� N��L�Ķ��� ################################*/
	/*##################################### ע�� N��L�Ķ��� ################################*/
					//˫�ݵ� Vi * Yii * Vi * cos(xita ij)     ����û�Ѹ���Viд��
					p3->zh = - ( x + 2 * v[n] * ygbd[i].g * cos( ygbd[i].b ));
				}
				else				//��ʾNij
				{
					p1 = tp[n];
					while( p1->lnxt != i )
						p1 = p1->next;
					p3->zh = - v[p1->row] * p1->y * cos( rad[p1->row] - rad[p1->lnxt] - p1->rad );
				}
			}
			else						//��ʾL
			{
				if( n == i )			//��ʾLii
				{
					p1 = tp[n];
					x = 0;
					k = 0;
					while(( p1->row == n ) && ( k == 0 ))
					{
						x = x + p1->y * v[p1->lnxt] * sin( rad[p1->row] - rad[p1->lnxt] - p1->rad);
						if( p1->next == NULL )
							k = 1;
						else
							p1 = p1->next;
					}
					//˫�ݵ� Vi * Yii * Vi * sin(xita ij)
					p3->zh = - ( x - 2 * v[n] * ygbd[i].g * sin( ygbd[i].b ) );
				}
				else				//��ʾLij
				{
					p1 = tp[n];
					while( p1->lnxt != i )
						p1 = p1->next;
					p3->zh = - v[p1->row] * p1->y * sin( rad[p1->row] - rad[p1->lnxt] - p1->rad );
				}
			}
			p3 = p3->next;
		}
	}

	struct Ycb  *p4, *p2;
	/****************************************����ƽ��ڵ�******************************************************/
	for (i = 1; i <= 2 * node_sum; i++)
	{
		if ((i == 2 * phd) || (i == 2 * phd - 1))	//ycb�ǰ����ŵģ�������ɾ��
		{
			while (ycb[i] != NULL)
			{
				p3 = ycb[i]->next;
				delete ycb[i];
				ycb[i] = p3;
			}
		}
		else													//ֻ��ɾ����Ӧ����һ�е�Ԫ��
		{
			n = 0;
			p3 = ycb[i];
			p4 = ycb[i];
			p2 = ycb[i];
			while (p3 != NULL)
			{
				p4 = p3;
				p3 = p3->next;									// P3 Ϊ P4 ����һ��
				if ((p4->row == 2 * phd) || (p4->row == 2 * phd - 1))
				{
					if (n == 0)
					{
						ycb[i] = p3;
						delete p4;
					}
					else
					{
						p2->next = p3;
						delete p4;
					}
				}
				else				//	��ӦPQ�е�Ԫ��ɾ��� n��1
				{
					n = 1;
					p2 = p4;
				}
			}
		}
	}
	/****************************************����PV�ڵ�******************************************************/
	struct pvd *p7 = headpvd;
	while (p7 != NULL)
	{
		for (i = 1; i <= 2 * node_sum; i++)
		{

			if ( i == 2 * p7->node )	//ycb�ǰ����ŵģ�������ɾ��	 PV�� Q ��Ӧ����
			{
				while (ycb[i] != NULL)
				{
					p3 = ycb[i]->next;
					delete ycb[i];
					ycb[i] = p3;
				}
			}
			else													//ֻ��ɾ����Ӧ����һ�е�Ԫ��
			{
				n = 0;
				p3 = ycb[i];
				p4 = ycb[i];
				p2 = ycb[i];
				while (p3 != NULL)
				{
					p4 = p3;
					p3 = p3->next;									// P3 Ϊ P4 ����һ��
					if ( p4->row == 2 * p7->node )
					{
						if (n == 0)
						{
							ycb[i] = p3;
							delete p4;
						}
						else
						{
							p2->next = p3;
							delete p4;
						}
					}
					else				//	��ӦPQ�е�Ԫ��ɾ��� n��1
					{
						n = 1;
						p2 = p4;
					}
				}
			}
		}
		p7 = p7->next;
	}

	p3 = new Ycb;				//ƽ���Ҫ���Խ���Ԫ��
	p3->row = 2 * phd - 1;
	p3->lnxt = 2* phd - 1;
	p3->zh = 99999;
	p3->next = NULL;
	ycb[p3->lnxt] = p3;

	p3 = new Ycb;				//ƽ���Ҫ���Խ���Ԫ��
	p3->row = 2 * phd;
	p3->lnxt  = 2 * phd;
	p3->zh = 99999;
	p3->next = NULL;
	ycb[p3->lnxt] = p3;

	p7 = headpvd;
	while (p7 != NULL)
	{
		p3 = new Ycb;				//PV�� Q Ҫ���Խ���Ԫ��
		p3->row = 2 * p7->node;
		p3->lnxt = 2 * p7->node;
		p3->zh = 99999;
		p3->next = NULL;
		ycb[p3->lnxt] = p3;
		p7 = p7->next;
	}
}

/********************************************** LU�ֽ����������� ***********************************************************/
void luss()
{
	struct Ycb *l[100] = { NULL }, *u[100] = { NULL };	//�ֱ�Ϊ�ſɱȾ���LU�ֽ�ϡ���
	struct Ycb *p1, *p2;
	double ud[100];	//�Խ���Ԫ��
	double t[100];
	double x;
	int i, j, r;

	for(int ii = 1; ii <= 2 * node_sum; ii++)
		radv[ii] = -radv[ii];

	/**********************���ſɱȾ���ŵ���Ӧ��������***************************/
	p1 = new Ycb;
	for( i = 1 ; i <= 2 * node_sum ; i++ )
	{
		ud[i] = 0;
		p2 = ycb[i];

		while( p2 != NULL )
		{

			p1->row = p2->row;
			p1->lnxt = p2->lnxt;
			p1->zh = p2->zh;
			p1->next = NULL;

			if( p2->row == p2->lnxt )					//ud �ſ˱Ⱦ���ĶԽ�ֵ ��ΪU��Խ�
				ud[p2->row] = p2->zh;

			if( p1->row > p1->lnxt )					//l,l�����ǰ������еģ����к�>�к�
			{
				l[p1->row] = insert3( l[p1->row] , p1 );
				if( kk == 0 )							// kk������insert�����У�����нڵ�����·�ص������kkΪ1
					p1 = new Ycb;
			}

			if( p1->row < p1->lnxt )					//u,u�����ǰ������еģ����к�>�к�
			{
				u[p1->lnxt] = insert2( u[p1->lnxt] , p1 );
				if( kk == 0 )
					p1 = new Ycb;
			}
			p2 = p2->next;
		}
	}
	delete p1;			// delete the newnode create at last circle

	for( i = 2 ; i <= 2 * node_sum ; i++ )			//����Li1 ��L�����Խ�Ϊ1���ӵڶ��ʼ
	{
		p2 = l[i];
		if (p2 != NULL)
		{
			if (p2->lnxt == 1)
				p2->zh = p2->zh / ud[1];
		}
	}

	/***************************************** LU�ֽ� **********************************/

	/*************************** L��Խ�ȫΪ 1 **********************************/
	/*************************** �кź� ycb ����ͬ ***************************/
	for( r = 2 ; r <= 2 * node_sum - 1 ; r++ ) 		 //�ӵڶ��п�ʼ, �㵽�����ڶ���
	{

		/****************************** compute Uri *****************************/
		for( i = 1 ; i <= r ; i++ )					// ÿ��ѭ���� L�� t����������
			t[i] = 0;								// ����L���֪ÿ�� l Ԫ�صĸ���Ϊ �к�-1 ��

		p2 = l[r];
		while( p2 != NULL )							//ȡ Lrk, r���кţ������ŵ�
		{
			t[p2->lnxt] = p2->zh;					// t����ΪL���к�
			p2 = p2->next;							// t����ΪL��ֵ
		}

		/******************************* U���ǳ�ȥ�Խǵľ��� ******************************/
		/************* U���ǶԽ�Ԫ�ص����洢��U���к���ycb��ͬ����һ��û�� *******************/
		for( i = r + 1 ; i <= 2 * node_sum ; i++ )			// rΪ�Խǣ�Ҫ�ֿ����㣬���Դ�r+1��ʼ
		{
			x = 0;
			p2 = u[i];										//�ӵ� r + 1 �� U ��ʼ��r�������Ϊ Urr�� ���д���
			j = 0;											//
			while(( p2 != NULL ) && ( j == 0 ))				//ȡ Uki , k=(1~r-1) ,�����ŵ�
			{
				if( p2->row >= r)							//U���кŲ����ܴ����кţ������˳�ѭ��
					j = 1;
				else
				{
					x = x + t[p2->row] * p2->zh;			// Lrk*Uki �� u[i]���ж�Ӧl����
					p2 = p2->next;
				}
			}

			if( x != 0 )
			{
				p1 = new Ycb;
				p1->row = r;
				p1->lnxt = i;
				p1->zh = - x;							//�ۼӵĺͣ��� Uri ��ǰ��׼��
				p1->next = NULL;
				u[p1->lnxt] = insert2( u[p1->lnxt] , p1 );
				if( kk == 1 )
					delete p1;
			}
		}

		/******************************* compute ud *****************************/
		x = 0;
		p2 = u[r];								//��r�� ��Խ�u
		j = 0;
		while(( p2 != NULL ) && ( j == 0 ))		//ȡ Uki , k=(1~r-1) ,�����ŵ�
		{
			if( p2->row >= r )					//U���кŲ����ܴ����кţ������˳�ѭ��
				j = 1;
			else
			{
				x = x + t[p2->row] * p2->zh;	// Lrk * Uki
				p2 = p2->next;
			}
		}
		ud[r] = ud[r] - x;

		/******************************* compute L ****************************/

		for( i = 1 ; i <= r ; i++ )
			t[i] = 0;

		p2 = u[r];
		while( p2 != NULL )						//ȡUkr,�����ŵ�
		{
			t[p2->row] = p2->zh;
			p2 = p2->next;
		}
		for( i = r + 1 ; i <= 2 * node_sum ; i++ )
		{
			x = 0;
			p2 = l[i];
			j = 0;
			while(( p2 != NULL ) && ( j == 0 ))			//ȡLik,k=(1~r-1)�������ŵ�
			{
				if( p2->lnxt >= r )
					j = 1;
				else
				{
					x = x + t[p2->lnxt] * p2->zh;		// Lik*Ukr , ���ͳһ��Urr
					p2 = p2->next;
				}
			}

			if( x != 0 )
			{
				p1 = new Ycb;
				p1->row = i;
				p1->lnxt = r;
				p1->zh = - x;
				p1->next = NULL;
				l[p1->row] = insert3( l[p1->row] , p1 );
				if(kk == 1)
					delete p1;
			}
		}
		for( i = r + 1 ; i <= 2 * node_sum ; i++ )			//   /Urr
		{
			p2 = l[i];
			j = 0;
			while(( p2 != NULL ) && ( j == 0 ))
			{
				if( p2->lnxt == r )
					p2->zh = p2->zh / ud[r];

				if( p2->lnxt >= r )
					j = 1;

				else
					p2 = p2->next;
			}
		}
	}

	/*************************** compute Unn�Խ�Ԫ�� ****************************/
	for( i = 1 ; i <= 2 * node_sum ; i++ )
		t[i] = 0;

	p2 = l[ 2 * node_sum ];					//���� i = 2*node_sum - 1 û�õ��� l ���һ��
	while( p2 != NULL )						//ȡLnk
	{
		t[p2->lnxt] = p2->zh;
			p2 = p2->next;
	}

	x = 0;
	p2 = u[ 2 * node_sum ];
	j = 0;
	while(( p2 != NULL ) && ( j == 0))		//ȡUki,k=(1~r-1)
	{
		if( p2->row >= 2 * node_sum )
			j = 1;
		else
		{
			x = x + t[p2->row] * p2->zh;		//Lrk*Uki
			p2 = p2->next;
		}
	}
	ud[ 2 * node_sum ] = ud[ 2 * node_sum ] - x;	//ud[]ԭ�ȴ��� ycb �����һ���Խ�Ԫ��

	/*************************** solvs Ly=b b=radv[] ***************************/
	for( i = 2 ; i <= 2 * node_sum ; i++ )
	{
		x = 0;
		j = 0;
		p2 = l[i];
		while( p2 != NULL )
		{
			x = x + radv[p2->lnxt] * p2->zh;	//Lik*yk,Lik�ǰ������е�
			p2 = p2->next;
		}
		radv[i] = radv[i] - x;
	}

	/****************************** solvs Ux=y y=radv[] ************************/
	radv[ 2 * node_sum ] = radv[ 2 * node_sum ] / ud[ 2 * node_sum ];
	for( i = 2 * node_sum - 1 ; i >= 1 ; i-- )
	{
		for( r = i + 1 ; r <= 2 * node_sum ; r++ )
			t[r] = 0;

		for( r = i + 1 ; r <= 2 * node_sum ; r++ )	//ȡUir,���ǰ������еģ�Ҫ��תһ��
		{
			p2 = u[r];
			j = 0;
			while(( p2 != NULL ) && ( j == 0 ))
			{
				if( p2->row == i )
					t[r] = p2->zh;
				if( p2->row >= i )
					j = 1;
				else
					p2 = p2->next;
			}
		}

		x = 0;
		for( r = i + 1 ; r <= 2 * node_sum ; r++ )
			x = x + t[r] * radv[r];
		radv[i] = (radv[i] - x) / ud[i];
	}

	for( i = 1 ; i <= 2 * node_sum ; i++ )			//�ͷſռ�
	{
		while( l[i] != NULL )
		{
			p2 = l[i]->next;
			delete l[i];
			l[i] = p2;				//l�����ǰ������еģ����к�>�к�
		}
	}
	for( i = 1 ; i <= 2 * node_sum ; i++ )		//�ͷſռ�
	{
		while( u[i] != NULL )
		{
			p2 = u[i]->next;
			delete u[i];
			u[i] = p2;				//u�����ǰ������еģ����к�>�к�
		}
	}
	return ;
}

/***************************************** ���ÿ�ڵ�� ��Pi ��Gi *********************************************************/
void deltapq()
{

	struct Yz *p1 = NULL;
	struct pvd *p2 = NULL;
	for(int ii = 1 ; ii <= node_sum ; ii++)		//  ��Pi  ��Qi
	{
		p1 = y[ii];				//ע��y����û�жԽ�Ԫ��
		radv[2 * ii - 1] = 0;
		radv[2 * ii] =  0;
		while( p1 != NULL )						//ԭ��radv[]�д����0
		{
			radv[2 * ii - 1] += v[ii] * p1->y * v[p1->lnxt] * cos( rad[p1->row] - rad[p1->lnxt] - p1->rad );
			radv[2 * ii] += v[ii] * p1->y * v[p1->lnxt] * sin( rad[p1->row] - rad[p1->lnxt] - p1->rad );
			p1 = p1->next;
		}
		//�ټ�ȥ�Խ�Ԫ��
		radv[2 * ii - 1] +=  v[ii] * ygbd[ii].g * v[ii] * cos( - ygbd[ii].b );
		radv[2 * ii] +=  v[ii] * ygbd[ii].g * v[ii] * sin( - ygbd[ii].b );

		radv[2 * ii - 1] = p[ii] - radv[2 * ii - 1];
		radv[2 * ii] = q[ii] - radv[2 * ii];
	}
	radv[2 * phd - 1] = 0;		// phd, ��Pi  ��Qi = 0
	radv[2 * phd] = 0;

	//******************************************** pvd
	p2 = headpvd;				//pvd, ��Qi = 0
	while (p2 != NULL)
	{
		radv[2 * p2->node] = 0;
		p2 = p2->next;
	}

	for (int ii = 1; ii <= 2 * node_sum; ii++)
	{
		if (fabs(radv[ii]) > err)						//�Ǽ�������
			err = fabs(radv[ii]);
	}
}

/***************************************** ������ĸ������ ��Ǧ� �� ��ֵv **********************************************/
void fixradv()
{
	for(int ii = 1 ; ii <= node_sum ; ii++)		// delta Pi || delta Qi
	{
		rad[ii] = rad[ii] + radv[2 * ii - 1];
		v[ii] = v[ii] + radv[2 * ii];
	}
}

/***************************************** ������� ƽ���� Pi Gi �� PV��� Gi ************************************************/
void fixphdpvd()
{
	double x = 0, r = 0;

	struct Yz *p1 = NULL;								//ƽ��㴦��
	p1 = y[phd];

	while( p1 != NULL )
	{
		x = x + p1->y * v[p1->lnxt] * cos(rad[phd] - rad[p1->lnxt] - p1->rad);
		r = r + p1->y * v[p1->lnxt] * sin(rad[phd] - rad[p1->lnxt] - p1->rad);
		p1 = p1->next;
	}
	/******************�Ƿ�Ҫ�ѶԽ�Ԫ�ص������ǣ�**************************/
	x = x + ygbd[phd].g * cos(rad[phd] - rad[phd] - ygbd[phd].b);
	r = r + ygbd[phd].g * sin(rad[phd] - rad[phd] - ygbd[phd].b);
	p[phd] = x * v[phd];
	q[phd] = r * v[phd];

	//*****************************PV�㴦��p2Ϊ pvd ����***************************

	struct pvd *p2 = headpvd;
	struct Yz *p3 = NULL;
	p3 = y[p2->node];
	int bz = 0;

	while (p2 != NULL)
	{
		r = 0;
		p3 = y[p2->node];		// p2Ϊ pvd ����, ȡ��ڵ�ţ��õ��������ͷ
		while (p3 != NULL)
		{
			r = r + p3->y * v[p3->lnxt] * sin(rad[p3->row] - rad[p3->lnxt] - p3->rad);
			p3 = p3->next;
		}

		// �޹�Խ��
		/*
		if (r > gmax[p2->node])
		{
			if (p2 == headpvd)
				bz = 1;
			q[p2->node] = gmax[p2->node];
			pvyjbz[p2->node] = 1;
			deletepvd(p2->node);
		}
		else if (r < gmin[p2->node])
		{
			if (p2 == headpvd)
				bz = 1;
			q[p2->node] = gmin[p2->node];
			pvyjbz[p2->node] = 1;
			deletepvd(p2->node);
		}
		else
			q[p2->node] = r * v[p2->node];
		p2 = p2->next;
		if (bz == 1)
			p2 = headpvd;*/
	}
	
}

/*****************************************ɾ��ָ����PV�ڵ�*********************************************************/
void deletepvd(int node0)
{
	struct pvd *p0 = headpvd;
	struct pvd *p1 = p0;
	while (p0 != NULL && p0->node != node0 )
	{
		p1 = p0;
		p0 = p0->next;
	}
	p1->next = p0->next;
	if (p0->node = headpvd->node)
		headpvd = p0->next;
	delete p0;
}

/*****************************************�ڵ㵼�ɾ������ָ������*********************************************************/
struct Yz *insert1(struct Yz *tp, struct Yz *z)	//�ڵ㵼�ɾ������ָ������
{
	struct Yz *p0, *p111, *p112 = NULL;
	double r, r1, x, x1;
	kk = 0;
	p111 = tp;			//ָ��ԭ����
	p112 = p111;
	p0 = z;				//ָ��Ҫ������Ǹ��ڵ�

	if (p0 == NULL)
		return tp;
	if (tp == NULL)
	{
		tp = p0;
		p0->next = NULL;
		return tp;
	}
	if (p0->lnxt < p111->lnxt)			//����Ľڵ�������ͷ֮ǰ
	{
		tp = p0;
		p0->next = p111;
		return tp;
	}
	while ((p0->lnxt > p111->lnxt) && (p111->next != NULL))
	{
		p112 = p111;
		p111 = p111->next;
	}

	if (p0->lnxt == p111->lnxt)		//������ж�����·���ѹ��
	{
		r = p111->y * cos(p111->rad);
		x = p111->y * sin(p111->rad);
		r1 = p0->y * cos(p0->rad);
		x1 = p0->y * sin(p0->rad);
		r = r + r1;
		x = x + x1;
		x1 = sqrt(r * r + x * x);
		p111->y = x1;
		p111->rad = atan2(x, r);
		kk = 1;
		return tp;
	}
	if ((p111->next == NULL) && (p0->lnxt > p111->lnxt))
	{
		p111->next = p0;
		p0->next = NULL;
	}
	else
	{
		p112->next = p0;
		p0->next = p111;
	}
	return tp;
}

/*****************************************�ſɱȾ������ָ������ (���� row)***********************************************/

struct Ycb *insert3(struct Ycb *tp, struct Ycb *z)	//�ڵ㵼�ɾ������ָ������
{
	struct Ycb *p0, *p111, *p112 = NULL;

	kk = 0;
	p111 = tp;				//ָ��ԭ����
	p112 = p111;
	p0 = z;					//ָ��Ҫ������Ǹ��ڵ�

	if (p0 == NULL)
		return tp;
	if (tp == NULL)
	{
		tp = p0;
		p0->next = NULL;
		return tp;
	}
	if (p0->lnxt < p111->lnxt)			//����Ľڵ�������ͷ֮ǰ
	{
		tp = p0;
		p0->next = p111;
		return tp;
	}
	while ((p0->lnxt > p111->lnxt) && (p111->next != NULL))			//��������ִ�Ҫ��λ�ã���ĩβֹͣ
	{
		p112 = p111;
		p111 = p111->next;
	}
	if (p0->lnxt == p111->lnxt)										//�����غ�
	{
		p111->zh += p0->zh;
		kk = 1;
		return tp;
	}
	if ((p111->next == NULL) && (p0->lnxt > p111->lnxt))			//Ҫ���λ��λ�ڱ�β֮��
	{
		p111->next = p0;
		p0->next = NULL;
	}
	else															//����λ��λ�ڱ����ִ��λ��
	{
		p112->next = p0;
		p0->next = p111;
	}
	return tp;
}


/*****************************************�ſɱȾ������ָ������ (���� lnxt)***********************************************/
struct Ycb *insert2(struct Ycb *tp, struct Ycb *z)	//�ڵ㵼�ɾ������ָ������
{
	struct Ycb *p0, *p111, *p112 = NULL;

	kk = 0;
	p111 = tp;				//ָ��ԭ����
	p112 = p111;
	p0 = z;					//ָ��Ҫ������Ǹ��ڵ�

	if (p0 == NULL)
		return tp;
	if (tp == NULL)
	{
		tp = p0;
		p0->next = NULL;
		return tp;
	}
	if (p0->row < p111->row)			//����Ľڵ�������ͷ֮ǰ
	{
		tp = p0;
		p0->next = p111;
		return tp;
	}
	while ((p0->row > p111->row) && (p111->next != NULL))			//��������ִ�Ҫ��λ�ã���ĩβֹͣ
	{
		p112 = p111;
		p111 = p111->next;
	}
	if (p0->row == p111->row)							//�����غ�
	{
		p111->zh += p0->zh;
		kk = 1;
		return tp;
	}
	if ((p111->next == NULL) && (p0->row > p111->row))			//Ҫ���λ��λ�ڱ�β֮��
	{
		p111->next = p0;
		p0->next = NULL;
	}
	else															//����λ��λ�ڱ����ִ��λ��
	{
		p112->next = p0;
		p0->next = p111;
	}
	return tp;
}
