#include <QtTest>
#include <QCoreApplication>

//#include "controller.h"


// add necessary includes here

class ControllerTest : public QObject
{
    Q_OBJECT

public:
    ControllerTest();
    ~ControllerTest();

        //Art *art;

        //Controller *controller;

private slots:

    void createController();

private:



};

ControllerTest::ControllerTest()
{

}

ControllerTest::~ControllerTest()
{

}



void ControllerTest::createController()
{
     //Art *art;
     //art = new Art();

     //Controller controller(art);
     QVERIFY(1==1);
}

QTEST_MAIN(ControllerTest)

#include "controller_test.moc"
