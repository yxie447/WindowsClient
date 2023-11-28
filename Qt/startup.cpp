// startup.cpp
#include "startup.h"
#include "ui_startup.h"
#include <QMessageBox>
#include <QApplication>
#include <QStandardItemModel>
#include <stdlib.h>
#include <stdio.h>

startup::startup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::startup)

{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(3); // set first page to always be scanner
    // here you will need to add an if statement to see if we want the user to see the scanner or not, if you dont want to see it
    // then set the index to 0


    connect(ui->MainLoginButton, SIGNAL(clicked()), this, SLOT(goToLogin()));
    connect(ui->MainSignupButton, SIGNAL(clicked()), this, SLOT(goToSignUp()));

    connect(ui->LoginPageLoginButton, SIGNAL(clicked()), this, SLOT(processLogin()));
    connect(ui->LoginPageBackButton, SIGNAL(clicked()), this, SLOT(goToMain()));

    connect(ui->SignUpPageSignUpButton, SIGNAL(clicked()), this, SLOT(processSignUp()));
    connect(ui->SignUpPageBackButton, SIGNAL(clicked()), this, SLOT(goToMain()));

    connect(ui->scannerReload, SIGNAL(clicked()), this, SLOT(processReload()));
    connect(ui->listView, &QListView::clicked, this, &startup::onListViewClicked);

    connect(ui->scannerConnect, SIGNAL(clicked()), this, SLOT(processConnect()));
}

startup::~startup()
{
    delete ui;
}

void startup::goToLogin()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void startup::goToSignUp()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void startup::goToMain()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void startup::goToScanner()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void startup::processLogin()
{
    // check if user login and matches then do the below
    close();
    webView.load(QUrl("http://0.0.0.0:8080/"));
    webView.resize(1024, 750);
    webView.show();
}

void startup::processSignUp()
{
    QString password = ui->SignUpPassword->text();
    QString confirmPassword = ui->SignUpPassword_2->text();

    QString username = ui->SignUpUsername->text();

    QLabel *errorLabel = ui->SignUpErrorLabel;

    if (password != confirmPassword) {
        // if passwords don't match show message
        errorLabel->setText("Passwords must match");
        errorLabel->setVisible(true);

    } else {
        errorLabel->clear();
        errorLabel->setVisible(false);
        // if match continue with the sign-up
        // put logic in here
        // compare password and username with db etc..
        // then call processSignUp(); to load into the Wt app
        close();
        webView.load(QUrl("http://0.0.0.0:8080/"));
        webView.resize(1024, 750);
        webView.show();
    }
}

void startup::processReload()
{
    // when you hit the reload button the scanner page it needs to show the list of devices, put the logic for that here and iterate over it to create the ui elements
    // ui->stackedWidget->setCurrentIndex(3);
    // index 3 for scanner page
    QStandardItemModel *model = new QStandardItemModel(this);
    model->clear();
    int randomN; //delete this placeholder
    randomN = rand() % 10 +1; //delete this placeholder
    // Add labels iteratively to the model
    for (int i = 0; i < randomN*2; ++i) {
        QString label = "Device #" + QString::number(i + 1) + "  |  IP address: " + QString::number(i + randomN * i); //reformat string as needed
        QStandardItem *item = new QStandardItem(label);
        model->appendRow(item);
    }


    ui->listView->setModel(model);
}

void startup::onListViewClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QString selectedText = index.data(Qt::DisplayRole).toString();
        ui->selectedLabel->setText("Selected: " + selectedText);

    }
}

void startup::processConnect()
{
    // you can take the selected text and parse it for the IP or however you set up the string, then you can send it to whatever function you need below
    goToMain();
}
