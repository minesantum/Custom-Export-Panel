#include "custom-export-dock.hpp"
#include <QDir>
#include <QStandardPaths>

CustomExportDock::CustomExportDock(QWidget *parent) : QDockWidget(parent) {
    setWindowTitle("Panel de Exportación Personalizado");
    setObjectName("CustomExportPanel"); // Internal name for saving state
    
    // Configurar features para que respete el bloqueo de paneles
    // Solo permitir cerrar, OBS maneja el resto según el estado de bloqueo
    setFeatures(QDockWidget::DockWidgetClosable);
    
    // Usar un widget vacío como barra de título para evitar la doble barra
    // OBS añadirá su propia barra de título
    setTitleBarWidget(new QWidget());

    QWidget *content = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(content);

    // Export Path
    QLabel *pathLabel = new QLabel("Ruta de exportación", content);
    layout->addWidget(pathLabel);

    QHBoxLayout *pathLayout = new QHBoxLayout();
    pathEdit = new QLineEdit(content);
    // Default to Movies dir
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    pathEdit->setText(defaultPath);

    browseBtn = new QPushButton("...", content);
    connect(browseBtn, &QPushButton::clicked, this, &CustomExportDock::handleBrowse);

    pathLayout->addWidget(pathEdit);
    pathLayout->addWidget(browseBtn);
    layout->addLayout(pathLayout);

    // Filename
    QLabel *fileLabel = new QLabel("Nombre del archivo", content);
    layout->addWidget(fileLabel);

    filenameEdit = new QLineEdit(content);
    filenameEdit->setPlaceholderText("video_output");
    layout->addWidget(filenameEdit);

    layout->addStretch();
    content->setLayout(layout);
    setWidget(content);
}

CustomExportDock::~CustomExportDock() {}

QString CustomExportDock::getExportPath() const {
    return pathEdit->text();
}

QString CustomExportDock::getFileName() const {
    return filenameEdit->text();
}

void CustomExportDock::handleBrowse() {
    QString dir = QFileDialog::getExistingDirectory(this, "Seleccionar Ruta", pathEdit->text());
    if (!dir.isEmpty()) {
        pathEdit->setText(dir);
    }
}
