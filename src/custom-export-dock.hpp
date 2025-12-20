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

class CustomExportDock : public QDockWidget {
    Q_OBJECT

public:
    CustomExportDock(QWidget *parent = nullptr);
    ~CustomExportDock() override;

    QString getExportPath() const;
    QString getFileName() const;

signals:
    void configChanged();

private slots:
    void handleBrowse();
    void onTextChanged();

private:
    QLineEdit *pathEdit;
    QLineEdit *filenameEdit;
    QPushButton *browseBtn;
};
