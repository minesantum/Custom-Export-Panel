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
#include <QTreeView>
#include <QFileSystemModel>
#include <QHeaderView>
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
    void onFileClicked(const QModelIndex &index);
    void showContextMenu(const QPoint &pos);
    void renameFile(const QModelIndex &index);

private:
    QLineEdit *pathEdit;
    QLineEdit *filenameEdit;
    QPushButton *browseBtn;
    QLabel *warningLabel;
    QTreeView *fileList;
    QFileSystemModel *fileModel;
};
