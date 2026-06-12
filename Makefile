TARGET_NAME := snakeR

SRC_DIR := src
BUILD_DIR := build

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=$(BUILD_DIR)/%.o)
TARGET := $(BUILD_DIR)/$(TARGET_NAME)
DESKTOP_TARGET := $(BUILD_DIR)/$(TARGET_NAME).desktop

PREFIX ?= $(HOME)/.local
BINDIR := $(PREFIX)/bin
DATADIR := $(PREFIX)/share
ICONDIR := $(DATADIR)/icons/hicolor/32x32/apps
APPDIR := $(DATADIR)/applications

BIN_ONLY ?= FALSE

CC := clang
CFLAGS := -Wall -Wextra -Iraylib-6.0/include
LDFLAGS := -fuse-ld=lld -Lraylib-6.0/lib/linux_amd64
LDLIBS := -lraylib -lGL -lm -lX11


.PHONY: all clean run install install-desktop uninstall

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

clean:
	@rm -rfv $(BUILD_DIR)

run: $(TARGET)
	@./$(TARGET)

	
ifeq ($(BIN_ONLY),FALSE)
DESKTOP_DEP := install-desktop
endif

install: $(TARGET) $(DESKTOP_DEP)
	install -D -m 755 $< $(DESTDIR)$(BINDIR)/$(TARGET_NAME)

install-desktop: $(DESKTOP_TARGET)
	install -D -m 644 icon32x32.png $(DESTDIR)$(ICONDIR)/$(TARGET_NAME).png
	install -D -m 644 $< $(DESTDIR)$(APPDIR)/$(TARGET_NAME).desktop

$(DESKTOP_TARGET): entry.desktop.in
	@BINDIR="$(BINDIR)" TARGET_NAME="$(TARGET_NAME)" envsubst < $< > $@

uninstall:
	@rm -fv $(DESTDIR)$(BINDIR)/$(TARGET_NAME)
	@rm -fv $(DESTDIR)$(ICONDIR)/$(TARGET_NAME).png
	@rm -fv $(DESTDIR)$(APPDIR)/$(TARGET_NAME).desktop
