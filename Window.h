#ifndef WINDOW_H
#define WINDOW_H

#include <QFutureWatcher>
#include <QWidget>

#include "Classifier.h"
#include "ClassificationOptions.h"

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

public slots:
    void appendLogLine(const QString & line);

private slots:
    void on_btnBrowseSourceFolder_clicked();
    void on_btnBrowseOutputFolder_clicked();
    void on_btnRun_clicked();

    void classificationFinished();

private:
    void enableInterface();
    void disableInterface();

    ClassificationOptions encapsulateOptions();
    QString browseDirectory(const QString & dialogTitle);

    Classifier * mClassifier;

    Ui::Window *ui;
};

#endif // WINDOW_H
