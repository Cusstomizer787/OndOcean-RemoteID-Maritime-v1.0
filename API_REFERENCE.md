# API Reference - OndOcéan RemoteID Maritime

## 🔌 Vue d'Ensemble des APIs

Cette documentation détaille toutes les APIs publiques du firmware OndOcéan RemoteID Maritime, organisées par modules fonctionnels.

## 📊 Index des Modules

- [Data Validation API](#data-validation-api)
- [Maritime Sensors API](#maritime-sensors-api)
- [Error Management API](#error-management-api)
- [Logging System API](#logging-system-api)
- [Unit Testing API](#unit-testing-api)
- [Communication APIs](#communication-apis)
- [Configuration APIs](#configuration-apis)

---

## 🛡️ Data Validation API

### Types de Données

#### `ValidationResult`
```cpp
typedef enum {
    VALIDATION_OK = 0,
    VALIDATION_ERROR_NULL_POINTER,
    VALIDATION_ERROR_OUT_OF_RANGE,
    VALIDATION_ERROR_INVALID_FORMAT,
    VALIDATION_ERROR_MARITIME_UNSAFE,
    VALIDATION_ERROR_SENSOR_FAULT,
    VALIDATION_ERROR_COMMUNICATION,
    VALIDATION_ERROR_SECURITY_BREACH
} ValidationResult;
```

#### `ValidationConfig`
```cpp
struct ValidationConfig {
    float temp_min_celsius = -20.0f;
    float temp_max_celsius = 60.0f;
    float humidity_max_pct = 95.0f;
    float pressure_min_hpa = 950.0f;
    float pressure_max_hpa = 1050.0f;
    float battery_min_voltage = 3.0f;
    float battery_critical_voltage = 3.2f;
    uint32_t sensor_timeout_ms = 1000;
};
```

### Fonctions de Validation

#### `validate_maritime_sensor_data()`
```cpp
ValidationResult validate_maritime_sensor_data(const MaritimeSensorData* data);
```
**Description** : Valide les données des capteurs maritimes  
**Paramètres** :
- `data` : Pointeur vers les données capteurs à valider
**Retour** : Code de résultat de validation  
**Exemple** :
```cpp
MaritimeSensorData sensor_data;
read_maritime_sensors(&sensor_data);
ValidationResult result = validate_maritime_sensor_data(&sensor_data);
if (result != VALIDATION_OK) {
    LOG_VALIDATION_ERROR("Sensor validation failed: %s", 
                         validation_result_to_string(result));
}
```

#### `validate_gnss_position()`
```cpp
ValidationResult validate_gnss_position(double lat, double lon, float alt, float accuracy);
```
**Description** : Valide une position GNSS pour usage maritime  
**Paramètres** :
- `lat` : Latitude en degrés décimaux (-90 à +90)
- `lon` : Longitude en degrés décimaux (-180 à +180)
- `alt` : Altitude en mètres (0 à 120 pour maritime)
- `accuracy` : Précision en mètres
**Retour** : Code de résultat de validation

#### `sanitize_coordinates()`
```cpp
bool sanitize_coordinates(double* lat, double* lon);
```
**Description** : Sanitise les coordonnées GPS dans les plages valides  
**Paramètres** :
- `lat` : Pointeur vers latitude à sanitiser
- `lon` : Pointeur vers longitude à sanitiser
**Retour** : `true` si sanitisation réussie

---

## 🌊 Maritime Sensors API

### Types de Données

#### `MaritimeSensorData`
```cpp
struct MaritimeSensorData {
    float temperature_c;
    float humidity_pct;
    float pressure_hpa;
    float accel_x, accel_y, accel_z;
    float gyro_x, gyro_y, gyro_z;
    float mag_x, mag_y, mag_z;
    bool case_tamper_detected;
    float battery_voltage;
    uint32_t timestamp_ms;
};
```

### Fonctions Principales

#### `setup_maritime_sensors()`
```cpp
void setup_maritime_sensors();
```
**Description** : Initialise tous les capteurs maritimes  
**Paramètres** : Aucun  
**Retour** : Void  
**Side Effects** : Configure I2C, initialise BME280, LSM6DS3, LIS3MDL

#### `read_maritime_sensors()`
```cpp
bool read_maritime_sensors(MaritimeSensorData* data);
```
**Description** : Lit toutes les données des capteurs maritimes  
**Paramètres** :
- `data` : Pointeur vers structure de données à remplir
**Retour** : `true` si lecture réussie, `false` en cas d'erreur  
**Exemple** :
```cpp
MaritimeSensorData sensor_data;
if (read_maritime_sensors(&sensor_data)) {
    LOG_SENSOR_INFO("Temperature: %.1f°C, Pressure: %.1f hPa", 
                    sensor_data.temperature_c, sensor_data.pressure_hpa);
} else {
    LOG_SENSOR_ERROR("Failed to read maritime sensors");
}
```

#### `read_battery_voltage()`
```cpp
float read_battery_voltage();
```
**Description** : Lit la tension de la batterie  
**Retour** : Tension en volts (0.0 à 5.0V typique)

#### `is_maritime_environment_safe()`
```cpp
bool is_maritime_environment_safe(const MaritimeSensorData* data);
```
**Description** : Vérifie si l'environnement maritime est sûr  
**Paramètres** :
- `data` : Données capteurs à analyser
**Retour** : `true` si environnement sûr

---

## 🚨 Error Management API

### Types d'Erreurs

#### `ErrorCode`
```cpp
typedef enum {
    // System errors (1-99)
    ERROR_SYSTEM_INIT_FAILED = 1,
    ERROR_SYSTEM_LOW_MEMORY = 2,
    ERROR_SYSTEM_WATCHDOG_TIMEOUT = 3,
    
    // Hardware errors (100-199)
    ERROR_HW_I2C_FAILURE = 100,
    ERROR_HW_SPI_FAILURE = 101,
    ERROR_HW_POWER_FAILURE = 104,
    
    // Sensor errors (200-299)
    ERROR_SENSOR_BME280_NOT_FOUND = 200,
    ERROR_SENSOR_CASE_TAMPER_DETECTED = 206,
    
    // Maritime errors (700-799)
    ERROR_MARITIME_ENVIRONMENT_UNSAFE = 700,
    ERROR_MARITIME_ALTITUDE_EXCEEDED = 702
} ErrorCode;
```

#### `ErrorSeverity`
```cpp
typedef enum {
    ERROR_SEVERITY_INFO = 0,
    ERROR_SEVERITY_WARNING = 1,
    ERROR_SEVERITY_ERROR = 2,
    ERROR_SEVERITY_CRITICAL = 3,
    ERROR_SEVERITY_FATAL = 4
} ErrorSeverity;
```

### Fonctions de Gestion d'Erreurs

#### `error_report()`
```cpp
void error_report(ErrorCode code, const char* function, const char* file, 
                  int line, const char* description, uint32_t data);
```
**Description** : Rapporte une erreur avec contexte complet  
**Macros Utilitaires** :
```cpp
#define REPORT_ERROR(code, desc) \
    error_report(code, __FUNCTION__, __FILE__, __LINE__, desc)

#define CRITICAL_ERROR(code, desc) do { \
    REPORT_ERROR(code, desc); \
    LOG_FATAL(LOG_CAT_SYSTEM, "CRITICAL ERROR: %s", desc); \
} while(0)
```

#### `error_attempt_recovery()`
```cpp
bool error_attempt_recovery(ErrorCode code);
```
**Description** : Tente une récupération automatique d'erreur  
**Paramètres** :
- `code` : Code d'erreur à récupérer
**Retour** : `true` si récupération réussie

#### `error_is_recoverable()`
```cpp
bool error_is_recoverable(ErrorCode code);
```
**Description** : Vérifie si une erreur est récupérable  
**Retour** : `true` si récupération possible

---

## 📝 Logging System API

### Niveaux de Log

#### `LogLevel`
```cpp
typedef enum {
    LOG_LEVEL_TRACE = 0,
    LOG_LEVEL_DEBUG = 1,
    LOG_LEVEL_INFO = 2,
    LOG_LEVEL_WARN = 3,
    LOG_LEVEL_ERROR = 4,
    LOG_LEVEL_FATAL = 5
} LogLevel;
```

#### `LogCategory`
```cpp
typedef enum {
    LOG_CAT_SYSTEM = 0,
    LOG_CAT_SENSOR = 1,
    LOG_CAT_GNSS = 2,
    LOG_CAT_COMM = 3,
    LOG_CAT_SECURITY = 6,
    LOG_CAT_MARITIME = 8,
    LOG_CAT_VALIDATION = 9
} LogCategory;
```

### Macros de Logging

#### Logging Général
```cpp
#define LOG_INFO(cat, fmt, ...)    logger_log(LOG_LEVEL_INFO, cat, fmt, ##__VA_ARGS__)
#define LOG_WARN(cat, fmt, ...)    logger_log(LOG_LEVEL_WARN, cat, fmt, ##__VA_ARGS__)
#define LOG_ERROR(cat, fmt, ...)   logger_log(LOG_LEVEL_ERROR, cat, fmt, ##__VA_ARGS__)
#define LOG_DEBUG_ONLY(cat, fmt, ...) // Actif uniquement en mode debug
```

#### Logging Spécialisé Maritime
```cpp
#define LOG_MARITIME_INFO(fmt, ...)    LOG_INFO(LOG_CAT_MARITIME, fmt, ##__VA_ARGS__)
#define LOG_SENSOR_ERROR(fmt, ...)     LOG_ERROR(LOG_CAT_SENSOR, fmt, ##__VA_ARGS__)
#define LOG_GNSS_WARN(fmt, ...)        LOG_WARN(LOG_CAT_GNSS, fmt, ##__VA_ARGS__)
#define LOG_SECURITY_ERROR(fmt, ...)   LOG_ERROR(LOG_CAT_SECURITY, fmt, ##__VA_ARGS__)
```

### Fonctions de Configuration

#### `logger_init()`
```cpp
void logger_init();
```
**Description** : Initialise le système de logging  
**Usage** : Appeler en premier dans `setup()`

#### `logger_set_level()`
```cpp
void logger_set_level(LogLevel level);
```
**Description** : Définit le niveau minimum de logging  
**Exemple** :
```cpp
#ifdef DEBUG
    logger_set_level(LOG_LEVEL_DEBUG);
#else
    logger_set_level(LOG_LEVEL_INFO);
#endif
```

#### `logger_enable_mqtt()`
```cpp
void logger_enable_mqtt(bool enable, const char* topic);
```
**Description** : Active/désactive le logging via MQTT  
**Paramètres** :
- `enable` : `true` pour activer
- `topic` : Topic MQTT pour les logs (optionnel)

### Fonctions Utilitaires

#### `log_performance_start()` / `log_performance_end()`
```cpp
void log_performance_start(const char* operation);
void log_performance_end(const char* operation);
```
**Description** : Mesure et log la performance d'une opération  
**Exemple** :
```cpp
log_performance_start("sensor_reading");
read_maritime_sensors(&data);
log_performance_end("sensor_reading");
// Output: "Performance: sensor_reading completed in 245 μs"
```

---

## 🧪 Unit Testing API

### Framework de Test

#### `test_run_single()`
```cpp
bool test_run_single(const char* test_name, bool (*test_func)());
```
**Description** : Exécute un test unitaire individuel  
**Paramètres** :
- `test_name` : Nom du test pour logging
- `test_func` : Pointeur vers fonction de test
**Retour** : `true` si test réussi

#### `test_run_all()`
```cpp
void test_run_all();
```
**Description** : Exécute tous les tests unitaires disponibles

### Macros d'Assertion

#### `TEST_ASSERT()`
```cpp
#define TEST_ASSERT(condition, message)
```
**Description** : Vérifie qu'une condition est vraie  
**Exemple** :
```cpp
bool test_battery_voltage() {
    float voltage = read_battery_voltage();
    TEST_ASSERT(voltage > 3.0f, "Battery voltage should be above 3.0V");
    TEST_ASSERT(voltage < 5.0f, "Battery voltage should be below 5.0V");
    return true;
}
```

#### `TEST_ASSERT_EQUAL()`
```cpp
#define TEST_ASSERT_EQUAL(expected, actual, message)
```
**Description** : Vérifie l'égalité de deux valeurs

#### `TEST_ASSERT_FLOAT_EQUAL()`
```cpp
#define TEST_ASSERT_FLOAT_EQUAL(expected, actual, tolerance, message)
```
**Description** : Vérifie l'égalité de deux flottants avec tolérance

---

## 🌐 Communication APIs

### MQTT Maritime

#### Configuration
```cpp
struct MQTTConfig {
    String broker = "anémone.local";
    int port = 1883;
    String topic_prefix = "ondocean/remoteid";
    bool enable_logging = false;
    uint32_t keepalive_ms = 60000;
};
```

#### Fonctions MQTT
```cpp
bool setup_mqtt();
bool mqtt_publish_sensor_data(const MaritimeSensorData* data);
bool mqtt_publish_position(double lat, double lon, float alt);
bool mqtt_publish_status(const char* status);
```

### WiFi Management

#### `setup_wifi()`
```cpp
bool setup_wifi();
```
**Description** : Initialise et connecte WiFi  
**Retour** : `true` si connexion réussie

#### `wifi_reconnect()`
```cpp
bool wifi_reconnect();
```
**Description** : Reconnecte WiFi en cas de perte de connexion

---

## ⚙️ Configuration APIs

### Maritime Configuration

#### `MaritimeConfig`
```cpp
struct MaritimeConfig {
    bool maritime_mode_enabled = true;
    bool gnss_required = true;
    bool mqtt_enabled = true;
    String device_id;
    String mqtt_broker = "anémone.local";
    
    // Position data
    double latitude = 0.0;
    double longitude = 0.0;
    float altitude = 0.0;
    bool position_valid = false;
    
    // Environmental limits
    float temperature = 0.0;
    float humidity = 0.0;
    float pressure = 0.0;
    float battery_voltage = 0.0;
    bool low_power_mode = false;
};
```

#### Fonctions de Configuration
```cpp
void load_maritime_config();
void save_maritime_config();
void reset_maritime_config_to_defaults();
```

---

## 📊 Exemples d'Usage Complets

### Exemple 1 : Lecture et Validation Capteurs
```cpp
void example_sensor_processing() {
    MaritimeSensorData sensor_data;
    
    // Lecture des capteurs
    if (!read_maritime_sensors(&sensor_data)) {
        REPORT_ERROR(ERROR_SENSOR_BME280_READ_FAILED, "Sensor read failed");
        return;
    }
    
    // Validation des données
    ValidationResult result = validate_maritime_sensor_data(&sensor_data);
    if (result != VALIDATION_OK) {
        LOG_VALIDATION_ERROR("Validation failed: %s", 
                             validation_result_to_string(result));
        
        // Tentative de sanitisation
        if (result == VALIDATION_ERROR_OUT_OF_RANGE) {
            sanitize_sensor_reading(&sensor_data.temperature_c, -20.0f, 60.0f);
            LOG_VALIDATION_INFO("Temperature sanitized");
        }
    }
    
    // Vérification sécurité maritime
    if (!is_maritime_environment_safe(&sensor_data)) {
        LOG_MARITIME_WARN("Maritime environment unsafe");
        REPORT_ERROR(ERROR_MARITIME_ENVIRONMENT_UNSAFE, "Unsafe conditions");
    }
}
```

### Exemple 2 : Test Unitaire Complet
```cpp
bool test_complete_sensor_validation() {
    // Setup
    MaritimeSensorData test_data;
    mock_sensor_data_valid(&test_data);
    
    // Test validation normale
    ValidationResult result = validate_maritime_sensor_data(&test_data);
    TEST_ASSERT_EQUAL(VALIDATION_OK, result, "Valid data should pass");
    
    // Test données invalides
    test_data.temperature_c = -30.0f;  // Trop froid
    result = validate_maritime_sensor_data(&test_data);
    TEST_ASSERT(result != VALIDATION_OK, "Invalid data should fail");
    
    // Test sanitisation
    float original_temp = test_data.temperature_c;
    bool sanitized = sanitize_sensor_reading(&test_data.temperature_c, -20.0f, 60.0f);
    TEST_ASSERT(sanitized, "Sanitization should succeed");
    TEST_ASSERT(test_data.temperature_c != original_temp, "Temperature should be modified");
    TEST_ASSERT(test_data.temperature_c >= -20.0f, "Temperature should be in valid range");
    
    return true;
}
```

---

## 🔧 Codes d'Erreur de Retour

| Code | Signification | Action Recommandée |
|------|---------------|-------------------|
| `VALIDATION_OK` | Validation réussie | Continuer traitement |
| `VALIDATION_ERROR_NULL_POINTER` | Pointeur null | Vérifier paramètres |
| `VALIDATION_ERROR_OUT_OF_RANGE` | Valeur hors limites | Sanitiser ou rejeter |
| `VALIDATION_ERROR_MARITIME_UNSAFE` | Conditions dangereuses | Mode sécurisé |
| `ERROR_SENSOR_*` | Erreur capteur | Réinitialiser I2C |
| `ERROR_COMM_*` | Erreur communication | Reconnexion |
| `ERROR_MARITIME_*` | Erreur maritime | Vérifier environnement |

---

Cette API Reference fournit tous les détails nécessaires pour utiliser efficacement le firmware OndOcéan RemoteID Maritime. 🌊⚓
