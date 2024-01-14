# Copyright (c) 2022 Weta Digital Ltd. All rights reserved
cmake_minimum_required(VERSION 3.21)

project(blocks
  VERSION 0.0.1
  DESCRIPTION "Server for managing a ham radio"
  HOMEPAGE_URL "https://gitlab.wetafx.co.nz/kdt3rd/hamware"
  LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

execute_process(COMMAND
  git log -1 --format=%ad --date=local
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/external/hamlib"
  OUTPUT_VARIABLE GIT_DATE
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
set(hamlib_date "#define HAMLIBDATETIME \"${GIT_DATE}\"\n\n")

file(CONFIGURE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/hamlibdatetime.h CONTENT ${hamlib_date})

file(COPY external/config.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/hamlib)

set(HAMLIB_RIG_SOURCE)

macro(hamlib_add_rig dir)
  set(cursrc ${ARGN})
  list(TRANSFORM cursrc PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/external/hamlib/rigs/${dir}/")
  list(APPEND HAMLIB_RIG_SOURCE ${cursrc})
endmacro()

hamlib_add_rig(dummy dummy_common.c dummy_common.h dummy.c dummy.h rot_dummy.c rot_dummy.h
  netrigctl.c netrotctl.c flrig.c flrig.h trxmanager.c trxmanager.h amp_dummy.c amp_dummy.h
  netampctl.c tci1x.c aclog.c sdrsharp.c quisk.c)

hamlib_add_rig(yaesu ft100.c ft100.h ft747.c ft747.h ft817.c ft817.h ft847.c ft847.h
	ft890.c ft890.h ft900.c ft900.h ft920.c ft920.h ft1000mp.c ft1000mp.h
	ft857.c ft857.h ft897.c ft897.h ft990.c ft990.h ft990v12.c ft990v12.h frg8800.c ft757gx.c
	ft757gx.h ft600.h ft600.c ft736.c frg100.c frg100.h frg9600.c ft1000d.c
	ft1000d.h vr5000.c ft767gx.c ft767gx.h ft840.c ft840.h ft980.c ft980.h
	vx1700.c vx1700.h ftdx10.h ft710.c
    newcat.c newcat.h ft450.c ft450.h ft950.c ft950.h ft991.c ft991.h
	ft2000.c ft2000.h ft9000.c ft9000.h ft5000.c ft5000.h ft1200.c ft1200.h
	ft891.c ft891.h ftdx101.c ftdx101.h ftdx101mp.c ft3000.c ftdx10.c ft710.h
    yaesu.c yaesu.h level_gran_yaesu.h)

hamlib_add_rig(kenwood
  ts850.c ts870s.c ts570.c ts450s.c ts950.c ts50s.c
  ts790.c ts2000.c k2.c k3.c xg3.c ts930.c
  ts680.c ts690.c ts140.c ts480.c trc80.c ts590.c ts890s.c
  ts990s.c ts990s.h flex6xxx.c pihpsdr.c tx500.c

  ts440.c ts940.c ts711.c ts811.c r5000.c

  thd7.c thf7.c thg71.c tmd700.c tmv7.c thf6a.c thd72.c tmd710.c thd74.c

  kenwood.c kenwood.h th.c th.h ic10.c ic10.h elecraft.c elecraft.h
  transfox.c flex.c flex.h level_gran_kenwood.h level_gran_elecraft.h
)

hamlib_add_rig(icom
  icom.c icom.h icom_defs.h frame.c frame.h ic706.c icr8500.c ic735.c ic775.c ic756.c
  ic275.c ic475.c ic1275.c ic820h.c ic821h.c
  icr7000.c ic910.c ic9100.c ic970.c ic725.c ic737.c ic718.c
  os535.c os456.c omni.c delta2.c ic92d.c
  ic736.c ic738.c ic7410.c ic746.c ic703.c ic726.c ic271.c
  ic765.c ic781.c ic471.c icr9000.c icr9500.c
  icr10.c icr20.c icr30.c icr6.c icr71.c icr72.c icr75.c icrx7.c icr8600.c
  id1.c id31.c id51.c id4100.c id5100.c perseus.c ic2730.c
  ic707.c ic728.c ic751.c ic761.c
  ic78.c ic7800.c ic785x.c
  ic7000.c ic7100.c ic7200.c ic7300.c ic7600.c ic7610.c ic7700.c icf8101.c
  ic7300.h optoscan.c optoscan.h xiegu.c level_gran_icom.h
)

hamlib_add_rig(icmarine
  icm700pro.c icm710.c icm710.h icm802.c icm803.c
  icmarine.c icmarine.h
)

hamlib_add_rig(pcr
  pcr1000.c pcr100.c pcr1500.c pcr2500.c pcr.c pcr.h
)

hamlib_add_rig(aor
  ar8200.c ar8000.c ar5000.c ar3000.c ar7030.c ar3030.c
  ar2700.c ar8600.c ar7030p.c ar7030p.h ar7030p_utils.c
  sr2200.c aor.c aor.h
)

hamlib_add_rig(jrc
  nrd535.c nrd545.c nrd525.c jrc.c jrc.h jst145.c
)

hamlib_add_rig(uniden
  bc895.c bc898.c bc245.c pro2052.c bc780.c bc250.c bcd396t.c
  bcd996t.c uniden.c uniden.h uniden_digital.c uniden_digital.h
)

hamlib_add_rig(drake
  r8a.c r8b.c drake.c drake.h
)

hamlib_add_rig(lowe
  hf235.c lowe.c lowe.h
)

hamlib_add_rig(racal
  ra6790.c ra3702.c ra37xx.c ra37xx.h racal.c racal.h
)

hamlib_add_rig(wj
  wj8888.c wj.c wj.h
)

hamlib_add_rig(skanti
  trp8000.c trp8255.c skanti.c skanti.h
)

hamlib_add_rig(tentec
  rx320.c rx340.c rx350.c rx331.c rx331.h tt550.c tt550.h
  pegasus.c argonaut.c orion.c orion.h jupiter.c omnivii.c paragon.c
  tentec.c tentec.h tentec2.c tentec2.h
)

hamlib_add_rig(alinco
  dx77.c dxsr8.c alinco.c alinco.h
)

hamlib_add_rig(kachina
  505dsp.c kachina.c kachina.h
)

hamlib_add_rig(tapr
  dsp10.c tapr.c tapr.h
)

hamlib_add_rig(flexradio
  flexradio.c flexradio.h sdr1k.c dttsp.c
)

hamlib_add_rig(rft
  ekd500.c rft.c rft.h
)

# USB!!!!
hamlib_add_rig(kit
  elektor304.c drt1.c dwt.c usrp.c elektor507.c dds60.c miniVNA.c
  si570avrusb.c si570avrusb.h funcube.c funcube.h fifisdr.c hiqsdr.c
  kit.c kit.h usrp_impl.h rs_hfiq.c
  pcrotor.c
)

hamlib_add_rig(tuner
  v4l.c v4l2.c tuner.c tuner.h videodev.h videodev2.h
)

hamlib_add_rig(rs
  esmc.c eb200.c rs.c rs.h gp2000.c gp2000.h xk2100.c ek89x.c ek89x.h
)

hamlib_add_rig(prm80
  prm8060.c prm80.c prm80.h
)

hamlib_add_rig(adat
  adt_200a.c adt_200a.h adat.c adat.h
)

hamlib_add_rig(dorji
  dorji.c dorji.h dra818.c dra818.h
)

hamlib_add_rig(barrett
  barrett.c barrett.h 950.c 4050.c 4100.c
)

hamlib_add_rig(elad
  elad.c elad.h fdm_duo.c
)

hamlib_add_rig(codan
  codan.c codan.h
)

hamlib_add_rig(gomspace
  gs100.c gs100.h
)

hamlib_add_rig(mds
  mds.c mds.h 4710.c 9710.c
)

hamlib_add_rig(anytone
  anytone.c d578.c anytone.h
)

add_library(hamlib_lib STATIC
  external/hamlib/lib/cJSON.c
  external/hamlib/lib/cJSON.h
  external/hamlib/lib/asyncpipe.c
  external/hamlib/lib/asyncpipe.h
  external/hamlib/lib/precise_time.c

  external/hamlib/c++/rigclass.cc
  external/hamlib/c++/rotclass.cc
  external/hamlib/c++/ampclass.cc

  external/hamlib/security/aes.c
  external/hamlib/security/AESStringCrypt.c
  external/hamlib/security/password.c
  external/hamlib/security/security.c
  external/hamlib/security/sha256.c
  external/hamlib/security/md5.c
  external/hamlib/security/aes.h
  external/hamlib/security/AESStringCrypt.h
  external/hamlib/security/password.h
  external/hamlib/security/security.h
  external/hamlib/security/sha256.h
  external/hamlib/security/md5.h

  external/hamlib/src/rig.c
  external/hamlib/src/serial.c
  external/hamlib/src/serial.h
  external/hamlib/src/misc.c
  external/hamlib/src/misc.h
  external/hamlib/src/register.c
  external/hamlib/src/register.h
  external/hamlib/src/event.c
  external/hamlib/src/event.h
  external/hamlib/src/cal.c
  external/hamlib/src/cal.h
  external/hamlib/src/conf.c
  external/hamlib/src/tones.c
  external/hamlib/src/tones.h
  external/hamlib/src/rotator.c
  external/hamlib/src/locator.c
  external/hamlib/src/rot_reg.c
  external/hamlib/src/rot_conf.c
  external/hamlib/src/rot_conf.h
  external/hamlib/src/rot_settings.c
  external/hamlib/src/rot_ext.c
  external/hamlib/src/iofunc.c
  external/hamlib/src/iofunc.h
  external/hamlib/src/ext.c
  external/hamlib/src/mem.c
  external/hamlib/src/settings.c
  external/hamlib/src/parallel.c
  external/hamlib/src/parallel.h
  external/hamlib/src/usb_port.c
  external/hamlib/src/usb_port.h
  external/hamlib/src/debug.c
  external/hamlib/src/network.c
  external/hamlib/src/network.h
  external/hamlib/src/cm108.c
  external/hamlib/src/cm108.h
  external/hamlib/src/gpio.c
  external/hamlib/src/gpio.h
  external/hamlib/src/idx_builtin.h
  external/hamlib/src/token.h
  external/hamlib/src/par_nt.h
  external/hamlib/src/microham.c
  external/hamlib/src/microham.h
  external/hamlib/src/amplifier.c
  external/hamlib/src/amp_reg.c
  external/hamlib/src/amp_conf.c
  external/hamlib/src/amp_conf.h
  external/hamlib/src/amp_settings.c
  external/hamlib/src/extamp.c
  external/hamlib/src/sleep.c
  external/hamlib/src/sleep.h
  external/hamlib/src/sprintflst.c
  external/hamlib/src/sprintflst.h
  external/hamlib/src/cache.c
  external/hamlib/src/cache.h
  external/hamlib/src/snapshot_data.c
  external/hamlib/src/snapshot_data.h
  external/hamlib/src/fifo.c
  external/hamlib/src/fifo.h
  external/hamlib/src/serial_cfg_params.h

  ${HAMLIB_RIG_SOURCE}
)

target_include_directories(hamlib_lib PUBLIC external/hamlib/include)
target_include_directories(hamlib_lib PRIVATE external/hamlib/include/hamlib)
target_include_directories(hamlib_lib PRIVATE external/hamlib/src)
target_include_directories(hamlib_lib PRIVATE external/hamlib/lib)
target_include_directories(hamlib_lib PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
target_include_directories(hamlib_lib PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include/hamlib")

##add_library(fldigi_lib STATIC
##)

add_executable(hamware
  src/configuration.c
  src/daemonize.c
  src/http.c
  src/jsonrpc.c
  src/main.c
  src/plugins.c

  src/config.h
  src/configuration.h
  src/http.h
  src/jsonrpc.h
  src/plugins.h
  src/plugin_register.h
  src/plugins.h
  src/daemonize.h
)

add_library(hamlib SHARED
  plugins/hamlib.c
)
target_include_directories(hamlib PRIVATE src)
target_link_libraries(hamlib hamlib_lib)

set_target_properties(hamlib
  PROPERTIES
  PREFIX ""
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)
