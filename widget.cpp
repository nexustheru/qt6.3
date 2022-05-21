#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QDir>

QString path1=QCoreApplication::applicationDirPath()+"/vertex.glsl";
QString path2=QCoreApplication::applicationDirPath()+"/fragment.glsl";

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->textEdit,SIGNAL(textChanged()),this,SLOT(compileShader()));
    connect(ui->textEdit_2,SIGNAL(textChanged()),this,SLOT(compileShader1()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(saveshaders()));
    LoadShaders();
}

void Widget::LoadShaders()
{
 QString path1="vertex.glsl";
 QFile vert(path1);
 vert.open(QIODevice::ReadOnly | QIODevice::Text);
 QByteArray line1 = vert.readAll();
 ui->textEdit->insertPlainText(line1);

 QString path2="fragment.glsl";
 QFile frag(path2);
 qDebug("openend");
 frag.open(QIODevice::ReadOnly | QIODevice::Text);
 QByteArray line2 = frag.readAll();
 ui->textEdit_2->insertPlainText(line2);
}

void Widget::compileShader()
{

    auto Shader = new QOpenGLShaderProgram();

    if(Shader->addShaderFromSourceCode(QOpenGLShader::Vertex, ui->textEdit->toPlainText()))
    {
        ui->label->setText("vertex shader compiled sucessfully");
    }
    else
    {
        ui->label->setText("Vertex: "+ Shader->log());
    }

}
void Widget::compileShader1()
{

    auto Shader = new QOpenGLShaderProgram();
    if(Shader->addShaderFromSourceCode(QOpenGLShader::Fragment, ui->textEdit_2->toPlainText()))
    {
          ui->label->setText("fragment shader compiled sucessfully");
    }
    else
    {
        ui->label->setText("Fragment: "+ Shader->log());
    }

}

void Widget::saveshaders()
{
    QString path1="vertex.glsl";
    QFile vert(path1);
    vert.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&vert);
    out << ui->textEdit->toPlainText();
    vert.close();

    QString path2="fragment.glsl";
    QFile frag(path2);
    frag.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out1(&frag);
    out1 << ui->textEdit_2->toPlainText();
    frag.close();
     ui->label->setText("Shaders been saved");
}
Widget::~Widget()
{
    delete ui;
}

