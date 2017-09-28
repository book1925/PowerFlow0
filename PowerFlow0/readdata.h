#include "stdio.h"
#include "math.h"
#include "malloc.h"
#include "string.h"
#include "iostream"
#include "string.h"
using namespace std;
#include "windows.h"
#include "windowsx.h"
#define CRTDBG_MAP_ALLOC
#include "stdlib.h"
#include "conio.h"
#include "crtdbg.h"
/******************************************************ȫ�ֱ�������*******************************************************/
int node_sum, phd;					//�ڵ�������ƽ��ڵ��
double uph, e;		//ƽ����ѹ�����㾫��
int kk;				//��Ϊ��ʶ���������Ƿ���������·�����ϵ��ӣ�������ӾͲ��ÿ��½ڵ���

/**************************************************��������********************************************************/
int readdata();					//���ݶ�ȡ�봦��ֱ��������ʽ��������Y��
//void cY(int);							//ת��Ϊ������ʽ
char option();
struct Yz *insert1(struct Yz *tp, struct Yz *z);
struct ycb *insert2(struct ycb *, struct ycb *);
/*************************************************�ṹ�嶨��******************************************************/
struct Ygb		// Y��ֱ�����꣩����
{
	int row, lnxt;			//����
	double g, b;
	struct Ygb *next;
} *ygb[2000] = { NULL };

struct Ygbd 						//Y��Խ�Ԫ�أ�ֱ�����꣩
{
	double g, b;
} ygbd[2000] = { 0 };

struct Yz		// Y�󣨷�ֵ��ǣ�����
{
	int row, lnxt;			//����
	double y, rad;
	struct Yz *next;
} *y[2000] = { NULL };

struct Yd 						//Y��Խǣ���ֵ��ǣ�Ԫ��
{
	double y, rad;
} yd[2000] = { 0 };

struct Ycb
{
	int row, lnxt;
	double zh;
	struct Ycb *next;
} *ycb[4000] = { NULL };

struct pqd
{
	int node;
	double p, q;
	struct pqd *next;
};			//pd������
struct pqd *head1;

struct pvd
{
	int node;
	double vi, qmin, qmax;
	struct pvd *next;
};			//pd������
struct pvd *head2;

/**************************************************************��ȡԴ�ļ�����***********************************************/
int readdata()
{
	FILE *fp;
	int i, j, m, lastnode = 0;
	char  ch;
	double r, x, bb, e, k0;
	double gij, bij;
	char fsource[20];

	printf("\n		�����볱������������ļ�\n		--");
getf: fflush(stdin);
	gets(fsource);
	while ((fp = fopen(fsource, "rb")) == NULL)
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

	fscanf(fp, "%d,%d,%lf,%lf", &node_sum, &phd, &uph, &e);		//1���ڵ�������ƽ��ڵ�ţ�ƽ����ѹ�����㾫��
	fscanf(fp, "%d", &m);									//�����м��0
	fscanf(fp, "%d", &m);									//�����ж��Ƿ�������
	/************************************************2����ȥ�Խǵĵ�������Խ�Ԫ��********************************************/
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
		p00->rad = atan2(bij, gij);
		y[i] = insert1(y[i], p00);
		if (kk == 0)
			p00 = new Yz;				//ֱ�Ӱ����������Ϊ��ֵ��ǣ���������ϰ벿��

		p11->row = j;
		p11->lnxt = i;
		p11->y = sqrt(gij * gij + bij * bij);
		p11->rad = atan2(bij, gij);
		y[j] = insert1(y[j], p11);
		if (kk == 0)
			p11 = new Yz;				//ֱ�Ӱ����������Ϊ��ֵ��ǣ�i j�ߵ�����Ϊ��������°벿��

		ygbd[i].g += gij;
		ygbd[i].b += bij + bb;
		ygbd[j].g += gij;
		ygbd[j].b += bij + bb;
		fscanf(fp, "%d", &m);
		/***************ͳ��y�������������Ľڵ��*******************/
		if (i > lastnode)
			lastnode = i;
	}
	delete p00, p11;
	/************************************************3��������·�м��ѹ��********************************************/
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
		p00->y = sqrt(x0 * x0 + b0 * b0);
		p00->rad = atan2(b0, x0);
		y[i] = insert1(y[i], p00);
		if (kk == 0)
			p00 = new Yz;

		p11->row = j;
		p11->lnxt = i;
		p11->y = p00->y;
		p11->rad = p00->rad;
		y[j] = insert1(y[j], p11);
		if (kk == 0)
			p11 = new Yz;

		ygbd[i].g += gij / k0 / k0;
		ygbd[i].b += bij / k0 / k0;
		ygbd[j].g += gij;										//��ѹ���ѹ��������
		ygbd[j].b += bij;
		fscanf(fp, "%d", &m);
	}
	delete p00, p11;
	/************************************************4�����Ͻӵ�֧·�Ե�********************************************/
	fscanf(fp, "%d", &m);
	while (m != 0)
	{
		fscanf(fp, ",%d,%lf,%lf", &i, &r, &x);
		ygbd[i].g += r;
		ygbd[i].b += x;
		fscanf(fp, "%d", &m);
	}

	for (int ii = 1; ii <= lastnode; ii++)
	{
		p00 = new Yz;
		p00->row = ii;
		p00->lnxt = ii;
		p00->y = sqrt(ygbd[ii].g * ygbd[ii].g + ygbd[ii].b * ygbd[ii].b);
		p00->rad = atan2(ygbd[ii].b, ygbd[ii].g);
		y[ii] = insert1(y[ii], p00);
	}
	delete p00;

	/************************************************5����ȡPQ�ڵ�����********************************************/
	fscanf(fp, "%d", &m);
	struct pqd *p1;
	struct pqd *tail1 = head1;
	while (m != 0)
	{
		fscanf(fp, ",%d,%lf,%lf,%lf,%lf", &i, &r, &x, &bb, &k0);
		p1 = new pqd;
		p1->node = i;
		p1->p = r - bb;
		p1->q = x - k0;

		if (head1 == NULL)
			head1 = p1;
		else
			tail1->next = p1;
		tail1 = p1;
		fscanf(fp, "%d", &m);
	}
	tail1->next = NULL;

	/************************************************6����ȡPV�ڵ�����********************************************/
	fscanf(fp, "%d", &m);
	struct pvd *p2;
	struct pvd *tail2 = head2;
	while (m != 0)
	{
		fscanf(fp, ",%d,%lf,%lf,%lf", &i, &r, &x, &bb);
		p2 = new pvd;
		p2->node = i;
		p2->vi = r;
		p2->qmin = x;
		p2->qmax = bb;

		if (head2 == NULL)
			head2 = p2;
		else
			tail2->next = p2;
		tail2 = p2;
		fscanf(fp, "%d", &m);
	}
	tail2->next = NULL;
	fclose(fp);
	/***************ͳ��y�������������Ľڵ��*******************/
	return lastnode;
}
