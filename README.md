# OndOcéan RemoteID Maritime

## 🌊 Vue d'Ensemble

OndOcéan RemoteID Maritime est un firmware ESP32-S3 spécialisé pour l'identification maritime des drones, basé sur ArduRemoteID avec des adaptations pour environnements marins IP67.

### 🎯 Objectifs du Projet

- **Conformité Réglementaire** : ASTM F3586-22 (FAA) + EU RemoteID
- **Environnement Maritime** : Résistance IP67, capteurs environnementaux
- **Transmission Multi-Protocole** : WiFi Beacon/NAN, BLE4/BLE5, MQTT
- **Sécurité Avancée** : Détection de tampering, chiffrement, validation robuste

## 🏗️ Architecture Technique

```
OndOcéan RemoteID Maritime
├── 🎯 Core System
│   ├── ESP32-S3 (240MHz, PSRAM enabled)
│   ├── Maritime sensors (BME280, LSM6DS3, LIS3MDL)
│   └── GNSS u-blox F9P (RTK capable)
├── 🌐 Communication
│   ├── WiFi 802.11 b/g/n (2.4GHz)
│   ├── Bluetooth 5.0 LE
│   └── MQTT over WiFi
├── 🛡️ Security & Validation
│   ├── Case tamper detection
│   ├── Data validation system
│   └── Error management
└── 📡 Protocols
    ├── OpenDroneID (ASTM F3586-22)
    ├── MAVLink v2
    └── JSON maritime schema
```

## 🚀 Fonctionnalités Principales

### ✅ Implémenté (Phase 1)
- [x] **Firmware ESP32-S3** compilable à 100%
- [x] **Configuration maritime** avec GPIO optimisés
- [x] **Capteurs environnementaux** (température, humidité, pression)
- [x] **Validation robuste** des données avec sanitisation
- [x] **Logging standardisé** avec niveaux et catégories
- [x] **Gestion d'erreurs complète** avec récupération automatique
- [x] **Tests unitaires** pour fonctions critiques
- [x] **Transmission MQTT** avec schéma JSON unifié
- [x] **Stubs MAVLink/OpenDroneID** complets

### 🔄 En Développement (Phase 2)
- [ ] **Intégration AIS Sniffer** (RTL-SDR + dAISy)
- [ ] **Module GNSS DePIN** avec logging précis
- [ ] **Station météo IoT** maritime
- [ ] **Tests hardware** complets

## 📁 Structure du Projet

```
ondocean-remoteid/
├── 📄 Core Files
│   ├── ondocean-remoteid.ino          # Main firmware
│   ├── board_config_maritime.h        # Hardware configuration
│   └── maritime_sensors.h/cpp         # Sensor management
├── 🛡️ Safety & Validation
│   ├── data_validation.h/cpp          # Robust data validation
│   ├── error_management.h/cpp         # Comprehensive error handling
│   └── unit_tests.h/cpp               # Unit testing framework
├── 📝 Logging & Monitoring
│   ├── ondocean_logger.h/cpp          # Standardized logging
│   └── logging_examples.cpp           # Usage examples
├── 🌐 Communication
│   ├── WiFi_TX.h/cpp                  # WiFi transmission
│   ├── BLE_TX.h/cpp                   # Bluetooth transmission
│   └── ondocean_mqtt.h                # MQTT maritime
├── 📡 Protocols
│   ├── generated/                     # MAVLink headers
│   ├── opendroneid_wifi_stubs.h/cpp   # OpenDroneID stubs
│   └── mavlink.h/cpp                  # MAVLink implementation
├── 🔧 Build System
│   ├── compile.bat                    # Full compilation
│   ├── test_minimal.bat               # Quick test
│   └── upload.bat                     # Firmware upload
└── 📚 Documentation
    ├── README.md                      # This file
    ├── ARCHITECTURE.md                # Technical architecture
    ├── API_REFERENCE.md               # API documentation
    └── DEPLOYMENT.md                  # Deployment guide
```

## 🛠️ Installation et Compilation

### Prérequis
- **Arduino CLI** installé et configuré
- **ESP32 Core 3.3.0** avec support PSRAM
- **Bibliothèques** : ArduinoJson, PubSubClient, Adafruit NeoPixel

### Compilation Rapide
```bash
# Test de compilation (rapide)
test_minimal.bat

# Compilation complète avec verbose
compile.bat

# Upload vers ESP32-S3
upload.bat
```

### Configuration Hardware
```cpp
// GPIO ESP32-S3 Maritime
#define PIN_GNSS_TX         GPIO_NUM_43  // u-blox F9P UART TX
#define PIN_GNSS_RX         GPIO_NUM_44  // u-blox F9P UART RX
#define PIN_CASE_DETECT     GPIO_NUM_45  // Case tamper detection
#define PIN_BATTERY_MONITOR GPIO_NUM_46  // Battery voltage monitoring
```

## 🧪 Tests et Validation

### Exécution des Tests Unitaires
```cpp
// Dans le firmware
void setup() {
    logger_init();
    test_run_all();  // Execute tous les tests
}
```

### Catégories de Tests
- **Data Validation** : Validation robuste des données capteurs
- **Sensor Operations** : Lecture et validation des capteurs
- **Error Management** : Gestion et récupération d'erreurs
- **Maritime Operations** : Fonctions spécifiques maritimes
- **Communication** : WiFi, BLE, MQTT

## 📊 Monitoring et Logging

### Système de Logging Standardisé
```cpp
// Logging par catégorie
LOG_MARITIME_INFO("Position: %.6f, %.6f", lat, lon);
LOG_SENSOR_WARN("Temperature high: %.1f°C", temp);
LOG_COMM_ERROR("MQTT connection failed");

// Logging conditionnel (debug uniquement)
LOG_DEBUG_ONLY(LOG_CAT_GNSS, "Raw NMEA: %s", nmea_data);

// Logging d'urgence
LOG_EMERGENCY("Critical system failure detected");
```

### Statistiques Système
```cpp
// Affichage des statistiques
logger_print_stats();      // Statistiques de logging
print_validation_stats();  // Statistiques de validation
error_print_stats();       // Statistiques d'erreurs
```

## 🌐 Communication MQTT

### Schéma JSON Maritime
```json
{
  "device_id": "OND001",
  "timestamp": 1642680000000,
  "position": {
    "latitude": 48.8566,
    "longitude": 2.3522,
    "altitude_m": 35.0,
    "accuracy_m": 2.5
  },
  "environment": {
    "temperature_c": 25.5,
    "humidity_pct": 65.0,
    "pressure_hpa": 1013.25
  },
  "power": {
    "battery_voltage": 3.7,
    "low_power_mode": false
  },
  "security": {
    "case_closed": true,
    "tamper_detected": false
  }
}
```

### Topics MQTT
- `ondocean/remoteid/{device_id}/data` - Données principales
- `ondocean/remoteid/{device_id}/status` - Statut système
- `ondocean/remoteid/{device_id}/logs` - Logs distants
- `ondocean/remoteid/{device_id}/errors` - Alertes d'erreurs

## 🛡️ Sécurité et Validation

### Validation Multi-Niveaux
1. **Validation Hardware** : Intégrité des capteurs
2. **Validation Données** : Plages de valeurs maritimes
3. **Validation Environnementale** : Conditions météo sûres
4. **Validation Sécurité** : Détection de tampering

### Gestion d'Erreurs Robuste
- **Classification** : 10 catégories d'erreurs (System, Hardware, Sensor, etc.)
- **Sévérité** : 5 niveaux (Info, Warning, Error, Critical, Fatal)
- **Récupération** : Stratégies automatiques de récupération
- **Monitoring** : Statistiques détaillées et alertes

## 📈 Performance

### Statistiques de Compilation
- **Taille firmware** : 1,278,407 octets (97% ESP32-S3)
- **Mémoire dynamique** : 61,056 octets (18% utilisés)
- **Temps compilation** : ~2 minutes (compilation complète)

### Benchmarks Tests
- **Validation données** : ~50μs par cycle
- **Lecture capteurs** : ~200μs par cycle
- **Sérialisation JSON** : ~100μs par message
- **Transmission MQTT** : ~10ms par publish

## 🚀 Roadmap

### Phase 1 (Actuelle) - MVP RemoteID Maritime ✅
- [x] Firmware ESP32-S3 compilable
- [x] Capteurs maritimes et validation
- [x] Transmission MQTT avec schéma unifié
- [x] Logging et gestion d'erreurs professionnels

### Phase 2 (6-12 mois) - Modules Vigie
- [ ] AIS Sniffer OndOcéan (RTL-SDR + dAISy)
- [ ] GNSS Logger/DePIN Node
- [ ] Station météo IoT maritime
- [ ] Packaging IP67 modulaire

### Phase 3 (12-24 mois) - Méduse Connectée
- [ ] Intégration ballon captif 3m³
- [ ] Relay 5G/VHF maritime
- [ ] Déploiement 0-120m profondeur
- [ ] Système complet multi-capteurs

## 🤝 Contribution

### Standards de Code
- **Langue** : Code et commentaires en anglais
- **Logging** : Système standardisé avec catégories
- **Tests** : Tests unitaires pour fonctions critiques
- **Documentation** : Markdown pour architecture et API

### Workflow de Développement
1. **Feature Branch** : Développement sur branche dédiée
2. **Tests** : Validation avec tests unitaires
3. **Code Review** : Revue selon bonnes pratiques
4. **Integration** : Merge après validation complète

## 📞 Support

### Contacts Techniques
- **Architecture** : Documentation dans `/docs`
- **API Reference** : `API_REFERENCE.md`
- **Deployment** : `DEPLOYMENT.md`
- **Issues** : GitHub Issues pour bugs et features

### Debugging
```cpp
// Activation debug complet
logger_set_level(LOG_LEVEL_DEBUG);
test_run_all();
logger_print_stats();
error_print_stats();
```

---

**OndOcéan RemoteID Maritime** - Firmware maritime professionnel pour ESP32-S3  
Version 1.0.0 - Septembre 2025 🌊🚁

### Setup Initial
```bash
# Repository GitHub officiel :
git clone https://github.com/Cusstomizer787/OndOcean-RemoteID-Maritime-v1.0.git
cd OndOcean-RemoteID-Maritime-v1.0

# Ou naviguer vers le projet local existant :
cd c:\Users\ncuss\CascadeProjects\windsurf-project\ondocean-remoteid

# Vérifier les dépendances
check_dependencies.bat
