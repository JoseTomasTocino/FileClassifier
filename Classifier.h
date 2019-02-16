#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <QDate>
#include <QFileInfo>
#include <QObject>
#include <QRunnable>

#include "ClassificationOptions.h"

class Classifier : public QObject, public QRunnable
{
    Q_OBJECT

public:

    Classifier(QObject * parent = nullptr);

    void run();

    void setOptions(const ClassificationOptions &options);

signals:
    void finished();
    void logOutput(const QString & line);

private:
    static QDate computeDateForFile(const QFileInfo &path);
    ClassificationOptions mOptions;
};

#endif // CLASSIFIER_H
