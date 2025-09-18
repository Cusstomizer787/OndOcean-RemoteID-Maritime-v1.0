# OndOcéan RemoteID Maritime Makefile
# Based on ArduRemoteID with maritime adaptations

# Configuration
BOARD ?= ONDOCEAN_MARITIME_ESP32S3
PORT ?= COM3
BAUD ?= 921600

# ESP-IDF Configuration
IDF_PATH ?= $(HOME)/esp-idf
IDF_TOOLS_PATH ?= $(HOME)/.espressif

# Project paths
PROJECT_DIR := $(shell pwd)
BUILD_DIR := build
COMPONENTS_DIR := components
MAIN_DIR := main

# ArduRemoteID integration
ARDUPILOT_REMOTEID_PATH ?= ../ArduRemoteID/RemoteIDModule

# Firmware version
FW_VERSION := 1.0.0-maritime
BUILD_DATE := $(shell date -u +"%Y%m%d_%H%M%S")

# Board-specific settings
ifeq ($(BOARD),ONDOCEAN_MARITIME_ESP32S3)
	TARGET := esp32s3
	BOARD_CONFIG := board_config_maritime.h
	SDKCONFIG := sdkconfig.ondocean_maritime
else
	$(error Unsupported board: $(BOARD))
endif

# Default target
.PHONY: all
all: build

# Setup environment
.PHONY: setup
setup:
	@echo "Setting up OndOcéan RemoteID Maritime build environment..."
	@if [ ! -d "$(IDF_PATH)" ]; then \
		echo "ESP-IDF not found. Please install ESP-IDF first:"; \
		echo "git clone --recursive https://github.com/espressif/esp-idf.git $(IDF_PATH)"; \
		echo "cd $(IDF_PATH) && ./install.sh"; \
		exit 1; \
	fi
	@echo "ESP-IDF found at $(IDF_PATH)"
	@cp $(BOARD_CONFIG) $(ARDUPILOT_REMOTEID_PATH)/ || echo "Warning: Could not copy board config"
	@echo "Environment setup complete"

# Configure project
.PHONY: configure
configure: setup
	@echo "Configuring for board: $(BOARD)"
	@. $(IDF_PATH)/export.sh && idf.py set-target $(TARGET)
	@if [ -f "$(SDKCONFIG)" ]; then \
		cp $(SDKCONFIG) sdkconfig; \
		echo "Using board-specific sdkconfig"; \
	fi
	@. $(IDF_PATH)/export.sh && idf.py menuconfig

# Build firmware
.PHONY: build
build: setup
	@echo "Building OndOcéan RemoteID Maritime firmware..."
	@echo "Board: $(BOARD)"
	@echo "Target: $(TARGET)"
	@echo "Version: $(FW_VERSION)"
	@echo "Build date: $(BUILD_DATE)"
	@. $(IDF_PATH)/export.sh && \
	 export BOARD_ONDOCEAN_MARITIME_ESP32S3=1 && \
	 export FW_VERSION=$(FW_VERSION) && \
	 export BUILD_DATE=$(BUILD_DATE) && \
	 idf.py build
	@echo "Build complete. Firmware: $(BUILD_DIR)/OndOceanRemoteID.bin"

# Flash firmware
.PHONY: flash
flash: build
	@echo "Flashing to $(PORT) at $(BAUD) baud..."
	@. $(IDF_PATH)/export.sh && idf.py -p $(PORT) -b $(BAUD) flash

# Monitor serial output
.PHONY: monitor
monitor:
	@echo "Monitoring serial output on $(PORT)..."
	@. $(IDF_PATH)/export.sh && idf.py -p $(PORT) monitor

# Flash and monitor
.PHONY: flash-monitor
flash-monitor: flash monitor

# Clean build files
.PHONY: clean
clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILD_DIR)
	@rm -f sdkconfig.old
	@echo "Clean complete"

# Full clean (including ESP-IDF cache)
.PHONY: distclean
distclean: clean
	@echo "Full clean (including ESP-IDF cache)..."
	@. $(IDF_PATH)/export.sh && idf.py fullclean
	@rm -f sdkconfig

# Create OTA firmware
.PHONY: ota
ota: build
	@echo "Creating OTA firmware..."
	@cp $(BUILD_DIR)/OndOceanRemoteID.bin $(BUILD_DIR)/OndOceanRemoteID_$(FW_VERSION)_$(BUILD_DATE)_OTA.bin
	@echo "OTA firmware: $(BUILD_DIR)/OndOceanRemoteID_$(FW_VERSION)_$(BUILD_DATE)_OTA.bin"

# Sign OTA firmware (requires private key)
.PHONY: sign-ota
sign-ota: ota
	@if [ -z "$(PRIVATE_KEY)" ]; then \
		echo "Error: PRIVATE_KEY not specified"; \
		echo "Usage: make sign-ota PRIVATE_KEY=path/to/private_key.dat"; \
		exit 1; \
	fi
	@if [ ! -f "$(PRIVATE_KEY)" ]; then \
		echo "Error: Private key file not found: $(PRIVATE_KEY)"; \
		exit 1; \
	fi
	@echo "Signing OTA firmware with $(PRIVATE_KEY)..."
	@python3 $(ARDUPILOT_REMOTEID_PATH)/../scripts/sign_fw.py \
		$(BUILD_DIR)/OndOceanRemoteID_$(FW_VERSION)_$(BUILD_DATE)_OTA.bin \
		$(PRIVATE_KEY) 100
	@echo "Signed OTA firmware created"

# Generate keys for firmware signing
.PHONY: generate-keys
generate-keys:
	@echo "Generating public/private key pair..."
	@python3 $(ARDUPILOT_REMOTEID_PATH)/../scripts/generate_keys.py ondocean_maritime
	@echo "Keys generated: ondocean_maritime_private_key.dat, ondocean_maritime_public_key.dat"

# Erase flash completely
.PHONY: erase
erase:
	@echo "Erasing flash on $(PORT)..."
	@. $(IDF_PATH)/export.sh && idf.py -p $(PORT) erase-flash

# Show build info
.PHONY: info
info:
	@echo "OndOcéan RemoteID Maritime Build Information"
	@echo "=========================================="
	@echo "Board: $(BOARD)"
	@echo "Target: $(TARGET)"
	@echo "Port: $(PORT)"
	@echo "Baud: $(BAUD)"
	@echo "Firmware Version: $(FW_VERSION)"
	@echo "Build Date: $(BUILD_DATE)"
	@echo "ESP-IDF Path: $(IDF_PATH)"
	@echo "Project Dir: $(PROJECT_DIR)"
	@echo "Build Dir: $(BUILD_DIR)"
	@if [ -f "$(BUILD_DIR)/OndOceanRemoteID.bin" ]; then \
		echo "Firmware Size: $$(stat -c%s $(BUILD_DIR)/OndOceanRemoteID.bin) bytes"; \
	fi

# Install dependencies
.PHONY: install-deps
install-deps:
	@echo "Installing Python dependencies..."
	@pip3 install --user esptool pyserial
	@echo "Dependencies installed"

# Test build environment
.PHONY: test-env
test-env:
	@echo "Testing build environment..."
	@. $(IDF_PATH)/export.sh && idf.py --version
	@python3 --version
	@echo "Environment test complete"

# Create release package
.PHONY: release
release: clean build ota
	@echo "Creating release package..."
	@mkdir -p release/$(FW_VERSION)
	@cp $(BUILD_DIR)/OndOceanRemoteID.bin release/$(FW_VERSION)/
	@cp $(BUILD_DIR)/OndOceanRemoteID_$(FW_VERSION)_$(BUILD_DATE)_OTA.bin release/$(FW_VERSION)/
	@cp $(BUILD_DIR)/bootloader/bootloader.bin release/$(FW_VERSION)/
	@cp $(BUILD_DIR)/partition_table/partition-table.bin release/$(FW_VERSION)/
	@cp maritime_config.json release/$(FW_VERSION)/
	@cp README.md release/$(FW_VERSION)/
	@echo "Release package created in release/$(FW_VERSION)/"

# Development shortcuts
.PHONY: dev
dev: clean configure build flash-monitor

.PHONY: quick
quick: build flash

# Help
.PHONY: help
help:
	@echo "OndOcéan RemoteID Maritime Makefile"
	@echo "=================================="
	@echo ""
	@echo "Main targets:"
	@echo "  setup          - Setup build environment"
	@echo "  configure      - Configure project (interactive)"
	@echo "  build          - Build firmware"
	@echo "  flash          - Flash firmware to device"
	@echo "  monitor        - Monitor serial output"
	@echo "  flash-monitor  - Flash and monitor"
	@echo "  clean          - Clean build files"
	@echo "  distclean      - Full clean"
	@echo ""
	@echo "OTA targets:"
	@echo "  ota            - Create OTA firmware"
	@echo "  sign-ota       - Sign OTA firmware (requires PRIVATE_KEY=path)"
	@echo "  generate-keys  - Generate signing keys"
	@echo ""
	@echo "Utility targets:"
	@echo "  info           - Show build information"
	@echo "  erase          - Erase device flash"
	@echo "  install-deps   - Install Python dependencies"
	@echo "  test-env       - Test build environment"
	@echo "  release        - Create release package"
	@echo ""
	@echo "Development shortcuts:"
	@echo "  dev            - Clean + configure + build + flash + monitor"
	@echo "  quick          - Build + flash"
	@echo ""
	@echo "Variables:"
	@echo "  BOARD          - Target board (default: ONDOCEAN_MARITIME_ESP32S3)"
	@echo "  PORT           - Serial port (default: COM3)"
	@echo "  BAUD           - Flash baud rate (default: 921600)"
	@echo "  PRIVATE_KEY    - Path to private key for OTA signing"
	@echo ""
	@echo "Examples:"
	@echo "  make build"
	@echo "  make flash PORT=/dev/ttyUSB0"
	@echo "  make sign-ota PRIVATE_KEY=ondocean_maritime_private_key.dat"
