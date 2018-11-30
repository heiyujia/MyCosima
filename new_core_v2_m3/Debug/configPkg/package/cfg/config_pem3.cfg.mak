# invoke SourceDir generated makefile for config.pem3
config.pem3: .libraries,config.pem3
.libraries,config.pem3: package/cfg/config_pem3.xdl
	$(MAKE) -f C:\Users\MICHAE~1\GIT\Concerto_OHMrichter_Master_V1\new_core_v2_m3/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\MICHAE~1\GIT\Concerto_OHMrichter_Master_V1\new_core_v2_m3/src/makefile.libs clean

