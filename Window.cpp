#include "ClassificationOptions.h"
#include "Window.h"
#include "ui_Window.h"

#include <QFileDialog>
#include <QMessageBox>

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);
}

Window::~Window()
{
    delete ui;
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

    ClassificationOptions options;

    options.mSourceFolder = ui->txtSourceFolder->text();
    options.mOutputFolder = ui->txtOutputFolder->text();

    switch(ui->cmbClassificationCriteria->currentIndex())
    {
    case 1: // By year
        options.mClassificationCriteria = ClassificationCriteria::ByYear;

    case 2: // By year and month
        options.mClassificationCriteria = ClassificationCriteria::ByYearAndMonth;

    case 3: // By year, month and date
        options.mClassificationCriteria = ClassificationCriteria::ByYearMonthAndDay;

    default:
    case 0: // None
        options.mClassificationCriteria = ClassificationCriteria::None;
    }

    options.mUseCopy = ui->chkUseCopy->isChecked();
    options.mUseExtensionFolders = ui->chkAddFoldersForExtensions->isChecked();
    options.mRecursive = ui->chkSearchRecursively->isChecked();

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
