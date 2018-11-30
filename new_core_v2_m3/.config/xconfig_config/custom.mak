## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,em3 linker.cmd package/cfg/config_pem3.oem3

linker.cmd: package/cfg/config_pem3.xdl
	$(SED) 's"^\"\(package/cfg/config_pem3cfg.cmd\)\"$""\"C:/ti_ccs55/workspace/core_v0/core_repo/core_v0_m3/.config/xconfig_config/\1\""' package/cfg/config_pem3.xdl > $@
