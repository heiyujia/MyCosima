## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,em3 linker.cmd package/cfg/Prototype_pem3.oem3

linker.cmd: package/cfg/Prototype_pem3.xdl
	$(SED) 's"^\"\(package/cfg/Prototype_pem3cfg.cmd\)\"$""\"C:/ti/workspace/WS-Git/Prototyp/Prototype_M3/.config/xconfig_Prototype/\1\""' package/cfg/Prototype_pem3.xdl > $@
