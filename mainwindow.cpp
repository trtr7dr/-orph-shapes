#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"
#include "QMessageBox"
#include "QTime"
#include <QThread>
#include <iostream>
#include <string>
#include <vector>
#include "QTimer"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

class Sleeper: public QThread
{
    public:
        static void msleep(int ms)
        {
            QThread::msleep(ms);
        }
};

class point{
public: float x,y;
};

class Line{
public: point start; point end;

	void newLine(point one, point two){

		start.x = one.x;
		start.y = one.y;
		end.x = two.x;
		end.y = two.y;
	}
};

class Poly{
public: std::vector<Line> poly;

	void addLine(Line l){
		poly.resize(poly.size()+1);
		poly[poly.size()-1] = l;
	}

};

void MainWindow::on_pushButton_clicked()
{

    QGraphicsScene *scene = new QGraphicsScene(ui->graphicsView);
    QPen pen(Qt::green);//Просто выбираем цвет для карандашика
    scene->addLine(0,90,180,90,pen);//x
    scene->addLine(90,0,90,180,pen);//y

	//координаты в строке
	QString beg = ui->lineEdit->text(); //1,4;4,5
	QString fin = ui->lineEdit_2->text();

	//разбивка на точки
	QStringList startDot = beg.split(";"); //1,4 4,5
	QStringList endDot = fin.split(";");
	
    std::swap(startDot, endDot);
    QStringList temp,temp2;
    float x,y,x2,y2;
    QPen pen2(Qt::red);//Карандашик для графика
    QPen pen4(Qt::black);
	scene->addLine(startDot[0].toInt()+91,91-startDot[1].toInt(),
			startDot[startDot.size()-2].toInt()
			+91,91-startDot[startDot.size()-1].toInt(),pen);
    for (int i = 1; i < startDot.count(); i++){
        temp = startDot[i].split(",");
        x = temp[0].toInt();
        y = temp[1].toInt();
        temp = startDot[i-1].split(",");
        x2 = temp[0].toInt();
        y2 = temp[1].toInt();
        scene->addLine(x+90,90-y,x2+90,90-y2,pen2);
    }

    temp = startDot[0].split(",");
    x = temp[0].toInt();
    y = temp[1].toInt();
	temp = startDot[startDot.count()-1].split(",");
	x2 = temp[0].toInt();
    y2 = temp[1].toInt();
	scene->addLine(x+90,90-y,x2+90,90-y2,pen2);
    temp2 = endDot[0].split(",");
    x = temp2[0].toInt();
    y = temp2[1].toInt();
    temp2 = endDot[endDot.count()-1].split(",");
    x2 = temp2[0].toInt();
    y2 = temp2[1].toInt();
    scene->addLine(x+90,90-y,x2+90,90-y2,pen4);
    ui->graphicsView->setScene(scene);
    QPen pen3(Qt::blue);
    int k = ui->spinBox->value();

	std::vector<point> start(startDot.count());
    for(int i=0; i < startDot.count(); i++){
        temp2 = startDot[i].split(",");
		start[i].x = temp2[0].toFloat();
		start[i].y = temp2[1].toFloat();
    }

	std::vector<point> end(endDot.count());
	for(int i = 0; i < endDot.count(); i++){
        temp2 = endDot[i].split(",");
		end[i].x = temp2[0].toFloat();
		end[i].y = temp2[1].toFloat();
    }


	if(start.size() < end.size()){
		while(start.size() < end.size()){
			start.resize(start.size() + 1);
			start[start.size() - 1].x = start[start.size() - 2].x;
			start[start.size() - 1].y = start[start.size() - 2].y;
		}
	}


	if(start.size() > end.size()){
		while(start.size() > end.size()){
			end.resize(end.size() + 1);
			end[end.size() - 1].x = end[end.size() - 2].x;
			end[end.size() - 1].y = end[end.size() - 2].y;
		}
	}

	//end start swap()
	swap(end,start);

	Poly share;
	for(int i = 0; i < end.size(); i++){
		Line l;
		l.newLine(start[i],end[i]);
		// share l addLine() .
		share.addLine(l);
	}
	// t 0 =
	float t = 0;
	// h 1 k (float) / =
	float h = 1 / (float)k;
	int tst;

	while(k >= 0){
		std::vector<float> vec(share.poly.size() * 2);
		// v 0 =
		int v = 0;
		for(int i = 0; i <= share.poly.size(); i++){

			x = share.poly[i].start.x * (1 - t) + share.poly[i].end.x * t;
			y = share.poly[i].start.y * (1 - t) + share.poly[i].end.y * t;
			vec[v] = x;
			vec[v+1] = y;
			v+=2;
		}

		int skr = 0;
		if(start.size() == 4 || end.size() == 4)
			skr = 3;
		else
			 skr = 2;

		for(int i = 0; i < vec.size()-3; i+=2){
			scene->addLine(vec[i]+90,90-vec[i+1],vec[i+2]+90,90-vec[i+3],pen3);
			ui->graphicsView->setScene(scene);
		}
		scene->addLine(vec[0]+90,90-vec[1],vec[vec.size()-2]+90,90-vec[vec.size()-1],pen3);
		ui->graphicsView->setScene(scene);
		t += h;
		k--;
		tst = vec[v];
	}


	for (int i = 1; i < start.size(); i++){
		temp = startDot[i].split(",");
		x = temp[0].toInt();
		y = temp[1].toInt();
		temp = startDot[i-1].split(",");
		x2 = temp[0].toInt();
		y2 = temp[1].toInt();
		scene->addLine(x+90,90-y,x2+90,90-y2,pen2);
	}

	ui->graphicsView->setScene(scene); // контрольное отображение

	if(tst != end[end.size()-1].x){
		this->on_pushButton_clicked();
	}

}
