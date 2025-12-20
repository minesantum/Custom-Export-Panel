#include <obs-module.h>
#include <obs-frontend-api.h>
#include <util/config-file.h>
#include <util/platform.h>
#include <util/dstr.h>
#include <string>
#include <fstream>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("custom-export-panel", "en-US")

// Creador: DonKolia
MODULE_EXPORT const char *obs_module_description(void)
{
	return "Custom Export Panel - Permite configurar ruta y nombre de grabación";
}

MODULE_EXPORT const char *obs_module_name(void)
{
	return "Custom Export Panel";
}

// Configuración del plugin
struct export_config {
	std::string export_path;
	std::string filename;
	std::string extension;
};

static export_config g_config;

// Obtener la ruta del archivo de configuración
static std::string get_config_path()
{
	char *config_dir = obs_module_config_path("");
	std::string path = std::string(config_dir) + "/export_config.ini";
	bfree(config_dir);
	return path;
}

// Cargar configuración desde archivo INI
static void load_config()
{
	std::string config_path = get_config_path();
	std::ifstream file(config_path);
	
	// Valores por defecto
	g_config.export_path = "";
	g_config.filename = "recording";
	g_config.extension = ""; // Auto-detectar
	
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			if (line.empty() || line[0] == '#' || line[0] == ';')
				continue;
				
			size_t pos = line.find('=');
			if (pos != std::string::npos) {
				std::string key = line.substr(0, pos);
				std::string value = line.substr(pos + 1);
				
				// Trim espacios
				key.erase(0, key.find_first_not_of(" \t"));
				key.erase(key.find_last_not_of(" \t") + 1);
				value.erase(0, value.find_first_not_of(" \t"));
				value.erase(value.find_last_not_of(" \t") + 1);
				
				if (key == "export_path") {
					g_config.export_path = value;
				} else if (key == "filename") {
					g_config.filename = value;
				} else if (key == "extension") {
					g_config.extension = value;
				}
			}
		}
		file.close();
		blog(LOG_INFO, "[CustomExport] Configuración cargada desde: %s", config_path.c_str());
	} else {
		blog(LOG_INFO, "[CustomExport] No se encontró archivo de configuración. Usando valores por defecto.");
		blog(LOG_INFO, "[CustomExport] Crea el archivo: %s", config_path.c_str());
		blog(LOG_INFO, "[CustomExport] Formato:");
		blog(LOG_INFO, "[CustomExport]   export_path=C:\\Users\\TuUsuario\\Videos");
		blog(LOG_INFO, "[CustomExport]   filename=mi_grabacion");
		blog(LOG_INFO, "[CustomExport]   extension=mkv");
	}
}

// Detectar extensión del formato de grabación
static std::string get_recording_extension()
{
	if (!g_config.extension.empty()) {
		return g_config.extension;
	}
	
	// Auto-detectar desde la configuración de OBS
	config_t *config = obs_frontend_get_profile_config();
	if (!config) {
		return "mkv"; // Por defecto
	}
	
	const char *format = config_get_string(config, "AdvOut", "RecFormat");
	if (!format || !*format) {
		format = config_get_string(config, "SimpleOutput", "RecFormat");
	}
	
	if (format && *format) {
		return std::string(format);
	}
	
	return "mkv";
}

// Actualizar ruta de grabación
static void update_recording_path()
{
	if (g_config.export_path.empty()) {
		blog(LOG_INFO, "[CustomExport] Ruta de exportación no configurada. Usando ruta por defecto de OBS.");
		return;
	}
	
	std::string extension = get_recording_extension();
	std::string full_path = g_config.export_path + "/" + g_config.filename + "." + extension;
	
	blog(LOG_INFO, "[CustomExport] Actualizando ruta de grabación a: %s", full_path.c_str());
	
	obs_output_t *output = obs_frontend_get_recording_output();
	if (output) {
		obs_data_t *settings = obs_output_get_settings(output);
		const char *id = obs_output_get_id(output);
		
		// ffmpeg_output usa "url", otros usan "path"
		if (strcmp(id, "ffmpeg_output") == 0 || strcmp(id, "ffmpeg_muxer") == 0) {
			obs_data_set_string(settings, "url", full_path.c_str());
		} else {
			obs_data_set_string(settings, "path", full_path.c_str());
		}
		
		obs_output_update(output, settings);
		
		obs_data_release(settings);
		obs_output_release(output);
		
		blog(LOG_INFO, "[CustomExport] Ruta actualizada exitosamente");
	} else {
		blog(LOG_WARNING, "[CustomExport] No se pudo obtener el output de grabación");
	}
}

// Callback de eventos de OBS
static void frontend_event_callback(enum obs_frontend_event event, void *data)
{
	UNUSED_PARAMETER(data);
	
	if (event == OBS_FRONTEND_EVENT_RECORDING_STARTING) {
		update_recording_path();
	}
}

MODULE_EXPORT bool obs_module_load(void)
{
	blog(LOG_INFO, "[CustomExport] Plugin cargado - Creador: DonKolia");
	
	// Cargar configuración
	load_config();
	
	// Registrar callback de eventos
	obs_frontend_add_event_callback(frontend_event_callback, nullptr);
	
	blog(LOG_INFO, "[CustomExport] Configuración actual:");
	blog(LOG_INFO, "[CustomExport]   Ruta: %s", g_config.export_path.empty() ? "(no configurada)" : g_config.export_path.c_str());
	blog(LOG_INFO, "[CustomExport]   Nombre: %s", g_config.filename.c_str());
	blog(LOG_INFO, "[CustomExport]   Extensión: %s", g_config.extension.empty() ? "(auto)" : g_config.extension.c_str());
	
	return true;
}

MODULE_EXPORT void obs_module_unload(void)
{
	blog(LOG_INFO, "[CustomExport] Plugin descargado");
}
