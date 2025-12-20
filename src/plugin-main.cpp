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

    if (dir.isEmpty() || filename.isEmpty()) {
        blog(LOG_WARNING, "[CustomExport] Path or Filename empty, ignoring custom export.");
        return;
    }

    // Determine extension based on current profile settings
    const char *ext = "mp4"; // generic fallback
    config_t *config = obs_frontend_get_profile_config();
    if (config) {
         const char *mode = config_get_string(config, "Output", "Mode");
         if (mode && strcmp(mode, "Advanced") == 0) {
             ext = config_get_string(config, "AdvOut", "RecFormat");
         } else {
             ext = config_get_string(config, "SimpleOutput", "RecFormat");
         }
    }
    
    // Fallback if ext is null or empty
    if (!ext || !*ext) ext = "mkv";

    QString fullPath = QDir(dir).filePath(filename);
    
    // Append extension if missing
    // We strictly assume the user wants the selected container format.
    if (!fullPath.endsWith(QString(".") + ext, Qt::CaseInsensitive)) {
        fullPath += QString(".") + ext;
    }

    std::string pathStr = fullPath.toUtf8().constData();
    blog(LOG_INFO, "[CustomExport] Updating recording path to: %s", pathStr.c_str());

    obs_output_t *output = obs_frontend_get_recording_output();
    if (output) {
        obs_data_t *settings = obs_output_get_settings(output);
        const char *id = obs_output_get_id(output);
        
        // ffmpeg_output uses "url", flv_output uses "path"
        if (strcmp(id, "ffmpeg_output") == 0) {
            obs_data_set_string(settings, "url", pathStr.c_str());
        } else {
            obs_data_set_string(settings, "path", pathStr.c_str());
        }
        
        // Apply settings
        obs_output_update(output, settings);
        
        obs_data_release(settings);
        obs_output_release(output);
    } else {
        blog(LOG_WARNING, "[CustomExport] Could not get recording output.");
    }
}

void frontend_event(enum obs_frontend_event event, void *data) {
    if (event == OBS_FRONTEND_EVENT_RECORDING_STARTING) {
        update_recording_path();
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
