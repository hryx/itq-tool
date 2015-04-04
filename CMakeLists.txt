cmake_minimum_required (VERSION 2.8.12)

project(itq)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
find_package(Vorbis REQUIRED)

include_directories(${VORBIS_INCLUDE_DIRS})

set(modplug_dir "${CMAKE_SOURCE_DIR}/src")
set(modplug_sources
	"${modplug_dir}/codec_vorbis.c"
	"${modplug_dir}/fastmix.cpp"
	"${modplug_dir}/load_ams.cpp"
	"${modplug_dir}/load_far.cpp"
	"${modplug_dir}/load_mdl.cpp"
	"${modplug_dir}/load_mt2.cpp"
	"${modplug_dir}/load_psm.cpp"
	"${modplug_dir}/load_ult.cpp"
	"${modplug_dir}/mmcmp.cpp"
	"${modplug_dir}/snd_fx.cpp"
	"${modplug_dir}/load_669.cpp"
	"${modplug_dir}/load_dbm.cpp"
	"${modplug_dir}/load_it.cpp"
	"${modplug_dir}/load_med.cpp"
	"${modplug_dir}/load_mtm.cpp"
	"${modplug_dir}/load_ptm.cpp"
	"${modplug_dir}/load_umx.cpp"
	"${modplug_dir}/modplug.cpp"
	"${modplug_dir}/sndfile.cpp"
	"${modplug_dir}/load_abc.cpp"
	"${modplug_dir}/load_dmf.cpp"
	"${modplug_dir}/load_itq.cpp"
	"${modplug_dir}/load_mid.cpp"
	"${modplug_dir}/load_okt.cpp"
	"${modplug_dir}/load_s3m.cpp"
	"${modplug_dir}/load_wav.cpp"
	"${modplug_dir}/snd_dsp.cpp"
	"${modplug_dir}/sndmix.cpp"
	"${modplug_dir}/load_amf.cpp"
	"${modplug_dir}/load_dsm.cpp"
	"${modplug_dir}/load_j2b.cpp"
	"${modplug_dir}/load_mod.cpp"
	"${modplug_dir}/load_pat.cpp"
	"${modplug_dir}/load_stm.cpp"
	"${modplug_dir}/load_xm.cpp"
	"${modplug_dir}/snd_flt.cpp"
)
set(itq_path "${CMAKE_SOURCE_DIR}/itq")
set(itq_sources
	"${itq_path}/itq.c"
)

# configure_file(
# 	"${modplug_dir}/configure.h.in"
# 	"${modplug_dir}/configure.h"
# )

include_directories(
	"${modplug_dir}"
	"${modplug_dir}/libmodplug"
	${VORBIS_INCLUDE_DIR}
)

if($<CXX_COMPILER_ID>:"Clang")
	add_compile_options("-std=c++11" "-stdlib=libc++")
endif()
add_compile_options("-g" "-O0" "-mmacosx-version-min=10.5")

# Needed for config.h flags to take effect
add_compile_options("-DHAVE_CONFIG_H")

add_executable(itq
	${itq_sources}
	${modplug_sources}
)

target_link_libraries(itq ${VORBIS_LIBRARIES})
