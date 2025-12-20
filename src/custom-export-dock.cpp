#include "custom-export-dock.hpp"
#include <obs-frontend-api.h>
#include <util/config-file.h>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QFileSystemWatcher>

CustomExportDock::CustomExportDock(QWidget *parent) : QDockWidget(parent) {
    setWindowTitle("Custom Export Panel");
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
    QLabel *pathLabel = new QLabel("Export Path", content);
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
    QLabel *fileLabel = new QLabel("File Name", content);
    layout->addWidget(fileLabel);

    filenameEdit = new QLineEdit(content);
    filenameEdit->setPlaceholderText("video_output");
    layout->addWidget(filenameEdit);
    
    // Warning label (inicialmente oculto)
    warningLabel = new QLabel(content);
    warningLabel->setStyleSheet("QLabel { color: #ff6b6b; font-weight: bold; padding: 5px; background-color: #2b2b2b; border-radius: 3px; }");
    warningLabel->setWordWrap(true);
    warningLabel->hide();
    layout->addWidget(warningLabel);

    // File List
    QLabel *listLabel = new QLabel("Existing Files:", content);
    layout->addWidget(listLabel);

    fileList = new QListWidget(content);
    layout->addWidget(fileList);

    layout->addStretch();
    content->setLayout(layout);
    setWidget(content);
    
    // Conectar señales para detectar cambios
    connect(pathEdit, &QLineEdit::textChanged, this, &CustomExportDock::onTextChanged);
    connect(filenameEdit, &QLineEdit::textChanged, this, &CustomExportDock::onTextChanged);
    
    // También verificar cuando cambie el texto
    connect(pathEdit, &QLineEdit::textChanged, this, &CustomExportDock::checkFileExists);
    connect(filenameEdit, &QLineEdit::textChanged, this, &CustomExportDock::checkFileExists);

    // Refresh file list when path changes
    connect(pathEdit, &QLineEdit::textChanged, this, &CustomExportDock::refreshFileList);
    
    // Handle file selection
    connect(fileList, &QListWidget::itemClicked, this, &CustomExportDock::onFileClicked);

    // File System Watcher
    fileWatcher = new QFileSystemWatcher(this);
    connect(fileWatcher, &QFileSystemWatcher::directoryChanged, this, &CustomExportDock::onDirectoryChanged);

    // Initial refresh
    refreshFileList();
}

CustomExportDock::~CustomExportDock() {}

QString CustomExportDock::getExportPath() const {
    return pathEdit->text();
}

QString CustomExportDock::getFileName() const {
    return filenameEdit->text();
}

void CustomExportDock::handleBrowse() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Path", pathEdit->text());
    if (!dir.isEmpty()) {
        pathEdit->setText(dir);
    }
}

void CustomExportDock::onTextChanged() {
    // Emitir señal cuando cambie cualquier texto
    emit configChanged();
}

void CustomExportDock::checkFileExists() {
    QString path = pathEdit->text();
    QString filename = filenameEdit->text();
    
    if (path.isEmpty() || filename.isEmpty()) {
        warningLabel->hide();
        return;
    }
    
    // Obtener la extensión del formato de grabación actual
    config_t *config = obs_frontend_get_profile_config();
    if (!config) {
        warningLabel->hide();
        return;
    }
    
    const char *mode = config_get_string(config, "Output", "Mode");
    const char *ext = "mp4";
    
    if (mode && strcmp(mode, "Advanced") == 0) {
        ext = config_get_string(config, "AdvOut", "RecFormat");
        if (!ext || !*ext) ext = "mkv";
    } else {
        ext = config_get_string(config, "SimpleOutput", "RecFormat");
        if (!ext || !*ext) ext = "mp4";
    }
    
    // Construir la ruta completa
    QString fullFilename = filename;
    if (!fullFilename.endsWith(QString(".") + ext, Qt::CaseInsensitive)) {
        fullFilename += QString(".") + ext;
    }
    QString fullPath = QDir(path).filePath(fullFilename);
    
    // Verificar si existe
    if (QFile::exists(fullPath)) {
        warningLabel->setText(QString("⚠ File '%1' already exists and will be overwritten").arg(fullFilename));
        warningLabel->show();
    } else {
        warningLabel->hide();
    }
}

void CustomExportDock::refreshFileList() {
    fileList->clear();
    QString path = pathEdit->text();
    QDir dir(path);
    
    // Update Watcher
    if (!fileWatcher->directories().isEmpty()) {
        fileWatcher->removePaths(fileWatcher->directories());
    }
    
    if (dir.exists()) {
        fileWatcher->addPath(path);
        
        QStringList filters;
        filters << "*.mp4" << "*.mkv" << "*.mov" << "*.flv" << "*.ts" << "*.m3u8";
        dir.setNameFilters(filters);
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        
        QFileInfoList list = dir.entryInfoList();
        for (const QFileInfo &fileInfo : list) {
             fileList->addItem(fileInfo.fileName());
        }
    }
}

void CustomExportDock::onDirectoryChanged(const QString &path) {
    Q_UNUSED(path);
    refreshFileList();
}

void CustomExportDock::onFileClicked(QListWidgetItem *item) {
    if (!item) return;
    QString name = item->text();
    QFileInfo fi(name);
    filenameEdit->setText(fi.completeBaseName());
}
