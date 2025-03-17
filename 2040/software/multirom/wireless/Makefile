SJASM = sjasmplus.exe

BUILD_DIR = build
DIST_DIR = dist
SRC_DIR = src
TOOLS_DIR = tools

all: clean rom package

$(BUILD_DIR)/ESP8266P.ROM: $(SRC_DIR)/ESP8266_memio.asm
	@echo "Assembling $<..."
	$(TOOLS_DIR)/$(SJASM) --nologo --sym=$(BUILD_DIR)/ESP8266P.sym --lst=$(BUILD_DIR)/ESP8266P.lst --raw=$(BUILD_DIR)/ESP8266P.ROM $<

rom: $(BUILD_DIR)/ESP8266P.ROM

clean:
	@echo "Cleaning..."
	@rm -f $(BUILD_DIR)/ESP8266P.rom $(BUILD_DIR)/ESP8266P.lst $(BUILD_DIR)/ESP8266P.sym
	@rm -f $(DIST_DIR)/ESP8266P.rom

package:
	@echo "Creating package..."
	@cp $(BUILD_DIR)/ESP8266P.rom $(DIST_DIR)/ESP8266P.ROM