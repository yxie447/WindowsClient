// startup.h
#ifndef STARTUP_H
#define STARTUP_H

#include <QWidget>
#include <QWebEngineView>

namespace Ui {
class startup;
}

class startup : public QWidget
{
    Q_OBJECT

public:
    explicit startup(QWidget *parent = nullptr);
    ~startup();

private slots:
    void goToLogin();
    void goToSignUp();
    void goToMain();
    void goToScanner();
    void processLogin();
    void processSignUp();
    void processReload();
    void processConnect();
    void onListViewClicked(const QModelIndex &index);


private:
    Ui::startup *ui;
    QWebEngineView webView;
};

#endif // STARTUP_H
