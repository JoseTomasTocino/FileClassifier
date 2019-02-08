#include "Window.h"
#include "ui_Window.h"

#include <QFileDialog>

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
