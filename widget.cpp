#include "widget.h"
#include "ui_widget.h"
#include <QFile>
QString path1=QCoreApplication::applicationDirPath()+"/vertex.glsl";
QString path2=QCoreApplication::applicationDirPath()+"/fragment.glsl";

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

//    QSurfaceFormat fmt;
//    fmt.setDepthBufferSize(24);
//    fmt.setSamples(8);
//    fmt.setVersion(3, 2);
//    fmt.setOption(QSurfaceFormat::DebugContext);
//    fmt.setProfile(QSurfaceFormat::CoreProfile);
//    QOpenGLContext *context = new QOpenGLContext;
//    context->setFormat(fmt);
//    context->create();
//    ui->openGLWidget->setFormat(fmt);
    connect(ui->textEdit,SIGNAL(textChanged()),this,SLOT(compileShader()));

    LoadShaders();
}

void Widget::LoadShaders()
{

 QFile vert(path1);
 QFile frag(path2);

 vert.open(QIODevice::ReadOnly | QIODevice::Text);
 frag.open(QIODevice::ReadOnly | QIODevice::Text);

 QByteArray line1 = vert.readAll();
 QByteArray line2 = frag.readAll();

 ui->textEdit->insertPlainText(line1);
 ui->textEdit_2->insertPlainText(line2);
}

void Widget::compileShader()
{
    qDebug("text edited");

}

Widget::~Widget()
{
    delete ui;
}

