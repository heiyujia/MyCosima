## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,em3 linker.cmd package/cfg/config_pem3.oem3

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/config_pem3.xdl
	$(SED) 's"^\"\(package/cfg/config_pem3cfg.cmd\)\"$""\"C:/Users/Michael Hoerner/GIT/Concerto_OHMrichter_Master_V1/new_core_v2_m3/Debug/configPkg/\1\""' package/cfg/config_pem3.xdl > $@
	-$(SETDATE) -r:max package/cfg/config_pem3.h compiler.opt compiler.opt.defs
