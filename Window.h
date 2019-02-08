#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

private slots:
    void on_btnBrowseSourceFolder_clicked();

    void on_btnBrowseOutputFolder_clicked();

    void on_btnRun_clicked();

private:
    QString browseDirectory(const QString & dialogTitle);

    Ui::Window *ui;
};

#endif // WINDOW_H
