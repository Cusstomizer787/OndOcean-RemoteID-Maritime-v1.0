# Guide de Déploiement - OndOcéan RemoteID Maritime

## Vue d'Ensemble du Déploiement

Ce guide détaille le processus complet de déploiement du firmware OndOcéan RemoteID Maritime, de la compilation au déploiement en environnement maritime.

## Prérequis Système

### Environnement de Développement

#### Logiciels Requis
- **Arduino CLI** v0.35.0 ou supérieur
- **ESP32 Core** v3.3.0 avec support PSRAM
- **Python 3.8+** (pour scripts de test)
- **Git** pour gestion de version

#### Installation Arduino CLI
```bash
# Windows (PowerShell)
Invoke-WebRequest -Uri https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Windows_64bit.zip -OutFile arduino-cli.zip
Expand-Archive arduino-cli.zip -DestinationPath "C:\Program Files\Arduino CLI"

# Vérification installation
arduino-cli version
```

#### Configuration ESP32
```bash
# Ajout du board manager ESP32
arduino-cli core update-index --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json

# Installation ESP32 core
arduino-cli core install esp32:esp32@3.3.0

# Installation des bibliothèques requises
arduino-cli lib install "ArduinoJson@7.4.2"
arduino-cli lib install "PubSubClient@2.8"
arduino-cli lib install "Adafruit NeoPixel@1.15.1"
```

### Hardware Requis

#### ESP32-S3 Development Board
- **Modèle** : ESP32-S3-DevKitC-1 ou compatible
- **Flash** : 16MB minimum
- **PSRAM** : 8MB requis
- **USB** : USB-C pour programmation

#### Capteurs Maritimes
- **BME280** : Capteur environnemental (I2C 0x76)
- **LSM6DS3** : IMU 6 axes (I2C 0x6A)
- **LIS3MDL** : Magnétomètre 3 axes (I2C 0x1C)
- **u-blox F9P** : Module GNSS RTK (UART)

#### Composants Additionnels
- **Antenne WiFi** : 2.4GHz externe (connecteur U.FL)
- **Antenne GNSS** : Active avec alimentation
- **Boîtier IP67** : Protection maritime
- **Batterie** : LiPo 3.7V 2000mAh minimum

## Processus de Compilation

### 1. Préparation de l'Environnement

#### Clonage du Projet
```bash
# Repository GitHub officiel :
git clone https://github.com/Cusstomizer787/OndOcean-RemoteID-Maritime-v1.0.git
cd OndOcean-RemoteID-Maritime-v1.0

# Ou utiliser le projet local existant :
cd /path/to/your/ondocean-remoteid
```

#### Vérification des Dépendances
```bash
# Exécution du script de vérification (à créer)
# check_dependencies.bat

# Vérification manuelle pour l'instant :
arduino-cli version
arduino-cli core list | findstr esp32
arduino-cli lib list | findstr "ArduinoJson\|PubSubClient\|Adafruit NeoPixel"

# Sortie attendue:
# Arduino CLI found: v0.35.0+
# ESP32 core found: v3.3.0
# Required libraries installed
```

### 2. Configuration du Build

#### Fichier `build_config.h`
```cpp
// Configuration de compilation
#define ONDOCEAN_VERSION_MAJOR    1
#define ONDOCEAN_VERSION_MINOR    0
#define ONDOCEAN_VERSION_PATCH    0

// Options de build
#define ENABLE_MARITIME_MODE      1
#define ENABLE_GNSS_RTK          1
#define ENABLE_ADVANCED_LOGGING  1
#define ENABLE_UNIT_TESTS        1

// Configuration debug
#ifdef DEBUG
    #define LOG_LEVEL_DEFAULT     LOG_LEVEL_DEBUG
    #define ENABLE_MEMORY_DEBUG   1
    #define ENABLE_PERFORMANCE_MONITORING 1
#else
    #define LOG_LEVEL_DEFAULT     LOG_LEVEL_INFO
#endif
```

### 3. Compilation

#### Test de Compilation Rapide
```bash
# Test compilation sans upload
test_minimal.bat

# Sortie attendue:
# Testing minimal compilation...
# Sketch uses 1,278,407 bytes (97%) of program storage space
# Variables use 61,056 bytes (18%) of dynamic memory
# Compilation test complete.
```

#### Compilation Complète
```bash
# Compilation avec verbose et génération binaires
compile.bat

# Fichiers générés:
# - build/ondocean-remoteid.ino.bin    (firmware flashable)
# - build/ondocean-remoteid.ino.elf    (fichier debug)
# - build/ondocean-remoteid.ino.map    (memory map)
```

## Processus de Flash

### 1. Préparation Hardware

#### Connexion ESP32-S3
```
ESP32-S3 DevKit ←→ USB-C ←→ PC
```

#### Vérification Port Série
```bash
# Windows
arduino-cli board list

# Sortie attendue:
# Port         Type              Board Name  FQBN            Core
# COM3         Serial Port (USB) Unknown     esp32:esp32:esp32s3
```

### 2. Flash du Firmware

#### Upload Automatique
```bash
# Flash avec configuration automatique
upload.bat

# Processus:
# 1. Détection automatique du port
# 2. Configuration FQBN ESP32-S3
# 3. Flash du firmware
# 4. Vérification upload
```

#### Upload Manuel
```bash
arduino-cli upload -p COM3 --fqbn esp32:esp32:esp32s3:PSRAM=enabled ondocean-remoteid.ino

# Options avancées:
# --verify     : Vérification après upload
# --verbose    : Mode verbose
```

### 3. Vérification Post-Flash

#### Monitoring Série
```bash
# Ouverture monitor série
arduino-cli monitor -p COM3 -c baudrate=57600

# Logs de démarrage attendus:
# [00000123] [INFO ][SYSTEM] OndOcean RemoteID Maritime v1.0 - System startup
# [00000234] [INFO ][SYSTEM] ESP32-S3 Chip ID: 1234567890ABCDEF
# [00000345] [INFO ][SENSOR] I2C initialized at 400kHz (SDA=8, SCL=9)
# [00000456] [INFO ][SYSTEM] System initialization complete - Ready for operation
```

## Configuration Initiale

### 1. Configuration WiFi

#### Via Interface Série
```cpp
// Commandes série pour configuration
CONFIG_WIFI_SSID=MonReseauMaritime
CONFIG_WIFI_PASSWORD=MotDePasseSecurise
CONFIG_MQTT_BROKER=anemone.local
CONFIG_MQTT_PORT=1883
CONFIG_DEVICE_ID=OND001
```

#### Via Interface Web
```
1. Connexion au point d'accès ESP32: "OndOcean-Setup"
2. Navigation vers: http://192.168.4.1
3. Configuration WiFi et MQTT
4. Sauvegarde et redémarrage
```

### 2. Calibration Capteurs

#### Calibration Automatique
```cpp
// Séquence de calibration au démarrage
void calibration_sequence() {
    LOG_SENSOR_INFO("Starting sensor calibration...");
    
    // Calibration magnétomètre (8 rotations)
    calibrate_magnetometer();
    
    // Calibration IMU (position stable 30s)
    calibrate_imu();
    
    // Test capteurs environnementaux
    test_environmental_sensors();
    
    LOG_SENSOR_INFO("Calibration complete");
}
```

#### Calibration Manuelle
```bash
# Via commandes série
CAL_MAG_START    # Démarrer calibration magnétomètre
CAL_IMU_START    # Démarrer calibration IMU
CAL_SAVE         # Sauvegarder calibration
CAL_RESET        # Reset calibration
```

## Tests de Validation

### 1. Tests Unitaires

#### Exécution Complète
```cpp
// Dans le firmware, fonction setup()
void setup() {
    logger_init();
    
    #ifdef ENABLE_UNIT_TESTS
    LOG_INFO(LOG_CAT_SYSTEM, "Running unit tests...");
    test_run_all();
    #endif
    
    // Suite de l'initialisation...
}
```

#### Tests Spécifiques
```cpp
// Tests par catégorie
test_run_validation_tests();    // Tests validation données
test_run_sensor_tests();        // Tests capteurs
test_run_maritime_tests();      // Tests spécifiques maritimes
test_run_communication_tests(); // Tests WiFi/MQTT/BLE
```

### 2. Tests d'Intégration

#### Test Capteurs → MQTT
```bash
# Script de test automatisé
python test_integration.py --test sensor_to_mqtt

# Vérifications:
# Lecture capteurs BME280, LSM6DS3, LIS3MDL
# Validation données dans plages maritimes
# Sérialisation JSON schema OndOcean
# Publication MQTT vers broker test
# Réception et validation côté broker
```

#### Test GNSS → Validation
```bash
python test_integration.py --test gnss_validation

# Vérifications:
# Réception données GNSS u-blox F9P
# Parsing NMEA sentences
# Validation position maritime (altitude < 120m)
# Précision acceptable (< 10m)
# Détection position au-dessus de l'eau
```

### 3. Tests de Performance

#### Benchmarks Temps Réel
```cpp
// Mesures de performance intégrées
void performance_monitoring() {
    log_performance_start("sensor_read_cycle");
    read_maritime_sensors(&sensor_data);
    log_performance_end("sensor_read_cycle");
    
    // Cibles de performance:
    // - Lecture capteurs: < 500μs
    // - Validation données: < 100μs
    // - Sérialisation JSON: < 200μs
    // - Transmission MQTT: < 50ms
}
```

## Configuration Réseau

### 1. Configuration MQTT

#### Broker Anémone
```json
{
  "mqtt_config": {
    "broker": "anemone.local",
    "port": 1883,
    "keepalive": 60,
    "clean_session": true,
    "topics": {
      "data": "ondocean/remoteid/{device_id}/data",
      "status": "ondocean/remoteid/{device_id}/status",
      "logs": "ondocean/remoteid/{device_id}/logs",
      "commands": "ondocean/remoteid/{device_id}/cmd"
    }
  }
}
```

#### Test Connectivité MQTT
```bash
# Test publication manuelle
mosquitto_pub -h anemone.local -t "ondocean/remoteid/OND001/test" -m "Hello from OndOcean"

# Test souscription
mosquitto_sub -h anemone.local -t "ondocean/remoteid/+/data"
```

### 2. Configuration WiFi Maritime

#### Paramètres Optimisés
```cpp
// Configuration WiFi pour environnement maritime
WiFi.setTxPower(WIFI_POWER_19_5dBm);  // Puissance max
WiFi.setAutoReconnect(true);           // Reconnexion auto
WiFi.setSleep(false);                  // Pas de sleep mode
```

#### Gestion Reconnexion
```cpp
void wifi_watchdog() {
    static uint32_t last_check = 0;
    
    if (millis() - last_check > 30000) {  // Check toutes les 30s
        if (WiFi.status() != WL_CONNECTED) {
            LOG_COMM_WARN("WiFi disconnected, attempting reconnection");
            recovery_reset_wifi();
        }
        last_check = millis();
    }
}
```

## Déploiement Sécurisé

### 1. Configuration Sécurité

#### Paramètres de Sécurité
```cpp
// Configuration sécurité maritime
struct SecurityConfig {
    bool case_tamper_detection = true;
    bool secure_boot_enabled = false;      // Pour production
    bool firmware_encryption = false;      // Pour production
    uint32_t watchdog_timeout_ms = 30000;
    bool emergency_shutdown_enabled = true;
};
```

#### Détection Tampering
```cpp
void security_monitoring() {
    bool case_open = !digitalRead(PIN_CASE_DETECT);
    
    if (case_open) {
        LOG_SECURITY_ERROR("Case opened - maritime seal compromised");
        REPORT_ERROR(ERROR_SECURITY_CASE_OPENED, "Case tampering detected");
        
        // Actions sécurité:
        // 1. Log événement avec timestamp
        // 2. Transmission alerte MQTT
        // 3. Réduction fonctionnalités non-critiques
        // 4. Augmentation fréquence monitoring
    }
}
```

### 2. Backup et Recovery

#### Sauvegarde Configuration
```cpp
void backup_critical_data() {
    // Sauvegarde NVS
    nvs_handle_t handle;
    nvs_open("ondocean", NVS_READWRITE, &handle);
    
    // Sauvegarde configuration maritime
    nvs_set_blob(handle, "maritime_config", &maritime_config, sizeof(maritime_config));
    
    // Sauvegarde statistiques erreurs
    nvs_set_blob(handle, "error_stats", &error_stats, sizeof(error_stats));
    
    nvs_commit(handle);
    nvs_close(handle);
    
    LOG_INFO(LOG_CAT_SYSTEM, "Critical data backed up to NVS");
}
```

## Monitoring et Maintenance

### 1. Dashboard de Monitoring

#### Métriques Système
```json
{
  "system_metrics": {
    "uptime_ms": 3600000,
    "free_heap": 245760,
    "min_free_heap": 198432,
    "cpu_frequency_mhz": 240,
    "temperature_c": 45.2
  },
  "sensor_metrics": {
    "read_success_rate": 99.8,
    "validation_success_rate": 97.5,
    "last_read_timestamp": 1642680123456
  },
  "communication_metrics": {
    "wifi_rssi_dbm": -45,
    "mqtt_publish_success_rate": 98.2,
    "last_mqtt_publish": 1642680123000
  }
}
```

### 2. Alertes et Notifications

#### Configuration Alertes
```cpp
struct AlertConfig {
    float battery_critical_voltage = 3.2f;
    float temperature_max_celsius = 55.0f;
    int32_t wifi_rssi_min_dbm = -80;
    uint32_t mqtt_timeout_ms = 60000;
    uint32_t sensor_timeout_ms = 5000;
};
```

#### Système d'Alertes MQTT
```json
{
  "alert": {
    "timestamp": 1642680123456,
    "device_id": "OND001",
    "severity": "CRITICAL",
    "category": "POWER",
    "message": "Battery voltage critical: 3.1V",
    "action_required": "Replace battery immediately"
  }
}
```

## Mise en Production

### 1. Checklist Pré-Déploiement

#### Validation Hardware
- [ ] ESP32-S3 fonctionnel avec PSRAM
- [ ] Tous capteurs détectés et calibrés
- [ ] Module GNSS avec fix satellite
- [ ] Antennes WiFi et GNSS connectées
- [ ] Boîtier IP67 étanche et scellé
- [ ] Batterie chargée > 80%

#### Validation Software
- [ ] Firmware compilé sans erreurs
- [ ] Tests unitaires 100% réussis
- [ ] Tests d'intégration validés
- [ ] Configuration WiFi/MQTT fonctionnelle
- [ ] Logging et monitoring opérationnels

#### Validation Environnementale
- [ ] Test étanchéité IP67 validé
- [ ] Test résistance vibrations
- [ ] Test température -10°C à +50°C
- [ ] Test humidité jusqu'à 95%
- [ ] Test interférences électromagnétiques

### 2. Déploiement Maritime

#### Installation Physique
```
1. Fixation sécurisée sur embarcation
2. Connexion antennes externes
3. Vérification étanchéité connecteurs
4. Test fonctionnel complet
5. Activation monitoring distant
```

#### Configuration Finale
```cpp
// Configuration production
#define PRODUCTION_MODE           1
#define ENABLE_WATCHDOG          1
#define ENABLE_SECURE_BOOT       1
#define DISABLE_DEBUG_LOGS       1
#define ENABLE_REMOTE_MONITORING 1
```

---

## Dépannage

### Problèmes Courants

#### Erreur de Compilation
```
Error: ESP32 core not found
Solution: arduino-cli core install esp32:esp32@3.3.0
```

#### Erreur de Flash
```
Error: Failed to connect to ESP32
Solution: 
1. Vérifier câble USB-C
2. Maintenir bouton BOOT pendant flash
3. Vérifier driver USB série
```

#### Capteurs Non Détectés
```
Error: BME280 not found on I2C
Solution:
1. Vérifier connexions SDA/SCL
2. Scanner bus I2C: arduino-cli monitor
3. Vérifier alimentation 3.3V
```

### Support Technique

#### Logs de Debug
```cpp
// Activation debug complet
logger_set_level(LOG_LEVEL_DEBUG);
logger_enable_mqtt(true, "ondocean/debug/logs");
```

#### Diagnostic Système
```cpp
// Rapport diagnostic complet
void system_diagnostic() {
    logger_log_system_info();
    error_print_stats();
    print_validation_stats();
    logger_print_stats();
}
```

---

**Ce guide garantit un déploiement réussi et sécurisé du firmware OndOcéan RemoteID Maritime en environnement maritime professionnel.** 
