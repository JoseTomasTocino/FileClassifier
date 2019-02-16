#include "Classifier.h"
#include "exif.h"

#include <QDir>
#include <QRegularExpression>
#include <QDebug>
#include <QDirIterator>
#include <QThread>
#include <QDir>

Classifier::Classifier(QObject *parent)
    : QObject(parent)
{

}

void Classifier::run()
{
    QStringList nameFilters;
    QDirIterator::IteratorFlags flags = QDirIterator::NoIteratorFlags;

    if (mOptions.mRecursive)
        flags = QDirIterator::Subdirectories;

    if (mOptions.mOnlyMedia)
        nameFilters = QStringList { "*.jpg", "*.jpeg", "*.mp4", "*.png", "*.mov" };
    else
        nameFilters = QStringList { "*" };


    QDirIterator it(mOptions.mSourceFolder, nameFilters, QDir::Files | QDir::NoDotAndDotDot, flags);

    int elems = 0;

    while (it.hasNext())
    {
        QFileInfo file(it.next());
        QString outputFullPath = mOptions.mOutputFolder + "/";

        QDate fileDate = computeDateForFile(file);

        if (fileDate.isValid())
        {
            if (mOptions.mClassificationCriteria == ClassificationCriteria::ByYear)
            {
                outputFullPath += QString("%1/").arg(fileDate.year());
            }

            else if (mOptions.mClassificationCriteria == ClassificationCriteria::ByYearAndMonth)
            {
                outputFullPath += QString("%1/%2/").arg(fileDate.year()).arg(fileDate.month(), 2, 10, QChar('0'));
            }

            else if (mOptions.mClassificationCriteria == ClassificationCriteria::ByYearMonthAndDay)
            {
                outputFullPath += QString("%1/%2/%3/").arg(fileDate.year()).arg(fileDate.month(), 2, 10, QChar('0')).arg(fileDate.day(), 2, 10, QChar('0'));
            }
        }

        if (mOptions.mUseExtensionFolders)
        {
            outputFullPath += file.suffix() + "/";
        }

        // Create the directories
        QDir().mkpath(outputFullPath);

        outputFullPath += file.fileName();

        if (mOptions.mUseCopy)
        {
            QFile::copy(file.absoluteFilePath(), outputFullPath);
        }

        else
        {
            QFile::rename(file.absoluteFilePath(), outputFullPath);
        }

        const QString logLine = QString("%1 -> %2").arg(file.absoluteFilePath()).arg(outputFullPath);

        emit logOutput(logLine);

        elems++;

        QThread::currentThread()->msleep(1);
    }

    emit finished();
}

void Classifier::setOptions(const ClassificationOptions &options)
{
    mOptions = options;
}

QDate Classifier::computeDateForFile(const QFileInfo &path)
{
    const QString & pathStr = path.absoluteFilePath();

    // If it's a JPEG, try with the EXIF detection
    if (pathStr.toLower().endsWith("jpeg") || pathStr.toLower().endsWith("jpg"))
    {
        QFile file(pathStr);

        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray fileContent = file.readAll();
            easyexif::EXIFInfo result;

            int parsingResult = result.parseFrom((unsigned char *)fileContent.data(), fileContent.size());

            if (parsingResult == 0 && result.DateTime != "")
            {
                auto dateTimeComponents = QString::fromStdString(result.DateTime).split(" ")[0].split(":");
                return QDate(dateTimeComponents[0].toInt(), dateTimeComponents[1].toInt(), dateTimeComponents[2].toInt());
            }
        }
    }

    // If the EXIF identification failed, try parsing the name
    QRegularExpression filenamePattern("[^\\d](\\d{4})(\\d{2})(\\d{2})[^\\d]");
    QRegularExpressionMatch match = filenamePattern.match(path.fileName());

    if (match.hasMatch())
    {
        return QDate(match.captured(1).toInt(), match.captured(2).toInt(), match.captured(3).toInt());
    }

    return QDate();
}
