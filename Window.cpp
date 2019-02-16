
#include "Window.h"
#include "ui_Window.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QtConcurrent>
#include <QDebug>
#include <QStandardPaths>

#include "Classifier.h"

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);

    // Populate the textboxes
    ui->txtSourceFolder->setText(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation)[0]);
    ui->txtOutputFolder->setText(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation)[0]);
}

Window::~Window()
{
    delete ui;
}

void Window::appendLogLine(const QString &line)
{
    ui->txtLog->appendPlainText(line);
}

void Window::on_btnBrowseSourceFolder_clicked()
{
    auto folder = browseDirectory("Select source folder");

    if (!folder.isEmpty())
        ui->txtSourceFolder->setText(folder);
}

void Window::on_btnBrowseOutputFolder_clicked()
{
    auto folder = browseDirectory("Select output folder");

    if (!folder.isEmpty())
        ui->txtOutputFolder->setText(folder);
}

void Window::on_btnRun_clicked()
{
    if (ui->txtSourceFolder->text().isEmpty())
    {
        QMessageBox::information(this, "Select source folder", "Please select the source folder");
        return;
    }

    if (ui->txtOutputFolder->text().isEmpty())
    {
        QMessageBox::information(this, "Select output folder", "Please select the output folder");
        return;
    }

    // Build the classification options container
    ClassificationOptions options = encapsulateOptions();

    // Setup the ui
    disableInterface();

    mClassifier = new Classifier();
    mClassifier->setOptions(options);

    connect(mClassifier, &Classifier::finished, this, &Window::classificationFinished);
    connect(mClassifier, &Classifier::logOutput, this, &Window::appendLogLine);

    QThreadPool::globalInstance()->start(mClassifier);
}

void Window::classificationFinished()
{
    enableInterface();

    qDebug() << ui->txtLog->toPlainText().count("\n");
    ui->lblStatus->setText("Classification finished");
}

void Window::enableInterface()
{
    ui->txtSourceFolder->setEnabled(true);
    ui->txtOutputFolder->setEnabled(true);
    ui->btnBrowseOutputFolder->setEnabled(true);
    ui->btnBrowseSourceFolder->setEnabled(true);

    ui->cmbClassificationCriteria->setEnabled(true);

    ui->chkAddFoldersForExtensions->setEnabled(true);
    ui->chkSearchRecursively->setEnabled(true);
    ui->chkUseCopy->setEnabled(true);

    ui->btnRun->setText("Run");
}

void Window::disableInterface()
{
    ui->txtSourceFolder->setEnabled(false);
    ui->txtOutputFolder->setEnabled(false);
    ui->btnBrowseOutputFolder->setEnabled(false);
    ui->btnBrowseSourceFolder->setEnabled(false);

    ui->cmbClassificationCriteria->setEnabled(false);

    ui->chkAddFoldersForExtensions->setEnabled(false);
    ui->chkSearchRecursively->setEnabled(false);
    ui->chkUseCopy->setEnabled(false);

    ui->btnRun->setText("Cancel");
}

ClassificationOptions Window::encapsulateOptions()
{
    ClassificationOptions options;

    options.mSourceFolder = ui->txtSourceFolder->text();
    options.mOutputFolder = ui->txtOutputFolder->text();

    switch(ui->cmbClassificationCriteria->currentIndex())
    {
    case 1: // By year
        options.mClassificationCriteria = ClassificationCriteria::ByYear;
        break;

    case 2: // By year and month
        options.mClassificationCriteria = ClassificationCriteria::ByYearAndMonth;
        break;

    case 3: // By year, month and date
        options.mClassificationCriteria = ClassificationCriteria::ByYearMonthAndDay;
        break;

    default:
    case 0: // None
        options.mClassificationCriteria = ClassificationCriteria::None;
        break;
    }

    options.mUseCopy = ui->chkUseCopy->isChecked();
    options.mUseExtensionFolders = ui->chkAddFoldersForExtensions->isChecked();
    options.mRecursive = ui->chkSearchRecursively->isChecked();
    options.mOnlyMedia = ui->chkOnlyMedia->isChecked();

    return options;
}

QString Window::browseDirectory(const QString &dialogTitle)
{
    QFileDialog dialog(this);
    dialog.setWindowTitle(dialogTitle);
    dialog.setFileMode(QFileDialog::Directory);

    if (!dialog.exec())
        return "";

    QStringList selectedFiles = dialog.selectedFiles();

    if (selectedFiles.count() == 0)
        return "";

    return selectedFiles[0];
}
