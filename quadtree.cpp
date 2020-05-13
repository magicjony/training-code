#include "stdafx.h"
#include <vector>
#include <list>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <fstream>
#include <time.h>
#include <locale>

using namespace std;

class QPoint{
protected:
	vector<double> val;
private:
	int dim, num;
public:
	inline QPoint(int _dim, int _num) :dim(_dim), num(_num){ val.resize(dim*num); }; // размерность вектора = количеству всех элементов массива
	inline int getDim(){ return dim; };
	inline int getNum(){ return num; };

	inline int getPrint(){
		for (int i = 0; i < val.size(); i++) cout << "val[" << i << "] = " << val[i] << '\n';
		cout << endl;
		return 0;
	}; // вывод массива

	inline double *operator[](int i){ return &val[i*dim]; };
	inline int push(double a){ val.push_back(a);  return 0; }; // добавление элемента в конец массива (с изменением размерности)

	inline bool findel(double a){
		for (vector<double>::iterator i = val.begin(); i < val.end(); i++)
			if (*i == a) return true;
		return false;
	};// проверка на вхождение элемента а в вектор
};

class QTree{
private:
	typedef list<int> QIndex; //  множество точек области
	typedef QIndex::iterator QIndexIter;
	int count; // счетчик узлов

	struct QTreeNode{ // структура узла
		QTreeNode *child[4];
		QIndex listIndex;
		double midx, midy;
	};
	size_t max_count;
	ofstream f, g;
	QTreeNode *root; // указатель на корень
	QPoint &val; // адрес массива
	void delNode(QTreeNode *node); // удаление узла
	QTreeNode *newNode(); // создание нового узла
	void QTreeNodeWrite(QTreeNode *node);
public:
	inline ~QTree(){ delNode(root); }; // деструктор
	QTree(QPoint &_val, size_t _max); // конструктор
	void QTreeWrite(); //вывод массива узлов, в которых есть вхождение данного элемента
	QTreeNode *addNode(QTreeNode *node, int ind);
	void Qsearch1(QTree::QTreeNode *node, double x0, double y0, double x1, double y1);
	void Qsearch2(QTree::QTreeNode *node, double x0, double y0, double x1, double y1);
	void cool();
};

QTree::QTree(QPoint &_val, size_t _max) : val(_val), max_count(_max) { // конструктор
	count = 0; // счетчик узлов
	root = newNode();
	for (int i = 0; i < val.getNum(); ++i) {
		addNode(root, i);
	}

	cout << "count == " << count << '\n'; // выводим кол-во узлов
};

QTree::QTreeNode *QTree::newNode(){ // создание узла (дерева)
	count++;
	QTreeNode *node = new QTreeNode();
	for (int j = 0; j < 4; ++j) {
		node->child[j] = NULL;
	}
	return node;
}

QTree::QTreeNode *QTree::addNode(QTree::QTreeNode *node, int ind) {

	if (node == NULL) {
		node = newNode();
		node->listIndex.push_back(ind);
		return node;
	}

	node->listIndex.push_back(ind);

	if (node->listIndex.size() > max_count) {
		node->midx = 0;
		node->midy = 0;

		for (QIndexIter it = node->listIndex.begin(); it != node->listIndex.end(); ++it) {
			node->midx += val[*it][0];
			node->midy += val[*it][1];
		}



		node->midx = node->midx / node->listIndex.size();
		node->midy = node->midy / node->listIndex.size();


		//cout << "midx == " << node->midx << endl;
		//cout << "midy == " << node->midy << endl;

		for (QIndexIter it = node->listIndex.begin(); it != node->listIndex.end(); ++it) {
			int h = (val[*it][0] < node->midx) * 2 + (val[*it][1] < node->midy);
			node->child[h] = addNode(node->child[h], *it);
		}

		node->listIndex.clear();
	}


	return node;
}

void QTree::delNode(QTreeNode *node){ // деструктор
	if (node == NULL) {
		return;
	}
	for (int j = 0; j < 4; ++j) {
		delNode(node->child[j]);
	}
	delete node;
};

void QTree::QTreeNodeWrite(QTree::QTreeNode *node) {


	QIndexIter i = node->listIndex.begin();
	while (i != node->listIndex.end()) {
		//cout << "val[" << (*i) << "] = (" << (val[*i][0]) << ", " << (val[*i][1]) << ");\n";
		f << val[*i][0] << "\n";
		g << val[*i][1] << "\n";
		++i;
	}

	for (int j = 0; j < 4; ++j)
		if (node->child[j] != NULL)
			QTreeNodeWrite(node->child[j]);


	return;
}

void QTree::QTreeWrite(){ // вывод массива узлов с искомым элементом
	f.open("allpoint_x.txt", ios::out);
	g.open("allpoint_y.txt", ios::out);
	QTreeNodeWrite(root);
	g.close();
	f.close();
}



void QTree::Qsearch1(QTree::QTreeNode *node, double x0, double y0, double x1, double y1){
	count++;
	QTreeNode *temp = node;
	QIndexIter i = temp->listIndex.begin();
	while (i != temp->listIndex.end()) {
		//if (((val[*i][0] >= x0) && (val[*i][0] <= x1)) && ((val[*i][1] <= y1) && (val[*i][1] >= y0))) // проверка на вхождение элемента в область
		//f << (val[*i][0]) << " " << (val[*i][1]) << "\n";
		//f << "val[" << *i << "] = (" << (val[*i][0]) << ", " << (val[*i][1]) << ");\n";
		//f << a << "\t";
		//cout << "val[" << *i << "] = (" << (val[*i][0]) << ", " << (val[*i][1]) << ");\n";
		++i;
	}
	for (int j = 0; j < 4; ++j) {
		if (temp->child[j] != NULL)
			Qsearch1(temp->child[j], x0, y0, x1, y1); // переход на дочерние, если они не пусты
	}
	return;
}



void QTree::Qsearch2(QTree::QTreeNode *node, double x0, double y0, double x1, double y1){
	count++;
	QTreeNode *temp = node;
	QIndexIter i = temp->listIndex.begin();
	while (i != temp->listIndex.end()) {

		if (((val[*i][0] >= x0) && (val[*i][0] <= x1)) && ((val[*i][1] <= y1) && (val[*i][1] >= y0))){ // проверка на вхождение элемента в область
			f << val[*i][0] << "\n";
			g << val[*i][1] << "\n";
		}
		++i;
	}


	if ((x0 < node->midx) && (y0 < node->midy))
		if (temp->child[3] != NULL)
			Qsearch2(temp->child[3], x0, y0, x1, y1);

	if ((x1 > node->midx) && (y0 < node->midy))
		if (temp->child[1] != NULL)
			Qsearch2(temp->child[1], x0, y0, x1, y1);

	if ((x1 > node->midx) && (y1 > node->midy))
		if (temp->child[0] != NULL)
			Qsearch2(temp->child[0], x0, y0, x1, y1);

	if ((x0 < node->midx) && (y1 > node->midy))
		if (temp->child[2] != NULL)
			Qsearch2(temp->child[2], x0, y0, x1, y1);

	return;

}

void QTree::cool(){
	count = 0;
	time_t t = clock();
	Qsearch1(root, -100, -100, 100, 100);
	cout << "count == " << count << endl;
	cout << (clock() - t) / (double)CLOCKS_PER_SEC << endl;
	count = 0;
	f.open("point_x.txt", ios::out);
	g.open("point_y.txt", ios::out);
	t = clock();
	Qsearch2(root, -100, -100, 100, 100);
	cout << "count == " << count << endl;
	cout << (clock() - t) / (double)CLOCKS_PER_SEC << endl;
	g.close();
	f.close();
}

int main(){
	setlocale(LC_ALL, "RUS");

	QPoint p(2, 1000000);
	srand(time(NULL));
	for (int k = 0; k < p.getNum(); k++)
		for (int i = 0; i < p.getDim(); i++)
		{
			p[k][i] = 2000 * (0.5 - (double)rand() / RAND_MAX); // числа от 0 до 10
		}

	QTree tree(p, 100); // создание дерева

	tree.QTreeWrite(); // вывод массива узлов, содержащих искомый элемент
	tree.cool();

	system("pause");
	return 0;
}


