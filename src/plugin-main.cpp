#include "custom-export-dock.hpp"
#include <obs-module.h>
#include <obs-frontend-api.h>
#include <util/platform.h>
#include <util/config-file.h>
#include <QDir>
#include <QStandardPaths>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QFileInfo>

OBS_DECLARE_MODULE()

CustomExportDock *exportDock = nullptr;

const char *obs_module_name(void) {
    return "Custom Export Panel";
}

const char *obs_module_description(void) {
    return "Adds a custom export panel to OBS";
}

void update_recording_path() {
    if (!exportDock) return;

    QString dir = exportDock->getExportPath();
    QString filename = exportDock->getFileName();

    blog(LOG_INFO, "[CustomExport] update_recording_path called");
    blog(LOG_INFO, "[CustomExport] Dir from panel: %s", dir.toUtf8().constData());
    blog(LOG_INFO, "[CustomExport] Filename from panel: %s", filename.toUtf8().constData());

    if (dir.isEmpty() || filename.isEmpty()) {
        blog(LOG_WARNING, "[CustomExport] Path or Filename empty, ignoring custom export.");
        return;
    }

    // Actualizar la configuración de OBS ANTES de iniciar la grabación
    config_t *config = obs_frontend_get_profile_config();
    if (config) {
        const char *mode = config_get_string(config, "Output", "Mode");
        blog(LOG_INFO, "[CustomExport] OBS Output Mode: %s", mode ? mode : "NULL");
        
        // Obtener la extensión actual
        const char *ext = "mp4";
        if (mode && strcmp(mode, "Advanced") == 0) {
            ext = config_get_string(config, "AdvOut", "RecFormat");
            if (!ext || !*ext) ext = "mkv";
        } else {
            ext = config_get_string(config, "SimpleOutput", "RecFormat");
            if (!ext || !*ext) ext = "mp4";
        }
        
        // Construir la ruta completa con extensión
        QString fullPath = QDir(dir).filePath(filename);
        if (!fullPath.endsWith(QString(".") + ext, Qt::CaseInsensitive)) {
            fullPath += QString(".") + ext;
        }
        
        blog(LOG_INFO, "[CustomExport] Full path: %s", fullPath.toUtf8().constData());
        
        if (mode && strcmp(mode, "Advanced") == 0) {
            // Modo avanzado
            blog(LOG_INFO, "[CustomExport] Using Advanced mode settings");
            config_set_string(config, "AdvOut", "RecFilePath", dir.toUtf8().constData());
            // Usar solo el nombre sin extensión, OBS añade la extensión
            config_set_string(config, "AdvOut", "RecFilenameFormatting", filename.toUtf8().constData());
        } else {
            // Modo simple
            blog(LOG_INFO, "[CustomExport] Using Simple mode settings");
            config_set_string(config, "SimpleOutput", "FilePath", dir.toUtf8().constData());
            // Usar solo el nombre sin extensión, OBS añade la extensión
            config_set_string(config, "SimpleOutput", "FilenameFormatting", filename.toUtf8().constData());
        }
        
        config_save(config);
        blog(LOG_INFO, "[CustomExport] Config saved - OBS should use this on next recording start");
        
        // IMPORTANTE: Forzar a OBS a recargar la configuración
        // Esto hace que OBS use el nuevo nombre en la próxima grabación
        obs_frontend_save();
    } else {
        blog(LOG_ERROR, "[CustomExport] Could not get profile config!");
    }
}

void frontend_event(enum obs_frontend_event event, void *data) {
    if (event == OBS_FRONTEND_EVENT_RECORDING_STOPPED) {
        // Renombrar el archivo después de que termine la grabación
        if (!exportDock) return;
        
        QString customName = exportDock->getFileName();
        QString customPath = exportDock->getExportPath();
        
        if (customName.isEmpty() || customPath.isEmpty()) {
            blog(LOG_INFO, "[CustomExport] No custom name/path set, skipping rename");
            return;
        }
        
        // Obtener la ruta del último archivo grabado
        config_t *config = obs_frontend_get_profile_config();
        if (!config) return;
        
        const char *mode = config_get_string(config, "Output", "Mode");
        const char *lastFile = nullptr;
        
        if (mode && strcmp(mode, "Advanced") == 0) {
            lastFile = config_get_string(config, "AdvOut", "RecFilePath");
        } else {
            lastFile = config_get_string(config, "SimpleOutput", "FilePath");
        }
        
        if (!lastFile) {
            blog(LOG_WARNING, "[CustomExport] Could not get last recording path");
            return;
        }
        
        // Buscar el archivo más reciente en el directorio
        QDir dir(lastFile);
        if (!dir.exists()) {
            blog(LOG_WARNING, "[CustomExport] Recording directory does not exist: %s", lastFile);
            return;
        }
        
        QFileInfoList files = dir.entryInfoList(QStringList() << "*.mp4" << "*.mkv" << "*.flv", 
                                                 QDir::Files, QDir::Time);
        
        if (files.isEmpty()) {
            blog(LOG_WARNING, "[CustomExport] No recording files found in %s", lastFile);
            return;
        }
        
        // El primer archivo es el más reciente
        QFileInfo mostRecent = files.first();
        QString oldPath = mostRecent.absoluteFilePath();
        
        // Construir el nuevo nombre
        QString extension = mostRecent.suffix();
        QString newFileName = customName;
        if (!newFileName.endsWith("." + extension, Qt::CaseInsensitive)) {
            newFileName += "." + extension;
        }
        
        QString newPath = QDir(customPath).filePath(newFileName);
        
        blog(LOG_INFO, "[CustomExport] Renaming recording:");
        blog(LOG_INFO, "[CustomExport]   From: %s", oldPath.toUtf8().constData());
        blog(LOG_INFO, "[CustomExport]   To:   %s", newPath.toUtf8().constData());
        
        // Renombrar el archivo
        QFile file(oldPath);
        if (file.rename(newPath)) {
            blog(LOG_INFO, "[CustomExport] File renamed successfully!");
        } else {
            blog(LOG_ERROR, "[CustomExport] Failed to rename file: %s", file.errorString().toUtf8().constData());
        }
    }
}

bool obs_module_load(void) {
    blog(LOG_INFO, "[CustomExport] obs_module_load called");
    
    // Add event callback
    obs_frontend_add_event_callback(frontend_event, nullptr);
    blog(LOG_INFO, "[CustomExport] Event callback registered");
    
    // Create Dock
    blog(LOG_INFO, "[CustomExport] Creating CustomExportDock...");
    exportDock = new CustomExportDock();
    blog(LOG_INFO, "[CustomExport] CustomExportDock created at %p", exportDock);
    
    
    blog(LOG_INFO, "[CustomExport] Registering dock with OBS...");
    
    // Usar add_dock_by_id que añade automáticamente al menú
    obs_frontend_add_dock_by_id("CustomExportPanel", "Panel de Exportación Personalizado", exportDock);
    
    blog(LOG_INFO, "[CustomExport] Dock registered successfully");
    
    return true;
}

void obs_module_unload(void) {
    // Cleanup if necessary
    // obs_frontend_add_dock transfers ownership to the main window usually?
    // Actually no, we should check docs. But standard plugins often don't explicitly delete if parented.
    // However, we didn't parent it to MainWindow yet (add_dock does).
}
