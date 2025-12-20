#pragma once

#include <QDockWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QFileDialog>
#include <QStandardPaths>
#include <QListWidget>
#include <QFileSystemWatcher>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QMessageBox>

class CustomExportDock : public QDockWidget {
    Q_OBJECT

public:
    CustomExportDock(QWidget *parent = nullptr);
    ~CustomExportDock() override;

    QString getExportPath() const;
    QString getFileName() const;

signals:
    void configChanged();

public slots:
    void refreshFileList();

private slots:
    void handleBrowse();
    void onTextChanged();
    void checkFileExists();
    void onFileClicked(QListWidgetItem *item);
    void onDirectoryChanged(const QString &path);
    void showContextMenu(const QPoint &pos);
    void renameFile(QListWidgetItem *item);

private:
    QLineEdit *pathEdit;
    QLineEdit *filenameEdit;
    QPushButton *browseBtn;
    QLabel *warningLabel;
    QListWidget *fileList;
    QFileSystemWatcher *fileWatcher;
};
