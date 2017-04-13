# 
# Copyright (C) 2017 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=tinycom
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION).$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk

define Package/tinycom
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+libc +libgcc
  TITLE:=Debug helper for TTY devices
endef

define Package/tinycom/description
  Tinycom is a linux package that used to debug tty devices,
  it can work under sender or receiver role.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/tinycom/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tinycom $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,tinycom))
