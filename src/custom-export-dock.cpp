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

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QStringList filters;
    filters << "*.mp4" << "*.mkv" << "*.mov" << "*.flv" << "*.ts" << "*.m3u8";
    fileModel->setNameFilters(filters);
    fileModel->setNameFilterDisables(false);

    fileList = new QTreeView(content);
    fileList->setModel(fileModel);
    fileList->setRootIsDecorated(false);
    fileList->setSortingEnabled(true);
    fileList->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    fileList->setAlternatingRowColors(true);
    
    // Enable Drag and Drop (Native behavior)
    fileList->setDragEnabled(true);
    fileList->setDragDropMode(QAbstractItemView::DragOnly);
    
    // Removed Custom CSS to use Native Windows Theme
    
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
    connect(fileList, &QTreeView::clicked, this, &CustomExportDock::onFileClicked);
    
    // Handle Double Click (Open File)
    connect(fileList, &QTreeView::doubleClicked, this, &CustomExportDock::onDoubleClicked);
    
    // Enable context menu
    fileList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(fileList, &QTreeView::customContextMenuRequested, this, &CustomExportDock::showContextMenu);

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
    QString path = pathEdit->text();
    // QFileSystemModel handles logic internally, we just set root path for it to load
    // and set the view's root index
    QModelIndex rootIndex = fileModel->setRootPath(path);
    fileList->setRootIndex(rootIndex);
}

void CustomExportDock::onFileClicked(const QModelIndex &index) {
    if (!index.isValid()) return;
    QString name = fileModel->fileName(index);
    QFileInfo fi(name);
    // Avoid updating if it's not a file we care about (though filter handles most)
    filenameEdit->setText(fi.completeBaseName());
}

void CustomExportDock::showContextMenu(const QPoint &pos) {
    QModelIndex index = fileList->indexAt(pos);
    if (!index.isValid()) return;

    QString filePath = fileModel->filePath(index);
    QMenu contextMenu(tr("Context menu"), this);
    
    // 1. Open
    QAction *openAction = new QAction(tr("Open"), this);
    connect(openAction, &QAction::triggered, this, [this, index]() {
        onDoubleClicked(index);
    });
    contextMenu.addAction(openAction);

    // 2. Show in Explorer
    QAction *showInExplorerAction = new QAction(tr("Show in Explorer"), this);
    connect(showInExplorerAction, &QAction::triggered, this, [this, filePath]() {
        QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(filePath).absolutePath()));
    });
    contextMenu.addAction(showInExplorerAction);
    
    contextMenu.addSeparator();

    // 3. Rename
    QAction *renameAction = new QAction(tr("Rename"), this);
    connect(renameAction, &QAction::triggered, this, [this, index]() {
        renameFile(index);
    });
    contextMenu.addAction(renameAction);

    // 4. Delete
    QAction *deleteAction = new QAction(tr("Delete"), this);
    connect(deleteAction, &QAction::triggered, this, [this, filePath]() {
        if (QMessageBox::question(this, tr("Delete File"), 
            tr("Are you sure you want to delete this file?\n") + filePath,
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            
            QFile::remove(filePath);
        }
    });
    contextMenu.addAction(deleteAction);

    contextMenu.exec(fileList->mapToGlobal(pos));
}

void CustomExportDock::onDoubleClicked(const QModelIndex &index) {
    if (!index.isValid()) return;
    QString filePath = fileModel->filePath(index);
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void CustomExportDock::renameFile(const QModelIndex &index) {
    if (!index.isValid()) return;
    
    QString oldName = fileModel->fileName(index);
    QFileInfo fi(oldName);
    QString baseName = fi.completeBaseName();
    QString suffix = fi.suffix();

    QString path = pathEdit->text();
    QDir dir(path);

    bool ok;
    QString newBaseName = QInputDialog::getText(this, tr("Rename File"),
                                          tr("New name:"), QLineEdit::Normal,
                                          baseName, &ok);
    
    if (ok && !newBaseName.isEmpty() && newBaseName != baseName) {
        // Reconstruct full filename with original extension
        QString finalName = newBaseName;
        if (!suffix.isEmpty()) {
            finalName += "." + suffix;
        }

        QString newPath = dir.filePath(finalName);
        
        if (QFile::exists(newPath)) {
            QMessageBox::warning(this, tr("Rename Failed"), tr("A file with that name already exists."));
            return;
        }

        if (dir.rename(oldName, finalName)) {
            // Update the filename edit if we just renamed the selected file
            if (filenameEdit->text() == baseName) {
                 filenameEdit->setText(QFileInfo(finalName).completeBaseName());
            }
        } else {
            QMessageBox::critical(this, tr("Rename Failed"), tr("Could not rename the file. Check permissions."));
        }
    }
}
